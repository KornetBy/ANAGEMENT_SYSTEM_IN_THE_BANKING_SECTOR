// Client/main.cpp
#include "Client.h"
#include "Menu.h"
#include <iostream>
#include <string>

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    Client client;
    std::string serverIP = "127.0.0.1"; // IP сервера
    int serverPort = 5000; // Порт сервера

    // Подключение к серверу
    if (!client.connectToServer(serverIP, serverPort)) {
        std::cerr << "Не удалось подключиться к серверу. Завершение программы." << std::endl;
        return 1;
    }
    std::cout << "Успешно подключено к серверу.\n";

    // Получение приветственного сообщения от сервера
    std::string welcome = client.receiveMessage();
    std::cout << "Сервер: " << welcome << std::endl;

    // Аутентификация
    std::string username, password;
    std::cout << "Введите логин: ";
    std::getline(std::cin, username);
    std::cout << "Введите пароль: ";
    std::getline(std::cin, password);

    // Формируем команду AUTH|username|password
    std::string authCommand = "AUTH|" + username + "|" + password;
    client.sendMessage(authCommand);

    // Получение ответа
    std::string authResponse = client.receiveMessage();
    if (authResponse.find("SUCCESS") == 0) {
        size_t pos = authResponse.find('|');
        std::string role = authResponse.substr(pos + 1);
        std::cout << "Успешная авторизация. Роль: " << role << std::endl;

        // Создание и запуск меню
        Menu menu(client, role);
        menu.showMenu();
    }
    else {
        std::cerr << "Ошибка аутентификации: " << authResponse << std::endl;
    }

    // Отключение от сервера
    client.disconnect();
    return 0;
}
