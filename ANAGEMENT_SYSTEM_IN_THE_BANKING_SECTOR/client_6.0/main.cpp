#include "Client.h"
#include "Auth.h"
#include "Menu.h"
#include <windows.h>

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    const std::string serverIP = "127.0.0.1"; // IP-адрес сервера
    const int port = 5000;                    // Порт сервера

    Client client(serverIP, port);

    if (client.connectToServer()) { // Устанавливаем соединение
        std::cout << "Подключение успешно!" << std::endl;

        // Авторизация
        Auth auth;
        std::string login, password;

        // Ввод логина с валидацией
        do {
            std::cout << "Введите логин: ";
            std::getline(std::cin, login);
        } while (!client.validateInput(login));

        // Ввод пароля с валидацией
        do {
            std::cout << "Введите пароль: ";
            std::getline(std::cin, password);
        } while (!client.validateInput(password));

        // Установка данных для авторизации
        auth.setCredentials(login, password);
        std::string authRequest = auth.getAuthRequest();

        // Отправка запроса авторизации
        client.sendMessage(authRequest);
        std::string serverResponse = client.receiveMessage();

        if (serverResponse.find("SUCCESS") != std::string::npos) {
            std::cout << "Авторизация успешна!" << std::endl;

            // Извлекаем роль пользователя
            std::string role = serverResponse.substr(serverResponse.find('|') + 1);

            // Отображение меню в зависимости от роли
            Menu menu(client, role);
            menu.showMenu();
        }
        else {
            std::cerr << "Ошибка авторизации: " << serverResponse << std::endl;
        }
    }
    else {
        std::cerr << "Не удалось подключиться к серверу." << std::endl;
    }

    return 0;
}
