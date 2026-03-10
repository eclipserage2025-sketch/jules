#pragma once
#include <string>
#include <vector>
#include <functional>
#include <curl/curl.h>

namespace core {

/**
 * @brief Stratum client with failover and intelligence tracking
 */
class StratumClient {
public:
    StratumClient(const std::string& user, const std::string& pass);
    ~StratumClient();

    bool connect(const std::string& host, int port);
    void disconnect();

    bool subscribe();
    bool authorize();
    bool submit(const std::string& job_id, const std::string& extranonce2, const std::string& ntime, const std::string& nonce);

    // Callbacks
    void setDifficultyCallback(std::function<void(double)> callback);
    void setTargetCoinCallback(std::function<void(const std::string&)> callback);

private:
    std::string user, pass;
    std::string current_host;
    int current_port;
    CURL* curl;
    std::function<void(double)> diff_callback;
    std::function<void(const std::string&)> coin_callback;

    std::string sendRequest(const std::string& method, const std::string& params);
    static size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp);
};

} // namespace core
