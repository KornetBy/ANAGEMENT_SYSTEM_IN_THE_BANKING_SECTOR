// EmployeeManager.h
#pragma once
#include <string>
#include <vector>
#include <mutex>
#include "PositionManager.h"

struct Employee {
    long employeeID;
    long positionID;
    std::string fullName;
    std::string birthDate; // ‘ормат YYYY-MM-DD
    std::string address;
    std::string contactInfo;
    std::string startDate; // ‘ормат YYYY-MM-DD
    std::string status;     // active, blocked и т.д.
};

class EmployeeManager {
private:
    std::vector<Employee> employees;
    std::string employeeFileName;
    std::mutex mtx;
public:
    EmployeeManager(const std::string& filename);
    Employee getEmployee(long employeeID);
    bool addEmployee(const Employee& employee);
    bool editEmployee(const Employee& employee);
    bool blockEmployee(long employeeID);
    std::vector<Employee> getAllEmployees();
};
