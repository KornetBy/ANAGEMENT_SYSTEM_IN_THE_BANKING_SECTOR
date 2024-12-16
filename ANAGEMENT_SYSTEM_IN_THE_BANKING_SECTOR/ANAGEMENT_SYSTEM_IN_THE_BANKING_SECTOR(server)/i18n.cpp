#include "i18n.h"
#include <unordered_map>

static Language currentLanguage = Language::EN;

struct Translation {
    std::unordered_map<std::string, std::string> en;
    std::unordered_map<std::string, std::string> ru;
    std::unordered_map<std::string, std::string> be;
};

static Translation T = {
    // ����������
    {
        {"WELCOME", "Welcome"},
        {"USERNAME_PROMPT", "Username"},
        {"PASSWORD_PROMPT", "Password"},
        {"BLOCK_SELF_ERROR", "You cannot block your own account"},
        {"MENU_USER", "User Menu"},
        {"MENU_WORKER", "Worker Menu"},
        {"DEPOSIT", "Deposit"},
        {"WITHDRAW", "Withdraw"},
        {"TRANSFER", "Transfer"},
        {"VIEW_BALANCE", "View Balance"},
        {"VIEW_HISTORY", "View Transaction History"},
        {"CREATE_ACCOUNT", "Create Account"},
        {"DELETE_ACCOUNT", "Delete Account"},
        {"BLOCK_ACCOUNT", "Block Account"},
        {"UNBLOCK_ACCOUNT", "Unblock Account"},
        {"APPLY_INTEREST", "Apply Interest to Accounts"},
        {"SEARCH_BALANCE", "Search Accounts by Balance"},
        {"GENERATE_REPORT", "Generate Report"},
        {"CHOOSE_STATUS", "Choose user status"},
        {"ACTIVE", "Active"},
        {"BLOCKED", "Blocked"},
        {"LOGIN_FAILED", "Login failed. Try again."},
        {"LOGIN_SUCCESS", "Login successful!"},
        {"ATTEMPTS_LEFT", "attempts left"},
        {"TOO_MANY_ATTEMPTS", "Too many failed attempts. Exiting..."},
        {"LOGOUT", "Logout"},
        {"ERROR_GENERIC", "An error occurred"},
        {"USER_BLOCKED", "User successfully blocked"},
        {"NOT_AUTHORIZED", "Not authorized"},
        {"ROLE_UNKNOWN", "Unknown role"}
    },
    // �������
    {
        {"WELCOME", u8"����� ����������"},
        {"USERNAME_PROMPT", u8"��� ������������"},
        {"PASSWORD_PROMPT", u8"������"},
        {"BLOCK_SELF_ERROR", u8"�� �� ������ ������������� ���� ����������� �������"},
        {"MENU_USER", u8"���� ������������"},
        {"MENU_WORKER", u8"���� ���������"},
        {"DEPOSIT", u8"��������� ����"},
        {"WITHDRAW", u8"����� ��������"},
        {"TRANSFER", u8"��������� ��������"},
        {"VIEW_BALANCE", u8"����������� ������"},
        {"VIEW_HISTORY", u8"����������� ������� ����������"},
        {"CREATE_ACCOUNT", u8"������� �������"},
        {"DELETE_ACCOUNT", u8"������� �������"},
        {"BLOCK_ACCOUNT", u8"������������� �������"},
        {"UNBLOCK_ACCOUNT", u8"�������������� �������"},
        {"APPLY_INTEREST", u8"��������� ��������"},
        {"SEARCH_BALANCE", u8"����� ��������� �� �������"},
        {"GENERATE_REPORT", u8"������������ �����"},
        {"CHOOSE_STATUS", u8"�������� ������ ������������"},
        {"ACTIVE", u8"�������"},
        {"BLOCKED", u8"������������"},
        {"LOGIN_FAILED", u8"������ �����. ���������� �����."},
        {"LOGIN_SUCCESS", u8"�������� ����!"},
        {"ATTEMPTS_LEFT", u8"������� ��������"},
        {"TOO_MANY_ATTEMPTS", u8"������� ����� ��������� �������. �����..."},
        {"LOGOUT", u8"�����"},
        {"ERROR_GENERIC", u8"��������� ������"},
        {"USER_BLOCKED", u8"������������ ������� ������������"},
        {"NOT_AUTHORIZED", u8"�� ��������������"},
        {"ROLE_UNKNOWN", u8"���� �� ����������"}
    },
    // �����������
    {
        {"WELCOME", u8"�������� ���������"},
        {"USERNAME_PROMPT", u8"��� ������������"},
        {"PASSWORD_PROMPT", u8"������"},
        {"BLOCK_SELF_ERROR", u8"�� �� ������ ��������� ������ �������"},
        {"MENU_USER", u8"���� ������������"},
        {"MENU_WORKER", u8"���� ��������"},
        {"DEPOSIT", u8"������� �������"},
        {"WITHDRAW", u8"����� �����"},
        {"TRANSFER", u8"���������� �����"},
        {"VIEW_BALANCE", u8"����������� ������"},
        {"VIEW_HISTORY", u8"����������� ������� ����������"},
        {"CREATE_ACCOUNT", u8"�������� �������"},
        {"DELETE_ACCOUNT", u8"������� �������"},
        {"BLOCK_ACCOUNT", u8"����������� �������"},
        {"UNBLOCK_ACCOUNT", u8"������������ �������"},
        {"APPLY_INTEREST", u8"������� ��������"},
        {"SEARCH_BALANCE", u8"����� ������� �� �������"},
        {"GENERATE_REPORT", u8"���������� �����������"},
        {"CHOOSE_STATUS", u8"������� ������ ������������"},
        {"ACTIVE", u8"�������"},
        {"BLOCKED", u8"�����������"},
        {"LOGIN_FAILED", u8"������� �������. ����������� ���."},
        {"LOGIN_SUCCESS", u8"��������� ������!"},
        {"ATTEMPTS_LEFT", u8"����� ���������"},
        {"TOO_MANY_ATTEMPTS", u8"������� ���� �������� ������. �����..."},
        {"LOGOUT", u8"�����"},
        {"ERROR_GENERIC", u8"�������� �������"},
        {"USER_BLOCKED", u8"����������� ��������� �����������"},
        {"NOT_AUTHORIZED", u8"�� �����������"},
        {"ROLE_UNKNOWN", u8"���� �� ���������"}
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
