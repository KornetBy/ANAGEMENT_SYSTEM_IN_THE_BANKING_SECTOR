// Server/RequestHandler.h
#pragma once
#include <string>
#include <vector>
#include "Logger.h"
#include "UserManager.h"
#include "RoleValidator.h"

class RequestHandler {
private:
    Logger logger;
    UserManager userManager;
    RoleValidator roleValidator;

    // Обработчики команд
    std::string handleAuth(const std::vector<std::string>& parts);
    std::string handleAddEmployee(const std::vector<std::string>& parts);
    std::string handleEditEmployee(const std::vector<std::string>& parts);
    std::string handleBlockUser(const std::vector<std::string>& parts);
    std::string handleGetLogs();
    std::string handleAddPosition(const std::vector<std::string>& parts);
    std::string handleGetActiveUsers();
    std::string handleManageSchedules(const std::vector<std::string>& parts);
    std::string handleEvaluatePerformance(const std::vector<std::string>& parts);
    std::string handleGetSalaries();
    std::string handleProcessCompensation(const std::vector<std::string>& parts);
    std::string handleGetSelfInfo(const std::string& username);
    std::string handleRequestCompensation(const std::vector<std::string>& parts);
    std::string handleAddUser(const std::vector<std::string>& parts);
    std::string handleDeleteUser(const std::vector<std::string>& parts);

public:
    RequestHandler();
    std::string processRequest(const std::string& request, const std::string& username, const std::string& role);
};
