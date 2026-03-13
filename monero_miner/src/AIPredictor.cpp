#include "AIPredictor.h"
#include <iostream>
#include <numeric>

AIPredictor::AIPredictor() : env(ORT_LOGGING_LEVEL_WARNING, "AIPredictor"), session(nullptr),
    memoryInfo(Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault)) {}

AIPredictor::~AIPredictor() {
    if (session) delete session;
}

bool AIPredictor::loadModel(const std::string& modelPath) {
    try {
        Ort::SessionOptions sessionOptions;
        sessionOptions.SetIntraOpNumThreads(1);
        sessionOptions.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_EXTENDED);

        session = new Ort::Session(env, modelPath.c_str(), sessionOptions);
        std::cout << "[AI] ONNX Model loaded successfully: " << modelPath << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "[AI] Error loading model: " << e.what() << std::endl;
        return false;
    }
}

float AIPredictor::predictHashrate(int cores, long long l3CacheKB, int threads) {
    if (!session) {
        // Fallback simple model if no ONNX model loaded
        return (float)(threads * 50.0); // Rough estimate: 50 H/s per thread
    }

    // Input data for the model: [cores, l3CacheKB, threads]
    std::vector<float> inputTensorValues = {(float)cores, (float)l3CacheKB, (float)threads};
    std::vector<int64_t> inputDims = {1, 3};

    Ort::Value inputTensor = Ort::Value::CreateTensor<float>(
        memoryInfo, inputTensorValues.data(), inputTensorValues.size(), inputDims.data(), inputDims.size());

    const char* inputNames[] = {"input"};
    const char* outputNames[] = {"output"};

    try {
        auto outputTensors = session->Run(Ort::RunOptions{nullptr}, inputNames, &inputTensor, 1, outputNames, 1);
        float* outputValues = outputTensors.front().GetTensorMutableData<float>();
        return outputValues[0];
    } catch (const std::exception& e) {
        std::cerr << "[AI] Prediction error: " << e.what() << std::endl;
        return 0.0f;
    }
}
