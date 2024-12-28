// Menu.cpp
#include "Menu.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <locale>
#include <regex>
#include "Validation.h"
#include "ScheduleManager.h"
// Используем Validation::isValidFullName и т.д.

Menu::Menu(Client& cli) : client(cli), role(""), username(""), scheduleManager(cli) {}

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
    while (true) { // Добавляем цикл для повторного отображения меню после выполнения действия
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
            << "| 10. Просмотреть всех работников    |\n" // Новый пункт
            << "| 11. Выйти                           |\n"
            << "+------------------------------------+\n"
            << "Выберите действие: ";
        int choice;
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear(); // Очистка флага ошибки
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Игнорирование неверного ввода
            std::cout << "Некорректный ввод. Пожалуйста, введите число от 1 до 11.\n";
            continue;
        }
        std::cin.ignore(); // Очистка буфера после ввода числа

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
            else { std::cout << "Отмена добавления сотрудника.\n"; break; }

            std::string fullName, birthDate, address, contactInfo, startDate, status;
            do {
                std::cout << "Введите ФИО сотрудника: ";
                getline(std::cin, fullName);
                if (!Validation::isValidFullName(fullName)) {
                    std::cout << "Некорректное ФИО. Попробуйте снова.\n";
                }
            } while (!Validation::isValidFullName(fullName));

            std::cout << "Введите дату рождения (YYYY-MM-DD): ";
            getline(std::cin, birthDate);
            if (!Validation::isValidDate(birthDate)) {
                std::cout << "Некорректный формат даты рождения. Ожидается YYYY-MM-DD.\n";
                break;
            }

            std::cout << "Введите адрес: ";
            getline(std::cin, address);
            std::cout << "Введите контактную информацию: ";
            getline(std::cin, contactInfo);
            std::cout << "Введите дату начала работы (YYYY-MM-DD): ";
            getline(std::cin, startDate);
            if (!Validation::isValidDate(startDate)) {
                std::cout << "Некорректный формат даты начала работы. Ожидается YYYY-MM-DD.\n";
                break;
            }
            std::cout << "Введите статус (active/blocked): ";
            getline(std::cin, status);
            if (status != "active" && status != "blocked") {
                std::cout << "Некорректный статус. Используйте 'active' или 'blocked'.\n";
                break;
            }

            std::ostringstream oss;
            oss << "ADD_EMPLOYEE|" << empID << "|" << posID << "|" << fullName << "|"
                << birthDate << "|" << address << "|" << contactInfo << "|"
                << startDate << "|" << status;
            client.sendMessage(oss.str());
            std::string response = client.receiveMessage();
            if (response.find("SUCCESS|") == 0) {
                std::cout << response.substr(8) << "\n"; // Вывод сообщения после SUCCESS|
            }
            else {
                std::cout << "Ошибка: " << response.substr(6) << "\n"; // Вывод сообщения после ERROR|
            }
            break;
        }
        case 2: { // Изменить данные сотрудника
            std::cout << "Введите ID сотрудника: ";
            long empID;
            std::cin >> empID;
            std::cin.ignore();

            std::string fullName, address, contactInfo;
            do {
                std::cout << "Введите новое ФИО сотрудника: ";
                getline(std::cin, fullName);
                if (!Validation::isValidFullName(fullName)) {
                    std::cout << "Некорректное ФИО. Попробуйте снова.\n";
                }
            } while (!Validation::isValidFullName(fullName));

            std::cout << "Введите новый адрес: ";
            getline(std::cin, address);
            std::cout << "Введите новые контактные данные: ";
            getline(std::cin, contactInfo);

            std::ostringstream oss;
            oss << "UPDATE_EMPLOYEE|" << empID << "|" << fullName << "|"
                << address << "|" << contactInfo;
            client.sendMessage(oss.str());
            std::string response = client.receiveMessage();
            if (response.find("SUCCESS|") == 0) {
                std::cout << response.substr(8) << "\n";
            }
            else {
                std::cout << "Ошибка: " << response.substr(6) << "\n";
            }
            break;
        }
        case 3: { // Блокировать сотрудника
            std::cout << "Введите ID сотрудника для блокировки: ";
            long empID;
            std::cin >> empID;
            std::cin.ignore();

            std::ostringstream oss;
            oss << "BLOCK_EMPLOYEE|" << empID;
            client.sendMessage(oss.str());
            std::string response = client.receiveMessage();
            if (response.find("SUCCESS|") == 0) {
                std::cout << response.substr(8) << "\n";
            }
            else {
                std::cout << "Ошибка: " << response.substr(6) << "\n";
            }
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
                std::cout << "Ошибка: " << response.substr(6) << "\n";
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
            if (response.find("SUCCESS|") == 0) {
                std::cout << response.substr(8) << "\n"; // Вывод сообщения после SUCCESS|
            }
            else {
                std::cout << "Ошибка: " << response.substr(6) << "\n"; // Вывод сообщения после ERROR|
            }
            break;
        }
        case 6: { // Просмотреть активные курсы
            std::string command = "VIEW_ACTIVE_COURSES";
            client.sendMessage(command);
            std::string response = client.receiveMessage();
            if (response.find("SUCCESS|") == 0) {
                std::cout << "Активные курсы:\n" << response.substr(8) << "\n";
            }
            else {
                std::cout << "Ошибка: " << response.substr(6) << "\n";
            }
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
                std::cout << "Ошибка: " << response.substr(6) << "\n";
            }
            break;
        }

        case 8: { // Управлять графиками работы
            scheduleManager.manageWorkSchedules();
            break;
        }
        case 9: { // Обработать заявку на компенсацию
            std::cout << "Введите ID заявки на компенсацию: ";
            long compensationID;
            std::cin >> compensationID;
            std::cin.ignore();

            std::cout << "Выберите действие:\n"
                << "1. Одобрить заявку\n"
                << "2. Отклонить заявку\n"
                << "Выберите действие: ";

            int actionChoice;
            std::cin >> actionChoice;
            std::cin.ignore();

            if (actionChoice == 1) {
                std::ostringstream oss;
                oss << "APPROVE_COMPENSATION|" << compensationID;
                client.sendMessage(oss.str());
                std::string response = client.receiveMessage();
                if (response.find("SUCCESS|") == 0) {
                    std::cout << response.substr(8) << "\n";
                }
                else {
                    std::cout << "Ошибка: " << response.substr(6) << "\n";
                }
            }
            else if (actionChoice == 2) {
                std::ostringstream oss;
                oss << "DECLINE_COMPENSATION|" << compensationID;
                client.sendMessage(oss.str());
                std::string response = client.receiveMessage();
                if (response.find("SUCCESS|") == 0) {
                    std::cout << response.substr(8) << "\n";
                }
                else {
                    std::cout << "Ошибка: " << response.substr(6) << "\n";
                }
            }
            else {
                std::cout << "Некорректный выбор.\n";
            }
            break;
        }
        case 10: { // Просмотреть всех работников
            std::string command = "VIEW_ALL_EMPLOYEES";
            client.sendMessage(command);
            std::string response = client.receiveMessage();
            if (response.find("SUCCESS|") == 0) {
                std::cout << "Все работники:\n" << response.substr(8) << "\n";
            }
            else {
                std::cout << "Ошибка: " << response.substr(6) << "\n";
            }
            break;
        }
        case 11: { // Выйти
            client.disconnect();
            std::cout << "Выход из системы.\n";
            exit(0);
        }
        default:
            std::cout << "Некорректный выбор.\n";
        }
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
        << "| 8. Управлять графиками работы      |\n"  // Дописываем пункт 8
        << "| 9. Обработать заявку на компенсацию|\n"  // Дописываем пункт 9
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
    case 2: { // Изменить данные сотрудника
        std::cout << "Введите ID сотрудника для изменения данных: ";
        long empID;
        std::cin >> empID;
        std::cin.ignore(); // Очистка буфера

        std::cout << "Введите новое ФИО сотрудника: ";
        std::string fullName;
        getline(std::cin, fullName);

        std::cout << "Введите новый адрес: ";
        std::string address;
        getline(std::cin, address);

        std::cout << "Введите новый контактный телефон: ";
        std::string contactInfo;
        getline(std::cin, contactInfo);

        std::cout << "Введите новый статус (active/blocked): ";
        std::string status;
        getline(std::cin, status);

        std::ostringstream oss;
        oss << "UPDATE_EMPLOYEE|" << empID << "|" << fullName << "|" << address << "|" << contactInfo << "|" << status;
        client.sendMessage(oss.str());
        std::string response = client.receiveMessage();
        std::cout << response.substr(8) << "\n"; // Вывод сообщения после SUCCESS|
        break;
    }

    case 3: { // Добавить банковский курс
        std::cout << "Введите ID курса: ";
        std::string courseID;
        getline(std::cin, courseID);

        // Проверка на пустой ID курса
        if (courseID.empty() || courseID.find_first_not_of(" \t") == std::string::npos) {
            std::cout << "Ошибка: ID курса не может быть пустым или содержать только пробелы.\n";
            break;
        }

        std::cout << "Введите тему курса: ";
        std::string topic;
        getline(std::cin, topic);

        // Проверка на пустую тему курса
        if (topic.empty()) {
            std::cout << "Ошибка: Тема курса не может быть пустой.\n";
            break;
        }

        std::cout << "Введите дату начала курса (YYYY-MM-DD): ";
        std::string startDate;
        getline(std::cin, startDate);

        // Проверка на корректность формата даты начала
        if (startDate.empty() || !std::regex_match(startDate, std::regex(R"(\d{4}-\d{2}-\d{2})"))) {
            std::cout << "Ошибка: Некорректный формат даты начала. Ожидается YYYY-MM-DD.\n";
            break;
        }

        std::cout << "Введите дату окончания курса (YYYY-MM-DD): ";
        std::string endDate;
        getline(std::cin, endDate);

        // Проверка на корректность формата даты окончания
        if (endDate.empty() || !std::regex_match(endDate, std::regex(R"(\d{4}-\d{2}-\d{2})"))) {
            std::cout << "Ошибка: Некорректный формат даты окончания. Ожидается YYYY-MM-DD.\n";
            break;
        }

        // Формирование строки для отправки
        std::ostringstream oss;
        oss << "ADD_COURSE|" << courseID << "|" << topic << "|" << startDate << "|" << endDate;

        // Отправка сообщения
        client.sendMessage(oss.str());

        // Получение ответа
        std::string response = client.receiveMessage();

        // Вывод сообщения после SUCCESS|
        if (response.find("SUCCESS") == 0) {
            std::cout << "Курс успешно добавлен: " << response.substr(8) << "\n";
        }
        else {
            std::cout << "Ошибка при добавлении курса: " << response.substr(7) << "\n";
        }
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
    case 6: { // Просмотреть активные курсы
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
    case 8: { // Управлять графиками работы
        std::cout << "Управление графиками работы:\n"
            << "1. Добавить новый график\n"
            << "2. Изменить график работы\n"
            << "3. Удалить график работы\n"
            << "4. Просмотреть графики работы\n"
            << "Выберите действие: ";

        int scheduleChoice;
        std::cin >> scheduleChoice;
        std::cin.ignore(); // Очистка буфера

        switch (scheduleChoice) {
        case 1: {
            std::cout << "Введите ID сотрудника: ";
            long empID;
            std::cin >> empID;
            std::cin.ignore();

            std::cout << "Введите день недели (1-7): ";
            int dayOfWeek;
            std::cin >> dayOfWeek;
            std::cin.ignore();

            std::cout << "Введите время начала (HH:MM): ";
            std::string startTime;
            getline(std::cin, startTime);

            std::cout << "Введите время окончания (HH:MM): ";
            std::string endTime;
            getline(std::cin, endTime);

            std::ostringstream oss;
            oss << "ADD_WORK_SCHEDULE|" << empID << "|" << dayOfWeek << "|" << startTime << "|" << endTime;
            client.sendMessage(oss.str());
            std::string response = client.receiveMessage();
            std::cout << response.substr(8) << "\n";
            break;
        }
        case 2: {
            std::cout << "Введите ID сотрудника для изменения графика: ";
            long empID;
            std::cin >> empID;
            std::cin.ignore();

            std::cout << "Введите новый день недели (1-7): ";
            int dayOfWeek;
            std::cin >> dayOfWeek;
            std::cin.ignore();

            std::cout << "Введите новое время начала (HH:MM): ";
            std::string startTime;
            getline(std::cin, startTime);

            std::cout << "Введите новое время окончания (HH:MM): ";
            std::string endTime;
            getline(std::cin, endTime);

            std::ostringstream oss;
            oss << "UPDATE_WORK_SCHEDULE|" << empID << "|" << dayOfWeek << "|" << startTime << "|" << endTime;
            client.sendMessage(oss.str());
            std::string response = client.receiveMessage();
            std::cout << response.substr(8) << "\n";
            break;
        }
        case 3: {
            std::cout << "Введите ID сотрудника для удаления графика: ";
            long empID;
            std::cin >> empID;
            std::cin.ignore();

            std::ostringstream oss;
            oss << "DELETE_WORK_SCHEDULE|" << empID;
            client.sendMessage(oss.str());
            std::string response = client.receiveMessage();
            std::cout << response.substr(8) << "\n";
            break;
        }
        case 4: {
            std::cout << "Введите ID сотрудника для просмотра графиков: ";
            long empID;
            std::cin >> empID;
            std::cin.ignore();

            std::ostringstream oss;
            oss << "VIEW_WORK_SCHEDULE|" << empID;
            client.sendMessage(oss.str());
            std::string response = client.receiveMessage();
            std::cout << response.substr(8) << "\n";
            break;
        }
        default:
            std::cout << "Некорректный выбор.\n";
        }
        break;
    }
    case 9: { // Обработать заявку на компенсацию
        std::cout << "Введите ID заявки на компенсацию: ";
        long compensationID;
        std::cin >> compensationID;
        std::cin.ignore();

        std::cout << "Выберите действие:\n"
            << "1. Одобрить заявку\n"
            << "2. Отклонить заявку\n"
            << "Выберите действие: ";

        int actionChoice;
        std::cin >> actionChoice;
        std::cin.ignore();

        if (actionChoice == 1) {
            std::ostringstream oss;
            oss << "APPROVE_COMPENSATION|" << compensationID;
            client.sendMessage(oss.str());
            std::string response = client.receiveMessage();
            std::cout << response.substr(8) << "\n";
        }
        else if (actionChoice == 2) {
            std::ostringstream oss;
            oss << "DECLINE_COMPENSATION|" << compensationID;
            client.sendMessage(oss.str());
            std::string response = client.receiveMessage();
            std::cout << response.substr(8) << "\n";
        }
        else {
            std::cout << "Некорректный выбор.\n";
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
        std::cout << "Управление графиками работы:\n"
            << "1. Добавить новый график\n"
            << "2. Изменить график работы\n"
            << "3. Удалить график работы\n"
            << "4. Просмотреть графики работы\n"
            << "Выберите действие: ";

        int scheduleChoice;
        std::cin >> scheduleChoice;
        std::cin.ignore(); // Очистка буфера

        switch (scheduleChoice) {
        case 1: {
            std::cout << "Введите ID сотрудника: ";
            long empID;
            std::cin >> empID;
            std::cin.ignore();

            std::cout << "Введите день недели (1-7): ";
            int dayOfWeek;
            std::cin >> dayOfWeek;
            std::cin.ignore();

            std::cout << "Введите время начала (HH:MM): ";
            std::string startTime;
            getline(std::cin, startTime);

            std::cout << "Введите время окончания (HH:MM): ";
            std::string endTime;
            getline(std::cin, endTime);

            std::ostringstream oss;
            oss << "ADD_WORK_SCHEDULE|" << empID << "|" << dayOfWeek << "|" << startTime << "|" << endTime;
            client.sendMessage(oss.str());
            std::string response = client.receiveMessage();
            std::cout << response.substr(8) << "\n";
            break;
        }
        case 2: {
            std::cout << "Введите ID сотрудника для изменения графика: ";
            long empID;
            std::cin >> empID;
            std::cin.ignore();

            std::cout << "Введите новый день недели (1-7): ";
            int dayOfWeek;
            std::cin >> dayOfWeek;
            std::cin.ignore();

            std::cout << "Введите новое время начала (HH:MM): ";
            std::string startTime;
            getline(std::cin, startTime);

            std::cout << "Введите новое время окончания (HH:MM): ";
            std::string endTime;
            getline(std::cin, endTime);

            std::ostringstream oss;
            oss << "UPDATE_WORK_SCHEDULE|" << empID << "|" << dayOfWeek << "|" << startTime << "|" << endTime;
            client.sendMessage(oss.str());
            std::string response = client.receiveMessage();
            std::cout << response.substr(8) << "\n";
            break;
        }
        case 3: {
            std::cout << "Введите ID сотрудника для удаления графика: ";
            long empID;
            std::cin >> empID;
            std::cin.ignore();

            std::ostringstream oss;
            oss << "DELETE_WORK_SCHEDULE|" << empID;
            client.sendMessage(oss.str());
            std::string response = client.receiveMessage();
            std::cout << response.substr(8) << "\n";
            break;
        }
        case 4: {
            std::cout << "Введите ID сотрудника для просмотра графиков: ";
            long empID;
            std::cin >> empID;
            std::cin.ignore();

            std::ostringstream oss;
            oss << "VIEW_WORK_SCHEDULE|" << empID;
            client.sendMessage(oss.str());
            std::string response = client.receiveMessage();
            std::cout << response.substr(8) << "\n";
            break;
        }
        default:
            std::cout << "Некорректный выбор.\n";
        }
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
