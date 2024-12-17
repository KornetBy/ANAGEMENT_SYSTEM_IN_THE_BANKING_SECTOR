// SalaryManager.cpp
#include "SalaryManager.h"
#include <fstream>
#include <sstream>

SalaryManager::SalaryManager(const std::string& filename) : salaryFileName(filename) {
    std::lock_guard<std::mutex> lock(mtx);
    std::ifstream ifs(salaryFileName);
    if (!ifs.is_open()) return;

    std::string line;
    while (getline(ifs, line)) {
        std::istringstream iss(line);
        std::string token;
        Salary sal;
        getline(iss, token, '|');
        sal.salaryID = std::stol(token);
        getline(iss, token, '|');
        sal.employeeID = std::stol(token);
        getline(iss, token, '|');
        sal.positionID = std::stol(token);
        getline(iss, token, '|');
        sal.baseSalary = std::stod(token);
        getline(iss, token, '|');
        sal.bonuses = std::stod(token);
        getline(iss, token, '|');
        sal.deductions = std::stod(token);
        getline(iss, token, '|');
        sal.netPay = std::stod(token);
        getline(iss, sal.calculationDate, '|');
        salaries.push_back(sal);
    }
    ifs.close();
}

Salary SalaryManager::getSalary(long salaryID) {
    std::lock_guard<std::mutex> lock(mtx);
    for (const auto& sal : salaries) {
        if (sal.salaryID == salaryID) return sal;
    }
    return Salary{ 0, 0, 0, 0.0, 0.0, 0.0, 0.0, "" };
}

bool SalaryManager::addSalary(const Salary& salary) {
    std::lock_guard<std::mutex> lock(mtx);
    for (const auto& sal : salaries) {
        if (sal.salaryID == salary.salaryID) return false; // Зарплата уже существует
    }
    salaries.push_back(salary);

    std::ofstream ofs(salaryFileName, std::ios::app);
    if (!ofs.is_open()) return false;
    ofs << salary.salaryID << "|" << salary.employeeID << "|" << salary.positionID << "|"
        << salary.baseSalary << "|" << salary.bonuses << "|" << salary.deductions << "|"
        << salary.netPay << "|" << salary.calculationDate << "\n";
    ofs.close();
    return true;
}

bool SalaryManager::updateSalary(const Salary& salary) {
    std::lock_guard<std::mutex> lock(mtx);
    bool found = false;
    for (auto& sal : salaries) {
        if (sal.salaryID == salary.salaryID) {
            sal = salary;
            found = true;
            break;
        }
    }
    if (!found) return false;

    // Перезапись файла
    std::ofstream ofs(salaryFileName, std::ios::trunc);
    if (!ofs.is_open()) return false;
    for (const auto& sal : salaries) {
        ofs << sal.salaryID << "|" << sal.employeeID << "|" << sal.positionID << "|"
            << sal.baseSalary << "|" << sal.bonuses << "|" << sal.deductions << "|"
            << sal.netPay << "|" << sal.calculationDate << "\n";
    }
    ofs.close();
    return true;
}

bool SalaryManager::deleteSalary(long salaryID) {
    std::lock_guard<std::mutex> lock(mtx);
    bool found = false;
    std::vector<Salary> updatedSalaries;
    for (const auto& sal : salaries) {
        if (sal.salaryID != salaryID) {
            updatedSalaries.push_back(sal);
        }
        else {
            found = true;
        }
    }
    if (!found) return false;
    salaries = updatedSalaries;

    // Перезапись файла
    std::ofstream ofs(salaryFileName, std::ios::trunc);
    if (!ofs.is_open()) return false;
    for (const auto& sal : salaries) {
        ofs << sal.salaryID << "|" << sal.employeeID << "|" << sal.positionID << "|"
            << sal.baseSalary << "|" << sal.bonuses << "|" << sal.deductions << "|"
            << sal.netPay << "|" << sal.calculationDate << "\n";
    }
    ofs.close();
    return true;
}

std::vector<Salary> SalaryManager::getAllSalaries() {
    std::lock_guard<std::mutex> lock(mtx);
    return salaries;
}
