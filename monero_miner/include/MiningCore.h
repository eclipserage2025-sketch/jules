#ifndef MINING_CORE_H
#define MINING_CORE_H

#include <vector>
#include <thread>
#include <atomic>
#include <randomx.h>

class MiningCore {
public:
    MiningCore();
    ~MiningCore();

    bool initialize(const std::vector<uint8_t>& seed);
    void start(int threadCount, const std::vector<uint8_t>& blockHeader);
    void stop();
    void updateThreadCount(int newCount);
    double getHashrate();

private:
    void workerThread(int threadId);

    randomx_flags flags;
    randomx_cache* cache;
    randomx_dataset* dataset;
    std::atomic<bool> running;
    std::vector<std::thread> threads;
    std::atomic<uint64_t> totalHashes;
    std::vector<uint8_t> currentBlockHeader;

    // Thread management
    std::atomic<int> targetThreads;
};

#endif
