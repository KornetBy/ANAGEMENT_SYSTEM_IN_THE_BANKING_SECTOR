// Menu.cpp
#include "Menu.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <locale>
Menu::Menu(Client& cli) : client(cli), role(""), username("") {}

std::string trim(const std::string& str) {
    auto start = str.begin();
    while (start != str.end() && std::isspace(*start)) {
        start++;
    }

    auto end = str.end();
    do {
        end--;
    } while (std::distance(start, end) > 0 && std::isspace(*end));

    return std::string(start, end + 1);
}
bool Menu::authenticate() {
    std::cout << "Введите логин: ";
    std::cin >> username;
    std::cout << "Введите пароль: ";
    std::string password;
    std::cin >> password;

    std::string authCommand = "AUTH|" + username + "|" + password;
    client.sendMessage(authCommand);
    std::string response = client.receiveMessage();

    if (response.find("SUCCESS|") == 0) {
        role = trim(response.substr(8)); // Обрезка пробелов
        std::cout << "Успешная авторизация. Роль: " << role << "\n";
        return true;
    }
    else if (response.find("ERROR|") == 0) {
        std::string errorMessage = (response.size() > 6) ? response.substr(6) : "Неизвестная ошибка";
        std::cout << "Ошибка авторизации: " << errorMessage << "\n";
        return false;
    }
    else {
        std::cout << "Некорректный ответ сервера: " << response << "\n";
        return false;
    }
}




void Menu::showMenu() {
    if (!authenticate()) return;

    while (true) {
        if (role == "admin") {
            handleAdminMenu();
        }
        else if (role == "hr") {
            handleHRMenu();
        }
        else if (role == "cashier") {
            handleCashierMenu();
        }
        else if (role == "manager") {
            handleManagerMenu();
        }
        else if (role == "employee") {
            handleEmployeeMenu();
        }
        else {
            std::cout << "Неизвестная роль.\n";
            break;
        }
    }
}

void Menu::handleAdminMenu() {
    std::cout << "\n+====================================+\n"
        << "|      Система управления персоналом |\n"
        << "+====================================+\n"
        << "| Роль: Admin                        |\n"
        << "+------------------------------------+\n"
        << "| 1. Добавить сотрудника             |\n"
        << "| 2. Изменить данные сотрудника      |\n"
        << "| 3. Блокировать сотрудника          |\n"
        << "| 4. Посмотреть историю изменений    |\n"
        << "| 5. Добавить должность              |\n"
        << "| 6. Просмотреть активные курсы      |\n"
        << "| 7. Просмотреть активные соединения |\n"
        << "| 8. Добавить пользователя           |\n"
        << "| 9. Удалить пользователя            |\n"
        << "| 10. Выйти                           |\n"
        << "+------------------------------------+\n"
        << "Выберите действие: ";
    int choice;
    std::cin >> choice;
    std::cin.ignore(); // Очистка буфера

    switch (choice) {
    case 1: { // Добавить сотрудника
        std::cout << "Введите ID сотрудника: ";
        long empID;
        std::cin >> empID;
        std::cin.ignore();

        std::cout << "Выберите должность:\n"
            << "1. Менеджер\n"
            << "2. Кассир\n"
            << "3. Отмена\n"
            << "Выберите действие: ";
        int posChoice;
        std::cin >> posChoice;
        std::cin.ignore();
        long posID;
        if (posChoice == 1) posID = 2001;
        else if (posChoice == 2) posID = 2002;
        else { std::cout << "Отмена добавления сотрудника.\n"; return; }

        std::string fullName, birthDate, address, contactInfo, startDate, status;
        std::cout << "Введите ФИО сотрудника: ";
        getline(std::cin, fullName);
        std::cout << "Введите дату рождения (YYYY-MM-DD): ";
        getline(std::cin, birthDate);
        std::cout << "Введите адрес: ";
        getline(std::cin, address);
        std::cout << "Введите контактную информацию: ";
        getline(std::cin, contactInfo);
        std::cout << "Введите дату начала работы (YYYY-MM-DD): ";
        getline(std::cin, startDate);
        std::cout << "Введите статус (active/blocked): ";
        getline(std::cin, status);

        std::ostringstream oss;
        oss << "ADD_EMPLOYEE|" << empID << "|" << posID << "|" << fullName << "|"
            << birthDate << "|" << address << "|" << contactInfo << "|"
            << startDate << "|" << status;
        client.sendMessage(oss.str());
        std::string response = client.receiveMessage();
        std::cout << response.substr(8) << "\n"; // Вывод сообщения после SUCCESS|
        break;
    }
    case 4: { // Посмотреть историю изменений
        std::string command = "VIEW_HISTORY";
        client.sendMessage(command);
        std::string response = client.receiveMessage();
        if (response.find("SUCCESS|") == 0) {
            std::cout << "История изменений:\n" << response.substr(8) << "\n";
        }
        else {
            std::cout << response.substr(6) << "\n";
        }
        break;
    }
    case 5: { // Добавить должность
        std::cout << "Введите ID должности: ";
        long posID;
        std::cin >> posID;
        std::cin.ignore();
        std::cout << "Введите название должности: ";
        std::string posName;
        getline(std::cin, posName);
        std::cout << "Введите минимальную зарплату: ";
        double minSal;
        std::cin >> minSal;
        std::cin.ignore();
        std::cout << "Введите максимальную зарплату: ";
        double maxSal;
        std::cin >> maxSal;
        std::cin.ignore();
        std::cout << "Введите квалификационные требования: ";
        std::string qualReq;
        getline(std::cin, qualReq);

        std::ostringstream oss;
        oss << "ADD_POSITION|" << posID << "|" << posName << "|" << minSal << "|"
            << maxSal << "|" << qualReq;
        client.sendMessage(oss.str());
        std::string response = client.receiveMessage();
        std::cout << response.substr(8) << "\n"; // Вывод сообщения после SUCCESS|
        break;
    }
    case 8: { // Добавить пользователя
        std::cout << "Введите логин: ";
        std::string newUsername;
        getline(std::cin, newUsername);
        std::cout << "Введите пароль: ";
        std::string newPassword;
        getline(std::cin, newPassword);
        std::cout << "Выберите роль пользователя:\n"
            << "1. HR\n"
            << "2. Cashier\n"
            << "3. Manager\n"
            << "4. Employee\n"
            << "5. Отмена\n"
            << "Выберите действие: ";
        int roleChoice;
        std::cin >> roleChoice;
        std::cin.ignore();
        std::string newRole;
        if (roleChoice == 1) newRole = "hr";
        else if (roleChoice == 2) newRole = "cashier";
        else if (roleChoice == 3) newRole = "manager";
        else if (roleChoice == 4) newRole = "employee";
        else { std::cout << "Отмена добавления пользователя.\n"; return; }

        std::string status = "active";

        std::ostringstream oss;
        oss << "ADD_USER|" << newUsername << "|" << newPassword << "|"
            << newRole << "|" << status;
        client.sendMessage(oss.str());
        std::string response = client.receiveMessage();
        std::cout << response.substr(8) << "\n"; // Вывод сообщения после SUCCESS|
        break;
    }
    case 10: { // Выйти
        client.disconnect();
        std::cout << "Выход из системы.\n";
        exit(0);
    }
    default:
        std::cout << "Некорректный выбор.\n";
    }
}

void Menu::handleHRMenu() {
    std::cout << "\n+====================================+\n"
        << "|      Система управления персоналом |\n"
        << "+====================================+\n"
        << "| Роль: HR                           |\n"
        << "+------------------------------------+\n"
        << "| 1. Добавить сотрудника             |\n"
        << "| 2. Изменить данные сотрудника      |\n"
        << "| 3. Добавить банковский курс        |\n"
        << "| 4. Изменить банковский курс        |\n"
        << "| 5. Удалить банковский курс         |\n"
        << "| 6. Просмотреть активные курсы      |\n"
        << "| 7. Просмотреть лог-файлы           |\n"
        << "| 8. Управлять графиками работы      |\n"
        << "| 9. Обработать заявку на компенсацию|\n"
        << "| 10. Выйти                           |\n"
        << "+------------------------------------+\n"
        << "Выберите действие: ";
    int choice;
    std::cin >> choice;
    std::cin.ignore(); // Очистка буфера

    switch (choice) {
    case 1: { // Добавить сотрудника
        std::cout << "Введите ID сотрудника: ";
        long empID;
        std::cin >> empID;
        std::cin.ignore();

        std::cout << "Выберите должность:\n"
            << "1. Менеджер\n"
            << "2. Кассир\n"
            << "3. Отмена\n"
            << "Выберите действие: ";
        int posChoice;
        std::cin >> posChoice;
        std::cin.ignore();
        long posID;
        if (posChoice == 1) posID = 2001;
        else if (posChoice == 2) posID = 2002;
        else { std::cout << "Отмена добавления сотрудника.\n"; return; }

        std::string fullName, birthDate, address, contactInfo, startDate, status;
        std::cout << "Введите ФИО сотрудника: ";
        getline(std::cin, fullName);
        std::cout << "Введите дату рождения (YYYY-MM-DD): ";
        getline(std::cin, birthDate);
        std::cout << "Введите адрес: ";
        getline(std::cin, address);
        std::cout << "Введите контактную информацию: ";
        getline(std::cin, contactInfo);
        std::cout << "Введите дату начала работы (YYYY-MM-DD): ";
        getline(std::cin, startDate);
        std::cout << "Введите статус (active/blocked): ";
        getline(std::cin, status);

        std::ostringstream oss;
        oss << "ADD_EMPLOYEE|" << empID << "|" << posID << "|" << fullName << "|"
            << birthDate << "|" << address << "|" << contactInfo << "|"
            << startDate << "|" << status;
        client.sendMessage(oss.str());
        std::string response = client.receiveMessage();
        std::cout << response.substr(8) << "\n"; // Вывод сообщения после SUCCESS|
        break;
    }
    case 3: { // Добавить банковский курс
        std::cout << "Введите ID курса: ";
        std::string courseID;
        getline(std::cin, courseID);
        std::cout << "Введите тему курса: ";
        std::string topic;
        getline(std::cin, topic);
        std::cout << "Введите дату начала курса (YYYY-MM-DD): ";
        std::string startDate;
        getline(std::cin, startDate);
        std::cout << "Введите дату окончания курса (YYYY-MM-DD): ";
        std::string endDate;
        getline(std::cin, endDate);

        std::ostringstream oss;
        oss << "ADD_COURSE|" << courseID << "|" << topic << "|" << startDate << "|" << endDate;
        client.sendMessage(oss.str());
        std::string response = client.receiveMessage();
        std::cout << response.substr(8) << "\n"; // Вывод сообщения после SUCCESS|
        break;
    }
    case 4: { // Изменить банковский курс
        std::cout << "Введите ID курса для изменения: ";
        std::string courseID;
        getline(std::cin, courseID);
        std::cout << "Введите новую тему курса: ";
        std::string topic;
        getline(std::cin, topic);
        std::cout << "Введите новую дату начала курса (YYYY-MM-DD): ";
        std::string startDate;
        getline(std::cin, startDate);
        std::cout << "Введите новую дату окончания курса (YYYY-MM-DD): ";
        std::string endDate;
        getline(std::cin, endDate);

        std::ostringstream oss;
        oss << "UPDATE_COURSE|" << courseID << "|" << topic << "|" << startDate << "|" << endDate;
        client.sendMessage(oss.str());
        std::string response = client.receiveMessage();
        std::cout << response.substr(8) << "\n";
        break;
    }
    case 5: { // Удалить банковский курс
        std::cout << "Введите ID курса для удаления: ";
        std::string courseID;
        getline(std::cin, courseID);

        std::ostringstream oss;
        oss << "DELETE_COURSE|" << courseID;
        client.sendMessage(oss.str());
        std::string response = client.receiveMessage();
        std::cout << response.substr(8) << "\n";
        break;
    }
    case 7: { // Просмотреть лог-файлы
        std::string command = "VIEW_LOGS";
        client.sendMessage(command);
        std::string response = client.receiveMessage();
        if (response.find("SUCCESS|") == 0) {
            std::cout << "Логи сервера:\n" << response.substr(8) << "\n";
        }
        else {
            std::cout << response.substr(6) << "\n";
        }
        break;
    }
    case 10: { // Выйти
        client.disconnect();
        std::cout << "Выход из системы.\n";
        exit(0);
    }
    default:
        std::cout << "Некорректный выбор.\n";
    }
}

void Menu::handleCashierMenu() {
    std::cout << "\n+====================================+\n"
        << "|      Система управления персоналом |\n"
        << "+====================================+\n"
        << "| Роль: Cashier                      |\n"
        << "+------------------------------------+\n"
        << "| 1. Просмотреть зарплаты сотрудников |\n"
        << "| 2. Обработать заявку на компенсацию |\n"
        << "| 3. Просмотреть активные курсы      |\n"
        << "| 4. Выйти                           |\n"
        << "+------------------------------------+\n"
        << "Выберите действие: ";
    int choice;
    std::cin >> choice;
    std::cin.ignore(); // Очистка буфера

    switch (choice) {
    case 1: { // Просмотреть зарплаты сотрудников
        std::string command = "VIEW_SALARIES";
        client.sendMessage(command);
        std::string response = client.receiveMessage();
        if (response.find("SUCCESS|") == 0) {
            std::cout << "Зарплаты сотрудников:\n" << response.substr(8) << "\n";
        }
        else {
            std::cout << response.substr(6) << "\n";
        }
        break;
    }
    case 2: { // Обработать заявку на компенсацию
        std::cout << "Введите ID заявки для обработки: ";
        long requestID;
        std::cin >> requestID;
        std::cin.ignore();
        std::cout << "Введите новый статус заявки (Одобрено/Отклонено): ";
        std::string newStatus;
        getline(std::cin, newStatus);

        std::ostringstream oss;
        oss << "PROCESS_COMPENSATION|" << requestID << "|" << newStatus;
        client.sendMessage(oss.str());
        std::string response = client.receiveMessage();
        std::cout << response.substr(8) << "\n";
        break;
    }
    case 3: { // Просмотреть активные курсы
        std::string command = "VIEW_ACTIVE_COURSES";
        client.sendMessage(command);
        std::string response = client.receiveMessage();
        if (response.find("SUCCESS|") == 0) {
            std::cout << "Активные курсы:\n" << response.substr(8) << "\n";
        }
        else {
            std::cout << response.substr(6) << "\n";
        }
        break;
    }
    case 4: { // Выйти
        client.disconnect();
        std::cout << "Выход из системы.\n";
        exit(0);
    }
    default:
        std::cout << "Некорректный выбор.\n";
    }
}

void Menu::handleManagerMenu() {
    std::cout << "\n+====================================+\n"
        << "|      Система управления персоналом |\n"
        << "+====================================+\n"
        << "| Роль: Manager                      |\n"
        << "+------------------------------------+\n"
        << "| 1. Управлять графиками работы      |\n"
        << "| 2. Оценить производительность      |\n"
        << "| 3. Просмотреть активные курсы      |\n"
        << "| 4. Выйти                           |\n"
        << "+------------------------------------+\n"
        << "Выберите действие: ";
    int choice;
    std::cin >> choice;
    std::cin.ignore(); // Очистка буфера

    switch (choice) {
    case 1: { // Управлять графиками работы
        // Реализация аналогична добавлению/редактированию графиков
        std::cout << "Функционал управления графиками работы еще не реализован.\n";
        break;
    }
    case 2: { // Оценить производительность
        // Реализация оценки производительности
        std::cout << "Функционал оценки производительности еще не реализован.\n";
        break;
    }
    case 3: { // Просмотреть активные курсы
        std::string command = "VIEW_ACTIVE_COURSES";
        client.sendMessage(command);
        std::string response = client.receiveMessage();
        if (response.find("SUCCESS|") == 0) {
            std::cout << "Активные курсы:\n" << response.substr(8) << "\n";
        }
        else {
            std::cout << response.substr(6) << "\n";
        }
        break;
    }
    case 4: { // Выйти
        client.disconnect();
        std::cout << "Выход из системы.\n";
        exit(0);
    }
    default:
        std::cout << "Некорректный выбор.\n";
    }
}

void Menu::handleEmployeeMenu() {
    std::cout << "\n+====================================+\n"
        << "|      Система управления персоналом |\n"
        << "+====================================+\n"
        << "| Роль: Employee                     |\n"
        << "+------------------------------------+\n"
        << "| 1. Просмотреть свою информацию     |\n"
        << "| 2. Просмотреть свой график работы  |\n"
        << "| 3. Выбрать и записаться на курс    |\n"
        << "| 4. Подать заявку на компенсацию    |\n"
        << "| 5. Выйти                           |\n"
        << "+------------------------------------+\n"
        << "Выберите действие: ";
    int choice;
    std::cin >> choice;
    std::cin.ignore(); // Очистка буфера

    switch (choice) {
    case 1: { // Просмотреть свою информацию
        std::string command = "VIEW_OWN_INFO|" + username;
        client.sendMessage(command);
        std::string response = client.receiveMessage();
        if (response.find("SUCCESS|") == 0) {
            std::cout << "Ваша информация:\n" << response.substr(8) << "\n";
        }
        else {
            std::cout << response.substr(6) << "\n";
        }
        break;
    }
    case 2: { // Просмотреть свой график работы
        std::string command = "VIEW_OWN_SCHEDULE|" + username;
        client.sendMessage(command);
        std::string response = client.receiveMessage();
        if (response.find("SUCCESS|") == 0) {
            std::cout << "Ваш график работы:\n" << response.substr(8) << "\n";
        }
        else {
            std::cout << response.substr(6) << "\n";
        }
        break;
    }
    case 3: { // Выбрать и записаться на курс
        std::string command = "VIEW_ACTIVE_COURSES";
        client.sendMessage(command);
        std::string response = client.receiveMessage();
        if (response.find("SUCCESS|") == 0) {
            std::cout << "Доступные курсы:\n" << response.substr(8) << "\n";
            std::cout << "Введите ID курса для записи: ";
            std::string courseID;
            getline(std::cin, courseID);
            // Предполагаем, что у сотрудника есть уникальный employeeID, например, 1001
            // В реальной системе employeeID нужно хранить после аутентификации
            long employeeID = 1001; // Пример
            std::ostringstream oss;
            oss << "ENROLL_COURSE|" << courseID << "|" << employeeID;
            client.sendMessage(oss.str());
            std::string enrollResponse = client.receiveMessage();
            std::cout << enrollResponse.substr(8) << "\n";
        }
        else {
            std::cout << response.substr(6) << "\n";
        }
        break;
    }
    case 4: { // Подать заявку на компенсацию
        std::cout << "Введите ID заявки: ";
        long requestID;
        std::cin >> requestID;
        std::cin.ignore();
        std::cout << "Введите тип компенсации: ";
        std::string compensationType;
        getline(std::cin, compensationType);
        std::cout << "Введите сумму: ";
        double amount;
        std::cin >> amount;
        std::cin.ignore();
        std::cout << "Введите дату подачи заявки (YYYY-MM-DD): ";
        std::string submissionDate;
        getline(std::cin, submissionDate);
        std::string status = "На рассмотрении";

        std::ostringstream oss;
        oss << "REQUEST_COMPENSATION|" << requestID << "|" << 1001 << "|" << 2001 << "|"
            << compensationType << "|" << amount << "|" << submissionDate << "|" << status;
        client.sendMessage(oss.str());
        std::string response = client.receiveMessage();
        std::cout << response.substr(8) << "\n";
        break;
    }
    case 5: { // Выйти
        client.disconnect();
        std::cout << "Выход из системы.\n";
        exit(0);
    }
    default:
        std::cout << "Некорректный выбор.\n";
    }
}
