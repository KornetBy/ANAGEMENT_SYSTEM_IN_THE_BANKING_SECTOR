#include "Auth.h"
#include "Client.h"
#include <iostream>
#include <sstream>

bool Auth::login(std::string& username, std::string& role) {
    std::string password;

    // Ввод логина и пароля
    std::cout << "Введите логин: ";
    std::cin >> username;
    std::cout << "Введите пароль: ";
    std::cin >> password;

    // Формирование запроса
    std::ostringstream request;
    request << "AUTH " << username << " " << password;
    Client::sendRequest(request.str());

    // Получение ответа от сервера
    std::string response = Client::receiveResponse();
    if (response.find("SUCCESS") != std::string::npos) {
        // Извлечение роли из ответа
        role = response.substr(response.find(":") + 1);
        return true;
    }
    else {
        std::cerr << "Ошибка авторизации: " << response << "\n";
        return false;
    }
}
