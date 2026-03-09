#include <QApplication>
#include "gui/mainwindow.h"
#include <iostream>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    gui::MainWindow window;
    window.show();

    std::cout << "AI Crypto Miner v1.0 [Windows/AMD Support] initialized." << std::endl;

    return app.exec();
}
