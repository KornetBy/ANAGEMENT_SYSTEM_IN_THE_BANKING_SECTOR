// JobResponsibilityManager.h
#pragma once
#include <string>
#include <vector>
#include <mutex>

struct Responsibility {
    long positionID;
    std::string responsibility;
};

class JobResponsibilityManager {
private:
    std::string filename;
    std::vector<Responsibility> responsibilities;
    std::mutex mtx;
    void load();
    void save();
public:
    JobResponsibilityManager(const std::string& filename);
    bool addResponsibility(long posID, const std::string& resp);
    std::string getResponsibilities(long posID);
};
