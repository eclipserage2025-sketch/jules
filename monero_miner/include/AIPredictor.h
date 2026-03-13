#ifndef AI_PREDICTOR_H
#define AI_PREDICTOR_H

#include <string>
#include <vector>
#include <onnxruntime_cxx_api.h>

class AIPredictor {
public:
    AIPredictor();
    ~AIPredictor();

    bool loadModel(const std::string& modelPath);
    float predictHashrate(int cores, long long l3CacheKB, int threads);

private:
    Ort::Env env;
    Ort::Session* session;
    Ort::MemoryInfo memoryInfo;
};

#endif
