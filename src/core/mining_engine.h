#ifndef CORE_MINING_ENGINE_H
#define CORE_MINING_ENGINE_H

#include <vector>
#include <cstdint>
#include <string>

namespace core {

/**
 * @brief Base interface for different mining algorithms.
 */
class MiningEngine {
public:
    virtual ~MiningEngine() = default;
    virtual std::vector<uint8_t> hash(const std::vector<uint8_t>& data) = 0;
    virtual std::string getName() const = 0;
};

class ScryptEngine : public MiningEngine {
public:
    std::vector<uint8_t> hash(const std::vector<uint8_t>& data) override;
    std::string getName() const override { return "Scrypt"; }
};

/**
 * @brief Placeholder for SHA256 algorithm to demonstrate multi-algo switching.
 */
class Sha256Engine : public MiningEngine {
public:
    std::vector<uint8_t> hash(const std::vector<uint8_t>& data) override;
    std::string getName() const override { return "SHA256"; }
};

} // namespace core

#endif
