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
#include <QCheckBox>
#include <iostream>

namespace gui {

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      aiEngine(std::make_unique<ai::LearningEngine>()),
      poolManager(std::make_unique<core::PoolManager>()),
      stratum(std::make_unique<core::StratumClient>("user.worker", "pass")),
      webServer(std::make_unique<core::TelemetryServer>(this)) {

    setWindowTitle("AI Crypto Miner v3.0 [Ultimate Neural Edition]");
    setMinimumSize(900, 750);

    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    // Pool Table
    QGroupBox* poolGroup = new QGroupBox("Enterprise Failover & Algorithm Control", centralWidget);
    QVBoxLayout* poolLayout = new QVBoxLayout(poolGroup);
    poolTable = new QTableWidget(3, 3, poolGroup);
    poolTable->setHorizontalHeaderLabels({"Pool URL", "Algorithm", "Status"});
    poolTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    poolLayout->addWidget(poolTable);
    mainLayout->addWidget(poolGroup);

    // Neural Stats
    QGroupBox* statsGroup = new QGroupBox("Neural AI Intelligence Dashboard", centralWidget);
    QGridLayout* statsGrid = new QGridLayout(statsGroup);
    hashrateLabel = new QLabel("Hashrate: 0.00 KH/s");
    tempLabel = new QLabel("Temperature: 45.0 °C");
    difficultyLabel = new QLabel("Difficulty: 1.0");
    aiPredictLabel = new QLabel("AI Forecast: 1.0");
    profitLabel = new QLabel("Optimal: LTC");
    healthLabel = new QLabel("Health: Optimal");
    nnLabel = new QLabel("Neural Net: Ready (Active Learning)");
    nnLabel->setStyleSheet("color: #6f42c1; font-weight: bold;");

    statsGrid->addWidget(hashrateLabel, 0, 0);
    statsGrid->addWidget(tempLabel, 0, 1);
    statsGrid->addWidget(difficultyLabel, 1, 0);
    statsGrid->addWidget(aiPredictLabel, 1, 1);
    statsGrid->addWidget(profitLabel, 2, 0);
    statsGrid->addWidget(healthLabel, 2, 1);
    statsGrid->addWidget(nnLabel, 3, 0, 1, 2);
    mainLayout->addWidget(statsGroup);

    // Web Dashboard Toggle
    QGroupBox* webGroup = new QGroupBox("Remote Monitoring (Web Telemetry)", centralWidget);
    QHBoxLayout* webLayout = new QHBoxLayout(webGroup);
    webToggle = new QCheckBox("Enable Web Dashboard (Port 8080)", webGroup);
    webUrlLabel = new QLabel("Dashboard URL: http://localhost:8080", webGroup);
    webUrlLabel->setEnabled(false);
    webLayout->addWidget(webToggle);
    webLayout->addWidget(webUrlLabel);
    mainLayout->addWidget(webGroup);

    // Optimization Pipeline
    QGroupBox* tuneGroup = new QGroupBox("AI Optimization & Neural Training", centralWidget);
    QVBoxLayout* tuneLayout = new QVBoxLayout(tuneGroup);
    tuneBtn = new QPushButton("Execute Global Optimization Pipeline", tuneGroup);
    tuneBtn->setEnabled(false);
    tuneBar = new QProgressBar(tuneGroup);
    tuneBar->setRange(0, 10);
    tuneLayout->addWidget(tuneBtn);
    tuneLayout->addWidget(tuneBar);
    mainLayout->addWidget(tuneGroup);

    // Master Controls
    QHBoxLayout* btnLayout = new QHBoxLayout();
    startBtn = new QPushButton("Initialize Ultimate Miner", centralWidget);
    stopBtn = new QPushButton("System Shutdown", centralWidget);
    stopBtn->setEnabled(false);
    btnLayout->addWidget(startBtn);
    btnLayout->addWidget(stopBtn);
    mainLayout->addLayout(btnLayout);

    connect(startBtn, &QPushButton::clicked, this, &MainWindow::onStartClicked);
    connect(stopBtn, &QPushButton::clicked, this, &MainWindow::onStopClicked);
    connect(tuneBtn, &QPushButton::clicked, this, &MainWindow::onAutoTuneClicked);
    connect(webToggle, &QCheckBox::toggled, this, &MainWindow::toggleWebDashboard);

    statsTimer = new QTimer(this);
    connect(statsTimer, &QTimer::timeout, this, &MainWindow::updateStats);
    aiTimer = new QTimer(this);
    connect(aiTimer, &QTimer::timeout, this, &MainWindow::runAI);

    webServer->setStatsProvider([this](){ return generateStatsJson(); });
}

MainWindow::~MainWindow() { onStopClicked(); }

void MainWindow::onStartClicked() {
    startBtn->setEnabled(false);
    stopBtn->setEnabled(true);
    tuneBtn->setEnabled(true);
    statsTimer->start(1000);
    aiTimer->start(5000);
}

void MainWindow::onStopClicked() {
    startBtn->setEnabled(true);
    stopBtn->setEnabled(false);
    tuneBtn->setEnabled(false);
    statsTimer->stop();
    aiTimer->stop();
}

void MainWindow::onAutoTuneClicked() {
    aiEngine->startAutoTuning();
    tuneBtn->setEnabled(false);
}

void MainWindow::toggleWebDashboard(bool checked) {
    if (checked) webServer->start(8080);
    else webServer->stop();
    webUrlLabel->setEnabled(checked);
}

void MainWindow::updateStats() {
    currentHashrate = 275.0f + (rand() % 30);
    currentTemp = 40.0f + (rand() % 10);
    hashrateLabel->setText(QString("Hashrate: %1 KH/s").arg((double)currentHashrate, 0, 'f', 2));
    tempLabel->setText(QString("Temperature: %1 °C").arg((double)currentTemp, 0, 'f', 1));
}

void MainWindow::runAI() {
    if (aiEngine->isTuning()) {
        aiEngine->updateTuningProgress(currentHashrate);
        tuneBar->setValue(rand() % 10);
        if (!aiEngine->isTuning()) tuneBtn->setEnabled(true);
    } else {
        float h_pred = aiEngine->neuralPredictHashrate(8.0f, 4);
        nnLabel->setText(QString("Neural Net: Active (Pred. Hashrate: %1 KH/s)").arg((double)h_pred, 0, 'f', 2));
    }
}

QString MainWindow::generateStatsJson() {
    return QString("{\"hashrate\": %1, \"temp\": %2, \"coin\": \"%3\"}")
           .arg((double)currentHashrate).arg((double)currentTemp).arg(QString::fromStdString(currentCoin));
}

} // namespace gui
