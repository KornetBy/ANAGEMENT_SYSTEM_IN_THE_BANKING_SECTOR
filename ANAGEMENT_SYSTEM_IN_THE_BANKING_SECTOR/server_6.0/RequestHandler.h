#pragma once
#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include <string>
#include <vector>
#include "Logger.h"
#include "UserManager.h"
#include "RoleValidator.h"
#include "ScheduleManager.h"
#include "CompensationManager.h"

class RequestHandler {
private:
    Logger logger;
    UserManager userManager;
    RoleValidator roleValidator;
    ScheduleManager scheduleManager;
    CompensationManager compensationManager;

    std::string handleAuth(const std::vector<std::string>& parts);
    std::string handleAddEmployee(const std::vector<std::string>& parts);
    std::string handleBlockUser(const std::vector<std::string>& parts);
    std::string handleGetLogs();
    std::string handleAddPosition(const std::vector<std::string>& parts);
    std::string handleManageSchedules(const std::vector<std::string>& parts);
    std::string handleRequestCompensation(const std::vector<std::string>& parts);

public:
    RequestHandler();
    std::string processRequest(const std::string& request, const std::string& username, const std::string& role);
};

#endif // REQUEST_HANDLER_H
