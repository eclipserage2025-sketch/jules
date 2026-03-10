#include "learning_engine.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>

namespace ai {

LearningEngine::LearningEngine() : tuning_state(TuningState::Idle), hardware_risk_alert(false) {}
LearningEngine::~LearningEngine() {}

void LearningEngine::addDataPoint(float intensity, int threads, float hashrate, float temp) {
    history.push_back({intensity, threads, hashrate, temp, std::chrono::steady_clock::now()});
    if (history.size() > 1000) history.erase(history.begin());
}

void LearningEngine::addDifficultyDataPoint(double difficulty) {
    diff_history.push_back({difficulty, std::chrono::system_clock::now()});
    if (diff_history.size() > 100) diff_history.erase(diff_history.begin());
}

double LearningEngine::predictNextDifficulty() {
    if (diff_history.size() < 2) return (diff_history.empty() ? 1.0 : diff_history.back().difficulty);
    size_t n = diff_history.size();
    double x_sum = 0, y_sum = 0, xy_sum = 0, x2_sum = 0;
    for (size_t i = 0; i < n; ++i) {
        double x = (double)i;
        double y = diff_history[i].difficulty;
        x_sum += x; y_sum += y; xy_sum += x * y; x2_sum += x * x;
    }
    double slope = (n * xy_sum - x_sum * y_sum) / (n * x2_sum - x_sum * x_sum);
    double intercept = (y_sum - slope * x_sum) / n;
    return std::max(1.0, slope * (double)n + intercept);
}

void LearningEngine::updateCoinProfitability(const std::string& coin, double difficulty, double price) {
    // Profitability Score = Price / Difficulty (Simplified)
    double score = price / (difficulty + 0.001);
    coin_profitability_map[coin] = score;
    std::cout << "[AI] Updated profitability for " << coin << ": " << score << std::endl;
}

std::string LearningEngine::getMostProfitableCoin() {
    std::string best_coin = "LTC";
    double best_score = -1.0;
    for (auto const& [coin, score] : coin_profitability_map) {
        if (score > best_score) {
            best_score = score;
            best_coin = coin;
        }
    }
    return best_coin;
}

void LearningEngine::updateHardwareHealth(float temp, float fan_speed) {
    temp_history.push_back(temp);
    if (temp_history.size() > 50) temp_history.erase(temp_history.begin());

    // Predictive Maintenance: Check for upward thermal drift (rising temp with constant fan)
    if (temp_history.size() >= 10) {
        float recent_avg = std::accumulate(temp_history.end() - 5, temp_history.end(), 0.0f) / 5.0f;
        float older_avg = std::accumulate(temp_history.begin(), temp_history.begin() + 5, 0.0f) / 5.0f;

        if (recent_avg > older_avg + 5.0f && fan_speed > 80.0f) {
            hardware_risk_alert = true;
            std::cout << "[AI] ALERT: Thermal drift detected! Throttling hardware..." << std::endl;
        } else if (temp < 60.0f) {
            hardware_risk_alert = false;
        }
    }
}

void LearningEngine::predictOptimalSettings(float target_temp, float current_difficulty, float& optimal_intensity, int& optimal_threads) {
    double predicted_diff = predictNextDifficulty();
    bool prioritize_efficiency = (predicted_diff > current_difficulty * 1.05) || hardware_risk_alert;

    if (history.empty()) {
        optimal_intensity = prioritize_efficiency ? 5.0f : 8.0f;
        optimal_threads = 4;
        return;
    }

    float max_metric = -1.0f;
    const DataPoint* best_match = nullptr;
    for (const auto& dp : history) {
        if (dp.temperature <= (hardware_risk_alert ? 65.0f : target_temp)) {
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
        optimal_intensity = std::max(1.0f, history.back().intensity - 1.0f);
        optimal_threads = std::max(1, history.back().threads - 1);
    }
}

void LearningEngine::startAutoTuning() {
    tuning_state = TuningState::Probing;
    probe_count = 0;
    current_probe_intensity = 1.0f;
    current_probe_threads = 1;
    last_probe_time = std::chrono::steady_clock::now();
}

void LearningEngine::getNextTuningProbe(float& intensity, int& threads) {
    intensity = current_probe_intensity;
    threads = current_probe_threads;
}

void LearningEngine::updateTuningProgress(float hashrate) {
    if (tuning_state != TuningState::Probing) return;
    auto now = std::chrono::steady_clock::now();
    if (std::chrono::duration_cast<std::chrono::seconds>(now - last_probe_time).count() < 10) {
        tuning_state = TuningState::Stabilizing;
        return;
    }
    addDataPoint(current_probe_intensity, current_probe_threads, hashrate, 55.0f);
    probe_count++;
    if (probe_count >= max_probes) {
        tuning_state = TuningState::Idle;
    } else {
        current_probe_intensity += 2.0f;
        current_probe_threads = (current_probe_threads % 8) + 1;
        last_probe_time = now;
        tuning_state = TuningState::Probing;
    }
}

bool LearningEngine::loadModel(const std::string& model_path) { return true; }

} // namespace ai
