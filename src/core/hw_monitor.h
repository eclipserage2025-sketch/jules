#pragma once
#include <vector>

namespace core {

struct HardwareStats {
    float gpu_temp;
    float gpu_fan_speed;
    float cpu_temp;
    float power_usage;
};

/**
 * @brief Monitor hardware using Windows ADL (AMD Display Library)
 */
class HardwareMonitor {
public:
    HardwareMonitor();
    ~HardwareMonitor();

    bool initialize();
    HardwareStats getStats();

private:
#ifdef _WIN32
    // Windows ADL Specific pointers
    // void* adl_context;
#endif
};

} // namespace core
