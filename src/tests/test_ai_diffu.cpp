#include "ai/learning_engine.h"
#include <iostream>
#include <cassert>
#include <vector>

void test_difficulty_prediction() {
    std::cout << "[Test] Running AI Difficulty Prediction test..." << std::endl;
    ai::LearningEngine engine;

    // Feed it an upward trend
    engine.addDifficultyDataPoint(10.0);
    engine.addDifficultyDataPoint(11.0);
    engine.addDifficultyDataPoint(12.0);
    engine.addDifficultyDataPoint(13.0);
    engine.addDifficultyDataPoint(14.0);

    double prediction = engine.predictNextDifficulty();
    std::cout << "[Test] Predicted next difficulty: " << prediction << " (Expected ~15.0)" << std::endl;

    assert(prediction > 14.0);
    assert(prediction < 16.0);
    std::cout << "[SUCCESS] Difficulty prediction verified." << std::endl;
}

void test_efficiency_strategy() {
    std::cout << "[Test] Running AI Efficiency Strategy test..." << std::endl;

    // Case 1: Constant/Falling difficulty (prioritize pure hashrate)
    {
        ai::LearningEngine engine;
        engine.addDataPoint(10.0f, 4, 1000.0f, 60.0f);
        engine.addDataPoint(2.0f, 4, 400.0f, 45.0f);

        engine.addDifficultyDataPoint(20.0);
        engine.addDifficultyDataPoint(20.0);

        float opt_intensity;
        int opt_threads;
        engine.predictOptimalSettings(70.0f, 20.0f, opt_intensity, opt_threads);
        std::cout << "[Test] Predicted intensity for constant difficulty: " << opt_intensity << " (Expected 10.0)" << std::endl;
        assert(opt_intensity == 10.0f);
    }

    // Case 2: Sharply rising difficulty (prioritize efficiency)
    {
        ai::LearningEngine engine;
        // Intensity 10: Hashrate 1000, Efficiency Metric = 1000 / (10+1) = 90.9
        // Intensity 2:  Hashrate 400,  Efficiency Metric = 400 / (2+1) = 133.3 (High efficiency)
        engine.addDataPoint(10.0f, 4, 1000.0f, 60.0f);
        engine.addDataPoint(2.0f, 4, 400.0f, 45.0f);

        engine.addDifficultyDataPoint(10.0);
        engine.addDifficultyDataPoint(50.0); // Steep rise

        float opt_intensity;
        int opt_threads;
        // Prediction for index 2: slope = 40, intercept = 10 -> 40*2 + 10 = 90
        // 90 > 50 * 1.05 (52.5) -> prioritize_efficiency = true
        engine.predictOptimalSettings(70.0f, 50.0f, opt_intensity, opt_threads);
        std::cout << "[Test] Predicted intensity for rising difficulty: " << opt_intensity << " (Expected 2.0)" << std::endl;
        assert(opt_intensity == 2.0f);
    }

    std::cout << "[SUCCESS] AI Efficiency Strategy verified." << std::endl;
}

int main() {
    test_difficulty_prediction();
    test_efficiency_strategy();
    return 0;
}
