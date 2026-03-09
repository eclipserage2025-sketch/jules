#pragma once
#include <string>
#include <vector>
#include <functional>
#include <curl/curl.h>

namespace core {

/**
 * @brief Simple Stratum client using libcurl for TCP communication
 */
class StratumClient {
public:
    StratumClient(const std::string& host, int port, const std::string& user, const std::string& pass);
    ~StratumClient();

    bool connect();
    void disconnect();

    // Work handling
    bool subscribe();
    bool authorize();
    bool submit(const std::string& job_id, const std::string& extranonce2, const std::string& ntime, const std::string& nonce);

    // Callbacks for new work
    void setWorkCallback(std::function<void(const std::string&, const std::string&, const std::vector<uint8_t>&)> callback);

private:
    std::string host, user, pass;
    int port;
    CURL* curl;

    // JSON-RPC handling helper
    std::string sendRequest(const std::string& method, const std::string& params);
    static size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp);
};

} // namespace core
