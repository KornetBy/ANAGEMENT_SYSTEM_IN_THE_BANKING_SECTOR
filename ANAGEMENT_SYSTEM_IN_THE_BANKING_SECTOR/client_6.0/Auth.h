#pragma once
#ifndef AUTH_H
#define AUTH_H

#include <string>
#include <iostream>
#include <conio.h> // Для функции getch() на Windows

class Auth {
private:
    std::string login; // Логин пользователя
    std::string password; // Пароль пользователя

public:
    Auth();                          // Конструктор
    void inputCredentials();         // Ввод логина и пароля
    std::string getAuthRequest();    // Формирование строки запроса для сервера
    void setCredentials(const std::string& login, const std::string& password); // Новый метод
};

#endif // AUTH_H
