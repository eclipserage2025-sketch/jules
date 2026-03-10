#include "neural_net.h"

namespace ai {

NeuralNet::NeuralNet(int in, int hid, int out)
    : inputSize(in), hiddenSize(hid), outputSize(out),
      weightsIH(in, std::vector<double>(hid)),
      weightsHO(hid, std::vector<double>(out)),
      biasH(hid), biasO(out) {

    std::default_random_engine gen;
    std::uniform_real_distribution<double> dist(-1.0, 1.0);

    for(auto& row : weightsIH) for(double& w : row) w = dist(gen);
    for(auto& row : weightsHO) for(double& w : row) w = dist(gen);
    for(double& b : biasH) b = dist(gen);
    for(double& b : biasO) b = dist(gen);
}

std::vector<double> NeuralNet::predict(const std::vector<double>& inputs) {
    std::vector<double> hidden(hiddenSize);
    for(int j=0; j<hiddenSize; ++j) {
        double sum = biasH[j];
        for(int i=0; i<inputSize; ++i) sum += inputs[i] * weightsIH[i][j];
        hidden[j] = sigmoid(sum);
    }

    std::vector<double> outputs(outputSize);
    for(int k=0; k<outputSize; ++k) {
        double sum = biasO[k];
        for(int j=0; j<hiddenSize; ++j) sum += hidden[j] * weightsHO[j][k];
        outputs[k] = sigmoid(sum);
    }
    return outputs;
}

void NeuralNet::train(const std::vector<double>& inputs, const std::vector<double>& targets, double lr) {
    // 1. Forward Pass
    std::vector<double> hidden(hiddenSize);
    for(int j=0; j<hiddenSize; ++j) {
        double sum = biasH[j];
        for(int i=0; i<inputSize; ++i) sum += inputs[i] * weightsIH[i][j];
        hidden[j] = sigmoid(sum);
    }

    std::vector<double> outputs(outputSize);
    for(int k=0; k<outputSize; ++k) {
        double sum = biasO[k];
        for(int j=0; j<hiddenSize; ++j) sum += hidden[j] * weightsHO[j][k];
        outputs[k] = sigmoid(sum);
    }

    // 2. Output Error & Gradients
    std::vector<double> outputErrors(outputSize);
    for(int k=0; k<outputSize; ++k) outputErrors[k] = targets[k] - outputs[k];

    // 3. Hidden Error & Gradients
    std::vector<double> hiddenErrors(hiddenSize, 0);
    for(int j=0; j<hiddenSize; ++j) {
        for(int k=0; k<outputSize; ++k) hiddenErrors[j] += outputErrors[k] * weightsHO[j][k];
    }

    // 4. Update Weights HO
    for(int k=0; k<outputSize; ++k) {
        double gradient = outputErrors[k] * sigmoidDerivative(outputs[k]) * lr;
        for(int j=0; j<hiddenSize; ++j) weightsHO[j][k] += gradient * hidden[j];
        biasO[k] += gradient;
    }

    // 5. Update Weights IH
    for(int j=0; j<hiddenSize; ++j) {
        double gradient = hiddenErrors[j] * sigmoidDerivative(hidden[j]) * lr;
        for(int i=0; i<inputSize; ++i) weightsIH[i][j] += gradient * inputs[i];
        biasH[j] += gradient;
    }
}

} // namespace ai
