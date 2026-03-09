#pragma once
#include <thread>
#include <atomic>
#include <vector>
#include <string>

namespace core {

class Worker {
public:
    virtual ~Worker() = default;
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual double getHashrate() const = 0;
    virtual void setWork(const std::vector<uint8_t>& work_data) = 0;

protected:
    std::atomic<bool> running{false};
    std::atomic<uint64_t> hashes_done{0};
};

class CpuWorker : public Worker {
public:
    CpuWorker(int threads) : num_threads(threads) {}
    void start() override;
    void stop() override;
    double getHashrate() const override;
    void setWork(const std::vector<uint8_t>& work_data) override;

private:
    int num_threads;
    std::vector<std::thread> thread_pool;
    std::vector<uint8_t> current_work;
    void mineLoop();
};

class GpuWorker : public Worker {
public:
    GpuWorker(int device_id, int intensity) : device_id(device_id), intensity(intensity) {}
    void start() override;
    void stop() override;
    double getHashrate() const override;
    void setWork(const std::vector<uint8_t>& work_data) override;

private:
    int device_id;
    int intensity;
    std::thread gpu_thread;
    void gpuLoop();
};

} // namespace core
