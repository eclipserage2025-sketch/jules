#pragma once
#include <vector>
#include <map>
#include <string>
#include <memory>

namespace ai {

/**
 * @brief Simple AI Learning architecture that collects data and optimizes settings
 */
class LearningEngine {
public:
    LearningEngine();
    ~LearningEngine();

    // Store a data point (Intensity/Threads, Hashrate, Temp)
    void addDataPoint(float intensity, int threads, float hashrate, float temp);

    // Predict the optimal settings based on collected data and current system state
    void predictOptimalSettings(float target_temp, float& optimal_intensity, int& optimal_threads);

    // Load or initialize the ONNX model
    bool loadModel(const std::string& model_path);

private:
    struct DataPoint {
        float intensity;
        int threads;
        float hashrate;
        float temperature;
    };

    std::vector<DataPoint> history;

    // ONNX Runtime session and environment placeholder (actual implementation in .cpp)
    // std::unique_ptr<Ort::Session> session;
    // std::unique_ptr<Ort::Env> env;
};

} // namespace ai
