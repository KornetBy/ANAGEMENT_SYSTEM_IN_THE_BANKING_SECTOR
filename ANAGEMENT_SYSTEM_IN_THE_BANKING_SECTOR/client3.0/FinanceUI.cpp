#include "FinanceUI.h"
#include "Client.h"
#include <iostream>
#include <string>

void FinanceUI::showMenu() {
    while (true) {
        std::cout << "\n--- Меню Бухгалтерии ---\n";
        std::cout << "1. Добавить зарплату сотрудника\n";
        std::cout << "2. Обновить зарплату сотрудника\n";
        std::cout << "3. Обработать заявку на компенсацию\n";
        std::cout << "4. Просмотреть отчет по зарплатам\n";
        std::cout << "5. Просмотреть отчет по компенсациям\n";
        std::cout << "0. Выход\n";
        std::cout << "Введите ваш выбор: ";

        int choice;
        std::cin >> choice;

        switch (choice) {
        case 1: {
            std::string employeeId, paymentDate;
            double baseSalary, bonuses, deductions;
            std::cout << "Введите ID сотрудника: ";
            std::cin >> employeeId;
            std::cout << "Введите базовую зарплату: ";
            std::cin >> baseSalary;
            std::cout << "Введите сумму бонусов: ";
            std::cin >> bonuses;
            std::cout << "Введите сумму удержаний: ";
            std::cin >> deductions;
            std::cout << "Введите дату выплаты (YYYY-MM-DD): ";
            std::cin >> paymentDate;

            // Формирование запроса
            std::string request = "ADD_SALARY " + employeeId + " " + std::to_string(baseSalary) + " " +
                std::to_string(bonuses) + " " + std::to_string(deductions) + " " + paymentDate;
            Client::sendRequest(request);

            // Получение ответа
            std::cout << Client::receiveResponse();
            break;
        }
        case 2: {
            std::string employeeId, paymentDate;
            double baseSalary, bonuses, deductions;
            std::cout << "Введите ID сотрудника: ";
            std::cin >> employeeId;
            std::cout << "Введите новую базовую зарплату: ";
            std::cin >> baseSalary;
            std::cout << "Введите новую сумму бонусов: ";
            std::cin >> bonuses;
            std::cout << "Введите новую сумму удержаний: ";
            std::cin >> deductions;
            std::cout << "Введите новую дату выплаты (YYYY-MM-DD): ";
            std::cin >> paymentDate;

            // Формирование запроса
            std::string request = "UPDATE_SALARY " + employeeId + " " + std::to_string(baseSalary) + " " +
                std::to_string(bonuses) + " " + std::to_string(deductions) + " " + paymentDate;
            Client::sendRequest(request);

            // Получение ответа
            std::cout << Client::receiveResponse();
            break;
        }
        case 3: {
            std::string requestId, newStatus;
            std::cout << "Введите ID заявки на компенсацию: ";
            std::cin >> requestId;
            std::cout << "Введите новый статус (Approved/Rejected): ";
            std::cin >> newStatus;

            // Формирование запроса
            std::string request = "UPDATE_COMPENSATION " + requestId + " " + newStatus;
            Client::sendRequest(request);

            // Получение ответа
            std::cout << Client::receiveResponse();
            break;
        }
        case 4: {
            Client::sendRequest("GET_REPORT SALARY");
            std::cout << Client::receiveResponse();
            break;
        }
        case 5: {
            Client::sendRequest("GET_REPORT COMPENSATION");
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
