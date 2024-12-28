// RoleValidator.cpp
#include "RoleValidator.h"

RoleValidator::RoleValidator() {
    roleCommands["admin"] = { "ADD_EMPLOYEE","UPDATE_EMPLOYEE","BLOCK_EMPLOYEE","VIEW_HISTORY","ADD_POSITION","VIEW_ACTIVE_COURSES","VIEW_ACTIVE_CONNECTIONS","ADD_USER","DELETE_USER","VIEW_ALL_EMPLOYEES" };
    roleCommands["hr"] = { "ADD_EMPLOYEE","UPDATE_EMPLOYEE","ADD_COURSE","UPDATE_COURSE","DELETE_COURSE","VIEW_ACTIVE_COURSES","VIEW_LOGS","ADD_WORK_SCHEDULE","UPDATE_WORK_SCHEDULE","DELETE_WORK_SCHEDULE","VIEW_WORK_SCHEDULE","APPROVE_COMPENSATION","DECLINE_COMPENSATION" };
    roleCommands["cashier"] = { "VIEW_SALARIES","PROCESS_COMPENSATION","VIEW_ACTIVE_COURSES" };
    roleCommands["manager"] = { "ADD_WORK_SCHEDULE","UPDATE_WORK_SCHEDULE","DELETE_WORK_SCHEDULE","VIEW_WORK_SCHEDULE","EVALUATE_PERFORMANCE","VIEW_ACTIVE_COURSES" };
    roleCommands["employee"] = { "VIEW_OWN_INFO","VIEW_OWN_SCHEDULE","ENROLL_COURSE","REQUEST_COMPENSATION" };
}

bool RoleValidator::isAllowed(const std::string& role, const std::string& command) {
    if (roleCommands.find(role) == roleCommands.end()) return false;
    for (auto& c : roleCommands[role]) {
        if (c == command) return true;
    }
    return false;
}
