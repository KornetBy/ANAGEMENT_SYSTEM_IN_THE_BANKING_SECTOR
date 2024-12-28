#pragma once
#pragma once
#include <string>

class Validation {
public:
    static bool isValidFullName(const std::string& fullName);
    static bool isValidPassword(const std::string& password);
    static bool isValidDate(const std::string& date);
};
