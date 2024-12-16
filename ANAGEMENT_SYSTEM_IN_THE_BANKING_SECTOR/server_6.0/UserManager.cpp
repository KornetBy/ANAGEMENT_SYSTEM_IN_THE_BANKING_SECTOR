#include "UserManager.h"
#include <fstream>
#include <sstream>

// Конструктор
UserManager::UserManager(const std::string& userFile) {
    loadUsers(userFile);
}

// Загрузка пользователей из файла
void UserManager::loadUsers(const std::string& filename) {
    std::vector<std::string> lines = dataManager.readFile(filename);
    for (const auto& line : lines) {
        std::vector<std::string> parts = dataManager.split(line, '|');
        if (parts.size() >= 2) {
            std::string username = parts[0];
            std::string role = parts[1];
            userRoles[username] = role;
            userStatus[username] = true; // Все пользователи по умолчанию активны
        }
    }
}

// Получение роли пользователя
std::string UserManager::getRole(const std::string& username) {
    if (userRoles.find(username) != userRoles.end()) {
        return userRoles[username];
    }
    return "";
}

// Проверка, активен ли пользователь
bool UserManager::isUserActive(const std::string& username) {
    if (userStatus.find(username) != userStatus.end()) {
        return userStatus[username];
    }
    return false;
}

// Блокировка пользователя
bool UserManager::blockUser(const std::string& username) {
    if (userStatus.find(username) != userStatus.end()) {
        userStatus[username] = false;
        return true;
    }
    return false;
}

// Добавление нового пользователя
bool UserManager::addUser(const std::string& username, const std::string& role) {
    if (userRoles.find(username) == userRoles.end()) {
        userRoles[username] = role;
        userStatus[username] = true;
        dataManager.writeFile("data/users.txt", username + "|" + role);
        return true;
    }
    return false;
}
