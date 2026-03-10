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
    enum class TuningState { Idle, Probing, Stabilizing, Analyzed };

    LearningEngine();
    ~LearningEngine();

    // Hardware Stats & Optimization
    void addDataPoint(float intensity, int threads, float hashrate, float temp);
    void predictOptimalSettings(float target_temp, float current_difficulty, float& optimal_intensity, int& optimal_threads);

    // Active Auto-Tuning
    void startAutoTuning();
    bool isTuning() const { return tuning_state != TuningState::Idle; }
    void getNextTuningProbe(float& intensity, int& threads);
    void updateTuningProgress(float hashrate);
    TuningState getTuningState() const { return tuning_state; }

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

    // Auto-Tuning Internal State
    TuningState tuning_state = TuningState::Idle;
    float current_probe_intensity = 1.0f;
    int current_probe_threads = 1;
    int probe_count = 0;
    const int max_probes = 10;
    std::chrono::steady_clock::time_point last_probe_time;

    double calculateDifficultyTrend();
};

} // namespace ai
