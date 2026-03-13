#include "SystemProfiler.h"
#include <algorithm>
#include <unistd.h>

CPUInfo SystemProfiler::getCPUInfo() {
    CPUInfo info;
    info.logicalCores = std::thread::hardware_concurrency();
    info.physicalCores = info.logicalCores / 2; // Default assumption, improved below if possible
    info.l3CacheKB = 0;

#ifdef __linux__
    // Try to get L3 cache size from sysfs
    std::ifstream cacheFile("/sys/devices/system/cpu/cpu0/cache/index3/size");
    if (cacheFile.is_open()) {
        std::string sizeStr;
        cacheFile >> sizeStr;
        if (!sizeStr.empty()) {
            char unit = sizeStr.back();
            if (unit == 'K') {
                info.l3CacheKB = std::stoll(sizeStr.substr(0, sizeStr.size() - 1));
            } else if (unit == 'M') {
                info.l3CacheKB = std::stoll(sizeStr.substr(0, sizeStr.size() - 1)) * 1024;
            } else {
                info.l3CacheKB = std::stoll(sizeStr);
            }
        }
    }

    // Try to get physical cores from /proc/cpuinfo
    std::ifstream cpuinfo("/proc/cpuinfo");
    if (cpuinfo.is_open()) {
        std::string line;
        std::vector<int> coreIds;
        while (std::getline(cpuinfo, line)) {
            if (line.find("core id") != std::string::npos) {
                size_t colonPos = line.find(':');
                if (colonPos != std::string::npos) {
                    int id = std::stoi(line.substr(colonPos + 1));
                    if (std::find(coreIds.begin(), coreIds.end(), id) == coreIds.end()) {
                        coreIds.push_back(id);
                    }
                }
            }
        }
        if (!coreIds.empty()) {
            info.physicalCores = coreIds.size();
        }
    }
#endif

    return info;
}

void SystemProfiler::printCPUInfo(const CPUInfo& info) {
    std::cout << "[System] Logical Cores: " << info.logicalCores << std::endl;
    std::cout << "[System] Physical Cores: " << info.physicalCores << std::endl;
    std::cout << "[System] L3 Cache: " << info.l3CacheKB << " KB" << std::endl;
}
