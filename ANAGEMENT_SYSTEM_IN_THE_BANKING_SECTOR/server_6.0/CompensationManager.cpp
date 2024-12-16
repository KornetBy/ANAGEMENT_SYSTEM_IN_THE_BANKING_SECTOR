#include "CompensationManager.h"

// �����������
CompensationManager::CompensationManager() {}

// ���������� ������ �� �����������
bool CompensationManager::addCompensationRequest(const std::string& employeeId, const std::string& requestDetails) {
    std::string requestId = std::to_string(std::hash<std::string>{}(employeeId + requestDetails));
    dataManager.writeFile("data/������_�����������.txt", requestId + "|" + employeeId + "|" + requestDetails + "|�������");
    return true;
}

// ��������� ���� ������
std::vector<std::string> CompensationManager::getAllRequests() {
    return dataManager.readFile("data/������_�����������.txt");
}

// ��������� ������
bool CompensationManager::processRequest(const std::string& requestId, const std::string& status) {
    std::vector<std::string> requests = dataManager.readFile("data/������_�����������.txt");
    bool updated = false;
    for (auto& line : requests) {
        if (line.find(requestId + "|") == 0) {
            size_t lastDelimiter = line.rfind('|');
            line = line.substr(0, lastDelimiter + 1) + status;
            updated = true;
        }
    }
    if (updated) {
        std::ofstream file("data/������_�����������.txt", std::ios::trunc);
        for (const auto& request : requests) {
            file << request << std::endl;
        }
        file.close();
    }
    return updated;
}
