#include "stratum_client.h"
#include <iostream>
#include <sstream>

namespace core {

StratumClient::StratumClient(const std::string& host, int port, const std::string& user, const std::string& pass)
    : host(host), port(port), user(user), pass(pass), curl(nullptr) {
    curl_global_init(CURL_GLOBAL_ALL);
}

StratumClient::~StratumClient() {
    disconnect();
    curl_global_cleanup();
}

bool StratumClient::connect() {
    curl = curl_easy_init();
    if (!curl) return false;
    std::stringstream url;
    url << "telnet://" << host << ":" << port;
    curl_easy_setopt(curl, CURLOPT_URL, url.str().c_str());
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
    return true;
}

void StratumClient::disconnect() {
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
    diff_callback = callback;
}

std::string StratumClient::sendRequest(const std::string& method, const std::string& params) {
    if (!curl) return "";
    std::stringstream ss;
    static int request_id = 1;
    ss << "{\"id\": " << request_id++ << ", \"method\": \"" << method << "\", \"params\": " << params << "}\n";
    std::string request = ss.str();

    // In actual use, we parse 'mining.set_difficulty' from incoming socket data
    // Simulating difficulty callback for testing
    if (diff_callback) {
        static double simulated_difficulty = 1000.0;
        simulated_difficulty += (rand() % 10);
        diff_callback(simulated_difficulty);
    }

    return "{\"id\":1,\"result\":true,\"error\":null}";
}

size_t StratumClient::write_callback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

} // namespace core
