#include "learning_engine.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>

namespace ai {

LearningEngine::LearningEngine() {}
LearningEngine::~LearningEngine() {}

void LearningEngine::addDataPoint(float intensity, int threads, float hashrate, float temp) {
    history.push_back({intensity, threads, hashrate, temp, std::chrono::steady_clock::now()});
    if (history.size() > 1000) history.erase(history.begin());
    std::cout << "[AI] Performance point: Intensity=" << intensity << ", Hashrate=" << hashrate << std::endl;
}

void LearningEngine::addDifficultyDataPoint(double difficulty) {
    diff_history.push_back({difficulty, std::chrono::system_clock::now()});
    if (diff_history.size() > 100) diff_history.erase(diff_history.begin());
    std::cout << "[AI] Difficulty update recorded: " << difficulty << std::endl;
}

double LearningEngine::predictNextDifficulty() {
    if (diff_history.size() < 2) return (diff_history.empty() ? 1.0 : diff_history.back().difficulty);

    // AI Prediction: Simple linear regression for difficulty trend
    size_t n = diff_history.size();
    double x_sum = 0, y_sum = 0, xy_sum = 0, x2_sum = 0;

    for (size_t i = 0; i < n; ++i) {
        double x = (double)i;
        double y = diff_history[i].difficulty;
        x_sum += x;
        y_sum += y;
        xy_sum += x * y;
        x2_sum += x * x;
    }

    double slope = (n * xy_sum - x_sum * y_sum) / (n * x2_sum - x_sum * x_sum);
    double intercept = (y_sum - slope * x_sum) / n;

    double prediction = slope * (double)n + intercept;
    return std::max(1.0, prediction); // Ensure difficulty is positive
}

void LearningEngine::predictOptimalSettings(float target_temp, float current_difficulty, float& optimal_intensity, int& optimal_threads) {
    // If difficulty is rising sharply (AI prediction), we might want to prioritize power efficiency.
    // If it's falling, we maximize hashrate.
    double predicted_diff = predictNextDifficulty();
    bool prioritize_efficiency = (predicted_diff > current_difficulty * 1.05);

    if (history.empty()) {
        optimal_intensity = prioritize_efficiency ? 6.0f : 8.0f;
        optimal_threads = 4;
        return;
    }

    // Heuristic: Filter history for safe temperatures, then pick based on strategy
    float max_metric = -1.0f;
    const DataPoint* best_match = nullptr;

    for (const auto& dp : history) {
        if (dp.temperature <= target_temp) {
            // Metric = Hashrate / (1 + (prioritize_efficiency ? intensity : 0))
            float metric = dp.hashrate / (prioritize_efficiency ? (dp.intensity + 1.0f) : 1.0f);
            if (metric > max_metric) {
                max_metric = metric;
                best_match = &dp;
            }
        }
    }

    if (best_match) {
        optimal_intensity = best_match->intensity;
        optimal_threads = best_match->threads;
    } else {
        // Fallback for thermal throttling
        optimal_intensity = std::max(1.0f, history.back().intensity - 1.0f);
        optimal_threads = std::max(1, history.back().threads - 1);
    }
}

bool LearningEngine::loadModel(const std::string& model_path) {
    return true;
}

} // namespace ai
