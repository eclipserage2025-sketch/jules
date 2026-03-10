#include "ai/neural_net.h"
#include <iostream>
#include <vector>
#include <cassert>

void test_neural_prediction() {
    std::cout << "[Test] Running Neural Network prediction test..." << std::endl;
    ai::NeuralNet net(2, 4, 1);

    std::vector<double> inputs = {0.5, 0.5};
    auto prediction = net.predict(inputs);

    assert(prediction.size() == 1);
    assert(prediction[0] >= 0.0 && prediction[0] <= 1.0);
    std::cout << "[SUCCESS] Neural prediction within valid range." << std::endl;
}

void test_neural_training() {
    std::cout << "[Test] Running Neural Network training test..." << std::endl;
    ai::NeuralNet net(1, 4, 1);

    std::vector<double> input = {0.8};
    std::vector<double> target = {0.2};

    double initial_error = std::abs(net.predict(input)[0] - target[0]);

    // Train for 1000 epochs
    for(int i=0; i<1000; ++i) {
        net.train(input, target, 0.5);
    }

    double final_error = std::abs(net.predict(input)[0] - target[0]);
    std::cout << "[Test] Initial error: " << initial_error << ", Final error: " << final_error << std::endl;

    assert(final_error < initial_error);
    std::cout << "[SUCCESS] Neural Network converged during training." << std::endl;
}

int main() {
    test_neural_prediction();
    test_neural_training();
    return 0;
}
