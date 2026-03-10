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
#include <memory>
#include "core/worker.h"
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
    QLineEdit* hostEdit;
    QLineEdit* userEdit;
    QLineEdit* passEdit;
    QLabel* hashrateLabel;
    QLabel* tempLabel;
    QLabel* difficultyLabel;
    QLabel* aiPredictLabel;
    QLabel* statusLabel;
    QPushButton* startBtn;
    QPushButton* stopBtn;
    QPushButton* tuneBtn;
    QProgressBar* loadBar;
    QProgressBar* tuneBar;

    // Core Logic
    std::vector<std::unique_ptr<core::Worker>> workers;
    std::unique_ptr<ai::LearningEngine> aiEngine;
    QTimer* statsTimer;
    QTimer* aiTimer;

    // Simulation variables
    float currentHashrate = 0.0f;
    float currentTemp = 45.0f;
    double currentDifficulty = 1.0;
    double predictedDifficulty = 1.0;
};

} // namespace gui
