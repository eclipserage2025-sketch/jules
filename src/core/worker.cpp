#include "worker.h"
#include "scrypt.h"
#include <chrono>
#include <iostream>

namespace core {

void CpuWorker::start() {
    running = true;
    for (int i = 0; i < num_threads; ++i) {
        thread_pool.emplace_back(&CpuWorker::mineLoop, this);
    }
}

void CpuWorker::stop() {
    running = false;
    for (auto& t : thread_pool) {
        if (t.joinable()) t.join();
    }
    thread_pool.clear();
}

double CpuWorker::getHashrate() const {
    static auto start_time = std::chrono::steady_clock::now();
    auto now = std::chrono::steady_clock::now();
    double seconds = std::chrono::duration<double>(now - start_time).count();
    return (seconds > 0) ? (hashes_done / seconds) : 0;
}

void CpuWorker::setWork(const std::vector<uint8_t>& work_data) {
    current_work = work_data;
}

void CpuWorker::reconfigure(float intensity, int threads) {
    std::cout << "[Worker] CPU Reconfiguring to " << threads << " threads." << std::endl;
    if (threads == num_threads) return;

    // Stop current threads
    stop();
    // Update thread count and restart
    num_threads = threads;
    start();
}

void CpuWorker::mineLoop() {
    uint32_t nonce = 0;
    while (running) {
        if (current_work.empty()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }
        auto hash = Scrypt::litecoin_scrypt(current_work);
        hashes_done++;
        nonce++;
    }
}

void GpuWorker::start() {
    running = true;
    gpu_thread = std::thread(&GpuWorker::gpuLoop, this);
}

void GpuWorker::stop() {
    running = false;
    if (gpu_thread.joinable()) gpu_thread.join();
}

double GpuWorker::getHashrate() const {
    return hashes_done.load() * 1024;
}

void GpuWorker::setWork(const std::vector<uint8_t>& work_data) {}

void GpuWorker::reconfigure(float new_intensity, int threads) {
    std::cout << "[Worker] GPU Reconfiguring to intensity " << new_intensity << std::endl;
    intensity = (int)new_intensity;
    // In a real OpenCL implementation, this would update the global work size
}

void GpuWorker::gpuLoop() {
    while (running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        hashes_done += (1 << intensity.load());
    }
}

} // namespace core
