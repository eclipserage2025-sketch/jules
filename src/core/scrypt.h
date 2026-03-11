#ifndef CORE_SCRYPT_H
#define CORE_SCRYPT_H

#include <cstdint>
#include <vector>
#include <string>

namespace core {

/**
 * @brief Refined ground-up implementation of the Scrypt hashing algorithm.
 * Optimized for reduced memory allocation and improved const-correctness.
 */
class Scrypt {
public:
    static std::vector<uint8_t> hash(const std::vector<uint8_t>& password,
                                     const std::vector<uint8_t>& salt,
                                     uint32_t N, uint32_t r, uint32_t p);

    static std::vector<uint8_t> litecoin_scrypt(const std::vector<uint8_t>& data);

private:
    // Internal helper functions
    static void pbkdf2_sha256(const uint8_t* pass, size_t pass_len,
                               const uint8_t* salt, size_t salt_len,
                               uint32_t count, uint8_t* output, size_t output_len);

    static void scrypt_romix(uint8_t* B, uint32_t r, uint32_t N, uint8_t* V);
    static void scrypt_blockmix(uint8_t* B, uint32_t r);
    static void salsa20_8(uint32_t* B);

    static inline uint32_t rotate_left(uint32_t x, int n) {
        return (x << n) | (x >> (32 - n));
    }
};

} // namespace core

#endif // CORE_SCRYPT_H
