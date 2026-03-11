#include <QApplication>
#include "gui/mainwindow.h"
#include <iostream>
#include <ctime>

int main(int argc, char *argv[]) {
    // Seed random number generator for realistic simulation
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    QApplication app(argc, argv);

    gui::MainWindow window;
    window.show();

    std::cout << "AI Crypto Miner v1.0 [Windows/AMD Support] initialized." << std::endl;

    return app.exec();
}
