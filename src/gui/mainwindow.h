#pragma once
#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QTimer>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QProgressBar>
#include <QFormLayout>
#include <QTableWidget>
#include <memory>
#include "core/worker.h"
#include "core/pool_manager.h"
#include "core/stratum_client.h"
#include "ai/learning_engine.h"

namespace gui {

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onStartClicked();
    void onStopClicked();
    void onAutoTuneClicked();
    void updateStats();
    void runAI();

private:
    // UI elements
    QLabel* hashrateLabel;
    QLabel* tempLabel;
    QLabel* difficultyLabel;
    QLabel* aiPredictLabel;
    QLabel* profitLabel;
    QLabel* statusLabel;
    QLabel* healthLabel;
    QPushButton* startBtn;
    QPushButton* stopBtn;
    QPushButton* tuneBtn;
    QProgressBar* loadBar;
    QProgressBar* tuneBar;
    QTableWidget* poolTable;

    // Core Logic
    std::vector<std::unique_ptr<core::Worker>> workers;
    std::unique_ptr<ai::LearningEngine> aiEngine;
    std::unique_ptr<core::PoolManager> poolManager;
    std::unique_ptr<core::StratumClient> stratum;

    QTimer* statsTimer;
    QTimer* aiTimer;

    // Simulation
    float currentHashrate = 0.0f;
    float currentTemp = 45.0f;
    double currentDifficulty = 1.0;
    std::string currentCoin = "LTC";
};

} // namespace gui
