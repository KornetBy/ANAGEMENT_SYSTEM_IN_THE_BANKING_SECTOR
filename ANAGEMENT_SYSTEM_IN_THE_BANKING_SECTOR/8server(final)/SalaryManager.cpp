// SalaryManager.cpp
#include "SalaryManager.h"
#include "utils.h"
#include <fstream>
#include <sstream>

SalaryManager::SalaryManager(const std::string& filename) : filename(filename) {
    load();
}

void SalaryManager::load() {
    salaries.clear();
    if (!fileExists(filename)) return;
    std::string data = readAll(filename);
    std::istringstream iss(data);
    std::string line;
    while (std::getline(iss, line)) {
        line = trim(line);
        if (line.empty()) continue;
        auto parts = split(line, '|');
        if (parts.size() < 2) continue;
        SalaryRecord sr;
        sr.employeeID = std::stol(parts[0]);
        sr.salary = std::stod(parts[1]);
        salaries.push_back(sr);
    }
}

void SalaryManager::save() {
    std::ostringstream oss;
    for (auto& s : salaries) {
        oss << s.employeeID << "|" << s.salary << "\n";
    }
    writeAll(filename, oss.str());
}

bool SalaryManager::setSalary(long employeeID, double salary) {
    std::lock_guard<std::mutex> lock(mtx);
    for (auto& s : salaries) {
        if (s.employeeID == employeeID) {
            s.salary = salary;
            save();
            return true;
        }
    }
    SalaryRecord sr{ employeeID, salary };
    salaries.push_back(sr);
    save();
    return true;
}

double SalaryManager::getSalary(long employeeID) {
    std::lock_guard<std::mutex> lock(mtx);
    for (auto& s : salaries) {
        if (s.employeeID == employeeID) return s.salary;
    }
    return 0.0;
}

std::string SalaryManager::getAllSalariesFormatted() {
    std::lock_guard<std::mutex> lock(mtx);
    std::ostringstream oss;
    for (auto& s : salaries) {
        oss << "ID: " << s.employeeID << ", Зарплата: " << s.salary << "\n";
    }
    return oss.str();
}
