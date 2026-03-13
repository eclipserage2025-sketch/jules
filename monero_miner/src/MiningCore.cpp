#include "MiningCore.h"
#include <iostream>
#include <chrono>
#include <cstring>

MiningCore::MiningCore() : cache(nullptr), dataset(nullptr), running(false), totalHashes(0), targetThreads(0) {
    flags = randomx_get_flags();
}

MiningCore::~MiningCore() {
    stop();
    if (dataset) randomx_release_dataset(dataset);
    if (cache) randomx_release_cache(cache);
}

bool MiningCore::initialize(const std::vector<uint8_t>& seed) {
    std::cout << "[MiningCore] Initializing RandomX Cache..." << std::endl;
    cache = randomx_alloc_cache(flags);
    if (!cache) return false;
    randomx_init_cache(cache, seed.data(), seed.size());
    std::cout << "[MiningCore] Initialization complete (Light Mode)." << std::endl;
    return true;
}

void MiningCore::start(int threadCount, const std::vector<uint8_t>& blockHeader) {
    if (running) return;

    currentBlockHeader = blockHeader;
    targetThreads = threadCount;
    running = true;
    totalHashes = 0;

    std::cout << "[MiningCore] Starting " << threadCount << " worker threads..." << std::endl;
    for (int i = 0; i < threadCount; ++i) {
        threads.emplace_back(&MiningCore::workerThread, this, i);
    }
}

void MiningCore::stop() {
    running = false;
    for (auto& t : threads) {
        if (t.joinable()) t.join();
    }
    threads.clear();
}

void MiningCore::updateThreadCount(int newCount) {
    targetThreads = newCount;
}

void MiningCore::workerThread(int threadId) {
    randomx_vm* vm = randomx_create_vm(flags, cache, nullptr);
    if (!vm) {
        std::cerr << "[MiningCore] Failed to create VM for thread " << threadId << std::endl;
        return;
    }

    char hash[RANDOMX_HASH_SIZE];
    std::vector<uint8_t> localInput = currentBlockHeader;
    if (localInput.empty()) localInput.resize(32, 0);

    while (running) {
        if (threadId >= targetThreads) {
            break;
        }

        randomx_calculate_hash(vm, localInput.data(), localInput.size(), hash);
        totalHashes.fetch_add(1, std::memory_order_relaxed);
    }

    randomx_destroy_vm(vm);
}

double MiningCore::getHashrate() {
    static auto lastTime = std::chrono::steady_clock::now();
    static uint64_t lastHashes = 0;

    auto currentTime = std::chrono::steady_clock::now();
    uint64_t currentHashes = totalHashes.load();

    std::chrono::duration<double> elapsed = currentTime - lastTime;
    double hashrate = 0.0;
    if (elapsed.count() > 0.001) {
        hashrate = (double)(currentHashes - lastHashes) / elapsed.count();
    }

    lastTime = currentTime;
    lastHashes = currentHashes;

    return hashrate;
}
