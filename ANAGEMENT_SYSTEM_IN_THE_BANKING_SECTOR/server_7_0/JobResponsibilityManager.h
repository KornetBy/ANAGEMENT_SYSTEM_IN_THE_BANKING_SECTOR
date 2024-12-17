#pragma once
// JobResponsibilityManager.h
#pragma once
#include <string>
#include <vector>
#include <mutex>

struct JobResponsibility {
    long responsibilityID;
    long employeeID;
    long positionID;
    std::string responsibilityDescription;
};

class JobResponsibilityManager {
private:
    std::vector<JobResponsibility> responsibilities;
    std::string responsibilityFileName;
    std::mutex mtx;
public:
    JobResponsibilityManager(const std::string& filename);
    JobResponsibility getResponsibility(long responsibilityID);
    bool addResponsibility(const JobResponsibility& responsibility);
    bool updateResponsibility(const JobResponsibility& responsibility);
    bool deleteResponsibility(long responsibilityID);
    std::vector<JobResponsibility> getAllResponsibilities();
};
