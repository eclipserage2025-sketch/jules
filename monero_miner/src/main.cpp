#include <iostream>
#include <string>
#include <vector>
#include "SystemProfiler.h"
#include "MiningCore.h"
#include "StratumClient.h"
#include "ThreadManager.h"
#include "MinerGUI.h"
#include "AIPredictor.h"
#include "Optimizations.h"

int main(int argc, char* argv[]) {
    // 1. Initial Optimizations
    Optimizations::enableLargePages();
    Optimizations::applyMSRTweaks();

    // 2. Profile System
    CPUInfo cpu = SystemProfiler::getCPUInfo();
    SystemProfiler::printCPUInfo(cpu);

    // 3. Initialize AI Predictor (ONNX)
    AIPredictor aiPredictor;
    aiPredictor.loadModel("models/hashrate_model.onnx");

    // 4. Setup GUI
    MinerGUI gui;
    if (!gui.initialize()) {
        std::cerr << "[Main] Failed to initialize GUI." << std::endl;
        return -1;
    }

    // 5. Setup Mining Components
    ThreadManager threadMgr(cpu);
    int activeThreads = threadMgr.getInitialThreadCount();

    MiningCore core;
    std::string poolUrl = "https://xmr-eu1.nanopool.org/api/v1/login";
    std::string wallet = "4BRLpszC0jY4H9P9657bH5Y9Y66Y966Y966Y966Y966Y966Y966Y966Y966Y966";
    StratumClient client(poolUrl, wallet);

    std::string currentStatus = "Initializing...";
    float predictedHashrate = aiPredictor.predictHashrate(cpu.physicalCores, cpu.l3CacheKB, activeThreads);

    client.setJobCallback([&](const JobData& job) {
        currentStatus = "Mining Active (Job: " + job.jobId + ")";
        std::vector<uint8_t> seed(32, 0);
        if (core.initialize(seed)) {
            core.start(activeThreads, seed);
        }
    });

    if (!client.connect()) {
        currentStatus = "Connection Error (Check Pool URL/SSL)";
    }

    // 6. Main GUI Loop
    while (!gui.shouldClose()) {
        double hashrate = core.getHashrate();

        // AI Dynamic Adjustment logic (simplified for GUI frequency)
        static auto lastAIUpdate = std::chrono::steady_clock::now();
        if (std::chrono::steady_clock::now() - lastAIUpdate > std::chrono::seconds(10)) {
            int suggested = threadMgr.suggestOptimalThreads(hashrate);
            if (suggested != activeThreads) {
                activeThreads = suggested;
                core.updateThreadCount(activeThreads);
                predictedHashrate = aiPredictor.predictHashrate(cpu.physicalCores, cpu.l3CacheKB, activeThreads);
            }
            lastAIUpdate = std::chrono::steady_clock::now();
        }

        gui.render(hashrate, activeThreads, currentStatus, predictedHashrate);
    }

    core.stop();
    return 0;
}
