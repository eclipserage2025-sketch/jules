#include "mining_engine.h"
#include "scrypt.h"
#include <openssl/sha.h>

namespace core {

std::vector<uint8_t> ScryptEngine::hash(const std::vector<uint8_t>& data) {
    return Scrypt::litecoin_scrypt(data);
}

std::vector<uint8_t> Sha256Engine::hash(const std::vector<uint8_t>& data) {
    std::vector<uint8_t> output(SHA256_DIGEST_LENGTH);
    SHA256(data.data(), data.size(), output.data());
    return output;
}

} // namespace core
