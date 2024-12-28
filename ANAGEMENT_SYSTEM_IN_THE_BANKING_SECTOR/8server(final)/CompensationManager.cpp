// CompensationManager.cpp
#include "CompensationManager.h"
#include "utils.h"
#include <fstream>
#include <sstream>
CompensationManager::CompensationManager(const std::string& filename) : filename(filename) {
    load();
}

void CompensationManager::load() {
    requests.clear();
    if (!fileExists(filename)) return;
    std::string data = readAll(filename);
    std::istringstream iss(data);
    std::string line;
    while (std::getline(iss, line)) {
        line = trim(line);
        if (line.empty()) continue;
        auto parts = split(line, '|');
        // Формат: id|employeeID|positionID|type|amount|date|status
        if (parts.size() < 7) continue;
        CompensationRequest r;
        r.id = std::stol(parts[0]);
        r.employeeID = std::stol(parts[1]);
        r.positionID = std::stol(parts[2]);
        r.type = parts[3];
        r.amount = std::stod(parts[4]);
        r.submissionDate = parts[5];
        r.status = parts[6];
        requests.push_back(r);
    }
}

void CompensationManager::save() {
    std::ostringstream oss;
    for (auto& r : requests) {
        oss << r.id << "|" << r.employeeID << "|" << r.positionID << "|" << r.type << "|" << r.amount << "|" << r.submissionDate << "|" << r.status << "\n";
    }
    writeAll(filename, oss.str());
}

bool CompensationManager::addRequest(long id, long empID, long posID, const std::string& type, double amount, const std::string& date, const std::string& status) {
    std::lock_guard<std::mutex> lock(mtx);
    for (auto& r : requests) {
        if (r.id == id) return false;
    }
    CompensationRequest req{ id, empID, posID, type, amount, date, status };
    requests.push_back(req);
    save();
    return true;
}

bool CompensationManager::approveRequest(long id) {
    std::lock_guard<std::mutex> lock(mtx);
    for (auto& r : requests) {
        if (r.id == id) {
            r.status = "Одобрено";
            save();
            return true;
        }
    }
    return false;
}

bool CompensationManager::declineRequest(long id) {
    std::lock_guard<std::mutex> lock(mtx);
    for (auto& r : requests) {
        if (r.id == id) {
            r.status = "Отклонено";
            save();
            return true;
        }
    }
    return false;
}

bool CompensationManager::processRequest(long id, const std::string& newStatus) {
    std::lock_guard<std::mutex> lock(mtx);
    for (auto& r : requests) {
        if (r.id == id) {
            r.status = newStatus;
            save();
            return true;
        }
    }
    return false;
}
