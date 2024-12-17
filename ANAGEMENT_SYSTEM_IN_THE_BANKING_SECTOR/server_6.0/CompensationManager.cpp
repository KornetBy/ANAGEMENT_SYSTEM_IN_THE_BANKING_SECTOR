// Server/CompensationManager.cpp
#include "CompensationManager.h"
#include <fstream>
#include <sstream>
#include <algorithm>

CompensationManager::CompensationManager(const std::string& fileName) : compensationFileName(fileName) {
    loadRequests();
}

void CompensationManager::loadRequests() {
    std::lock_guard<std::mutex> lock(mtx);
    requests.clear();
    std::ifstream file(compensationFileName);
    if (!file.is_open()) {
        // Файл не существует, создадим пустой
        std::ofstream outfile(compensationFileName);
        outfile.close();
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string requestId, employeeName, compensationData, status;
        if (std::getline(iss, requestId, '|') &&
            std::getline(iss, employeeName, '|') &&
            std::getline(iss, compensationData, '|') &&
            std::getline(iss, status, '|')) {
            CompensationRequest req{ requestId, employeeName, compensationData, status };
            requests.push_back(req);
        }
    }
    file.close();
}

bool CompensationManager::addRequest(const CompensationRequest& request) {
    std::lock_guard<std::mutex> lock(mtx);
    // Проверяем, существует ли уже заявка с таким ID
    auto it = std::find_if(requests.begin(), requests.end(),
        [&](const CompensationRequest& r) { return r.requestId == request.requestId; });
    if (it != requests.end()) {
        return false; // Заявка уже существует
    }
    requests.push_back(request);
    // Сохраняем изменения в файл
    std::ofstream file(compensationFileName, std::ios::app);
    if (file.is_open()) {
        file << request.requestId << "|" << request.employeeName << "|" << request.compensationData << "|" << request.status << std::endl;
        file.close();
        return true;
    }
    return false;
}

bool CompensationManager::processRequest(const std::string& requestId) {
    std::lock_guard<std::mutex> lock(mtx);
    for (auto& req : requests) {
        if (req.requestId == requestId) {
            req.status = "processed";
            // Перезаписываем файл
            std::ofstream file(compensationFileName, std::ios::trunc);
            if (file.is_open()) {
                for (const auto& r : requests) {
                    file << r.requestId << "|" << r.employeeName << "|" << r.compensationData << "|" << r.status << std::endl;
                }
                file.close();
                return true;
            }
            return false;
        }
    }
    return false; // Заявка не найдена
}

std::vector<CompensationRequest> CompensationManager::getAllRequests() {
    std::lock_guard<std::mutex> lock(mtx);
    return requests;
}
