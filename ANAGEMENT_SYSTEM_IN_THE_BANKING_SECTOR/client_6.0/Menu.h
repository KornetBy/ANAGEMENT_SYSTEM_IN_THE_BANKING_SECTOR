#pragma once
#ifndef MENU_H
#define MENU_H

#include <string>
#include <iostream>
#include "Client.h"

class Menu {
private:
    Client& client;                 // ������ �� ������ ��� �������� ��������
    std::string role;               // ���� ������������

    void adminMenu();               // ���� ��������������
    void hrMenu();                  // ���� ��������� HR
    void cashierMenu();             // ���� �������/���������
    void employeeMenu();            // ���� �������� ����������
    void managerMenu();             // ���� ������������ �������������
    void displayHeader();           // ��������� ����

public:
    Menu(Client& client, const std::string& role);
    void showMenu();                // ������ ����
};

#endif // MENU_H
