#pragma once
#include <cstdint>
#include <vector>
#include <string>

namespace core {

/**
 * @brief Ground-up implementation of the Scrypt hashing algorithm
 */
class Scrypt {
public:
    static std::vector<uint8_t> hash(const std::vector<uint8_t>& password,
                                     const std::vector<uint8_t>& salt,
                                     uint32_t N, uint32_t r, uint32_t p);

    static std::vector<uint8_t> litecoin_scrypt(const std::vector<uint8_t>& data);

private:
    // Internal helper functions: Salsa20/8, PBKDF2, BlockMix, ROMix
    static void pbkdf2_sha256(const uint8_t* pass, size_t pass_len,
                               const uint8_t* salt, size_t salt_len,
                               uint32_t count, uint8_t* output, size_t output_len);

    static void scrypt_romix(uint8_t* B, uint32_t r, uint32_t N, uint8_t* V);
    static void scrypt_blockmix(uint8_t* B, uint32_t r);
    static void salsa20_8(uint32_t* B);
};

} // namespace core
