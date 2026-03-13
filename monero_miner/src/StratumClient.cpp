#include "StratumClient.h"
#include <iostream>

StratumClient::StratumClient(const std::string& url, const std::string& wallet)
    : poolUrl(url), walletAddress(wallet), curl(nullptr) {
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
}

StratumClient::~StratumClient() {
    if (curl) curl_easy_cleanup(curl);
    curl_global_cleanup();
}

size_t StratumClient::writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

bool StratumClient::connect() {
    if (!curl) return false;

    std::cout << "[Stratum] Connecting to " << poolUrl << " (SSL/TLS Enabled)..." << std::endl;

    json loginReq = {
        {"method", "login"},
        {"params", {
            {"login", walletAddress},
            {"pass", "x"},
            {"agent", "AI-Monero-Miner-Pro/2.0"}
        }},
        {"id", 1}
    };

    std::string responseString;
    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, poolUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, loginReq.dump().c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseString);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    // SSL/TLS Options
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
    curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "[Stratum] Secure connection failed: " << curl_easy_strerror(res) << std::endl;
        return false;
    }

    try {
        auto resp = json::parse(responseString);
        if (resp.contains("result") && resp["result"].contains("job")) {
            JobData job;
            job.jobId = resp["result"]["job"]["job_id"];
            job.blob = resp["result"]["job"]["blob"];
            job.target = resp["result"]["job"]["target"];
            if (jobCallback) jobCallback(job);
            std::cout << "[Stratum] Secure handshake complete. Received first job." << std::endl;
            return true;
        }
    } catch (...) {
        std::cerr << "[Stratum] Failed to parse pool response." << std::endl;
    }

    return false;
}

void StratumClient::setJobCallback(std::function<void(const JobData&)> callback) {
    jobCallback = callback;
}

void StratumClient::submitShare(const std::string& jobId, uint32_t nonce, const std::string& result) {
    std::cout << "[Stratum] Submitting share for job " << jobId << "..." << std::endl;
}
