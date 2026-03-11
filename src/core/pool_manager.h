#pragma once
#include <string>
#include <vector>
#include <memory>

namespace core {

struct PoolConfig {
    std::string url;
    int port;
    std::string user;
    std::string pass;
    int priority;
    bool is_healthy;
};

class PoolManager {
public:
    PoolManager();
    void addPool(const std::string& url, int port, const std::string& user, const std::string& pass, int priority = 0);
    PoolConfig* getBestPool();
    void markPoolStatus(const std::string& url, bool healthy);

    std::vector<PoolConfig>& getAllPools() { return pools; }

private:
    std::vector<PoolConfig> pools;
};

} // namespace core
