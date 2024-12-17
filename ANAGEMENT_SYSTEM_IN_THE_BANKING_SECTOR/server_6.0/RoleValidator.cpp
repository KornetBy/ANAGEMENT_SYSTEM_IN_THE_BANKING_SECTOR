// Server/RoleValidator.cpp
#include "RoleValidator.h"
#include <unordered_map>
#include <vector>
#include <algorithm>

bool RoleValidator::isAllowed(const std::string& role, const std::string& command) {
    // ќпредел€ем доступные команды дл€ каждой роли
    static std::unordered_map<std::string, std::vector<std::string>> rolePermissions = {
        {"admin", {"ADD_EMPLOYEE", "EDIT_EMPLOYEE", "BLOCK_USER", "GET_LOGS", "ADD_POSITION", "GET_ACTIVE_USERS", "MANAGE_SCHEDULES", "EVALUATE_PERFORMANCE", "GET_SALARIES", "PROCESS_COMPENSATION", "GET_SELF_INFO", "REQUEST_COMPENSATION", "ADD_USER", "DELETE_USER"}},
        {"hr", {"ADD_EMPLOYEE", "EDIT_EMPLOYEE", "GET_LOGS", "ADD_POSITION", "GET_ACTIVE_USERS"}},
        {"cashier", {"GET_SALARIES", "PROCESS_COMPENSATION"}},
        {"manager", {"MANAGE_SCHEDULES", "EVALUATE_PERFORMANCE"}},
        {"employee", {"GET_SELF_INFO", "REQUEST_COMPENSATION"}}
    };

    auto it = rolePermissions.find(role);
    if (it != rolePermissions.end()) {
        const auto& commands = it->second;
        return std::find(commands.begin(), commands.end(), command) != commands.end();
    }
    return false;
}
