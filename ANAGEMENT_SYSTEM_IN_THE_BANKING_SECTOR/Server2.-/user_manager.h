#pragma once
#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include <string>
#include <vector>

// Структура пользователя
struct User {
    int id;                 // Уникальный идентификатор пользователя
    std::string username;   // Имя пользователя
    std::string password;   // Пароль пользователя
    std::string role;       // Роль пользователя ("admin", "client", "worker", "hr")
};

// Класс для управления пользователями
class UserManager {
private:
    std::vector<User> users;    // Список пользователей
    std::string filename;       // Имя файла с данными пользователей

    // Вспомогательный метод для парсинга строки из файла
    User parseUser(const std::string& line);

public:
    // Конструктор с указанием файла данных
    UserManager(const std::string& filename);

    // Загрузка данных из файла
    void loadUsers();

    // Сохранение данных в файл
    void saveUsers();

    // Добавление нового пользователя
    void addUser(const User& user);

    // Удаление пользователя по ID
    bool removeUser(int id);

    // Получение пользователя по логину
    User* getUserByUsername(const std::string& username);

    // Получение всех пользователей
    const std::vector<User>& getAllUsers() const;

    // Обновление данных пользователя
    bool updateUser(int id, const std::string& newPassword, const std::string& newRole);
};

#endif // USER_MANAGER_H

