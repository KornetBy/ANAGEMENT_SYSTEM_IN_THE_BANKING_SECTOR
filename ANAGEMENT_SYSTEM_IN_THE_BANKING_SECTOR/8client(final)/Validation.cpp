// Validation.cpp
#include "Validation.h"
#include <cctype>
#include <regex>
#include <algorithm>
#include <cwctype>  // Для iswalpha, iswspace

bool Validation::isValidFullName(const std::string& fullName) {
    if (fullName.empty()) return false;

    bool spaceFound = false;

    // Преобразуем строку в wide string (wchar_t)
    std::wstring wFullName(fullName.begin(), fullName.end());

    for (size_t i = 0; i < wFullName.size(); ++i) {
        wchar_t c = wFullName[i];

        // Разрешаем буквы (кириллические и латинские) и пробелы
        if (!std::iswalpha(c) && !std::iswspace(c)) {
            return false;  // Если не буква или не пробел — невалидное имя
        }

        // Проверка на два пробела подряд или пробел в начале/конце
        if (std::iswspace(c)) {
            if (i == 0 || i == wFullName.size() - 1 || spaceFound) {
                return false;  // Пробел в начале, в конце или два пробела подряд
            }
            spaceFound = true;
        }
        else {
            spaceFound = false;  // Сброс, если это не пробел
        }
    }

    return true;
}


bool Validation::isValidPassword(const std::string& password) {
    bool hasLetter = false, hasDigit = false, hasSpecial = false;
    if (password.empty()) return false;
    for (auto c : password) {
        if (std::isalpha((unsigned char)c)) hasLetter = true;
        else if (std::isdigit((unsigned char)c)) hasDigit = true;
        else if (!std::isspace((unsigned char)c)) hasSpecial = true;
        else return false;
    }
    return hasLetter && hasDigit && hasSpecial;
}

bool Validation::isValidDate(const std::string& date) {
    return std::regex_match(date, std::regex(R"(\d{4}-\d{2}-\d{2})"));
}

std::string Validation::trim(const std::string& str) {
    auto start = str.begin();
    while (start != str.end() && std::isspace((unsigned char)*start)) start++;
    auto end = str.end();
    do {
        end--;
    } while (std::distance(start, end) > 0 && std::isspace((unsigned char)*end));
    return std::string(start, end + 1);
}
