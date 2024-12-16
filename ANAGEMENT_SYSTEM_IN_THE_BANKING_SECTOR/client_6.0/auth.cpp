#include "Auth.h"

// Конструктор
Auth::Auth() : login(""), password("") {}

// Ввод логина и пароля
void Auth::inputCredentials() {
    std::cout << "Введите логин: ";
    std::getline(std::cin, login);

    std::cout << "Введите пароль: ";
    char ch;
    password.clear();

    // Ввод пароля символ за символом с отображением звездочек
    while ((ch = _getch()) != '\r') { // '\r' — Enter
        if (ch == '\b') { // Обработка Backspace
            if (!password.empty()) {
                password.pop_back();
                std::cout << "\b \b"; // Удаление последнего символа с экрана
            }
        }
        else if (isprint(ch)) { // Только печатные символы
            password += ch;
            std::cout << '*';
        }
    }
    std::cout << std::endl;
}
// Установка логина и пароля через параметры
void Auth::setCredentials(const std::string& login, const std::string& password) {
    this->login = login;
    this->password = password;
}
// Формирование строки для отправки запроса на сервер
std::string Auth::getAuthRequest() {
    return "AUTH|" + login + "|" + password;
}
