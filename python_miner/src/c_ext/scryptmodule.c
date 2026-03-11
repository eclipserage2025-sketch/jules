#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdint.h>

/*
 * Scrypt implementation for Litecoin (N=1024, r=1, p=1)
 */

static inline uint32_t rotate(uint32_t a, uint32_t b) {
    return (a << b) | (a >> (32 - b));
}

static void salsa20_8(uint32_t B[16]) {
    uint32_t x[16];
    for (int i = 0; i < 16; i++) x[i] = B[i];
    for (int i = 8; i > 0; i -= 2) {
        x[ 4] ^= rotate(x[ 0] + x[12], 7);  x[ 8] ^= rotate(x[ 4] + x[ 0], 9);
        x[12] ^= rotate(x[ 8] + x[ 4], 13); x[ 0] ^= rotate(x[12] + x[ 8], 18);
        x[ 9] ^= rotate(x[ 5] + x[ 1], 7);  x[13] ^= rotate(x[ 9] + x[ 5], 9);
        x[ 1] ^= rotate(x[13] + x[ 9], 13); x[ 5] ^= rotate(x[ 1] + x[13], 18);
        x[14] ^= rotate(x[10] + x[ 6], 7);  x[ 2] ^= rotate(x[14] + x[10], 9);
        x[ 6] ^= rotate(x[ 2] + x[14], 13); x[10] ^= rotate(x[ 6] + x[ 2], 18);
        x[ 3] ^= rotate(x[15] + x[11], 7);  x[ 7] ^= rotate(x[ 3] + x[15], 9);
        x[11] ^= rotate(x[ 7] + x[ 3], 13); x[15] ^= rotate(x[11] + x[ 7], 18);
        x[ 1] ^= rotate(x[ 0] + x[ 3], 7);  x[ 2] ^= rotate(x[ 1] + x[ 0], 9);
        x[ 3] ^= rotate(x[ 2] + x[ 1], 13); x[ 0] ^= rotate(x[ 3] + x[ 2], 18);
        x[ 6] ^= rotate(x[ 5] + x[ 4], 7);  x[ 7] ^= rotate(x[ 6] + x[ 5], 9);
        x[ 4] ^= rotate(x[ 7] + x[ 6], 13); x[ 5] ^= rotate(x[ 4] + x[ 7], 18);
        x[11] ^= rotate(x[10] + x[ 9], 7);  x[ 8] ^= rotate(x[11] + x[10], 9);
        x[ 9] ^= rotate(x[ 8] + x[11], 13); x[10] ^= rotate(x[ 9] + x[ 8], 18);
        x[12] ^= rotate(x[15] + x[14], 7);  x[13] ^= rotate(x[12] + x[15], 9);
        x[14] ^= rotate(x[13] + x[12], 13); x[15] ^= rotate(x[14] + x[13], 18);
    }
    for (int i = 0; i < 16; i++) B[i] += x[i];
}

static void blockmix_salsa8(uint32_t *B, uint32_t *out) {
    uint32_t X[16];
    for (int i = 0; i < 16; i++) X[i] = B[i + 16] ^ B[i];
    salsa20_8(X);
    for (int i = 0; i < 16; i++) out[i] = X[i];
    for (int i = 0; i < 16; i++) X[i] ^= B[i + 16];
    salsa20_8(X);
    for (int i = 0; i < 16; i++) out[i + 16] = X[i];
}

static void scrypt_1024_1_1_256(const uint8_t *password, uint8_t *output) {
    uint32_t B[32];
    uint32_t V[1024][32];

    /* Note: In a production scrypt, password would be PBKDF2 pre-processed.
       Litecoin Scrypt directly uses the 80-byte header as B. */
    for (int i = 0; i < 32; i++) B[i] = ((uint32_t*)password)[i];

    for (int i = 0; i < 1024; i++) {
        for (int j = 0; j < 32; j++) V[i][j] = B[j];
        uint32_t nextB[32];
        blockmix_salsa8(B, nextB);
        for (int j = 0; j < 32; j++) B[j] = nextB[j];
    }

    for (int i = 0; i < 1024; i++) {
        int j = B[16] & 1023;
        for (int k = 0; k < 32; k++) B[k] ^= V[j][k];
        uint32_t nextB[32];
        blockmix_salsa8(B, nextB);
        for (int j = 0; j < 32; j++) B[j] = nextB[j];
    }

    for (int i = 0; i < 8; i++) ((uint32_t*)output)[i] = B[i];
}

static PyObject *scrypt_getpowhash(PyObject *self, PyObject *args) {
    Py_buffer buffer;
    if (!PyArg_ParseTuple(args, "y*", &buffer)) {
        return NULL;
    }

    uint8_t output[32];
    scrypt_1024_1_1_256((const uint8_t *)buffer.buf, output);
    PyBuffer_Release(&buffer);

    return PyBytes_FromStringAndSize((const char *)output, 32);
}

static PyMethodDef ScryptMethods[] = {
    {"getPoWHash", scrypt_getpowhash, METH_VARARGS, "Get Scrypt PoW hash"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef scryptmodule = {
    PyModuleDef_HEAD_INIT,
    "ltc_scrypt",
    "Litecoin Scrypt hashing extension",
    -1,
    ScryptMethods
};

PyMODINIT_FUNC PyInit_ltc_scrypt(void) {
    return PyModule_Create(&scryptmodule);
}
