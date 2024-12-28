// Menu.cpp
#include "Menu.h"
#include "Validation.h"
#include "utils.h"
#include <iomanip>

#include <iostream>
#include <sstream>
#include <limits>

Menu::Menu(Client& cli) : client(cli), role(""), username(""), scheduleManager(cli) {}

bool Menu::authenticate() {
    std::cout << "Введите логин: ";
    std::cin >> username;
    std::cout << "Введите пароль: ";
    std::string password;
    std::cin >> password;
    if (!Validation::isValidPassword(password)) return false;
    client.sendMessage("AUTH|" + username + "|" + password);
    std::string response = client.receiveMessage();
    if (response.find("SUCCESS|") == 0) {
        role = Validation::trim(response.substr(8));
        return true;
    }
    return false;
}

void Menu::showMenu() {
    if (!authenticate()) return;
    while (true) {
        if (role == "admin") handleAdminMenu();
        else if (role == "hr") handleHRMenu();
        else if (role == "cashier") handleCashierMenu();
        else if (role == "manager") handleManagerMenu();
        else if (role == "employee") handleEmployeeMenu();
        else break;
    }
}

void Menu::addEmployeeCommon() {


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
    else return;
    std::string fullName, birthDate, address, contactInfo, startDate, status;
    do {
        std::cout << "Введите ФИО: ";
        getline(std::cin, fullName);
    } while (!Validation::isValidFullName(fullName));
    std::cout << "Введите дату рождения (YYYY-MM-DD): ";
    getline(std::cin, birthDate);
    if (!Validation::isValidDate(birthDate)) return;
    std::cout << "Введите адрес: ";
    getline(std::cin, address);
    std::cout << "Введите контактную информацию: ";
    getline(std::cin, contactInfo);
    std::cout << "Введите дату начала работы (YYYY-MM-DD): ";
    getline(std::cin, startDate);
    if (!Validation::isValidDate(startDate)) return;
    std::cout << "Введите статус (active/blocked): ";
    getline(std::cin, status);
    if (status != "active" && status != "blocked") return;
    std::ostringstream oss;
    oss << "ADD_EMPLOYEE|" << 0 << "|" << posID << "|" << fullName << "|"
        << birthDate << "|" << address << "|" << contactInfo << "|"
        << startDate << "|" << status;

    client.sendMessage(oss.str());
    std::cout << client.receiveMessage() << "\n";
}

void Menu::updateEmployeeCommon(const std::string& cmd) {
    std::cout << "Введите ID сотрудника: ";
    long empID;
    std::cin >> empID;
    std::cin.ignore();
    std::string fullName, address, contactInfo, status;
    do {
        std::cout << "Введите новое ФИО: ";
        getline(std::cin, fullName);
    } while (!Validation::isValidFullName(fullName));
    std::cout << "Введите новый адрес: ";
    getline(std::cin, address);
    std::cout << "Введите новые контактные данные: ";
    getline(std::cin, contactInfo);
    if (cmd == "UPDATE_EMPLOYEE") {
        std::ostringstream oss;
        oss << "UPDATE_EMPLOYEE|" << empID << "|" << fullName << "|" << address << "|" << contactInfo;
        client.sendMessage(oss.str());
        std::cout << client.receiveMessage() << "\n";
    }
    else if (cmd == "UPDATE_EMPLOYEE_STATUS") {
        std::cout << "Введите новый статус (active/blocked): ";
        getline(std::cin, status);
        if (status != "active" && status != "blocked") return;
        std::ostringstream oss;
        oss << "UPDATE_EMPLOYEE|" << empID << "|" << fullName << "|" << address << "|" << contactInfo << "|" << status;
        client.sendMessage(oss.str());
        std::cout << client.receiveMessage() << "\n";
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
        << "| 4. История изменений               |\n"
        << "| 5. Добавить должность              |\n"
        << "| 6. Активные курсы                  |\n"
        << "| 7. Добавить пользователя           |\n"
        << "| 8. Удалить пользователя            |\n"
        << "| 9. Все работники                  |\n"
        << "| 10. Выйти                          |\n"
        << "+------------------------------------+\n"
        << "Выберите действие: ";
    int choice;
    std::cin >> choice;
    std::cin.ignore();
    if (choice == 1) addEmployeeCommon();
    else if (choice == 2) {
        updateEmployeeCommon("UPDATE_EMPLOYEE_STATUS");
    }
    else if (choice == 3) {
        std::cout << "Введите ID сотрудника: ";
        long empID;
        std::cin >> empID;
        std::cin.ignore();
        std::ostringstream oss; oss << "BLOCK_EMPLOYEE|" << empID;
        client.sendMessage(oss.str());
        std::cout << client.receiveMessage() << "\n";
    }
    else if (choice == 4) {
        client.sendMessage("VIEW_HISTORY");
        std::cout << client.receiveMessage() << "\n";
    }
    else if (choice == 5) {
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
        oss << "ADD_POSITION|" << posID << "|" << posName << "|" << minSal << "|" << maxSal << "|" << qualReq;
        client.sendMessage(oss.str());
        std::cout << client.receiveMessage() << "\n";
    }
    else if (choice == 6) {
        client.sendMessage("VIEW_ACTIVE_COURSES");
        std::string response = client.receiveMessage();
        if (response.find("SUCCESS|") == 0) {
            std::string data = response.substr(8);
            std::istringstream iss(data);
            std::string line;
            std::cout << "-------------------------------------------------------------------------------------\n";
            std::cout << "| ID   | Тема                         | Начало       | Конец        |\n";
            std::cout << "-------------------------------------------------------------------------------------\n";
            while (std::getline(iss, line)) {
                line = trim(line);
                if (line.empty()) continue;
                auto parts = split(line, ',');
                // parts:
                // 0: "ID: C01"
                // 1: " Тема: Основы кредитования"
                // 2: " Начало: 2021-01-01"
                // 3: " Конец: 2021-01-10"

                if (parts.size() < 4) continue;

                std::string cid = trim(parts[0].substr(parts[0].find(':') + 1));
                std::string topic = trim(parts[1].substr(parts[1].find(':') + 1));
                std::string start = trim(parts[2].substr(parts[2].find(':') + 1));
                std::string end = trim(parts[3].substr(parts[3].find(':') + 1));

                std::cout << "| " << std::setw(4) << cid << " | " << std::setw(28) << topic
                    << " | " << std::setw(12) << start
                    << " | " << std::setw(12) << end << " |\n";
            }
            std::cout << "-------------------------------------------------------------------------------------\n";
        }
        else {
            std::cout << response << "\n";
        }
    }
    /*else if (choice == 7) {
        client.sendMessage("VIEW_ACTIVE_CONNECTIONS");
        std::string response = client.receiveMessage();
        if (response.find("SUCCESS|") == 0) {
            std::string data = response.substr(8);
            std::istringstream iss(data);
            std::string line;
            std::cout << "---------------------------------------------------------------\n";
            std::cout << "| ID  | IP              | Время подключения          |\n";
            std::cout << "---------------------------------------------------------------\n";
            while (std::getline(iss, line)) {
                line = trim(line);
                if (line.empty()) continue;
                auto parts = split(line, ',');
                // parts:
                // 0: "ID: 1"
                // 1: " IP: 192.168.0.10"
                // 2: " Время подключения: 2021-01-01 10:00:00"

                if (parts.size() < 3) continue;

                std::string id = trim(parts[0].substr(parts[0].find(':') + 1));
                std::string ip = trim(parts[1].substr(parts[1].find(':') + 1));
                std::string time = trim(parts[2].substr(parts[2].find(':') + 1));

                std::cout << "| " << std::setw(3) << id << " | " << std::setw(15) << ip
                    << " | " << std::setw(24) << time << " |\n";
            }
            std::cout << "---------------------------------------------------------------\n";
        }
        else {
            std::cout << response << "\n";
        }
    }*/

    else if (choice == 7) {
        std::cout << "Введите логин: ";
        std::string newUsername;
        getline(std::cin, newUsername);
        std::string newPassword;
        do {
            std::cout << "Введите пароль: ";
            getline(std::cin, newPassword);
        } while (!Validation::isValidPassword(newPassword));
        std::cout << "Выберите роль:\n1. hr\n2. cashier\n3. manager\n4. employee\n5. Отмена\n";
        int rc; std::cin >> rc; std::cin.ignore();
        std::string newRole;
        if (rc == 1) newRole = "hr";
        else if (rc == 2) newRole = "cashier";
        else if (rc == 3) newRole = "manager";
        else if (rc == 4) newRole = "employee";
        else return;
        std::string status = "active";
        std::ostringstream oss;
        oss << "ADD_USER|" << newUsername << "|" << newPassword << "|" << newRole << "|" << status;
        client.sendMessage(oss.str());
        std::cout << client.receiveMessage() << "\n";
    }
    else if (choice == 8) {
        std::cout << "Введите логин для удаления: ";
        std::string delUser; getline(std::cin, delUser);
        std::ostringstream oss;
        oss << "DELETE_USER|" << delUser;
        client.sendMessage(oss.str());
        std::cout << client.receiveMessage() << "\n";
    }
    else if (choice == 9) {
        client.sendMessage("VIEW_ALL_EMPLOYEES");
        std::string response = client.receiveMessage();
        if (response.find("SUCCESS|") == 0) {
            std::string data = response.substr(8);
            std::istringstream iss(data);
            std::string line;
            std::cout << "-----------------------------------------------------------------------------------------------\n";
            std::cout << "| ID  | ФИО                         | Должность         | Отдел              | Зарплата         |\n";
            std::cout << "-----------------------------------------------------------------------------------------------\n";
            while (std::getline(iss, line)) {
                line = trim(line);
                if (line.empty()) continue;
                auto parts = split(line, ',');
                // parts:
                // 0: "ID: 1"
                // 1: " ФИО: Иванов Иван..."
                // 2: " Должность: Менеджер"
                // 3: " Отдел: Отдел продаж"
                // 4: " Зарплата: 50000"
                if (parts.size() < 5) continue;

                std::string id = trim(parts[0].substr(parts[0].find(':') + 1));
                std::string fio = trim(parts[1].substr(parts[1].find(':') + 1));
                std::string posName = trim(parts[2].substr(parts[2].find(':') + 1));
                std::string dept = trim(parts[3].substr(parts[3].find(':') + 1));
                std::string sal = trim(parts[4].substr(parts[4].find(':') + 1));

                // Выравнивание по ширине (пример без iomanip, просто демонстрация):
                // Можно использовать <iomanip> и std::setw(...) для более красивого форматирования.
                // Здесь упрощенно.
                std::cout << "| " << std::setw(3) << id << " | " << std::setw(25) << fio
                    << " | " << std::setw(15) << posName
                    << " | " << std::setw(18) << dept
                    << " | " << std::setw(10) << sal << " |\n";
            }
            std::cout << "-----------------------------------------------------------------------------------------------\n";
        }
        else {
            std::cout << response << "\n";
        }
    }

    else if (choice == 10) {
        client.disconnect();
        exit(0);
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
        << "| 3. Добавить курс                   |\n"
        << "| 4. Изменить курс                   |\n"
        << "| 5. Удалить курс                    |\n"
        << "| 6. Активные курсы                  |\n"
        << "| 7. Просмотреть логи                |\n"
        << "| 8. Графики работы                  |\n"
        << "| 9. Обработать заявку на компенсацию|\n"
        << "| 10. Выйти                          |\n"
        << "+------------------------------------+\n"
        << "Выберите действие: ";
    int ch; std::cin >> ch; std::cin.ignore();
    if (ch == 1) addEmployeeCommon();
    else if (ch == 2) {
        std::cout << "Введите ID сотрудника: ";
        long empID; std::cin >> empID; std::cin.ignore();
        std::string fullName, address, contactInfo, status;
        do {
            std::cout << "Новое ФИО: ";
            getline(std::cin, fullName);
        } while (!Validation::isValidFullName(fullName));
        std::cout << "Новый адрес: ";
        getline(std::cin, address);
        std::cout << "Новый телефон: ";
        getline(std::cin, contactInfo);
        std::cout << "Новый статус (active/blocked): ";
        getline(std::cin, status);
        if (status != "active" && status != "blocked") return;
        std::ostringstream oss;
        oss << "UPDATE_EMPLOYEE|" << empID << "|" << fullName << "|" << address << "|" << contactInfo << "|" << status;
        client.sendMessage(oss.str());
        std::cout << client.receiveMessage() << "\n";
    }
    else if (ch == 3) {
        std::cout << "ID курса: ";
        std::string courseID; getline(std::cin, courseID);
        std::cout << "Тема: ";
        std::string topic; getline(std::cin, topic);
        std::cout << "Дата начала (YYYY-MM-DD): ";
        std::string sDate; getline(std::cin, sDate);
        if (!Validation::isValidDate(sDate)) return;
        std::cout << "Дата окончания (YYYY-MM-DD): ";
        std::string eDate; getline(std::cin, eDate);
        if (!Validation::isValidDate(eDate)) return;
        std::ostringstream oss;
        oss << "ADD_COURSE|" << courseID << "|" << topic << "|" << sDate << "|" << eDate;
        client.sendMessage(oss.str());
        std::cout << client.receiveMessage() << "\n";
    }
    else if (ch == 4) {
        std::cout << "ID курса: ";
        std::string cid; getline(std::cin, cid);
        std::cout << "Новая тема: ";
        std::string topic; getline(std::cin, topic);
        std::cout << "Новая дата начала: ";
        std::string sd; getline(std::cin, sd);
        if (!Validation::isValidDate(sd)) return;
        std::cout << "Новая дата окончания: ";
        std::string ed; getline(std::cin, ed);
        if (!Validation::isValidDate(ed)) return;
        std::ostringstream oss;
        oss << "UPDATE_COURSE|" << cid << "|" << topic << "|" << sd << "|" << ed;
        client.sendMessage(oss.str());
        std::cout << client.receiveMessage() << "\n";
    }
    else if (ch == 5) {
        std::cout << "ID курса: ";
        std::string cid; getline(std::cin, cid);
        std::ostringstream oss;
        oss << "DELETE_COURSE|" << cid;
        client.sendMessage(oss.str());
        std::cout << client.receiveMessage() << "\n";
    }
    else if (ch == 6) {
        client.sendMessage("VIEW_ACTIVE_COURSES");
        std::string response = client.receiveMessage();
        if (response.find("SUCCESS|") == 0) {
            std::string data = response.substr(8);
            std::istringstream iss(data);
            std::string line;
            std::cout << "-------------------------------------------------------------------------------------\n";
            std::cout << "| ID   | Тема                         | Начало       | Конец        |\n";
            std::cout << "-------------------------------------------------------------------------------------\n";
            while (std::getline(iss, line)) {
                line = trim(line);
                if (line.empty()) continue;
                auto parts = split(line, ',');
                // parts:
                // 0: "ID: C01"
                // 1: " Тема: Основы кредитования"
                // 2: " Начало: 2021-01-01"
                // 3: " Конец: 2021-01-10"

                if (parts.size() < 4) continue;

                std::string cid = trim(parts[0].substr(parts[0].find(':') + 1));
                std::string topic = trim(parts[1].substr(parts[1].find(':') + 1));
                std::string start = trim(parts[2].substr(parts[2].find(':') + 1));
                std::string end = trim(parts[3].substr(parts[3].find(':') + 1));

                std::cout << "| " << std::setw(4) << cid << " | " << std::setw(28) << topic
                    << " | " << std::setw(12) << start
                    << " | " << std::setw(12) << end << " |\n";
            }
            std::cout << "-------------------------------------------------------------------------------------\n";
        }
        else {
            std::cout << response << "\n";
        }
    }
    else if (ch == 7) {
        client.sendMessage("VIEW_LOGS");
        std::cout << client.receiveMessage() << "\n";
    }
    else if (ch == 8) {
        scheduleManager.manageWorkSchedules();
    }
    else if (ch == 9) {
        std::cout << "ID заявки: ";
        long cID; std::cin >> cID; std::cin.ignore();
        std::cout << "1. Одобрить\n2. Отклонить\n";
        int ac; std::cin >> ac; std::cin.ignore();
        if (ac == 1) {
            std::ostringstream oss; oss << "APPROVE_COMPENSATION|" << cID;
            client.sendMessage(oss.str());
            std::cout << client.receiveMessage() << "\n";
        }
        else if (ac == 2) {
            std::ostringstream oss; oss << "DECLINE_COMPENSATION|" << cID;
            client.sendMessage(oss.str());
            std::cout << client.receiveMessage() << "\n";
        }
    }
    else if (ch == 10) {
        client.disconnect();
        exit(0);
    }
}

void Menu::handleCashierMenu() {
    std::cout << "\n+====================================+\n"
        << "|      Система управления персоналом |\n"
        << "+====================================+\n"
        << "| Роль: Cashier                      |\n"
        << "+------------------------------------+\n"
        << "| 1. Просмотреть зарплаты            |\n"
        << "| 2. Обработать заявку на компенсацию |\n"
        << "| 3. Активные курсы                  |\n"
        << "| 4. Выйти                           |\n"
        << "+------------------------------------+\n"
        << "Выберите действие: ";
    int c; std::cin >> c; std::cin.ignore();
    if (c == 1) {
        client.sendMessage("VIEW_SALARIES");
        std::cout << client.receiveMessage() << "\n";
    }
    else if (c == 2) {
        std::cout << "ID заявки: ";
        long rID; std::cin >> rID; std::cin.ignore();
        std::cout << "Статус (Одобрено/Отклонено): ";
        std::string ns; getline(std::cin, ns);
        std::ostringstream oss; oss << "PROCESS_COMPENSATION|" << rID << "|" << ns;
        client.sendMessage(oss.str());
        std::cout << client.receiveMessage() << "\n";
    }
    else if (c == 3) {
        client.sendMessage("VIEW_ACTIVE_COURSES");
        std::string response = client.receiveMessage();
        if (response.find("SUCCESS|") == 0) {
            std::string data = response.substr(8);
            std::istringstream iss(data);
            std::string line;
            std::cout << "-------------------------------------------------------------------------------------\n";
            std::cout << "| ID   | Тема                         | Начало       | Конец        |\n";
            std::cout << "-------------------------------------------------------------------------------------\n";
            while (std::getline(iss, line)) {
                line = trim(line);
                if (line.empty()) continue;
                auto parts = split(line, ',');
                // parts:
                // 0: "ID: C01"
                // 1: " Тема: Основы кредитования"
                // 2: " Начало: 2021-01-01"
                // 3: " Конец: 2021-01-10"

                if (parts.size() < 4) continue;

                std::string cid = trim(parts[0].substr(parts[0].find(':') + 1));
                std::string topic = trim(parts[1].substr(parts[1].find(':') + 1));
                std::string start = trim(parts[2].substr(parts[2].find(':') + 1));
                std::string end = trim(parts[3].substr(parts[3].find(':') + 1));

                std::cout << "| " << std::setw(4) << cid << " | " << std::setw(28) << topic
                    << " | " << std::setw(12) << start
                    << " | " << std::setw(12) << end << " |\n";
            }
            std::cout << "-------------------------------------------------------------------------------------\n";
        }
        else {
            std::cout << response << "\n";
        }
    }
    else if (c == 4) {
        client.disconnect();
        exit(0);
    }
}

void Menu::handleManagerMenu() {
    std::cout << "\n+====================================+\n"
        << "|      Система управления персоналом |\n"
        << "+====================================+\n"
        << "| Роль: Manager                      |\n"
        << "+------------------------------------+\n"
        << "| 1. Графики работы                  |\n"
        << "| 2. Оценить производительность      |\n"
        << "| 3. Активные курсы                  |\n"
        << "| 4. Выйти                           |\n"
        << "+------------------------------------+\n"
        << "Выберите действие: ";
    int ch; std::cin >> ch; std::cin.ignore();
    if (ch == 1) {
        scheduleManager.manageWorkSchedules();
    }
    else if (ch == 2) {
        std::cout << "Оценка производительности еще не реализована.\n";
    }
    else if (ch == 3) {
        client.sendMessage("VIEW_ACTIVE_COURSES");
        std::string response = client.receiveMessage();
        if (response.find("SUCCESS|") == 0) {
            std::string data = response.substr(8);
            std::istringstream iss(data);
            std::string line;
            std::cout << "-------------------------------------------------------------------------------------\n";
            std::cout << "| ID   | Тема                         | Начало       | Конец        |\n";
            std::cout << "-------------------------------------------------------------------------------------\n";
            while (std::getline(iss, line)) {
                line = trim(line);
                if (line.empty()) continue;
                auto parts = split(line, ',');
                // parts:
                // 0: "ID: C01"
                // 1: " Тема: Основы кредитования"
                // 2: " Начало: 2021-01-01"
                // 3: " Конец: 2021-01-10"

                if (parts.size() < 4) continue;

                std::string cid = trim(parts[0].substr(parts[0].find(':') + 1));
                std::string topic = trim(parts[1].substr(parts[1].find(':') + 1));
                std::string start = trim(parts[2].substr(parts[2].find(':') + 1));
                std::string end = trim(parts[3].substr(parts[3].find(':') + 1));

                std::cout << "| " << std::setw(4) << cid << " | " << std::setw(28) << topic
                    << " | " << std::setw(12) << start
                    << " | " << std::setw(12) << end << " |\n";
            }
            std::cout << "-------------------------------------------------------------------------------------\n";
        }
        else {
            std::cout << response << "\n";
        }
    }
    else if (ch == 4) {
        client.disconnect();
        exit(0);
    }
}

void Menu::handleEmployeeMenu() {
    std::cout << "\n+====================================+\n"
        << "|      Система управления персоналом |\n"
        << "+====================================+\n"
        << "| Роль: Employee                     |\n"
        << "+------------------------------------+\n"
        << "| 1. Своя информация                 |\n"
        << "| 2. Свой график работы              |\n"
        << "| 3. Записаться на курс              |\n"
        << "| 4. Заявка на компенсацию           |\n"
        << "| 5. Выйти                           |\n"
        << "+------------------------------------+\n"
        << "Выберите действие: ";
    int c; std::cin >> c; std::cin.ignore();
    if (c == 1) {
        client.sendMessage("VIEW_OWN_INFO|" + username);
        std::cout << client.receiveMessage() << "\n";
    }
    else if (c == 2) {
        client.sendMessage("VIEW_OWN_SCHEDULE|" + username);
        std::cout << client.receiveMessage() << "\n";
    }
    else if (c == 3) {
        client.sendMessage("VIEW_ACTIVE_COURSES");
        {
            std::string resp = client.receiveMessage();
            std::cout << resp << "\n";
            std::cout << "Введите ID курса: ";
            std::string cid; getline(std::cin, cid);
            long employeeID = 1001;
            std::ostringstream oss; oss << "ENROLL_COURSE|" << cid << "|" << employeeID;
            client.sendMessage(oss.str());
            std::cout << client.receiveMessage() << "\n";
        }
    }
    else if (c == 4) {
        std::cout << "ID заявки: ";
        long reqID; std::cin >> reqID; std::cin.ignore();
        std::cout << "Тип компенсации: ";
        std::string ctype; getline(std::cin, ctype);
        std::cout << "Сумма: ";
        double amount; std::cin >> amount; std::cin.ignore();
        std::cout << "Дата подачи (YYYY-MM-DD): ";
        std::string sdate; getline(std::cin, sdate);
        if (!Validation::isValidDate(sdate)) return;
        long empID = 1001; long posID = 2001;
        std::string stat = "На рассмотрении";
        std::ostringstream oss; oss << "REQUEST_COMPENSATION|" << reqID << "|" << empID << "|" << posID << "|" << ctype << "|" << amount << "|" << sdate << "|" << stat;
        client.sendMessage(oss.str());
        std::cout << client.receiveMessage() << "\n";
    }
    else if (c == 5) {
        client.disconnect();
        exit(0);
    }
}
