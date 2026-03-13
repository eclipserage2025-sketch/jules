#ifndef THREAD_MANAGER_H
#define THREAD_MANAGER_H

#include "SystemProfiler.h"
#include <vector>
#include <deque>

class ThreadManager {
public:
    ThreadManager(const CPUInfo& info);

    int getInitialThreadCount();
    int suggestOptimalThreads(double currentHashrate, double temperature = 0.0);

private:
    CPUInfo cpuInfo;
    std::deque<double> hashrateHistory;
    int currentThreads;
    const size_t historyLimit = 10;
};

#endif
