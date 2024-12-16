#include "CashierUI.h"
#include "Client.h"
#include <iostream>
#include <string>

void CashierUI::showMenu() {
    while (true) {
        std::cout << "\n--- Меню Кассира ---\n";
        std::cout << "1. Просмотреть график работы\n";
        std::cout << "0. Выход\n";
        std::cout << "Введите ваш выбор: ";

        int choice;
        std::cin >> choice;

        switch (choice) {
        case 1: {
            std::string employeeId;
            std::cout << "Введите ваш ID: ";
            std::cin >> employeeId;

            // Формирование запроса
            std::string request = "GET_SCHEDULE " + employeeId;
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
