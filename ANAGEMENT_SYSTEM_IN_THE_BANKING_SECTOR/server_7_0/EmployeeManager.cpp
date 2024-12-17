// EmployeeManager.cpp
#include "EmployeeManager.h"
#include <fstream>
#include <sstream>"
#include <iostream>
EmployeeManager::EmployeeManager(const std::string& filename) : employeeFileName(filename) {
    std::lock_guard<std::mutex> lock(mtx);
    std::ifstream ifs(employeeFileName);
    if (!ifs.is_open()) {
        std::cerr << "Не удалось открыть файл: " << employeeFileName << "\n";
        return;
    }

    std::string line;
    while (getline(ifs, line)) {
        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string token;
        Employee emp;

        try {
            getline(iss, token, '|');
            emp.employeeID = std::stol(token);

            getline(iss, token, '|');
            emp.positionID = std::stol(token);

            if (!getline(iss, emp.fullName, '|') ||
                !getline(iss, emp.birthDate, '|') ||
                !getline(iss, emp.address, '|') ||
                !getline(iss, emp.contactInfo, '|') ||
                !getline(iss, emp.startDate, '|') ||
                !getline(iss, emp.status, '|')) {
                throw std::runtime_error("Некорректный формат строки: " + line);
            }

            employees.push_back(emp);

        }
        catch (const std::exception& e) {
            std::cerr << "Ошибка при обработке строки: " << line << "\n"
                << "Детали: " << e.what() << "\n";
        }
    }
    ifs.close();
}


Employee EmployeeManager::getEmployee(long employeeID) {
    std::lock_guard<std::mutex> lock(mtx);
    for (const auto& emp : employees) {
        if (emp.employeeID == employeeID) return emp;
    }
    return Employee{ 0, 0, "", "", "", "", "", "" };
}

bool EmployeeManager::addEmployee(const Employee& employee) {
    std::lock_guard<std::mutex> lock(mtx);
    for (const auto& emp : employees) {
        if (emp.employeeID == employee.employeeID) return false; // Сотрудник уже существует
    }
    employees.push_back(employee);

    std::ofstream ofs(employeeFileName, std::ios::app);
    if (!ofs.is_open()) return false;
    ofs << employee.employeeID << "|" << employee.positionID << "|"
        << employee.fullName << "|" << employee.birthDate << "|"
        << employee.address << "|" << employee.contactInfo << "|"
        << employee.startDate << "|" << employee.status << "\n";
    ofs.close();
    return true;
}

bool EmployeeManager::editEmployee(const Employee& employee) {
    std::lock_guard<std::mutex> lock(mtx);
    bool found = false;
    for (auto& emp : employees) {
        if (emp.employeeID == employee.employeeID) {
            emp = employee;
            found = true;
            break;
        }
    }
    if (!found) return false;

    // Перезапись файла
    std::ofstream ofs(employeeFileName, std::ios::trunc);
    if (!ofs.is_open()) return false;
    for (const auto& emp : employees) {
        ofs << emp.employeeID << "|" << emp.positionID << "|"
            << emp.fullName << "|" << emp.birthDate << "|"
            << emp.address << "|" << emp.contactInfo << "|"
            << emp.startDate << "|" << emp.status << "\n";
    }
    ofs.close();
    return true;
}

bool EmployeeManager::blockEmployee(long employeeID) {
    std::lock_guard<std::mutex> lock(mtx);
    bool found = false;
    for (auto& emp : employees) {
        if (emp.employeeID == employeeID) {
            emp.status = "blocked";
            found = true;
            break;
        }
    }
    if (!found) return false;

    // Перезапись файла
    std::ofstream ofs(employeeFileName, std::ios::trunc);
    if (!ofs.is_open()) return false;
    for (const auto& emp : employees) {
        ofs << emp.employeeID << "|" << emp.positionID << "|"
            << emp.fullName << "|" << emp.birthDate << "|"
            << emp.address << "|" << emp.contactInfo << "|"
            << emp.startDate << "|" << emp.status << "\n";
    }
    ofs.close();
    return true;
}

std::vector<Employee> EmployeeManager::getAllEmployees() {
    std::lock_guard<std::mutex> lock(mtx);
    return employees;
}
