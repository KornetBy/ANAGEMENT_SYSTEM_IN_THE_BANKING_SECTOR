#include "RoleValidator.h"

// Конструктор
RoleValidator::RoleValidator() {
    initializePermissions();
}

// Инициализация разрешений
void RoleValidator::initializePermissions() {
    rolePermissions["admin"] = { "ADD_EMPLOYEE", "BLOCK_USER", "GET_LOGS", "ADD_POSITION" };
    rolePermissions["hr"] = { "ADD_EMPLOYEE", "EDIT_EMPLOYEE", "ADD_POSITION" };
    rolePermissions["cashier"] = { "GET_SALARIES", "PROCESS_COMPENSATION" };
    rolePermissions["manager"] = { "MANAGE_SCHEDULES", "EVALUATE_PERFORMANCE" };
    rolePermissions["employee"] = { "GET_SELF_INFO", "REQUEST_COMPENSATION" };
}

// Проверка разрешений
bool RoleValidator::isAllowed(const std::string& role, const std::string& command) {
    if (rolePermissions.find(role) != rolePermissions.end()) {
        const auto& commands = rolePermissions[role];
        return std::find(commands.begin(), commands.end(), command) != commands.end();
    }
    return false;
}
