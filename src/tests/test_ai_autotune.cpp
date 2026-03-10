#include "ai/learning_engine.h"
#include <iostream>
#include <cassert>
#include <vector>
#include <thread>
#include <chrono>

void test_autotune_statemachine() {
    std::cout << "[Test] Running AI Auto-Tuning State Machine test..." << std::endl;
    ai::LearningEngine engine;

    assert(!engine.isTuning());
    engine.startAutoTuning();
    assert(engine.isTuning());
    assert(engine.getTuningState() == ai::LearningEngine::TuningState::Probing);

    float intensity;
    int threads;
    engine.getNextTuningProbe(intensity, threads);
    std::cout << "[Test] First probe configuration: Intensity=" << intensity << ", Threads=" << threads << std::endl;

    // Simulate hashrate report
    engine.updateTuningProgress(100.0f);
    assert(engine.getTuningState() == ai::LearningEngine::TuningState::Stabilizing);

    std::cout << "[SUCCESS] Auto-Tuning state machine logic verified." << std::endl;
}

int main() {
    test_autotune_statemachine();
    return 0;
}
