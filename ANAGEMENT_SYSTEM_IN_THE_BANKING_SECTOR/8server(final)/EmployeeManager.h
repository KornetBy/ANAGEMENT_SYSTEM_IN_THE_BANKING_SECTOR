#pragma once
#include <string>
#include <vector>
#include <mutex>

struct Employee {
    long id;
    long posID;
    std::string fullName;
    std::string birthDate;
    std::string address;
    std::string contactInfo;
    std::string startDate;
    std::string status;
    long departmentID; // Для полноты, пусть будет отдел
};

class EmployeeManager {
private:
    std::string filename;
    std::vector<Employee> employees;
    std::mutex mtx;
    void load();
    void save();
public:
    bool deleteEmployee(long id);
    EmployeeManager(const std::string& filename);
    bool addEmployee(long posID, const std::string& fullName, const std::string& birthDate, const std::string& address, const std::string& contactInfo, const std::string& startDate, const std::string& status);
    bool updateEmployeeData(long id, const std::string& fullName, const std::string& address, const std::string& contactInfo);
    bool updateEmployeeFullData(long id, const std::string& fullName, const std::string& address, const std::string& contactInfo, const std::string& status);
    bool blockEmployee(long id);
    Employee getEmployee(long id);
    std::vector<Employee> getAllEmployees();
    bool setEmployeeDepartment(long empID, long deptID);
};
