#pragma once
#include <string>
#include "Logger.h"
#include "UserManager.h"
#include "RoleValidator.h"
#include "HistoryManager.h"
#include "EmployeeManager.h"
#include "PositionManager.h"
#include "CourseManager.h"
#include "CompensationManager.h"
#include "SalaryManager.h"
#include "ScheduleManager.h"
#include "JobResponsibilityManager.h"
#include "DepartmentManager.h"
#include "PerformanceEvaluationManager.h"
#include "ConnectionManager.h"

class RequestHandler {
private:
    Logger logger;
    UserManager userManager;
    EmployeeManager employeeManager;
    PositionManager positionManager;
    CourseManager courseManager;
    CompensationManager compensationManager;
    SalaryManager salaryManager;
    ScheduleManager scheduleManager;
    JobResponsibilityManager jobResponsibilityManager;
    DepartmentManager departmentManager;
    PerformanceEvaluationManager performanceEvaluationManager;
    HistoryManager historyManager;
    RoleValidator roleValidator;
    ConnectionManager connectionManager;
public:
    RequestHandler(const std::string& logFilename,
        const std::string& userFilename,
        const std::string& employeeFilename,
        const std::string& positionFilename,
        const std::string& courseFilename,
        const std::string& compensationFilename,
        const std::string& salaryFilename,
        const std::string& scheduleFilename,
        const std::string& responsibilityFilename,
        const std::string& departmentFilename,
        const std::string& evaluationFilename,
        const std::string& historyFilename);

    std::string processRequest(const std::string& request, const std::string& username, const std::string& role);
    UserManager& getUserManager();
    ConnectionManager& getConnectionManager() { return connectionManager; }
};
