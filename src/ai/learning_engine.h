#ifndef AI_LEARNING_ENGINE_H
#define AI_LEARNING_ENGINE_H

#include <vector>
#include <map>
#include <string>
#include <memory>
#include <chrono>
#include <mutex>

namespace ai {

/**
 * @brief Thread-safe AI Learning Engine for performance optimization and maintenance.
 */
class LearningEngine {
public:
    enum class TuningState { Idle, Probing, Stabilizing, Analyzed };

    LearningEngine();
    ~LearningEngine();

    // Performance Data
    void addDataPoint(float intensity, int threads, float hashrate, float temp);
    void predictOptimalSettings(float target_temp, float current_difficulty, float& optimal_intensity, int& optimal_threads);

    // Auto-Tuning
    void startAutoTuning();
    bool isTuning() const;
    void getNextTuningProbe(float& intensity, int& threads) const;
    void updateTuningProgress(float hashrate);
    TuningState getTuningState() const;

    // Intelligence
    void addDifficultyDataPoint(double difficulty);
    double predictNextDifficulty();
    void updateCoinProfitability(const std::string& coin, double difficulty, double price);
    std::string getMostProfitableCoin();

    // Health
    void updateHardwareHealth(float temp, float fan_speed);
    bool isInstabilityPredicted() const;

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

    // Thread Safety
    mutable std::mutex data_mutex;

    // History
    std::vector<DataPoint> history;
    std::vector<DifficultyPoint> diff_history;
    std::map<std::string, double> coin_profitability_map;
    std::vector<float> temp_history;

    // State
    TuningState tuning_state = TuningState::Idle;
    float current_probe_intensity = 1.0f;
    int current_probe_threads = 1;
    int probe_count = 0;
    const int max_probes = 10;
    std::chrono::steady_clock::time_point last_probe_time;
    bool hardware_risk_alert = false;
};

} // namespace ai

#endif // AI_LEARNING_ENGINE_H
