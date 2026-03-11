#include "pool_manager.h"
#include <algorithm>
#include <iostream>

namespace core {

PoolManager::PoolManager() {}

void PoolManager::addPool(const std::string& url, int port, const std::string& user, const std::string& pass, int priority) {
    if (url.empty() || port <= 0) {
        std::cerr << "[CORE] Warning: Invalid pool configuration ignored." << std::endl;
        return;
    }
    pools.push_back({url, port, user, pass, priority, true});
}

PoolConfig* PoolManager::getBestPool() {
    PoolConfig* best = nullptr;
    for (auto& p : pools) {
        if (p.is_healthy) {
            if (!best || p.priority > best->priority) {
                best = &p;
            }
        }
    }
    return best;
}

void PoolManager::markPoolStatus(const std::string& url, bool healthy) {
    for (auto& p : pools) {
        if (p.url == url) {
            p.is_healthy = healthy;
            if (!healthy) std::cout << "[CORE] Pool failover triggered for: " << url << std::endl;
            break;
        }
    }
}

} // namespace core
