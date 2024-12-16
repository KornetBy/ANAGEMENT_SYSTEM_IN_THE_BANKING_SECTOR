#pragma once
#ifndef COMPENSATION_MANAGER_H
#define COMPENSATION_MANAGER_H

#include <string>
#include <vector>
#include "DataManager.h"

class CompensationManager {
private:
    DataManager dataManager;

public:
    CompensationManager();

    // Добавление заявки на компенсацию
    bool addCompensationRequest(const std::string& employeeId, const std::string& requestDetails);

    // Получение всех заявок
    std::vector<std::string> getAllRequests();

    // Обработка заявки
    bool processRequest(const std::string& requestId, const std::string& status);
};

#endif // COMPENSATION_MANAGER_H
