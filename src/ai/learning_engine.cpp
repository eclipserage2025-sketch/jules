#include "learning_engine.h"
#include <algorithm>
#include <cmath>
#include <iostream>

namespace ai {

LearningEngine::LearningEngine() {
    // In a real environment, we'd initialize the ONNX Runtime session here.
}

LearningEngine::~LearningEngine() {
}

void LearningEngine::addDataPoint(float intensity, int threads, float hashrate, float temp) {
    history.push_back({intensity, threads, hashrate, temp});

    // Maintain a limited history for on-the-fly learning
    if (history.size() > 1000) {
        history.erase(history.begin());
    }

    std::cout << "[AI] Learning point added: Intensity=" << intensity
              << ", Threads=" << threads << ", Hashrate=" << hashrate
              << ", Temp=" << temp << std::endl;
}

void LearningEngine::predictOptimalSettings(float target_temp, float& optimal_intensity, int& optimal_threads) {
    if (history.empty()) {
        // Default safe values if no data exists
        optimal_intensity = 8.0f;
        optimal_threads = 4;
        return;
    }

    // Simple Linear Regression / Heuristic model for optimization
    // We want to maximize Hashrate while keeping Temp <= target_temp

    float max_hashrate = -1.0f;
    const DataPoint* best_match = nullptr;

    for (const auto& dp : history) {
        if (dp.temperature <= target_temp) {
            if (dp.hashrate > max_hashrate) {
                max_hashrate = dp.hashrate;
                best_match = &dp;
            }
        }
    }

    if (best_match) {
        optimal_intensity = best_match->intensity;
        optimal_threads = best_match->threads;
        std::cout << "[AI] Predicted optimal settings (Heuristic): Intensity=" << optimal_intensity
                  << ", Threads=" << optimal_threads << std::endl;
    } else {
        // If all points are too hot, reduce settings
        optimal_intensity = std::max(1.0f, history.back().intensity - 1.0f);
        optimal_threads = std::max(1, history.back().threads - 1);
        std::cout << "[AI] Thermal throttling predicted: Intensity=" << optimal_intensity
                  << ", Threads=" << optimal_threads << std::endl;
    }
}

bool LearningEngine::loadModel(const std::string& model_path) {
    // Load pre-trained ONNX model logic
    // SessionOptions session_options;
    // session = std::make_unique<Ort::Session>(*env, model_path.c_str(), session_options);
    return true; // Mocked for now
}

} // namespace ai
