#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdint.h>
#include <emmintrin.h> // SSE2

/*
 * High-Performance Scrypt implementation for Litecoin (N=1024, r=1, p=1)
 * Optimized with SSE2 SIMD and GIL release.
 */

#define ROTL(a, b) _mm_slli_epi32(a, b)
#define ROTR(a, b) _mm_srli_epi32(a, b)
#define ROTATE(a, b) _mm_or_si128(_mm_slli_epi32(a, b), _mm_srli_epi32(a, 32 - b))

static inline void salsa20_8_sse2(__m128i B[4]) {
    __m128i x[4];
    for (int i = 0; i < 4; i++) x[i] = B[i];

    for (int i = 8; i > 0; i -= 2) {
        // Column rounds
        x[1] = _mm_xor_si128(x[1], ROTATE(_mm_add_epi32(x[0], x[3]), 7));
        x[2] = _mm_xor_si128(x[2], ROTATE(_mm_add_epi32(x[1], x[0]), 9));
        x[3] = _mm_xor_si128(x[3], ROTATE(_mm_add_epi32(x[2], x[1]), 13));
        x[0] = _mm_xor_si128(x[0], ROTATE(_mm_add_epi32(x[3], x[2]), 18));

        // Diagonal/Row rounds (shuffling for SIMD efficiency)
        x[1] = _mm_shuffle_epi32(x[1], _MM_SHUFFLE(0, 3, 2, 1));
        x[2] = _mm_shuffle_epi32(x[2], _MM_SHUFFLE(1, 0, 3, 2));
        x[3] = _mm_shuffle_epi32(x[3], _MM_SHUFFLE(2, 1, 0, 3));

        x[3] = _mm_xor_si128(x[3], ROTATE(_mm_add_epi32(x[0], x[1]), 7));
        x[2] = _mm_xor_si128(x[2], ROTATE(_mm_add_epi32(x[3], x[0]), 9));
        x[1] = _mm_xor_si128(x[1], ROTATE(_mm_add_epi32(x[2], x[3]), 13));
        x[0] = _mm_xor_si128(x[0], ROTATE(_mm_add_epi32(x[1], x[2]), 18));

        x[1] = _mm_shuffle_epi32(x[1], _MM_SHUFFLE(2, 1, 0, 3));
        x[2] = _mm_shuffle_epi32(x[2], _MM_SHUFFLE(1, 0, 3, 2));
        x[3] = _mm_shuffle_epi32(x[3], _MM_SHUFFLE(0, 3, 2, 1));
    }
    for (int i = 0; i < 4; i++) B[i] = _mm_add_epi32(B[i], x[i]);
}

static void scrypt_core_sse2(const uint8_t *input, uint8_t *output) {
    __m128i B[8]; // 128 bytes (2 blocks of 64 bytes)
    __m128i *V = (__m128i *)aligned_alloc(16, 1024 * 128);

    // Initial load (80 bytes input padded to 128)
    uint8_t local_B[128] = {0};
    memcpy(local_B, input, 80);
    for (int i = 0; i < 8; i++) B[i] = _mm_loadu_si128((__m128i *)&local_B[i * 16]);

    // ROMix Stage 1
    for (int i = 0; i < 1024; i++) {
        for (int j = 0; j < 8; j++) V[i * 8 + j] = B[j];

        // BlockMix Salsa8
        __m128i X[4];
        for(int j=0; j<4; j++) X[j] = _mm_xor_si128(B[j], B[j+4]);
        salsa20_8_sse2(X);
        for(int j=0; j<4; j++) {
            __m128i t = _mm_xor_si128(X[j], B[j+4]);
            B[j] = X[j];
            B[j+4] = t;
        }
        salsa20_8_sse2(&B[4]);
    }

    // ROMix Stage 2
    for (int i = 0; i < 1024; i++) {
        uint32_t *B_ptr = (uint32_t *)&B[4];
        int j = B_ptr[0] & 1023;
        for (int k = 0; k < 8; k++) B[k] = _mm_xor_si128(B[k], V[j * 8 + k]);

        // BlockMix Salsa8
        __m128i X[4];
        for(int k=0; k<4; k++) X[k] = _mm_xor_si128(B[k], B[k+4]);
        salsa20_8_sse2(X);
        for(int k=0; k<4; k++) {
            __m128i t = _mm_xor_si128(X[k], B[k+4]);
            B[k] = X[k];
            B[k+4] = t;
        }
        salsa20_8_sse2(&B[4]);
    }

    _mm_storeu_si128((__m128i *)output, B[0]);
    _mm_storeu_si128((__m128i *)(output + 16), B[1]);

    free(V);
}

static PyObject *scrypt_getpowhash(PyObject *self, PyObject *args) {
    Py_buffer buffer;
    if (!PyArg_ParseTuple(args, "y*", &buffer)) return NULL;

    uint8_t output[32];

    // Release GIL for performance
    Py_BEGIN_ALLOW_THREADS
    scrypt_core_sse2((const uint8_t *)buffer.buf, output);
    Py_END_ALLOW_THREADS

    PyBuffer_Release(&buffer);
    return PyBytes_FromStringAndSize((const char *)output, 32);
}

static PyMethodDef ScryptMethods[] = {
    {"getPoWHash", scrypt_getpowhash, METH_VARARGS, "High-performance Scrypt PoW hash"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef scryptmodule = {
    PyModuleDef_HEAD_INIT, "ltc_scrypt", NULL, -1, ScryptMethods
};

PyMODINIT_FUNC PyInit_ltc_scrypt(void) {
    return PyModule_Create(&scryptmodule);
}
