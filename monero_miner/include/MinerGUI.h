#ifndef MINER_GUI_H
#define MINER_GUI_H

#include <string>
#include <vector>
#include <deque>

struct GLFWwindow;

class MinerGUI {
public:
    MinerGUI();
    ~MinerGUI();

    bool initialize();
    void render(double hashrate, int threads, const std::string& status, float predictedHashrate);
    bool shouldClose();

private:
    GLFWwindow* window;
    std::deque<float> hashrateHistory;
    const size_t historyLimit = 60;
};

#endif
