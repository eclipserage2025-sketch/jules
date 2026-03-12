#ifndef STRATUM_CLIENT_H
#define STRATUM_CLIENT_H

#include <string>
#include <functional>
#include <vector>
// #include <curl/curl.h>
#include "../external/json.hpp"

using json = nlohmann::json;

struct JobData {
    std::string jobId;
    std::string blob;
    std::string target;
};

class StratumClient {
public:
    StratumClient(const std::string& poolUrl, const std::string& walletAddress);
    ~StratumClient();

    bool connect();
    void setJobCallback(std::function<void(const JobData&)> callback);
    void submitShare(const std::string& jobId, uint32_t nonce, const std::string& result);

private:
    static size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp);

    std::string poolUrl;
    std::string walletAddress;
    std::function<void(const JobData&)> jobCallback;
    void* curl; // Use void* instead of CURL*
};

#endif
