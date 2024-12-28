// JobResponsibilityManager.cpp
#include "JobResponsibilityManager.h"
#include "utils.h"
#include <fstream>
#include <sstream>
JobResponsibilityManager::JobResponsibilityManager(const std::string& filename) : filename(filename) {
    load();
}

void JobResponsibilityManager::load() {
    responsibilities.clear();
    if (!fileExists(filename)) return;
    std::string data = readAll(filename);
    std::istringstream iss(data);
    std::string line;
    while (std::getline(iss, line)) {
        line = trim(line);
        if (line.empty()) continue;
        auto parts = split(line, '|');
        if (parts.size() < 2) continue;
        Responsibility r;
        r.positionID = std::stol(parts[0]);
        r.responsibility = parts[1];
        responsibilities.push_back(r);
    }
}

void JobResponsibilityManager::save() {
    std::ostringstream oss;
    for (auto& r : responsibilities) {
        oss << r.positionID << "|" << r.responsibility << "\n";
    }
    writeAll(filename, oss.str());
}

bool JobResponsibilityManager::addResponsibility(long posID, const std::string& resp) {
    std::lock_guard<std::mutex> lock(mtx);
    Responsibility r{ posID,resp };
    responsibilities.push_back(r);
    save();
    return true;
}

std::string JobResponsibilityManager::getResponsibilities(long posID) {
    std::lock_guard<std::mutex> lock(mtx);
    std::ostringstream oss;
    for (auto& r : responsibilities) {
        if (r.positionID == posID) oss << r.responsibility << "\n";
    }
    return oss.str();
}
