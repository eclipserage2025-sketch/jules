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
#include <QCheckBox>
#include <memory>
#include "core/worker.h"
#include "core/pool_manager.h"
#include "core/stratum_client.h"
#include "core/telemetry_server.h"
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
    void toggleWebDashboard(bool checked);

private:
    QLabel* hashrateLabel;
    QLabel* tempLabel;
    QLabel* difficultyLabel;
    QLabel* aiPredictLabel;
    QLabel* profitLabel;
    QLabel* statusLabel;
    QLabel* healthLabel;
    QLabel* nnLabel; // Neural Net status
    QLabel* webUrlLabel;

    QPushButton* startBtn;
    QPushButton* stopBtn;
    QPushButton* tuneBtn;
    QCheckBox* webToggle;

    QProgressBar* loadBar;
    QProgressBar* tuneBar;
    QTableWidget* poolTable;

    std::vector<std::unique_ptr<core::Worker>> workers;
    std::unique_ptr<ai::LearningEngine> aiEngine;
    std::unique_ptr<core::PoolManager> poolManager;
    std::unique_ptr<core::StratumClient> stratum;
    std::unique_ptr<core::TelemetryServer> webServer;

    QTimer* statsTimer;
    QTimer* aiTimer;

    float currentHashrate = 0.0f;
    float currentTemp = 45.0f;
    double currentDifficulty = 1.0;
    std::string currentCoin = "LTC";

    QString generateStatsJson();
};

} // namespace gui
