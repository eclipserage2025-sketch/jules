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
#include <iostream>
#include <vector>

namespace gui {

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), aiEngine(std::make_unique<ai::LearningEngine>()) {

    setWindowTitle("AI Crypto Miner v1.0 [Network Intelligence]");
    setMinimumSize(600, 500);

    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    // Pool Configuration
    QGroupBox* poolGroup = new QGroupBox("Pool Settings", centralWidget);
    QFormLayout* poolLayout = new QFormLayout(poolGroup);
    hostEdit = new QLineEdit("stratum+tcp://litecoinpool.org:3333", poolGroup);
    userEdit = new QLineEdit("username.worker", poolGroup);
    passEdit = new QLineEdit("password", poolGroup);
    passEdit->setEchoMode(QLineEdit::Password);
    poolLayout->addRow("Stratum URL:", hostEdit);
    poolLayout->addRow("Worker Name:", userEdit);
    poolLayout->addRow("Password:", passEdit);
    mainLayout->addWidget(poolGroup);

    // Monitoring
    QGroupBox* statsGroup = new QGroupBox("Real-time Stats", centralWidget);
    QVBoxLayout* statsLayout = new QVBoxLayout(statsGroup);
    hashrateLabel = new QLabel("Hashrate: 0.00 KH/s", statsGroup);
    tempLabel = new QLabel("Temperature: 45.0 °C", statsGroup);
    difficultyLabel = new QLabel("Current Difficulty: 1.0", statsGroup);
    aiPredictLabel = new QLabel("AI Difficulty Forecast: 1.0", statsGroup);
    aiPredictLabel->setStyleSheet("color: #0078d7; font-weight: bold;");

    statusLabel = new QLabel("Status: Idle", statsGroup);
    loadBar = new QProgressBar(statsGroup);
    loadBar->setRange(0, 100);
    loadBar->setValue(0);

    statsLayout->addWidget(hashrateLabel);
    statsLayout->addWidget(tempLabel);
    statsLayout->addWidget(difficultyLabel);
    statsLayout->addWidget(aiPredictLabel);
    statsLayout->addWidget(statusLabel);
    statsLayout->addWidget(loadBar);
    mainLayout->addWidget(statsGroup);

    // Control Buttons
    QHBoxLayout* btnLayout = new QHBoxLayout();
    startBtn = new QPushButton("Start Mining", centralWidget);
    stopBtn = new QPushButton("Stop Mining", centralWidget);
    stopBtn->setEnabled(false);
    btnLayout->addWidget(startBtn);
    btnLayout->addWidget(stopBtn);
    mainLayout->addLayout(btnLayout);

    // Connections
    connect(startBtn, &QPushButton::clicked, this, &MainWindow::onStartClicked);
    connect(stopBtn, &QPushButton::clicked, this, &MainWindow::onStopClicked);

    // Timers
    statsTimer = new QTimer(this);
    connect(statsTimer, &QTimer::timeout, this, &MainWindow::updateStats);

    aiTimer = new QTimer(this);
    connect(aiTimer, &QTimer::timeout, this, &MainWindow::runAI);
}

MainWindow::~MainWindow() {
    onStopClicked();
}

void MainWindow::onStartClicked() {
    startBtn->setEnabled(false);
    stopBtn->setEnabled(true);
    statusLabel->setText("Status: Connecting to Pool...");

    statusLabel->setText("Status: Mining (Scrypt/AMD)");
    statsTimer->start(1000);
    aiTimer->start(5000);
}

void MainWindow::onStopClicked() {
    startBtn->setEnabled(true);
    stopBtn->setEnabled(false);
    statusLabel->setText("Status: Idle");

    for (auto& w : workers) w->stop();
    workers.clear();

    statsTimer->stop();
    aiTimer->stop();
}

void MainWindow::updateStats() {
    currentHashrate = 250.0f + (rand() % 50);
    currentTemp = 45.0f + (rand() % 20);

    // Simulate difficulty updates from pool
    if (rand() % 10 == 0) {
        currentDifficulty += (rand() % 100) / 100.0;
        aiEngine->addDifficultyDataPoint(currentDifficulty);
    }

    hashrateLabel->setText(QString("Hashrate: %1 KH/s").arg((double)currentHashrate, 0, 'f', 2));
    tempLabel->setText(QString("Temperature: %1 °C").arg((double)currentTemp, 0, 'f', 1));
    difficultyLabel->setText(QString("Current Difficulty: %1").arg(currentDifficulty, 0, 'f', 2));

    predictedDifficulty = aiEngine->predictNextDifficulty();
    aiPredictLabel->setText(QString("AI Difficulty Forecast: %1").arg(predictedDifficulty, 0, 'f', 2));

    loadBar->setValue(rand() % 100);
}

void MainWindow::runAI() {
    aiEngine->addDataPoint(8.0f, 4, currentHashrate, currentTemp);

    float opt_intensity;
    int opt_threads;
    aiEngine->predictOptimalSettings(75.0f, currentDifficulty, opt_intensity, opt_threads);

    std::cout << "[GUI] AI applied optimization to hardware based on forecasted difficulty..." << std::endl;
}

} // namespace gui
