#include "i18n.h"
#include <unordered_map>

static Language currentLanguage = Language::EN;

struct Translation {
    std::unordered_map<std::string, std::string> en;
    std::unordered_map<std::string, std::string> ru;
    std::unordered_map<std::string, std::string> be;
};

static Translation T = {
    // Английский
    {
        {"WELCOME", "Welcome"},
        {"ACCOUNT_CREATED", "Account created"},
        {"ERROR_LOGIN", "Invalid login or password"},
        {"ERROR_BLOCKED", "Account is blocked"},
        {"SUCCESS_BALANCE", "Your balance:"},
        {"EMPLOYEE_ADDED", "Employee added"},
        {"ERROR_GENERIC", "Error occurred"}
    },
    // Русский
    {
        {"WELCOME", u8"Добро пожаловать"},
        {"ACCOUNT_CREATED", u8"Аккаунт создан"},
        {"ERROR_LOGIN", u8"Неверный логин или пароль"},
        {"ERROR_BLOCKED", u8"Аккаунт заблокирован"},
        {"SUCCESS_BALANCE", u8"Ваш баланс:"},
        {"EMPLOYEE_ADDED", u8"Сотрудник добавлен"},
        {"ERROR_GENERIC", u8"Произошла ошибка"}
    },
    // Белорусский
    {
        {"WELCOME", u8"Сардэчна запрашаем"},
        {"ACCOUNT_CREATED", u8"Рахунак створаны"},
        {"ERROR_LOGIN", u8"Няправільны лагін альбо пароль"},
        {"ERROR_BLOCKED", u8"Рахунак заблакаваны"},
        {"SUCCESS_BALANCE", u8"Ваш баланс:"},
        {"EMPLOYEE_ADDED", u8"Супрацоўнік дададзены"},
        {"ERROR_GENERIC", u8"Адбылася памылка"}
    }
};


void setLanguage(Language lang) { currentLanguage = lang; }
Language getLanguage() { return currentLanguage; }
std::string tr(const std::string& key) {
    switch (currentLanguage) {
    case Language::EN:if (T.en.count(key))return T.en.at(key); break;
    case Language::RU:if (T.ru.count(key))return T.ru.at(key); break;
    case Language::BE:if (T.be.count(key))return T.be.at(key); break;
    }
    return key;
}
