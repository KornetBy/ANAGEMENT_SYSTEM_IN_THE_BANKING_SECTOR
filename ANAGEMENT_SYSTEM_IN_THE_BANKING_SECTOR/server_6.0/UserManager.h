#pragma once
#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include <string>
#include <unordered_map>
#include <vector>
#include "DataManager.h"

class UserManager {
private:
    std::unordered_map<std::string, std::string> userRoles; // Карта логинов и ролей
    std::unordered_map<std::string, bool> userStatus;      // Карта логинов и статусов (активен/заблокирован)
    DataManager dataManager;                               // Работа с текстовыми файлами

    void loadUsers(const std::string& filename);           // Загрузка пользователей из файла

public:
    UserManager(const std::string& userFile);

    std::string getRole(const std::string& username);      // Получение роли пользователя
    bool isUserActive(const std::string& username);        // Проверка, активен ли пользователь
    bool blockUser(const std::string& username);           // Блокировка пользователя
    bool addUser(const std::string& username, const std::string& role); // Добавление нового пользователя
};

#endif // USER_MANAGER_H
