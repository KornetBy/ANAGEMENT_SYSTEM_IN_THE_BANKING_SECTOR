#include "DirectorUI.h"
#include "Client.h"
#include <iostream>
#include <string>

void DirectorUI::showMenu() {
    while (true) {
        std::cout << "\n--- Меню Директора ---\n";
        std::cout << "1. Просмотр отчета по зарплатам\n";
        std::cout << "2. Просмотр сводного отчета по подразделению\n";
        std::cout << "0. Выход\n";
        std::cout << "Введите ваш выбор: ";

        int choice;
        std::cin >> choice;

        switch (choice) {
        case 1: {
            Client::sendRequest("GET_REPORT SALARY");
            std::cout << Client::receiveResponse();
            break;
        }
        case 2: {
            std::cout << "Введите ID подразделения: ";
            std::string divisionId;
            std::cin >> divisionId;
            Client::sendRequest("GET_DIVISION_REPORT " + divisionId);
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
