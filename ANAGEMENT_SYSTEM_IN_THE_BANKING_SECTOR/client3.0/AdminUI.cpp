#include "AdminUI.h"
#include "Client.h"
#include <iostream>
#include <string>

void AdminUI::showMenu() {
    while (true) {
        std::cout << "\n--- Меню Администратора ---\n";
        std::cout << "1. Добавить пользователя\n";
        std::cout << "2. Удалить пользователя\n";
        std::cout << "0. Выход\n";
        std::cout << "Введите ваш выбор: ";

        int choice;
        std::cin >> choice;

        switch (choice) {
        case 1: {
            std::string id, username, password, role;
            std::cout << "Введите ID пользователя: ";
            std::cin >> id;
            std::cout << "Введите логин: ";
            std::cin >> username;
            std::cout << "Введите пароль: ";
            std::cin >> password;
            std::cout << "Введите роль: ";
            std::cin >> role;

            // Формирование запроса
            std::string request = "ADD_USER " + id + " " + username + " " + password + " " + role;
            Client::sendRequest(request);

            // Получение ответа
            std::cout << Client::receiveResponse();
            break;
        }
        case 2: {
            std::string username;
            std::cout << "Введите логин пользователя для удаления: ";
            std::cin >> username;

            // Формирование запроса
            std::string request = "DELETE_USER " + username;
            Client::sendRequest(request);

            // Получение ответа
            std::cout << Client::receiveResponse();
            break;
        }
        case 0:
            std::cout << "Возврат в главное меню.\n";
            return;
        default:
            std::cerr << "Ошибка: Неверный выбор. Попробуйте снова.\n";
        }
    }
}
