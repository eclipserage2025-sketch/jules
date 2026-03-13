#include "ThreadManager.h"
#include <numeric>
#include <algorithm>
#include <iostream>

ThreadManager::ThreadManager(const CPUInfo& info) : cpuInfo(info) {
    int cacheBasedThreads = (info.l3CacheKB > 0) ? (info.l3CacheKB / 2048) : info.physicalCores;
    currentThreads = std::min(info.physicalCores, cacheBasedThreads);
    if (currentThreads < 1) currentThreads = 1;
}

int ThreadManager::getInitialThreadCount() {
    return currentThreads;
}

int ThreadManager::suggestOptimalThreads(double currentHashrate, double temperature) {
    hashrateHistory.push_back(currentHashrate);
    if (hashrateHistory.size() > historyLimit) {
        hashrateHistory.pop_front();
    }

    if (hashrateHistory.size() < 3) return currentThreads;

    // AI heuristic:
    // If hashrate is declining while thread count is high, we might be hitting thermal throttling or cache thrashing.
    // If hashrate is stable, try increasing by 1 to see if it improves (Hill Climbing).

    double avgHashrate = std::accumulate(hashrateHistory.begin(), hashrateHistory.end(), 0.0) / hashrateHistory.size();

    // Check for "stagnation" or "improvement"
    if (currentHashrate > avgHashrate * 1.02) {
        // Improvement! Maybe we can go higher if we have cores left
        if (currentThreads < cpuInfo.logicalCores) {
            std::cout << "[AI] Hashrate improved, testing +1 thread..." << std::endl;
            currentThreads++;
        }
    } else if (currentHashrate < avgHashrate * 0.98) {
        // Significant drop! Scale back to protect stability
        if (currentThreads > 1) {
            std::cout << "[AI] Hashrate dropped significantly, scaling back -1 thread..." << std::endl;
            currentThreads--;
        }
    }

    // Thermal safety (placeholder)
    if (temperature > 85.0 && currentThreads > 1) {
        std::cout << "[AI] Temperature high (" << temperature << "C), reducing threads for cooling..." << std::endl;
        currentThreads--;
    }

    return currentThreads;
}
