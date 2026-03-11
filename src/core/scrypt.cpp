#include "scrypt.h"
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <cstring>
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <memory>

namespace core {

void Scrypt::salsa20_8(uint32_t* B) {
    uint32_t x[16];
    std::memcpy(x, B, 64);

    for (int i = 8; i > 0; i -= 2) {
        x[ 4] ^= rotate_left(x[ 0] + x[12], 7);
        x[ 8] ^= rotate_left(x[ 4] + x[ 0], 9);
        x[12] ^= rotate_left(x[ 8] + x[ 4], 13);
        x[ 0] ^= rotate_left(x[12] + x[ 8], 18);

        x[ 9] ^= rotate_left(x[ 5] + x[ 1], 7);
        x[13] ^= rotate_left(x[ 9] + x[ 5], 9);
        x[ 1] ^= rotate_left(x[13] + x[ 9], 13);
        x[ 5] ^= rotate_left(x[ 1] + x[13], 18);

        x[14] ^= rotate_left(x[10] + x[ 6], 7);
        x[ 2] ^= rotate_left(x[14] + x[10], 9);
        x[ 6] ^= rotate_left(x[ 2] + x[14], 13);
        x[10] ^= rotate_left(x[ 6] + x[ 2], 18);

        x[ 3] ^= rotate_left(x[15] + x[11], 7);
        x[ 7] ^= rotate_left(x[ 3] + x[15], 9);
        x[11] ^= rotate_left(x[ 7] + x[ 3], 13);
        x[15] ^= rotate_left(x[11] + x[ 7], 18);

        x[ 1] ^= rotate_left(x[ 0] + x[ 3], 7);
        x[ 2] ^= rotate_left(x[ 1] + x[ 0], 9);
        x[ 3] ^= rotate_left(x[ 2] + x[ 1], 13);
        x[ 0] ^= rotate_left(x[ 3] + x[ 2], 18);

        x[ 6] ^= rotate_left(x[ 5] + x[ 4], 7);
        x[ 7] ^= rotate_left(x[ 6] + x[ 5], 9);
        x[ 4] ^= rotate_left(x[ 7] + x[ 6], 13);
        x[ 5] ^= rotate_left(x[ 4] + x[ 7], 18);

        x[11] ^= rotate_left(x[10] + x[ 9], 7);
        x[ 8] ^= rotate_left(x[11] + x[10], 9);
        x[ 9] ^= rotate_left(x[ 8] + x[11], 13);
        x[10] ^= rotate_left(x[ 9] + x[ 8], 18);

        x[12] ^= rotate_left(x[15] + x[14], 7);
        x[13] ^= rotate_left(x[12] + x[15], 9);
        x[14] ^= rotate_left(x[13] + x[12], 13);
        x[15] ^= rotate_left(x[14] + x[13], 18);
    }

    for (int i = 0; i < 16; ++i) B[i] += x[i];
}

void Scrypt::scrypt_blockmix(uint8_t* B, uint32_t r) {
    uint8_t X[64];
    std::memcpy(X, &B[(2 * r - 1) * 64], 64);

    std::unique_ptr<uint8_t[]> Y = std::make_unique<uint8_t[]>(128 * r);
    for (uint32_t i = 0; i < 2 * r; ++i) {
        for (uint32_t j = 0; j < 64; ++j) X[j] ^= B[i * 64 + j];
        salsa20_8(reinterpret_cast<uint32_t*>(X));
        std::memcpy(&Y[i * 64], X, 64);
    }

    for (uint32_t i = 0; i < r; ++i) {
        std::memcpy(&B[i * 64], &Y[(i * 2) * 64], 64);
        std::memcpy(&B[(i + r) * 64], &Y[(i * 2 + 1) * 64], 64);
    }
}

void Scrypt::scrypt_romix(uint8_t* B, uint32_t r, uint32_t N, uint8_t* V) {
    const uint32_t block_len = 128 * r;
    for (uint32_t i = 0; i < N; ++i) {
        std::memcpy(&V[i * block_len], B, block_len);
        scrypt_blockmix(B, r);
    }
    for (uint32_t i = 0; i < N; ++i) {
        uint32_t j = (*reinterpret_cast<const uint32_t*>(&B[(2 * r - 1) * 64])) % N;
        for (uint32_t k = 0; k < block_len; ++k) B[k] ^= V[j * block_len + k];
        scrypt_blockmix(B, r);
    }
}

void Scrypt::pbkdf2_sha256(const uint8_t* pass, size_t pass_len,
                           const uint8_t* salt, size_t salt_len,
                           uint32_t count, uint8_t* output, size_t output_len) {
    if (PKCS5_PBKDF2_HMAC(reinterpret_cast<const char*>(pass), pass_len,
                           salt, salt_len, count, EVP_sha256(),
                           output_len, output) != 1) {
        throw std::runtime_error("[CORE] PBKDF2 SHA256 failed");
    }
}

std::vector<uint8_t> Scrypt::hash(const std::vector<uint8_t>& password,
                                 const std::vector<uint8_t>& salt,
                                 uint32_t N, uint32_t r, uint32_t p) {
    const uint32_t block_len = 128 * r;
    std::vector<uint8_t> B(p * block_len);
    std::vector<uint8_t> V(N * block_len);

    pbkdf2_sha256(password.data(), password.size(), salt.data(), salt.size(), 1, B.data(), p * block_len);
    for (uint32_t i = 0; i < p; ++i) scrypt_romix(&B[i * block_len], r, N, V.data());

    std::vector<uint8_t> output(32);
    pbkdf2_sha256(password.data(), password.size(), B.data(), B.size(), 1, output.data(), 32);
    return output;
}

std::vector<uint8_t> Scrypt::litecoin_scrypt(const std::vector<uint8_t>& data) {
    return hash(data, data, 1024, 1, 1);
}

} // namespace core
