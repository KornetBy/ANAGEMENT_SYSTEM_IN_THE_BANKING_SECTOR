#include "EmployeeManager.h"
#include "utils.h"
#include <fstream>
#include <sstream>
EmployeeManager::EmployeeManager(const std::string& filename) : filename(filename) {
    load();
}

void EmployeeManager::load() {
    employees.clear();
    if (!fileExists(filename)) return;
    std::string data = readAll(filename);
    std::istringstream iss(data);
    std::string line;
    while (std::getline(iss, line)) {
        line = trim(line);
        if (line.empty()) continue;
        auto parts = split(line, '|');
        // Формат: id|posID|fullName|birthDate|address|contactInfo|startDate|status|departmentID
        if (parts.size() < 9) continue;
        Employee e;
        e.id = std::stol(parts[0]);
        e.posID = std::stol(parts[1]);
        e.fullName = parts[2];
        e.birthDate = parts[3];
        e.address = parts[4];
        e.contactInfo = parts[5];
        e.startDate = parts[6];
        e.status = parts[7];
        e.departmentID = std::stol(parts[8]);
        employees.push_back(e);
    }
}

void EmployeeManager::save() {
    std::ostringstream oss;
    for (auto& e : employees) {
        oss << e.id << "|" << e.posID << "|" << e.fullName << "|" << e.birthDate << "|" << e.address << "|" << e.contactInfo << "|" << e.startDate << "|" << e.status << "|" << e.departmentID << "\n";
    }
    writeAll(filename, oss.str());
}

bool EmployeeManager::addEmployee(long posID, const std::string& fullName, const std::string& birthDate, const std::string& address, const std::string& contactInfo, const std::string& startDate, const std::string& status) {
    std::lock_guard<std::mutex> lock(mtx);
    // Назначаем ID автоматически: следующий ID = текущий размер + 1
    long newID = (long)employees.size() + 1;
    // Проверять, что такого ID нет не нужно, он всегда уникален
    Employee emp;
    emp.id = newID;
    emp.posID = posID;
    emp.fullName = fullName;
    emp.birthDate = birthDate;
    emp.address = address;
    emp.contactInfo = contactInfo;
    emp.startDate = startDate;
    emp.status = status;
    emp.departmentID = 0;
    employees.push_back(emp);
    save();
    return true;
}

bool EmployeeManager::deleteEmployee(long id) {
    std::lock_guard<std::mutex> lock(mtx);
    int index = -1;
    for (int i = 0; i < (int)employees.size(); i++) {
        if (employees[i].id == id) {
            index = i;
            break;
        }
    }
    if (index == -1) return false;
    employees.erase(employees.begin() + index);
    // После удаления пересчитаем ID
    for (int i = 0; i < (int)employees.size(); i++) {
        employees[i].id = i + 1;
    }
    save();
    return true;
}


bool EmployeeManager::updateEmployeeData(long id, const std::string& fullName, const std::string& address, const std::string& contactInfo) {
    std::lock_guard<std::mutex> lock(mtx);
    for (auto& e : employees) {
        if (e.id == id) {
            e.fullName = fullName;
            e.address = address;
            e.contactInfo = contactInfo;
            save();
            return true;
        }
    }
    return false;
}

bool EmployeeManager::updateEmployeeFullData(long id, const std::string& fullName, const std::string& address, const std::string& contactInfo, const std::string& status) {
    std::lock_guard<std::mutex> lock(mtx);
    for (auto& e : employees) {
        if (e.id == id) {
            e.fullName = fullName;
            e.address = address;
            e.contactInfo = contactInfo;
            e.status = status;
            save();
            return true;
        }
    }
    return false;
}

bool EmployeeManager::blockEmployee(long id) {
    std::lock_guard<std::mutex> lock(mtx);
    for (auto& e : employees) {
        if (e.id == id) {
            e.status = "blocked";
            save();
            return true;
        }
    }
    return false;
}

Employee EmployeeManager::getEmployee(long id) {
    std::lock_guard<std::mutex> lock(mtx);
    for (auto& e : employees) {
        if (e.id == id) return e;
    }
    return Employee{ 0,0,"","","","","","",0 };
}

std::vector<Employee> EmployeeManager::getAllEmployees() {
    std::lock_guard<std::mutex> lock(mtx);
    return employees;
}

bool EmployeeManager::setEmployeeDepartment(long empID, long deptID) {
    std::lock_guard<std::mutex> lock(mtx);
    for (auto& e : employees) {
        if (e.id == empID) {
            e.departmentID = deptID;
            save();
            return true;
        }
    }
    return false;
}
