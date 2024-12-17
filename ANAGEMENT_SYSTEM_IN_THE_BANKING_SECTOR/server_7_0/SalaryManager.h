#pragma once
// SalaryManager.h
#pragma once
#include <string>
#include <vector>
#include <mutex>

struct Salary {
    long salaryID;
    long employeeID;
    long positionID;
    double baseSalary;
    double bonuses;
    double deductions;
    double netPay;
    std::string calculationDate; // Формат YYYY-MM-DD
};

class SalaryManager {
private:
    std::vector<Salary> salaries;
    std::string salaryFileName;
    std::mutex mtx;
public:
    SalaryManager(const std::string& filename);
    Salary getSalary(long salaryID);
    bool addSalary(const Salary& salary);
    bool updateSalary(const Salary& salary);
    bool deleteSalary(long salaryID);
    std::vector<Salary> getAllSalaries();
};
