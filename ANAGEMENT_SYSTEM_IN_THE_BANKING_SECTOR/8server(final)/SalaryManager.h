// SalaryManager.h
#pragma once
#include <string>
#include <vector>
#include <mutex>

struct SalaryRecord {
    long employeeID;
    double salary;
};

class SalaryManager {
private:
    std::string filename;
    std::vector<SalaryRecord> salaries;
    std::mutex mtx;
    void load();
    void save();
public:
    SalaryManager(const std::string& filename);
    bool setSalary(long employeeID, double salary);
    double getSalary(long employeeID);
    std::string getAllSalariesFormatted();
};
