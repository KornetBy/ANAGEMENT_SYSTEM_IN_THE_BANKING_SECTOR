// Server/UserManager.cpp
#include "UserManager.h"
#include <fstream>
#include <sstream>
#include <algorithm>

UserManager::UserManager(const std::string& fileName) : userFileName(fileName) {
    loadUsers();
}

void UserManager::loadUsers() {
    std::lock_guard<std::mutex> lock(mtx);
    users.clear();
    std::ifstream file(userFileName);
    if (!file.is_open()) {
        // Файл не существует, создадим пустой
        std::ofstream outfile(userFileName);
        outfile.close();
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string username, password, role, status;
        if (std::getline(iss, username, '|') &&
            std::getline(iss, password, '|') &&
            std::getline(iss, role, '|') &&
            std::getline(iss, status, '|')) {
            User user{ username, password, role, status };
            users.push_back(user);
        }
    }
    file.close();
}

User UserManager::getUser(const std::string& username) {
    std::lock_guard<std::mutex> lock(mtx);
    for (const auto& user : users) {
        if (user.username == username) {
            return user;
        }
    }
    return User(); // Возвращаем пустого пользователя
}

bool UserManager::addUser(const User& user) {
    std::lock_guard<std::mutex> lock(mtx);
    // Проверяем, существует ли уже пользователь
    auto it = std::find_if(users.begin(), users.end(),
        [&](const User& u) { return u.username == user.username; });
    if (it != users.end()) {
        return false; // Пользователь уже существует
    }
    users.push_back(user);
    // Сохраняем изменения в файл
    std::ofstream file(userFileName, std::ios::app);
    if (file.is_open()) {
        file << user.username << "|" << user.password << "|" << user.role << "|" << user.status << std::endl;
        file.close();
        return true;
    }
    return false;
}

bool UserManager::deleteUser(const std::string& username) {
    std::lock_guard<std::mutex> lock(mtx);
    auto it = std::remove_if(users.begin(), users.end(),
        [&](const User& u) { return u.username == username; });
    if (it != users.end()) {
        users.erase(it, users.end());
        // Перезаписываем файл
        std::ofstream file(userFileName, std::ios::trunc);
        if (file.is_open()) {
            for (const auto& user : users) {
                file << user.username << "|" << user.password << "|" << user.role << "|" << user.status << std::endl;
            }
            file.close();
            return true;
        }
    }
    return false;
}

bool UserManager::updateUserStatus(const std::string& username, const std::string& status) {
    std::lock_guard<std::mutex> lock(mtx);
    for (auto& user : users) {
        if (user.username == username) {
            user.status = status;
            // Перезаписываем файл
            std::ofstream file(userFileName, std::ios::trunc);
            if (file.is_open()) {
                for (const auto& u : users) {
                    file << u.username << "|" << u.password << "|" << u.role << "|" << u.status << std::endl;
                }
                file.close();
                return true;
            }
        }
    }
    return false;
}
