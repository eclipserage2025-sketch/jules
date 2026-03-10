#pragma once
#include <vector>
#include <map>
#include <string>
#include <memory>
#include <chrono>

namespace ai {

/**
 * @brief Simple AI Learning architecture for performance and network difficulty optimization
 */
class LearningEngine {
public:
    LearningEngine();
    ~LearningEngine();

    // Hardware Stats & Optimization
    void addDataPoint(float intensity, int threads, float hashrate, float temp);
    void predictOptimalSettings(float target_temp, float current_difficulty, float& optimal_intensity, int& optimal_threads);

    // Network Difficulty Tracking
    void addDifficultyDataPoint(double difficulty);
    double predictNextDifficulty();

    // Model I/O
    bool loadModel(const std::string& model_path);

private:
    struct DataPoint {
        float intensity;
        int threads;
        float hashrate;
        float temperature;
        std::chrono::steady_clock::time_point timestamp;
    };

    struct DifficultyPoint {
        double difficulty;
        std::chrono::system_clock::time_point timestamp;
    };

    std::vector<DataPoint> history;
    std::vector<DifficultyPoint> diff_history;

    // Heuristic difficulty prediction (e.g., Simple Moving Average + Trend Factor)
    double calculateDifficultyTrend();
};

} // namespace ai
