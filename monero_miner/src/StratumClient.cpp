#include "StratumClient.h"
#include <iostream>

StratumClient::StratumClient(const std::string& url, const std::string& wallet)
    : poolUrl(url), walletAddress(wallet), curl(nullptr) {
    // curl_global_init(CURL_GLOBAL_ALL);
    // curl = curl_easy_init();
}

StratumClient::~StratumClient() {
    // if (curl) curl_easy_cleanup(curl);
    // curl_global_cleanup();
}

size_t StratumClient::writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    // ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

bool StratumClient::connect() {
    std::cout << "[Stratum] Mock Connecting to " << poolUrl << " (libcurl missing)..." << std::endl;

    // Simulate a job for testing without libcurl
    JobData job;
    job.jobId = "mock-job-123";
    job.blob = "00000000000000000000000000000000";
    job.target = "ffffffff";

    if (jobCallback) {
        std::cout << "[Stratum] Mocking job callback..." << std::endl;
        jobCallback(job);
    }

    return true;
}

void StratumClient::setJobCallback(std::function<void(const JobData&)> callback) {
    jobCallback = callback;
}

void StratumClient::submitShare(const std::string& jobId, uint32_t nonce, const std::string& result) {
    std::cout << "[Stratum] Submitting share for job " << jobId << "..." << std::endl;
}
