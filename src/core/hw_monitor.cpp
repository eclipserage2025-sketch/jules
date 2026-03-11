#include "hw_monitor.h"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
// #include "adl_sdk.h" // Assuming ADL SDK header is available on Windows build machine
#endif

namespace core {

HardwareMonitor::HardwareMonitor() {}
HardwareMonitor::~HardwareMonitor() {}

bool HardwareMonitor::initialize() {
#ifdef _WIN32
    std::cout << "[Hardware] Initializing ADL (AMD Display Library) on Windows..." << std::endl;
    // ADL_Main_Control_Create, etc.
    return true;
#else
    std::cout << "[Hardware] Monitoring limited to simulated stats on non-Windows platform." << std::endl;
    return true;
#endif
}

HardwareStats HardwareMonitor::getStats() {
    HardwareStats stats;
#ifdef _WIN32
    // Real Windows implementation would call ADL_Overdrive5_Temperature_Get
    stats.gpu_temp = 65.0f; // Placeholder
    stats.gpu_fan_speed = 45.0f; // Placeholder
    stats.cpu_temp = 55.0f; // Placeholder
    stats.power_usage = 120.0f; // Placeholder
#else
    stats.gpu_temp = 45.0f + (rand() % 10);
    stats.gpu_fan_speed = 30.0f;
    stats.cpu_temp = 40.0f;
    stats.power_usage = 80.0f;
#endif
    return stats;
}

} // namespace core
