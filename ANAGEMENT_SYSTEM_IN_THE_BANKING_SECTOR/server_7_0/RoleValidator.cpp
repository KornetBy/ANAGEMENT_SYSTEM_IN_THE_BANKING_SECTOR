// RoleValidator.cpp
#include "RoleValidator.h"

RoleValidator::RoleValidator() {
    // »нициализаци€ разрешенных команд дл€ каждой роли
    roleCommands["admin"] = {
        "ADD_EMPLOYEE", "EDIT_EMPLOYEE", "BLOCK_EMPLOYEE",
        "VIEW_HISTORY", "ADD_POSITION", "VIEW_ACTIVE_COURSES",
        "VIEW_ACTIVE_CONNECTIONS", "ADD_USER", "DELETE_USER",
        "VIEW_SALARIES", "VIEW_LOGS", "VIEW_ACTIVE_CONNECTIONS"
    };
    roleCommands["hr"] = {
        "ADD_EMPLOYEE", "EDIT_EMPLOYEE", "ADD_COURSE",
        "EDIT_COURSE", "DELETE_COURSE", "VIEW_ACTIVE_COURSES",
        "VIEW_LOGS", "MANAGE_SCHEDULES", "PROCESS_COMPENSATION",
        "VIEW_HISTORY"
    };
    roleCommands["cashier"] = {
        "VIEW_SALARIES", "PROCESS_COMPENSATION", "VIEW_ACTIVE_COURSES"
    };
    roleCommands["manager"] = {
        "MANAGE_SCHEDULES", "EVALUATE_PERFORMANCE", "VIEW_ACTIVE_COURSES"
    };
    roleCommands["employee"] = {
        "VIEW_OWN_INFO", "VIEW_OWN_SCHEDULE", "ENROLL_COURSE",
        "REQUEST_COMPENSATION"
    };
}

bool RoleValidator::isAllowed(const std::string& role, const std::string& command) {
    if (roleCommands.find(role) == roleCommands.end()) return false;
    const auto& commands = roleCommands[role];
    for (const auto& cmd : commands) {
        if (cmd == command) return true;
    }
    return false;
}
