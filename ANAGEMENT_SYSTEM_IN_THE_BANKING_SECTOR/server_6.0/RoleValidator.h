#pragma once
#ifndef ROLE_VALIDATOR_H
#define ROLE_VALIDATOR_H

#include <string>
#include <unordered_map>
#include <vector>

class RoleValidator {
private:
    std::unordered_map<std::string, std::vector<std::string>> rolePermissions; // Роли и разрешённые команды

    void initializePermissions(); // Инициализация разрешений для ролей

public:
    RoleValidator();
    bool isAllowed(const std::string& role, const std::string& command); // Проверка разрешений
};

#endif // ROLE_VALIDATOR_H
