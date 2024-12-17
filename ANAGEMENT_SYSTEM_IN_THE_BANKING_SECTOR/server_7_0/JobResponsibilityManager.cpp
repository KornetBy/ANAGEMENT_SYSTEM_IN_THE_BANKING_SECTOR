// JobResponsibilityManager.cpp
#include "JobResponsibilityManager.h"
#include <fstream>
#include <sstream>

JobResponsibilityManager::JobResponsibilityManager(const std::string& filename) : responsibilityFileName(filename) {
    std::lock_guard<std::mutex> lock(mtx);
    std::ifstream ifs(responsibilityFileName);
    if (!ifs.is_open()) return;

    std::string line;
    while (getline(ifs, line)) {
        std::istringstream iss(line);
        std::string token;
        JobResponsibility resp;
        getline(iss, token, '|');
        resp.responsibilityID = std::stol(token);
        getline(iss, token, '|');
        resp.employeeID = std::stol(token);
        getline(iss, token, '|');
        resp.positionID = std::stol(token);
        getline(iss, resp.responsibilityDescription, '|');
        responsibilities.push_back(resp);
    }
    ifs.close();
}

JobResponsibility JobResponsibilityManager::getResponsibility(long responsibilityID) {
    std::lock_guard<std::mutex> lock(mtx);
    for (const auto& resp : responsibilities) {
        if (resp.responsibilityID == responsibilityID) return resp;
    }
    return JobResponsibility{ 0, 0, 0, "" };
}

bool JobResponsibilityManager::addResponsibility(const JobResponsibility& responsibility) {
    std::lock_guard<std::mutex> lock(mtx);
    for (const auto& resp : responsibilities) {
        if (resp.responsibilityID == responsibility.responsibilityID) return false; // Обязанность уже существует
    }
    responsibilities.push_back(responsibility);

    std::ofstream ofs(responsibilityFileName, std::ios::app);
    if (!ofs.is_open()) return false;
    ofs << responsibility.responsibilityID << "|" << responsibility.employeeID << "|"
        << responsibility.positionID << "|" << responsibility.responsibilityDescription << "\n";
    ofs.close();
    return true;
}

bool JobResponsibilityManager::updateResponsibility(const JobResponsibility& responsibility) {
    std::lock_guard<std::mutex> lock(mtx);
    bool found = false;
    for (auto& resp : responsibilities) {
        if (resp.responsibilityID == responsibility.responsibilityID) {
            resp = responsibility;
            found = true;
            break;
        }
    }
    if (!found) return false;

    // Перезапись файла
    std::ofstream ofs(responsibilityFileName, std::ios::trunc);
    if (!ofs.is_open()) return false;
    for (const auto& resp : responsibilities) {
        ofs << resp.responsibilityID << "|" << resp.employeeID << "|"
            << resp.positionID << "|" << resp.responsibilityDescription << "\n";
    }
    ofs.close();
    return true;
}

bool JobResponsibilityManager::deleteResponsibility(long responsibilityID) {
    std::lock_guard<std::mutex> lock(mtx);
    bool found = false;
    std::vector<JobResponsibility> updatedResponsibilities;
    for (const auto& resp : responsibilities) {
        if (resp.responsibilityID != responsibilityID) {
            updatedResponsibilities.push_back(resp);
        }
        else {
            found = true;
        }
    }
    if (!found) return false;
    responsibilities = updatedResponsibilities;

    // Перезапись файла
    std::ofstream ofs(responsibilityFileName, std::ios::trunc);
    if (!ofs.is_open()) return false;
    for (const auto& resp : responsibilities) {
        ofs << resp.responsibilityID << "|" << resp.employeeID << "|"
            << resp.positionID << "|" << resp.responsibilityDescription << "\n";
    }
    ofs.close();
    return true;
}

std::vector<JobResponsibility> JobResponsibilityManager::getAllResponsibilities() {
    std::lock_guard<std::mutex> lock(mtx);
    return responsibilities;
}
