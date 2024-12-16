#include "HRUI.h"
#include "Client.h"
#include <iostream>
#include <string>

void HRUI::showMenu() {
    while (true) {
        std::cout << "\n--- Меню HR-Специалиста ---\n";
        std::cout << "1. Добавить сотрудника\n";
        std::cout << "2. Удалить сотрудника\n";
        std::cout << "3. Редактировать данные сотрудника\n";
        std::cout << "0. Выход\n";
        std::cout << "Введите ваш выбор: ";

        int choice;
        std::cin >> choice;

        switch (choice) {
        case 1: {
            std::string id, fullName, birthDate, position, contact, startDate, divisionId;
            std::cout << "Введите ID сотрудника: ";
            std::cin >> id;
            std::cout << "Введите ФИО: ";
            std::cin.ignore(); // Очистка потока
            std::getline(std::cin, fullName);
            std::cout << "Введите дату рождения (YYYY-MM-DD): ";
            std::cin >> birthDate;
            std::cout << "Введите должность: ";
            std::cin.ignore();
            std::getline(std::cin, position);
            std::cout << "Введите контактные данные: ";
            std::getline(std::cin, contact);
            std::cout << "Введите дату начала работы (YYYY-MM-DD): ";
            std::cin >> startDate;
            std::cout << "Введите ID подразделения: ";
            std::cin >> divisionId;

            // Формирование запроса
            std::string request = "ADD_EMPLOYEE " + id + " " + fullName + " " + birthDate + " " +
                position + " " + contact + " " + startDate + " " + divisionId;
            Client::sendRequest(request);

            // Получение ответа
            std::cout << Client::receiveResponse();
            break;
        }
        case 2: {
            std::string id;
            std::cout << "Введите ID сотрудника для удаления: ";
            std::cin >> id;

            // Формирование запроса
            std::string request = "DELETE_EMPLOYEE " + id;
            Client::sendRequest(request);

            // Получение ответа
            std::cout << Client::receiveResponse();
            break;
        }
        case 3: {
            std::string id, fullName, birthDate, position, contact, startDate, divisionId;
            std::cout << "Введите ID сотрудника для редактирования: ";
            std::cin >> id;
            std::cout << "Введите новые данные сотрудника (оставьте поле пустым для сохранения текущего значения):\n";

            std::cout << "ФИО: ";
            std::cin.ignore();
            std::getline(std::cin, fullName);
            std::cout << "Дата рождения (YYYY-MM-DD): ";
            std::getline(std::cin, birthDate);
            std::cout << "Должность: ";
            std::getline(std::cin, position);
            std::cout << "Контактные данные: ";
            std::getline(std::cin, contact);
            std::cout << "Дата начала работы (YYYY-MM-DD): ";
            std::getline(std::cin, startDate);
            std::cout << "ID подразделения: ";
            std::getline(std::cin, divisionId);

            // Формирование запроса
            std::string request = "UPDATE_EMPLOYEE " + id + " " + fullName + " " + birthDate + " " +
                position + " " + contact + " " + startDate + " " + divisionId;
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
