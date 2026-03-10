#include "stratum_client.h"
#include <iostream>
#include <sstream>

namespace core {

StratumClient::StratumClient(const std::string& user, const std::string& pass)
    : user(user), pass(pass), curl(nullptr) {
    curl_global_init(CURL_GLOBAL_ALL);
}

StratumClient::~StratumClient() {
    disconnect();
    curl_global_cleanup();
}

bool StratumClient::connect(const std::string& host, int port) {
    std::lock_guard<std::mutex> lock(client_mutex);
    current_host = host;
    current_port = port;
    curl = curl_easy_init();
    if (!curl) return false;
    std::stringstream url;
    url << "telnet://" << host << ":" << port;
    curl_easy_setopt(curl, CURLOPT_URL, url.str().c_str());
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5L);
    std::cout << "[STRATUM] Session established: " << host << ":" << port << std::endl;
    return true;
}

void StratumClient::disconnect() {
    std::lock_guard<std::mutex> lock(client_mutex);
    if (curl) {
        curl_easy_cleanup(curl);
        curl = nullptr;
    }
}

bool StratumClient::subscribe() {
    std::string response = sendRequest("mining.subscribe", "[]");
    return !response.empty();
}

bool StratumClient::authorize() {
    std::stringstream params;
    params << "[\"" << user << "\", \"" << pass << "\"]";
    std::string response = sendRequest("mining.authorize", params.str());
    return !response.empty();
}

bool StratumClient::submit(const std::string& job_id, const std::string& extranonce2, const std::string& ntime, const std::string& nonce) {
    std::stringstream params;
    params << "[\"" << user << "\", \"" << job_id << "\", \"" << extranonce2 << "\", \"" << ntime << "\", \"" << nonce << "\"]";
    std::string response = sendRequest("mining.submit", params.str());
    return !response.empty();
}

void StratumClient::setDifficultyCallback(std::function<void(double)> callback) {
    std::lock_guard<std::mutex> lock(client_mutex);
    diff_callback = callback;
}

void StratumClient::setTargetCoinCallback(std::function<void(const std::string&)> callback) {
    std::lock_guard<std::mutex> lock(client_mutex);
    coin_callback = callback;
}

std::string StratumClient::sendRequest(const std::string& method, const std::string& params) {
    std::lock_guard<std::mutex> lock(client_mutex);
    if (!curl) return "";

    // Simulate difficulty and coin updates from pool stream
    if (diff_callback) diff_callback(1000.0 + (std::rand() % 15));
    if (coin_callback) coin_callback(std::rand() % 2 == 0 ? "LTC" : "DOGE");

    return "{\"id\":1,\"result\":true,\"error\":null}";
}

size_t StratumClient::write_callback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

} // namespace core
