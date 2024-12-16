#include "ManagerUI.h"
#include "Client.h"
#include <iostream>
#include <string>

void ManagerUI::showMenu() {
    while (true) {
        std::cout << "\n--- Меню Руководителя Подразделения ---\n";
        std::cout << "1. Просмотреть сотрудников подразделения\n";
        std::cout << "2. Управление графиками работы\n";
        std::cout << "3. Оценка производительности сотрудников\n";
        std::cout << "0. Выход\n";
        std::cout << "Введите ваш выбор: ";

        int choice;
        std::cin >> choice;

        switch (choice) {
        case 1: {
            std::string divisionId;
            std::cout << "Введите ID вашего подразделения: ";
            std::cin >> divisionId;

            // Формирование запроса
            std::string request = "GET_EMPLOYEES_BY_DIVISION " + divisionId;
            Client::sendRequest(request);

            // Получение ответа
            std::cout << Client::receiveResponse();
            break;
        }
        case 2: {
            std::string employeeId, date, startTime, endTime, shiftType;
            std::cout << "Введите ID сотрудника: ";
            std::cin >> employeeId;
            std::cout << "Введите дату смены (YYYY-MM-DD): ";
            std::cin >> date;
            std::cout << "Введите время начала смены (HH:MM): ";
            std::cin >> startTime;
            std::cout << "Введите время окончания смены (HH:MM): ";
            std::cin >> endTime;
            std::cout << "Введите тип смены (Regular/Overtime/Leave/SickLeave): ";
            std::cin >> shiftType;

            // Формирование запроса
            std::string request = "ADD_SHIFT " + employeeId + " " + date + " " +
                startTime + " " + endTime + " " + shiftType;
            Client::sendRequest(request);

            // Получение ответа
            std::cout << Client::receiveResponse();
            break;
        }
        case 3: {
            std::string employeeId, date, comments;
            int kpi;
            std::cout << "Введите ID сотрудника: ";
            std::cin >> employeeId;
            std::cout << "Введите дату оценки (YYYY-MM-DD): ";
            std::cin >> date;
            std::cout << "Введите KPI (0-100): ";
            std::cin >> kpi;
            std::cout << "Введите комментарии: ";
            std::cin.ignore();
            std::getline(std::cin, comments);

            // Формирование запроса
            std::string request = "ADD_PERFORMANCE " + employeeId + " " + date + " " +
                std::to_string(kpi) + " " + comments;
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
