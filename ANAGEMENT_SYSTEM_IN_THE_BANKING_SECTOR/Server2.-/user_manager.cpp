#include "user_manager.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

// Конструктор
UserManager::UserManager(const std::string& filename) : filename(filename) {}

// Вспомогательный метод для парсинга строки из файла
User UserManager::parseUser(const std::string& line) {
    User user;
    std::istringstream iss(line);
    std::string id, username, password, role;

    // Разделение строки по символу '|'
    if (!(std::getline(iss, id, '|') && std::getline(iss, username, '|') &&
        std::getline(iss, password, '|') && std::getline(iss, role, '|'))) {
        throw std::runtime_error("Некорректный формат строки.");
    }

    // Преобразование ID в число и проверка
    user.id = std::stoi(id);
    if (user.id <= 0) {
        throw std::runtime_error("Некорректный ID пользователя: " + id);
    }

    // Присвоение оставшихся значений
    user.username = username;
    user.password = password;
    user.role = role;
    return user;
}

// Загрузка данных из файла
void UserManager::loadUsers() {
    users.clear();
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл " << filename << std::endl;
        std::ofstream errorLog("logs/error.log", std::ios::app);
        errorLog << "Ошибка: файл " << filename << " не найден или недоступен." << std::endl;
        errorLog.close();
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        try {
            // Проверяем количество разделителей
            if (std::count(line.begin(), line.end(), '|') != 3) {
                throw std::runtime_error("Некорректный формат строки: " + line);
            }

            User user = parseUser(line);

            // Проверка на уникальность ID
            for (const auto& existingUser : users) {
                if (existingUser.id == user.id) {
                    throw std::runtime_error("Дублирующийся ID пользователя: " + std::to_string(user.id));
                }
            }

            // Добавляем пользователя только если проверки успешны
            users.push_back(user);
        }
        catch (const std::exception& e) {
            std::cerr << "Ошибка: " << e.what() << " Строка: " << line << std::endl;
            std::ofstream errorLog("logs/error.log", std::ios::app);
            errorLog << "Ошибка при загрузке пользователя: " << e.what() << " Строка: " << line << std::endl;
            errorLog.close();
        }
    }

    if (users.empty()) {
        std::cerr << "Предупреждение: файл " << filename << " пустой. Список пользователей не загружен." << std::endl;
    }

    file.close();
}


// Сохранение данных в файл
void UserManager::saveUsers() {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл " << filename << " для записи." << std::endl;
        std::ofstream errorLog("logs/error.log", std::ios::app);
        errorLog << "Ошибка: не удалось открыть файл " << filename << " для записи." << std::endl;
        errorLog.close();
        return;
    }

    for (const auto& user : users) {
        file << user.id << " | " << user.username << " | "
            << user.password << " | " << user.role << std::endl;
    }
    file.close();
}

// Добавление нового пользователя
void UserManager::addUser(const User& user) {
    // Проверка уникальности ID
    for (const auto& existingUser : users) {
        if (existingUser.id == user.id) {
            std::cerr << "Ошибка: пользователь с ID " << user.id << " уже существует." << std::endl;
            return;
        }
        if (existingUser.username == user.username) {
            std::cerr << "Ошибка: пользователь с логином " << user.username << " уже существует." << std::endl;
            return;
        }
    }

    // Проверка на недопустимые символы в логине и роли
    if (user.username.find('|') != std::string::npos || user.role.find('|') != std::string::npos) {
        std::cerr << "Ошибка: логин или роль содержит недопустимый символ '|'" << std::endl;
        return;
    }

    users.push_back(user);
    saveUsers();
}

// Удаление пользователя по ID
bool UserManager::removeUser(int id) {
    for (auto it = users.begin(); it != users.end(); ++it) {
        if (it->id == id) {
            users.erase(it);
            saveUsers();
            return true;
        }
    }
    std::cerr << "Ошибка: пользователь с ID " << id << " не найден." << std::endl;
    return false;
}

// Получение пользователя по логину
User* UserManager::getUserByUsername(const std::string& username) {
    for (auto& user : users) {
        if (user.username == username) {
            return &user;
        }
    }
    return nullptr;
}

// Получение всех пользователей
const std::vector<User>& UserManager::getAllUsers() const {
    return users;
}

// Обновление данных пользователя
bool UserManager::updateUser(int id, const std::string& newPassword, const std::string& newRole) {
    for (auto& user : users) {
        if (user.id == id) {
            // Проверка на недопустимые символы
            if (newPassword.find('|') != std::string::npos || newRole.find('|') != std::string::npos) {
                std::cerr << "Ошибка: пароль или роль содержит недопустимый символ '|'" << std::endl;
                return false;
            }

            user.password = newPassword;
            user.role = newRole;
            saveUsers();
            return true;
        }
    }
    std::cerr << "Ошибка: пользователь с ID " << id << " не найден." << std::endl;
    return false;
}
