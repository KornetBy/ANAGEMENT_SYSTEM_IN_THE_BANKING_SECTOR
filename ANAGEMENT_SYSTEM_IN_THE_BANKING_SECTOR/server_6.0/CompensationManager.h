// Server/CompensationManager.h
#pragma once
#include <string>
#include <vector>
#include <mutex>

struct CompensationRequest {
    std::string requestId;
    std::string employeeName;
    std::string compensationData;
    std::string status;
};

class CompensationManager {
private:
    std::vector<CompensationRequest> requests;
    std::string compensationFileName;
    std::mutex mtx;

    void loadRequests();
public:
    CompensationManager(const std::string& fileName);
    bool addRequest(const CompensationRequest& request);
    bool processRequest(const std::string& requestId);
    std::vector<CompensationRequest> getAllRequests();
};
