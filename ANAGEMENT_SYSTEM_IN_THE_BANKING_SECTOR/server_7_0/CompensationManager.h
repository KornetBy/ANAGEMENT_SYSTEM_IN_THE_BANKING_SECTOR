// CompensationManager.h
#pragma once
#include <string>
#include <vector>
#include <mutex>

struct CompensationRequest {
    long requestID;
    long employeeID;
    long positionID;
    std::string compensationType;
    double amount;
    std::string submissionDate; // Формат YYYY-MM-DD
    std::string status;         // Одобрено, На рассмотрении и пр.
};

class CompensationManager {
private:
    std::vector<CompensationRequest> requests;
    std::string compensationFileName;
    std::mutex mtx;
public:
    CompensationManager(const std::string& filename);
    CompensationRequest getRequest(long requestID);
    bool addRequest(const CompensationRequest& request);
    bool processRequest(long requestID, const std::string& newStatus);
    std::vector<CompensationRequest> getAllRequests();
};
