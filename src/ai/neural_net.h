#ifndef AI_NEURAL_NET_H
#define AI_NEURAL_NET_H

#include <vector>
#include <cmath>
#include <random>

namespace ai {

/**
 * @brief Simple Feed-Forward Neural Network (MLP) for performance prediction.
 * Features: 1 Hidden Layer, Sigmoid Activation, Backpropagation.
 */
class NeuralNet {
public:
    NeuralNet(int inputSize, int hiddenSize, int outputSize);

    std::vector<double> predict(const std::vector<double>& inputs);
    void train(const std::vector<double>& inputs, const std::vector<double>& targets, double learningRate);

private:
    int inputSize, hiddenSize, outputSize;
    std::vector<std::vector<double>> weightsIH; // Input to Hidden
    std::vector<std::vector<double>> weightsHO; // Hidden to Output
    std::vector<double> biasH;
    std::vector<double> biasO;

    double sigmoid(double x) { return 1.0 / (1.0 + std::exp(-x)); }
    double sigmoidDerivative(double x) { return x * (1.0 - x); }
};

} // namespace ai

#endif // AI_NEURAL_NET_H
