// DepartmentManager.cpp
#include "DepartmentManager.h"
#include "utils.h"
#include <fstream>
#include <sstream>
DepartmentManager::DepartmentManager(const std::string& filename) : filename(filename) {
    load();
}

void DepartmentManager::load() {
    departments.clear();
    if (!fileExists(filename)) return;
    std::string data = readAll(filename);
    std::istringstream iss(data);
    std::string line;
    while (std::getline(iss, line)) {
        line = trim(line);
        if (line.empty()) continue;
        auto parts = split(line, '|');
        if (parts.size() < 2) continue;
        Department d;
        d.id = std::stol(parts[0]);
        d.name = parts[1];
        departments.push_back(d);
    }
}

void DepartmentManager::save() {
    std::ostringstream oss;
    for (auto& d : departments) {
        oss << d.id << "|" << d.name << "\n";
    }
    writeAll(filename, oss.str());
}

bool DepartmentManager::addDepartment(long id, const std::string& name) {
    std::lock_guard<std::mutex> lock(mtx);
    for (auto& d : departments) {
        if (d.id == id) return false;
    }
    Department dep{ id,name };
    departments.push_back(dep);
    save();
    return true;
}

std::string DepartmentManager::getDepartmentName(long id) {
    std::lock_guard<std::mutex> lock(mtx);
    for (auto& d : departments) {
        if (d.id == id) return d.name;
    }
    return "Неизвестно";
}
