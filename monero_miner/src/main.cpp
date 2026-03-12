#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <atomic>
#include <signal.h>
#include "SystemProfiler.h"
#include "MiningCore.h"
#include "StratumClient.h"
#include "ThreadManager.h"

std::atomic<bool> keepRunning(true);

void signalHandler(int signum) {
    std::cout << "\n[Main] Shutdown signal received (" << signum << "). Stopping miner..." << std::endl;
    keepRunning = false;
}

void printBanner() {
    std::cout << "========================================" << std::endl;
    std::cout << "   AI MONERO MINER (RandomX) v1.0       " << std::endl;
    std::cout << "      Built from scratch by Jules       " << std::endl;
    std::cout << "========================================" << std::endl;
}

int main(int argc, char* argv[]) {
    signal(SIGINT, signalHandler);
    printBanner();

    std::string poolUrl = "https://xmr-eu1.nanopool.org/api/v1/login"; // Simplified for skeleton
    std::string wallet = "4BRLpszC0jY4H9P9657bH5Y9Y66Y966Y966Y966Y966Y966Y966Y966Y966Y966"; // Example XMR address

    // 1. Profile System
    CPUInfo cpu = SystemProfiler::getCPUInfo();
    SystemProfiler::printCPUInfo(cpu);

    // 2. Initialize AI Thread Manager
    ThreadManager threadMgr(cpu);
    int startThreads = threadMgr.getInitialThreadCount();
    std::cout << "[AI] Initial thread count set to: " << startThreads << std::endl;

    // 3. Setup Mining Core and Stratum
    MiningCore core;
    StratumClient client(poolUrl, wallet);

    client.setJobCallback([&](const JobData& job) {
        std::cout << "[Main] Received new job: " << job.jobId << std::endl;

        // In a real miner, we'd convert job.blob (hex) to binary seed for RandomX
        std::vector<uint8_t> seed(32, 0); // Placeholder seed
        if (core.initialize(seed)) {
            core.start(startThreads, seed); // Using seed as placeholder for header
        }
    });

    // 4. Connect to Pool
    if (!client.connect()) {
        std::cerr << "[Main] FATAL: Could not connect to pool." << std::endl;
        // In this skeleton, we'll continue to show the loop even if connection fails
    }

    // 5. Main Monitoring Loop
    while (keepRunning) {
        std::this_thread::sleep_for(std::chrono::seconds(5));

        double hashrate = core.getHashrate();
        std::cout << "[Status] Hashrate: " << hashrate << " H/s | Threads: " << startThreads << std::endl;

        // AI Dynamic Adjustment
        int newThreads = threadMgr.suggestOptimalThreads(hashrate);
        if (newThreads != startThreads) {
            std::cout << "[Main] AI updating thread count: " << startThreads << " -> " << newThreads << std::endl;
            startThreads = newThreads;
            core.updateThreadCount(newThreads);
        }
    }

    core.stop();
    std::cout << "[Main] Miner stopped. Happy mining!" << std::endl;

    return 0;
}
