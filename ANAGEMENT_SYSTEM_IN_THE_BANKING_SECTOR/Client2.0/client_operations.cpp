#include "client_operations.h"
#include <iostream>
#include <string>
#include <stdexcept>

// Реализация команд администратора
void executeAdminCommand(SOCKET clientSocket, int choice) {
    std::string request;
    switch (choice) {
    case 1: // Добавить пользователя
        request = "ADD_USER\n";
        break;
    case 2: // Удалить пользователя
        request = "DELETE_USER\n";
        break;
    case 3: // Изменить роль пользователя
        request = "CHANGE_ROLE\n";
        break;
    case 4: // Просмотреть всех пользователей
        request = "LIST_USERS\n";
        break;
    case 5: // Выход
        return;
    default:
        std::cerr << "Неверная команда.\n";
        return;
    }
    send(clientSocket, request.c_str(), request.size(), 0);
    char response[1024] = { 0 };
    int bytesRead = recv(clientSocket, response, sizeof(response), 0);

    if (bytesRead <= 0) {
        std::cerr << "Ошибка: сервер не ответил на запрос.\n";
        return;
    }

    std::cout << "Ответ от сервера: " << response << "\n";
}

// Реализация команд клиента
void executeClientCommand(SOCKET clientSocket, int choice) {
    std::string request;
    switch (choice) {
    case 1: // Просмотр счетов
        request = "GET_ACCOUNTS\n";
        break;
    case 2: // Перевод средств
        request = "TRANSFER\n";
        break;
    case 3: // Просмотр выписки
        request = "ACCOUNT_STATEMENT\n";
        break;
    case 4: // Выход
        return;
    default:
        std::cerr << "Неверная команда.\n";
        return;
    }
    send(clientSocket, request.c_str(), request.size(), 0);
    char response[1024] = { 0 };
    recv(clientSocket, response, sizeof(response), 0);
    std::cout << "Ответ от сервера: " << response << "\n";
}

// Реализация команд сотрудника банка
void executeWorkerCommand(SOCKET clientSocket, int choice) {
    std::string request;
    switch (choice) {
    case 1: // Блокировка счёта
        request = "BLOCK_ACCOUNT\n";
        break;
    case 2: // Разблокировка счёта
        request = "UNBLOCK_ACCOUNT\n";
        break;
    case 3: // Обработка кредитного запроса
        request = "CREATE_LOAN_REQUEST\n";
        break;
    case 4: // Генерация отчёта
        request = "GENERATE_ACCOUNT_REPORT\n";
        break;
    case 5: // Выход
        return;
    default:
        std::cerr << "Неверная команда.\n";
        return;
    }
    send(clientSocket, request.c_str(), request.size(), 0);
    char response[1024] = { 0 };
    recv(clientSocket, response, sizeof(response), 0);
    std::cout << "Ответ от сервера: " << response << "\n";
}

// Реализация команд менеджера по персоналу
void executeHRCommand(SOCKET clientSocket, int choice) {
    std::string request;
    switch (choice) {
    case 1: // Управление сотрудниками
        request = "MANAGE_EMPLOYEES\n";
        break;
    case 2: // Генерация отчёта
        request = "EMPLOYEE_REPORT\n";
        break;
    case 3: // Выход
        return;
    default:
        std::cerr << "Неверная команда.\n";
        return;
    }
    send(clientSocket, request.c_str(), request.size(), 0);
    char response[1024] = { 0 };
    recv(clientSocket, response, sizeof(response), 0);
    std::cout << "Ответ от сервера: " << response << "\n";
}
