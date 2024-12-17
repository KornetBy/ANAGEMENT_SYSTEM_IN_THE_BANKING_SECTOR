// DepartmentManager.cpp
#include "DepartmentManager.h"
#include <fstream>
#include <sstream>

DepartmentManager::DepartmentManager(const std::string& filename) : departmentFileName(filename) {
    std::lock_guard<std::mutex> lock(mtx);
    std::ifstream ifs(departmentFileName);
    if (!ifs.is_open()) return;

    std::string line;
    while (getline(ifs, line)) {
        std::istringstream iss(line);
        std::string token;
        Department dept;
        getline(iss, token, '|');
        dept.departmentID = std::stol(token);
        getline(iss, dept.departmentName, '|');
        getline(iss, dept.location, '|');
        getline(iss, token, '|');
        dept.positionID = std::stol(token);
        departments.push_back(dept);
    }
    ifs.close();
}

Department DepartmentManager::getDepartment(long departmentID) {
    std::lock_guard<std::mutex> lock(mtx);
    for (const auto& dept : departments) {
        if (dept.departmentID == departmentID) return dept;
    }
    return Department{ 0, "", "", 0 };
}

bool DepartmentManager::addDepartment(const Department& department) {
    std::lock_guard<std::mutex> lock(mtx);
    for (const auto& dept : departments) {
        if (dept.departmentID == department.departmentID) return false; // Отдел уже существует
    }
    departments.push_back(department);

    std::ofstream ofs(departmentFileName, std::ios::app);
    if (!ofs.is_open()) return false;
    ofs << department.departmentID << "|" << department.departmentName << "|"
        << department.location << "|" << department.positionID << "\n";
    ofs.close();
    return true;
}

bool DepartmentManager::updateDepartment(const Department& department) {
    std::lock_guard<std::mutex> lock(mtx);
    bool found = false;
    for (auto& dept : departments) {
        if (dept.departmentID == department.departmentID) {
            dept = department;
            found = true;
            break;
        }
    }
    if (!found) return false;

    // Перезапись файла
    std::ofstream ofs(departmentFileName, std::ios::trunc);
    if (!ofs.is_open()) return false;
    for (const auto& dept : departments) {
        ofs << dept.departmentID << "|" << dept.departmentName << "|"
            << dept.location << "|" << dept.positionID << "\n";
    }
    ofs.close();
    return true;
}

bool DepartmentManager::deleteDepartment(long departmentID) {
    std::lock_guard<std::mutex> lock(mtx);
    bool found = false;
    std::vector<Department> updatedDepartments;
    for (const auto& dept : departments) {
        if (dept.departmentID != departmentID) {
            updatedDepartments.push_back(dept);
        }
        else {
            found = true;
        }
    }
    if (!found) return false;
    departments = updatedDepartments;

    // Перезапись файла
    std::ofstream ofs(departmentFileName, std::ios::trunc);
    if (!ofs.is_open()) return false;
    for (const auto& dept : departments) {
        ofs << dept.departmentID << "|" << dept.departmentName << "|"
            << dept.location << "|" << dept.positionID << "\n";
    }
    ofs.close();
    return true;
}

std::vector<Department> DepartmentManager::getAllDepartments() {
    std::lock_guard<std::mutex> lock(mtx);
    return departments;
}
