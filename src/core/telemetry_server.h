#ifndef CORE_TELEMETRY_SERVER_H
#define CORE_TELEMETRY_SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include <functional>
#include <QString>

namespace core {

/**
 * @brief Lightweight HTTP Telemetry Server using Qt6 Network components.
 * Exports miner stats in JSON format for remote monitoring.
 */
class TelemetryServer : public QObject {
    Q_OBJECT
public:
    explicit TelemetryServer(QObject *parent = nullptr);
    ~TelemetryServer();

    bool start(quint16 port = 8080);
    void stop();

    // Callback to get live stats from the application
    void setStatsProvider(std::function<QString()> provider);

private slots:
    void onNewConnection();
    void onReadyRead();
    void onDisconnected();

private:
    QTcpServer *tcpServer;
    std::function<QString()> statsProvider;
    void sendResponse(QTcpSocket *socket, const QString &content, const QString &contentType = "application/json");
};

} // namespace core

#endif // CORE_TELEMETRY_SERVER_H
