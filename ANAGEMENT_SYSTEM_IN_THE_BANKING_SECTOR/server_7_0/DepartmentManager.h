#pragma once
// DepartmentManager.h
#pragma once
#include <string>
#include <vector>
#include <mutex>

struct Department {
    long departmentID;
    std::string departmentName;
    std::string location;
    long positionID;
};

class DepartmentManager {
private:
    std::vector<Department> departments;
    std::string departmentFileName;
    std::mutex mtx;
public:
    DepartmentManager(const std::string& filename);
    Department getDepartment(long departmentID);
    bool addDepartment(const Department& department);
    bool updateDepartment(const Department& department);
    bool deleteDepartment(long departmentID);
    std::vector<Department> getAllDepartments();
};
