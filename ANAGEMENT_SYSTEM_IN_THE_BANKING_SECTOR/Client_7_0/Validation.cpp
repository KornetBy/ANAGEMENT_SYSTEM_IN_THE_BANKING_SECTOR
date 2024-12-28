#include "Validation.h"
#include <cctype>
#include <regex>

bool Validation::isValidFullName(const std::string& fullName) {
    for (char c : fullName) {
        if (!std::isalpha(c) && !std::isspace(c)) {
            return false;
        }
    }
    return true;
}

bool Validation::isValidPassword(const std::string& password) {
    bool hasLetter = false, hasDigit = false, hasSpecial = false;
    for (char c : password) {
        if (std::isalpha(c)) hasLetter = true;
        else if (std::isdigit(c)) hasDigit = true;
        else if (!std::isspace(c)) hasSpecial = true;
        else return false; // Содержит пробел
    }
    return hasLetter && hasDigit && hasSpecial;
}

bool Validation::isValidDate(const std::string& date) {
    // Проверка формата YYYY-MM-DD
    return std::regex_match(date, std::regex(R"(\d{4}-\d{2}-\d{2})"));
}
