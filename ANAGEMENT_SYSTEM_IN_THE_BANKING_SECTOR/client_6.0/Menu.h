#pragma once
#ifndef MENU_H
#define MENU_H

#include <string>
#include <iostream>
#include "Client.h"

class Menu {
private:
    Client& client;                 // Ссылка на клиент для отправки запросов
    std::string role;               // Роль пользователя

    void adminMenu();               // Меню администратора
    void hrMenu();                  // Меню менеджера HR
    void cashierMenu();             // Меню кассира/оператора
    void employeeMenu();            // Меню обычного сотрудника
    void managerMenu();             // Меню руководителя подразделения
    void displayHeader();           // Заголовок меню

public:
    Menu(Client& client, const std::string& role);
    void showMenu();                // Запуск меню
};

#endif // MENU_H
