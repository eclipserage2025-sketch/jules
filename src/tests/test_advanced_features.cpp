#include "ai/learning_engine.h"
#include "core/pool_manager.h"
#include <iostream>
#include <cassert>

int main() {
    std::cout << "[Test] Verifying AI Learning Engine v3.0 logic..." << std::endl;
    ai::LearningEngine engine;
    engine.addDataPoint(10.0, 4, 1000.0, 50.0);
    float pred = engine.neuralPredictHashrate(10.0, 4);
    assert(pred > 0);
    std::cout << "[SUCCESS] Neural prediction integrated into LearningEngine." << std::endl;
    return 0;
}
