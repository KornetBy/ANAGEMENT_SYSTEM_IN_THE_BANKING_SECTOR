// UserManager.cpp
#include "UserManager.h"
#include <fstream>
#include <sstream>
#include <iostream>

UserManager::UserManager(const std::string& filename) : userFileName(filename) {
    std::lock_guard<std::mutex> lock(mtx);
    std::ifstream ifs(userFileName);
    if (!ifs.is_open()) {
        std::cerr << "Не удалось открыть файл: " << userFileName << "\n";
        return;
    }

    std::string line;
    while (getline(ifs, line)) {
        if (line.empty()) continue; // Пропускаем пустые строки

        std::istringstream iss(line);
        std::string username, password, role, status;

        // Считываем токены
        if (!getline(iss, username, '|') ||
            !getline(iss, password, '|') ||
            !getline(iss, role, '|') ||
            !getline(iss, status, '|')) {
            std::cerr << "Некорректный формат строки: " << line << "\n";
            continue; // Пропускаем некорректную строку
        }

        // Добавляем пользователя только при корректном формате
        User user{ username, password, role, status };
        users.push_back(user);
    }
    ifs.close();
}


User UserManager::getUser(const std::string& username) {
    std::lock_guard<std::mutex> lock(mtx);
    for (const auto& user : users) {
        if (user.username == username) return user;
    }
    return User{ "", "", "", "" };
}

bool UserManager::addUser(const User& user) {
    std::lock_guard<std::mutex> lock(mtx);
    for (const auto& u : users) {
        if (u.username == user.username) return false; // Пользователь уже существует
    }
    users.push_back(user);

    std::ofstream ofs(userFileName, std::ios::app);
    if (!ofs.is_open()) return false;
    ofs << user.username << "|" << user.password << "|" << user.role << "|" << user.status << "\n";
    ofs.close();
    return true;
}

bool UserManager::deleteUser(const std::string& username) {
    std::lock_guard<std::mutex> lock(mtx);
    bool found = false;
    std::vector<User> updatedUsers;
    for (const auto& user : users) {
        if (user.username != username) {
            updatedUsers.push_back(user);
        }
        else {
            found = true;
        }
    }
    if (!found) return false;
    users = updatedUsers;

    // Перезапись файла
    std::ofstream ofs(userFileName, std::ios::trunc);
    if (!ofs.is_open()) return false;
    for (const auto& user : users) {
        ofs << user.username << "|" << user.password << "|" << user.role << "|" << user.status << "\n";
    }
    ofs.close();
    return true;
}

bool UserManager::updateUserStatus(const std::string& username, const std::string& status) {
    std::lock_guard<std::mutex> lock(mtx);
    bool found = false;
    for (auto& user : users) {
        if (user.username == username) {
            user.status = status;
            found = true;
            break;
        }
    }
    if (!found) return false;

    // Перезапись файла
    std::ofstream ofs(userFileName, std::ios::trunc);
    if (!ofs.is_open()) return false;
    for (const auto& user : users) {
        ofs << user.username << "|" << user.password << "|" << user.role << "|" << user.status << "\n";
    }
    ofs.close();
    return true;
}
