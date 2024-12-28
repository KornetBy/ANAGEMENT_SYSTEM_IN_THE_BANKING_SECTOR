#include "PositionManager.h"
#include "utils.h"
#include <fstream>
#include <sstream>
PositionManager::PositionManager(const std::string& filename) : filename(filename) {
    load();
}

void PositionManager::load() {
    positions.clear();
    if (!fileExists(filename)) return;
    std::string data = readAll(filename);
    std::istringstream iss(data);
    std::string line;
    while (std::getline(iss, line)) {
        line = trim(line);
        if (line.empty()) continue;
        auto parts = split(line, '|');
        if (parts.size() < 5) continue;
        Position p;
        p.id = std::stol(parts[0]);
        p.name = parts[1];
        p.minSalary = std::stod(parts[2]);
        p.maxSalary = std::stod(parts[3]);
        p.qualifications = parts[4];
        positions.push_back(p);
    }
}

void PositionManager::save() {
    std::ostringstream oss;
    for (auto& p : positions) {
        oss << p.id << "|" << p.name << "|" << p.minSalary << "|" << p.maxSalary << "|" << p.qualifications << "\n";
    }
    writeAll(filename, oss.str());
}

bool PositionManager::addPosition(long id, const std::string& name, double minSal, double maxSal, const std::string& qual) {
    std::lock_guard<std::mutex> lock(mtx);
    for (auto& p : positions) {
        if (p.id == id) return false;
    }
    Position pos{ id,name,minSal,maxSal,qual };
    positions.push_back(pos);
    save();
    return true;
}

Position PositionManager::getPosition(long id) {
    std::lock_guard<std::mutex> lock(mtx);
    for (auto& p : positions) {
        if (p.id == id) return p;
    }
    return Position{ 0,"",0.0,0.0,"" };
}

std::string PositionManager::getPositionName(long id) {
    auto p = getPosition(id);
    return p.id == 0 ? "Неизвестно" : p.name;
}
