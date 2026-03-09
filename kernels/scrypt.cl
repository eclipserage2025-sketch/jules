/**
 * @file scrypt.cl
 * @brief OpenCL kernels for Scrypt (Litecoin) mining
 * Ground-up implementation for AMD GPUs
 */

typedef unsigned int uint32_t;

inline void salsa20_8_core(uint32_t* B) {
    uint32_t x[16];
    for (int i = 0; i < 16; i++) x[i] = B[i];

    for (int i = 8; i > 0; i -= 2) {
        // Column rounds
        x[ 4] ^= rotate((x[ 0] + x[12]), 7u);
        x[ 8] ^= rotate((x[ 4] + x[ 0]), 9u);
        x[12] ^= rotate((x[ 8] + x[ 4]), 13u);
        x[ 0] ^= rotate((x[12] + x[ 8]), 18u);

        x[ 9] ^= rotate((x[ 5] + x[ 1]), 7u);
        x[13] ^= rotate((x[ 9] + x[ 5]), 9u);
        x[ 1] ^= rotate((x[13] + x[ 9]), 13u);
        x[ 5] ^= rotate((x[ 1] + x[13]), 18u);

        x[14] ^= rotate((x[10] + x[ 6]), 7u);
        x[ 2] ^= rotate((x[14] + x[10]), 9u);
        x[ 6] ^= rotate((x[ 2] + x[14]), 13u);
        x[10] ^= rotate((x[ 6] + x[ 2]), 18u);

        x[ 3] ^= rotate((x[15] + x[11]), 7u);
        x[ 7] ^= rotate((x[ 3] + x[15]), 9u);
        x[11] ^= rotate((x[ 7] + x[ 3]), 13u);
        x[15] ^= rotate((x[11] + x[ 7]), 18u);

        // Row rounds
        x[ 1] ^= rotate((x[ 0] + x[ 3]), 7u);
        x[ 2] ^= rotate((x[ 1] + x[ 0]), 9u);
        x[ 3] ^= rotate((x[ 2] + x[ 1]), 13u);
        x[ 0] ^= rotate((x[ 3] + x[ 2]), 18u);

        x[ 6] ^= rotate((x[ 5] + x[ 4]), 7u);
        x[ 7] ^= rotate((x[ 6] + x[ 5]), 9u);
        x[ 4] ^= rotate((x[ 7] + x[ 6]), 13u);
        x[ 5] ^= rotate((x[ 4] + x[ 7]), 18u);

        x[11] ^= rotate((x[10] + x[ 9]), 7u);
        x[ 8] ^= rotate((x[11] + x[10]), 9u);
        x[ 9] ^= rotate((x[ 8] + x[11]), 13u);
        x[10] ^= rotate((x[ 9] + x[ 8]), 18u);

        x[12] ^= rotate((x[15] + x[14]), 7u);
        x[13] ^= rotate((x[12] + x[15]), 9u);
        x[14] ^= rotate((x[13] + x[12]), 13u);
        x[15] ^= rotate((x[14] + x[13]), 18u);
    }

    for (int i = 0; i < 16; i++) B[i] += x[i];
}

inline void blockmix_salsa8(uint32_t* bin, uint32_t* bout) {
    uint32_t x[16];
    for (int i = 0; i < 16; i++) x[i] = bin[i + 16] ^ bin[i];
    salsa20_8_core(x);
    for (int i = 0; i < 16; i++) bout[i] = x[i];

    for (int i = 0; i < 16; i++) x[i] ^= bin[i + 16];
    salsa20_8_core(x);
    for (int i = 0; i < 16; i++) bout[i + 16] = x[i];
}

__kernel void scrypt_core(__global const uint32_t* input,
                           __global uint32_t* output,
                           __global uint32_t* scratchpad,
                           uint32_t n_threads) {
    uint gid = get_global_id(0);
    if (gid >= n_threads) return;

    // Each thread gets a scratchpad of N * 128 bytes (1024 * 32 uints)
    __global uint32_t* V = &scratchpad[gid * 1024 * 32];
    uint32_t B[32];

    // Initial block (simplified for Litecoin Scrypt N=1024, r=1)
    // In real use, this would be the output of PBKDF2
    for(int i=0; i<32; i++) B[i] = input[i];

    // ROMix - Stage 1
    for (int i = 0; i < 1024; i++) {
        for(int k=0; k<32; k++) V[i * 32 + k] = B[k];
        uint32_t temp[32];
        blockmix_salsa8(B, temp);
        for(int k=0; k<32; k++) B[k] = temp[k];
    }

    // ROMix - Stage 2
    for (int i = 0; i < 1024; i++) {
        uint32_t j = B[16] & 1023; // integerify mod N
        for(int k=0; k<32; k++) B[k] ^= V[j * 32 + k];
        uint32_t temp[32];
        blockmix_salsa8(B, temp);
        for(int k=0; k<32; k++) B[k] = temp[k];
    }

    // Final result
    for(int i=0; i<32; i++) output[gid * 32 + i] = B[i];
}
