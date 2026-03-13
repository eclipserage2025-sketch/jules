#ifndef SYSTEM_PROFILER_H
#define SYSTEM_PROFILER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>

struct CPUInfo {
    int logicalCores;
    int physicalCores;
    long long l3CacheKB;
};

class SystemProfiler {
public:
    static CPUInfo getCPUInfo();
    static void printCPUInfo(const CPUInfo& info);
};

#endif
