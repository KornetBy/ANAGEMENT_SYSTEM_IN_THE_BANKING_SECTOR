// CompensationManager.cpp
#include "CompensationManager.h"
#include <fstream>
#include <sstream>

CompensationManager::CompensationManager(const std::string& filename) : compensationFileName(filename) {
    std::lock_guard<std::mutex> lock(mtx);
    std::ifstream ifs(compensationFileName);
    if (!ifs.is_open()) return;

    std::string line;
    while (getline(ifs, line)) {
        std::istringstream iss(line);
        std::string token;
        CompensationRequest req;
        getline(iss, token, '|');
        req.requestID = std::stol(token);
        getline(iss, token, '|');
        req.employeeID = std::stol(token);
        getline(iss, token, '|');
        req.positionID = std::stol(token);
        getline(iss, req.compensationType, '|');
        getline(iss, token, '|');
        req.amount = std::stod(token);
        getline(iss, req.submissionDate, '|');
        getline(iss, req.status, '|');
        requests.push_back(req);
    }
    ifs.close();
}

CompensationRequest CompensationManager::getRequest(long requestID) {
    std::lock_guard<std::mutex> lock(mtx);
    for (const auto& req : requests) {
        if (req.requestID == requestID) return req;
    }
    return CompensationRequest{ 0, 0, 0, "", 0.0, "", "" };
}

bool CompensationManager::addRequest(const CompensationRequest& request) {
    std::lock_guard<std::mutex> lock(mtx);
    for (const auto& req : requests) {
        if (req.requestID == request.requestID) return false; // Заявка уже существует
    }
    requests.push_back(request);

    std::ofstream ofs(compensationFileName, std::ios::app);
    if (!ofs.is_open()) return false;
    ofs << request.requestID << "|" << request.employeeID << "|"
        << request.positionID << "|" << request.compensationType << "|"
        << request.amount << "|" << request.submissionDate << "|"
        << request.status << "\n";
    ofs.close();
    return true;
}

bool CompensationManager::processRequest(long requestID, const std::string& newStatus) {
    std::lock_guard<std::mutex> lock(mtx);
    bool found = false;
    for (auto& req : requests) {
        if (req.requestID == requestID) {
            req.status = newStatus;
            found = true;
            break;
        }
    }
    if (!found) return false;

    // Перезапись файла
    std::ofstream ofs(compensationFileName, std::ios::trunc);
    if (!ofs.is_open()) return false;
    for (const auto& req : requests) {
        ofs << req.requestID << "|" << req.employeeID << "|"
            << req.positionID << "|" << req.compensationType << "|"
            << req.amount << "|" << req.submissionDate << "|"
            << req.status << "\n";
    }
    ofs.close();
    return true;
}

std::vector<CompensationRequest> CompensationManager::getAllRequests() {
    std::lock_guard<std::mutex> lock(mtx);
    return requests;
}
