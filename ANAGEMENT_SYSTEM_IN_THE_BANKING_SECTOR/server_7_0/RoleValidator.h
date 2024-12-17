// RoleValidator.h
#pragma once
#include <string>
#include <unordered_map>
#include <vector>

class RoleValidator {
private:
    std::unordered_map<std::string, std::vector<std::string>> roleCommands;
public:
    RoleValidator();
    bool isAllowed(const std::string& role, const std::string& command);
};
