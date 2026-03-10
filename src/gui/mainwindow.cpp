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

    setWindowTitle("AI Crypto Miner v2.0 [Pro Edition]");
    setMinimumSize(800, 650);

    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    // Advanced Pool Manager
    QGroupBox* poolGroup = new QGroupBox("Multi-Pool Manager (Failover)", centralWidget);
    QVBoxLayout* poolLayout = new QVBoxLayout(poolGroup);
    poolTable = new QTableWidget(3, 3, poolGroup);
    poolTable->setHorizontalHeaderLabels({"URL", "Priority", "Status"});
    poolTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Add default pools
    poolManager->addPool("litecoinpool.org", 3333, "user", "x", 10);
    poolManager->addPool("viabtc.com", 3333, "user", "x", 5);
    poolManager->addPool("f2pool.com", 3333, "user", "x", 1);

    for(int i=0; i<3; ++i) {
        auto& p = poolManager->getAllPools()[i];
        poolTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(p.url)));
        poolTable->setItem(i, 1, new QTableWidgetItem(QString::number(p.priority)));
        poolTable->setItem(i, 2, new QTableWidgetItem("Standby"));
    }
    poolLayout->addWidget(poolTable);
    mainLayout->addWidget(poolGroup);

    // Intelligence & Stats
    QGroupBox* statsGroup = new QGroupBox("AI Network & Hardware Intelligence", centralWidget);
    QGridLayout* statsGrid = new QGridLayout(statsGroup);

    hashrateLabel = new QLabel("Hashrate: 0.00 KH/s");
    tempLabel = new QLabel("Temperature: 45.0 °C");
    difficultyLabel = new QLabel("Current Difficulty: 1.0");
    aiPredictLabel = new QLabel("AI Difficulty Forecast: 1.0");
    aiPredictLabel->setStyleSheet("color: #0078d7; font-weight: bold;");

    profitLabel = new QLabel("Most Profitable: LTC");
    profitLabel->setStyleSheet("color: #28a745; font-weight: bold;");

    healthLabel = new QLabel("Hardware Health: Optimal");
    healthLabel->setStyleSheet("color: #28a745;");

    statusLabel = new QLabel("Status: Idle");
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

    // AI Auto-Tuning
    QGroupBox* tuneGroup = new QGroupBox("AI Optimization Controls", centralWidget);
    QVBoxLayout* tuneLayout = new QVBoxLayout(tuneGroup);
    tuneBtn = new QPushButton("Run AI Auto-Tuning Pipeline", tuneGroup);
    tuneBtn->setEnabled(false);
    tuneBar = new QProgressBar(tuneGroup);
    tuneBar->setRange(0, 10);
    tuneLayout->addWidget(tuneBtn);
    tuneLayout->addWidget(tuneBar);
    mainLayout->addWidget(tuneGroup);

    // Controls
    QHBoxLayout* btnLayout = new QHBoxLayout();
    startBtn = new QPushButton("Start AI Miner", centralWidget);
    stopBtn = new QPushButton("Stop Miner", centralWidget);
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
        statusLabel->setText(QString("Status: Mining on %1").arg(QString::fromStdString(pool->url)));
    }

    statsTimer->start(1000);
    aiTimer->start(5000);
}

void MainWindow::onStopClicked() {
    startBtn->setEnabled(true);
    stopBtn->setEnabled(false);
    tuneBtn->setEnabled(false);
    statusLabel->setText("Status: Idle");
    statsTimer->stop();
    aiTimer->stop();
    stratum->disconnect();
}

void MainWindow::onAutoTuneClicked() {
    aiEngine->startAutoTuning();
    tuneBtn->setEnabled(false);
    statusLabel->setText("Status: Running AI Optimization Pipeline...");
}

void MainWindow::updateStats() {
    currentHashrate = 250.0f + (rand() % 50);
    currentTemp = 45.0f + (rand() % 15);

    // Simulate Profitability & Difficulty from Stratum intelligence
    aiEngine->updateCoinProfitability("LTC", currentDifficulty, 85.0);
    aiEngine->updateCoinProfitability("DOGE", currentDifficulty * 0.8, 0.15);

    hashrateLabel->setText(QString("Hashrate: %1 KH/s").arg((double)currentHashrate, 0, 'f', 2));
    tempLabel->setText(QString("Temperature: %1 °C").arg((double)currentTemp, 0, 'f', 1));

    currentCoin = aiEngine->getMostProfitableCoin();
    profitLabel->setText(QString("Most Profitable: %1").arg(QString::fromStdString(currentCoin)));

    // Hardware Health
    aiEngine->updateHardwareHealth(currentTemp, 85.0f);
    if (aiEngine->isInstabilityPredicted()) {
        healthLabel->setText("Hardware Health: RISK DETECTED");
        healthLabel->setStyleSheet("color: #dc3545; font-weight: bold;");
    } else {
        healthLabel->setText("Hardware Health: Optimal");
        healthLabel->setStyleSheet("color: #28a745;");
    }

    loadBar->setValue(rand() % 100);
}

void MainWindow::runAI() {
    if (!aiEngine->isTuning()) {
        float opt_intensity;
        int opt_threads;
        aiEngine->predictOptimalSettings(75.0f, currentDifficulty, opt_intensity, opt_threads);
        std::cout << "[GUI] AI rebalancing for target " << currentCoin << std::endl;
    } else {
        aiEngine->updateTuningProgress(currentHashrate);
        if (!aiEngine->isTuning()) {
            tuneBtn->setEnabled(true);
            statusLabel->setText("Status: AI Rebalanced & Optimized");
        }
    }
}

} // namespace gui
