// Server/RoleValidator.h
#pragma once
#include <string>

class RoleValidator {
public:
    bool isAllowed(const std::string& role, const std::string& command);
};
