#include "telemetry_server.h"
#include <QTextStream>
#include <iostream>

namespace core {

TelemetryServer::TelemetryServer(QObject *parent) : QObject(parent), tcpServer(new QTcpServer(this)) {}

TelemetryServer::~TelemetryServer() { stop(); }

bool TelemetryServer::start(quint16 port) {
    connect(tcpServer, &QTcpServer::newConnection, this, &TelemetryServer::onNewConnection);
    if (!tcpServer->listen(QHostAddress::Any, port)) {
        std::cerr << "[TELEMETRY] Failed to start server on port " << port << std::endl;
        return false;
    }
    std::cout << "[TELEMETRY] Web Dashboard available at http://localhost:" << port << std::endl;
    return true;
}

void TelemetryServer::stop() {
    tcpServer->close();
}

void TelemetryServer::setStatsProvider(std::function<QString()> provider) {
    statsProvider = provider;
}

void TelemetryServer::onNewConnection() {
    QTcpSocket *clientSocket = tcpServer->nextPendingConnection();
    connect(clientSocket, &QTcpSocket::readyRead, this, &TelemetryServer::onReadyRead);
    connect(clientSocket, &QTcpSocket::disconnected, this, &TelemetryServer::onDisconnected);
}

void TelemetryServer::onReadyRead() {
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket *>(sender());
    if (!clientSocket) return;

    // Simple HTTP GET parser
    QString request = clientSocket->readAll();
    if (request.startsWith("GET /stats")) {
        QString json = statsProvider ? statsProvider() : "{\"status\": \"offline\"}";
        sendResponse(clientSocket, json);
    } else if (request.startsWith("GET / ")) {
        QString html = "<html><body><h1>AI Miner Telemetry</h1><p><a href='/stats'>View JSON Stats</a></p></body></html>";
        sendResponse(clientSocket, html, "text/html");
    } else {
        sendResponse(clientSocket, "{\"error\": \"not found\"}", "application/json");
    }
}

void TelemetryServer::onDisconnected() {
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket *>(sender());
    if (clientSocket) clientSocket->deleteLater();
}

void TelemetryServer::sendResponse(QTcpSocket *socket, const QString &content, const QString &contentType) {
    QTextStream os(socket);
    os.setEncoding(QStringConverter::Utf8);
    os << "HTTP/1.1 200 OK\r\n"
       << "Content-Type: " << contentType << "; charset=\"utf-8\"\r\n"
       << "Content-Length: " << content.toUtf8().size() << "\r\n"
       << "Access-Control-Allow-Origin: *\r\n"
       << "Connection: close\r\n"
       << "\r\n"
       << content;
    socket->disconnectFromHost();
}

} // namespace core
