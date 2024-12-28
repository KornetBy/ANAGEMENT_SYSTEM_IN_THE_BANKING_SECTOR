// CompensationManager.h
#pragma once
#include <string>
#include <vector>
#include <mutex>

struct CompensationRequest {
    long id;
    long employeeID;
    long positionID;
    std::string type;
    double amount;
    std::string submissionDate;
    std::string status;
};

class CompensationManager {
private:
    std::string filename;
    std::vector<CompensationRequest> requests;
    std::mutex mtx;
    void load();
    void save();
public:
    CompensationManager(const std::string& filename);
    bool addRequest(long id, long empID, long posID, const std::string& type, double amount, const std::string& date, const std::string& status);
    bool approveRequest(long id);
    bool declineRequest(long id);
    bool processRequest(long id, const std::string& newStatus);
};
