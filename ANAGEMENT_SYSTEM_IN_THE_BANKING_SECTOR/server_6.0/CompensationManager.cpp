#include "CompensationManager.h"

// Конструктор
CompensationManager::CompensationManager() {}

// Добавление заявки на компенсацию
bool CompensationManager::addCompensationRequest(const std::string& employeeId, const std::string& requestDetails) {
    std::string requestId = std::to_string(std::hash<std::string>{}(employeeId + requestDetails));
    dataManager.writeFile("data/заявки_компенсаций.txt", requestId + "|" + employeeId + "|" + requestDetails + "|Ожидает");
    return true;
}

// Получение всех заявок
std::vector<std::string> CompensationManager::getAllRequests() {
    return dataManager.readFile("data/заявки_компенсаций.txt");
}

// Обработка заявки
bool CompensationManager::processRequest(const std::string& requestId, const std::string& status) {
    std::vector<std::string> requests = dataManager.readFile("data/заявки_компенсаций.txt");
    bool updated = false;
    for (auto& line : requests) {
        if (line.find(requestId + "|") == 0) {
            size_t lastDelimiter = line.rfind('|');
            line = line.substr(0, lastDelimiter + 1) + status;
            updated = true;
        }
    }
    if (updated) {
        std::ofstream file("data/заявки_компенсаций.txt", std::ios::trunc);
        for (const auto& request : requests) {
            file << request << std::endl;
        }
        file.close();
    }
    return updated;
}
