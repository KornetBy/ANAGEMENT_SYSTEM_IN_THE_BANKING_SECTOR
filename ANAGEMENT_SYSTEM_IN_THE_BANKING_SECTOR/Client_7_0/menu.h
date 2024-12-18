#pragma once
// Menu.h
#pragma once
#include <string>
#include "Client.h"

class Menu {
    Menu(); // ����������� �� ���������
    Menu(const std::string& title); // ����������� � ����������
private:
    Client& client;
    std::string role;
    std::string username;
public:
    Menu(Client& cli);
    void showMenu();
    bool authenticate();
    void handleAdminMenu();
    void handleHRMenu();
    void handleCashierMenu();
    void handleManagerMenu();
    void handleEmployeeMenu();
};
