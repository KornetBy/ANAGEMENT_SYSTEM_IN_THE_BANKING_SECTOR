// PositionManager.cpp
#include "PositionManager.h"
#include <fstream>
#include <sstream>

PositionManager::PositionManager(const std::string& filename) : positionFileName(filename) {
    std::lock_guard<std::mutex> lock(mtx);
    std::ifstream ifs(positionFileName);
    if (!ifs.is_open()) return;

    std::string line;
    while (getline(ifs, line)) {
        std::istringstream iss(line);
        std::string token;
        Position pos;
        getline(iss, token, '|');
        pos.positionID = std::stol(token);
        getline(iss, pos.positionName, '|');
        getline(iss, token, '|');
        pos.minSalary = std::stod(token);
        getline(iss, token, '|');
        pos.maxSalary = std::stod(token);
        getline(iss, pos.qualificationRequirements, '|');
        positions.push_back(pos);
    }
    ifs.close();
}

Position PositionManager::getPosition(long positionID) {
    std::lock_guard<std::mutex> lock(mtx);
    for (const auto& pos : positions) {
        if (pos.positionID == positionID) return pos;
    }
    return Position{ 0, "", 0.0, 0.0, "" };
}

bool PositionManager::addPosition(const Position& position) {
    std::lock_guard<std::mutex> lock(mtx);
    for (const auto& pos : positions) {
        if (pos.positionID == position.positionID) return false; // Должность уже существует
    }
    positions.push_back(position);

    std::ofstream ofs(positionFileName, std::ios::app);
    if (!ofs.is_open()) return false;
    ofs << position.positionID << "|" << position.positionName << "|"
        << position.minSalary << "|" << position.maxSalary << "|"
        << position.qualificationRequirements << "\n";
    ofs.close();
    return true;
}

bool PositionManager::updatePosition(const Position& position) {
    std::lock_guard<std::mutex> lock(mtx);
    bool found = false;
    for (auto& pos : positions) {
        if (pos.positionID == position.positionID) {
            pos = position;
            found = true;
            break;
        }
    }
    if (!found) return false;

    // Перезапись файла
    std::ofstream ofs(positionFileName, std::ios::trunc);
    if (!ofs.is_open()) return false;
    for (const auto& pos : positions) {
        ofs << pos.positionID << "|" << pos.positionName << "|"
            << pos.minSalary << "|" << pos.maxSalary << "|"
            << pos.qualificationRequirements << "\n";
    }
    ofs.close();
    return true;
}

bool PositionManager::deletePosition(long positionID) {
    std::lock_guard<std::mutex> lock(mtx);
    bool found = false;
    std::vector<Position> updatedPositions;
    for (const auto& pos : positions) {
        if (pos.positionID != positionID) {
            updatedPositions.push_back(pos);
        }
        else {
            found = true;
        }
    }
    if (!found) return false;
    positions = updatedPositions;

    // Перезапись файла
    std::ofstream ofs(positionFileName, std::ios::trunc);
    if (!ofs.is_open()) return false;
    for (const auto& pos : positions) {
        ofs << pos.positionID << "|" << pos.positionName << "|"
            << pos.minSalary << "|" << pos.maxSalary << "|"
            << pos.qualificationRequirements << "\n";
    }
    ofs.close();
    return true;
}

std::vector<Position> PositionManager::getAllPositions() {
    std::lock_guard<std::mutex> lock(mtx);
    return positions;
}
