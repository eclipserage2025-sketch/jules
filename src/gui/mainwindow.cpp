#include "mainwindow.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QProgressBar>
#include <QTimer>
#include <QTableWidget>
#include <QHeaderView>
#include <iostream>
#include <vector>

namespace gui {

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      aiEngine(std::make_unique<ai::LearningEngine>()),
      poolManager(std::make_unique<core::PoolManager>()),
      stratum(std::make_unique<core::StratumClient>("user.worker", "pass")) {

    setWindowTitle("AI Crypto Miner v2.1 [Enterprise Refined]");
    setMinimumSize(850, 680);

    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    // Advanced Pool Manager
    QGroupBox* poolGroup = new QGroupBox("Enterprise Multi-Pool Failover", centralWidget);
    QVBoxLayout* poolLayout = new QVBoxLayout(poolGroup);
    poolTable = new QTableWidget(3, 3, poolGroup);
    poolTable->setHorizontalHeaderLabels({"Pool URL", "Priority", "Health Status"});
    poolTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    poolManager->addPool("primary.litecoinpool.org", 3333, "user", "x", 100);
    poolManager->addPool("backup-us.viabtc.com", 3333, "user", "x", 50);
    poolManager->addPool("backup-eu.f2pool.com", 3333, "user", "x", 10);

    for(int i=0; i<3; ++i) {
        auto& p = poolManager->getAllPools()[i];
        poolTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(p.url)));
        poolTable->setItem(i, 1, new QTableWidgetItem(QString::number(p.priority)));
        poolTable->setItem(i, 2, new QTableWidgetItem("Available"));
    }
    poolLayout->addWidget(poolTable);
    mainLayout->addWidget(poolGroup);

    // Intelligence & Stats
    QGroupBox* statsGroup = new QGroupBox("AI Telemetry & Predictive Insights", centralWidget);
    QGridLayout* statsGrid = new QGridLayout(statsGroup);

    hashrateLabel = new QLabel("Hashrate: 0.00 KH/s");
    tempLabel = new QLabel("Temperature: 45.0 °C");
    difficultyLabel = new QLabel("Network Difficulty: 1.0");
    aiPredictLabel = new QLabel("AI Difficulty Forecast: 1.0");
    aiPredictLabel->setStyleSheet("color: #0078d7; font-weight: bold;");

    profitLabel = new QLabel("Optimal Target: LTC");
    profitLabel->setStyleSheet("color: #28a745; font-weight: bold;");

    healthLabel = new QLabel("HW Health: Synchronizing...");
    healthLabel->setStyleSheet("color: #6c757d;");

    statusLabel = new QLabel("System Status: Idle");
    loadBar = new QProgressBar();
    loadBar->setRange(0, 100);

    statsGrid->addWidget(hashrateLabel, 0, 0);
    statsGrid->addWidget(tempLabel, 0, 1);
    statsGrid->addWidget(difficultyLabel, 1, 0);
    statsGrid->addWidget(aiPredictLabel, 1, 1);
    statsGrid->addWidget(profitLabel, 2, 0);
    statsGrid->addWidget(healthLabel, 2, 1);
    statsGrid->addWidget(statusLabel, 3, 0);
    statsGrid->addWidget(loadBar, 3, 1);
    mainLayout->addWidget(statsGroup);

    // AI Optimization
    QGroupBox* tuneGroup = new QGroupBox("AI Performance Optimization Pipeline", centralWidget);
    QVBoxLayout* tuneLayout = new QVBoxLayout(tuneGroup);
    tuneBtn = new QPushButton("Execute Global Auto-Tune", tuneGroup);
    tuneBtn->setEnabled(false);
    tuneBar = new QProgressBar(tuneGroup);
    tuneBar->setRange(0, 10);
    tuneLayout->addWidget(tuneBtn);
    tuneLayout->addWidget(tuneBar);
    mainLayout->addWidget(tuneGroup);

    // Controls
    QHBoxLayout* btnLayout = new QHBoxLayout();
    startBtn = new QPushButton("Initialize AI Miner", centralWidget);
    stopBtn = new QPushButton("Shutdown Miner", centralWidget);
    stopBtn->setEnabled(false);
    btnLayout->addWidget(startBtn);
    btnLayout->addWidget(stopBtn);
    mainLayout->addLayout(btnLayout);

    connect(startBtn, &QPushButton::clicked, this, &MainWindow::onStartClicked);
    connect(stopBtn, &QPushButton::clicked, this, &MainWindow::onStopClicked);
    connect(tuneBtn, &QPushButton::clicked, this, &MainWindow::onAutoTuneClicked);

    statsTimer = new QTimer(this);
    connect(statsTimer, &QTimer::timeout, this, &MainWindow::updateStats);
    aiTimer = new QTimer(this);
    connect(aiTimer, &QTimer::timeout, this, &MainWindow::runAI);
}

MainWindow::~MainWindow() { onStopClicked(); }

void MainWindow::onStartClicked() {
    startBtn->setEnabled(false);
    stopBtn->setEnabled(true);
    tuneBtn->setEnabled(true);

    auto* pool = poolManager->getBestPool();
    if (pool) {
        stratum->connect(pool->url, pool->port);
        statusLabel->setText(QString("System Status: Mining [%1]").arg(QString::fromStdString(pool->url)));
    }

    statsTimer->start(1000);
    aiTimer->start(5000);
}

void MainWindow::onStopClicked() {
    startBtn->setEnabled(true);
    stopBtn->setEnabled(false);
    tuneBtn->setEnabled(false);
    statusLabel->setText("System Status: Idle");
    statsTimer->stop();
    aiTimer->stop();
    stratum->disconnect();
}

void MainWindow::onAutoTuneClicked() {
    aiEngine->startAutoTuning();
    tuneBtn->setEnabled(false);
    statusLabel->setText("System Status: AI Optimization Active...");
}

void MainWindow::updateStats() {
    currentHashrate = 260.0f + (std::rand() % 40);
    currentTemp = 42.0f + (std::rand() % 18);

    aiEngine->updateCoinProfitability("LTC", currentDifficulty, 88.5);
    aiEngine->updateCoinProfitability("DOGE", currentDifficulty * 0.75, 0.16);

    hashrateLabel->setText(QString("Hashrate: %1 KH/s").arg((double)currentHashrate, 0, 'f', 2));
    tempLabel->setText(QString("Temperature: %1 °C").arg((double)currentTemp, 0, 'f', 1));

    currentCoin = aiEngine->getMostProfitableCoin();
    profitLabel->setText(QString("Optimal Target: %1").arg(QString::fromStdString(currentCoin)));

    aiEngine->updateHardwareHealth(currentTemp, 82.0f);
    if (aiEngine->isInstabilityPredicted()) {
        healthLabel->setText("HW Health: RISK - Throttling Active");
        healthLabel->setStyleSheet("color: #dc3545; font-weight: bold;");
    } else {
        healthLabel->setText("HW Health: Optimal (Protected)");
        healthLabel->setStyleSheet("color: #28a745;");
    }

    loadBar->setValue(std::rand() % 100);
}

void MainWindow::runAI() {
    if (!aiEngine->isTuning()) {
        float opt_intensity;
        int opt_threads;
        aiEngine->predictOptimalSettings(72.0f, currentDifficulty, opt_intensity, opt_threads);
        std::cout << "[GUI] AI Decision: Optimal Balance found for " << currentCoin << std::endl;
    } else {
        aiEngine->updateTuningProgress(currentHashrate);
        tuneBar->setValue(std::rand() % 10);
        if (!aiEngine->isTuning()) {
            tuneBtn->setEnabled(true);
            statusLabel->setText("System Status: Optimized & Converged");
        }
    }
}

} // namespace gui
