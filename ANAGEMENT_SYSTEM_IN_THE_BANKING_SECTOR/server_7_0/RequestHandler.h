// RequestHandler.h
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

class RequestHandler {
private:
    Logger logger;
    UserManager userManager;
    RoleValidator roleValidator;
    HistoryManager historyManager;
    EmployeeManager employeeManager;
    PositionManager positionManager;
    CourseManager courseManager;
    CompensationManager compensationManager;
    SalaryManager salaryManager;
    ScheduleManager scheduleManager;
    JobResponsibilityManager jobResponsibilityManager;
    DepartmentManager departmentManager;
    PerformanceEvaluationManager performanceEvaluationManager;
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
    // RequestHandler.h
    std::string processRequest(const std::string& request, const std::string& username, const std::string& role);


    // Публичный геттер для userManager
    UserManager& getUserManager() { return userManager; }
};
