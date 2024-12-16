#include "Menu.h"

// �����������
Menu::Menu(Client& client, const std::string& role) : client(client), role(role) {}

// ����� ���������
void Menu::displayHeader() {
    std::cout << "\n=====================================\n";
    std::cout << "      ������� ���������� ����������   \n";
    std::cout << "=====================================\n";
    std::cout << "������� ����: " << role << "\n";
    std::cout << "-------------------------------------\n";
}

// ����� ���� � ����������� �� ����
void Menu::showMenu() {
    if (role == "admin") {
        adminMenu();
    } else if (role == "hr") {
        hrMenu();
    } else if (role == "cashier") {
        cashierMenu();
    } else if (role == "manager") {
        managerMenu();
    } else {
        employeeMenu();
    }
}

// ���� ��������������
void Menu::adminMenu() {
    int choice = 0;
    while (true) {
        displayHeader();
        std::cout << "1. �������� ����������\n";
        std::cout << "2. ����������� ����������\n";
        std::cout << "3. ���������� ������� ���������\n";
        std::cout << "4. �������� ���������\n";
        std::cout << "5. �����\n";
        std::cin >> choice;

        switch (choice) {
            case 1: client.sendMessage("ADD_EMPLOYEE"); break;
            case 2: client.sendMessage("BLOCK_USER"); break;
            case 3: client.sendMessage("GET_LOGS"); break;
            case 4: client.sendMessage("ADD_POSITION"); break;
            case 5: return;
            default: std::cout << "������ �����.\n";
        }
    }
}

// ���� ��������� HR
void Menu::hrMenu() {
    int choice = 0;
    while (true) {
        displayHeader();
        std::cout << "1. �������� ����������\n";
        std::cout << "2. �������� ������ ����������\n";
        std::cout << "3. �������� ���������\n";
        std::cout << "4. �����\n";
        std::cin >> choice;

        switch (choice) {
            case 1: client.sendMessage("ADD_EMPLOYEE"); break;
            case 2: client.sendMessage("EDIT_EMPLOYEE"); break;
            case 3: client.sendMessage("ADD_POSITION"); break;
            case 4: return;
            default: std::cout << "������ �����.\n";
        }
    }
}

// ���� �������/���������
void Menu::cashierMenu() {
    int choice = 0;
    while (true) {
        displayHeader();
        std::cout << "1. ����������� �������� �����������\n";
        std::cout << "2. ���������� ������ �� �����������\n";
        std::cout << "3. �����\n";
        std::cin >> choice;

        switch (choice) {
            case 1: client.sendMessage("GET_SALARIES"); break;
            case 2: client.sendMessage("PROCESS_COMPENSATION"); break;
            case 3: return;
            default: std::cout << "������ �����.\n";
        }
    }
}

// ���� ������������ �������������
void Menu::managerMenu() {
    int choice = 0;
    while (true) {
        displayHeader();
        std::cout << "1. ��������� ��������� ������\n";
        std::cout << "2. ������� ������������������ �����������\n";
        std::cout << "3. �����\n";
        std::cin >> choice;

        switch (choice) {
            case 1: client.sendMessage("MANAGE_SCHEDULES"); break;
            case 2: client.sendMessage("EVALUATE_PERFORMANCE"); break;
            case 3: return;
            default: std::cout << "������ �����.\n";
        }
    }
}

// ���� �������� ����������
void Menu::employeeMenu() {
    int choice = 0;
    while (true) {
        displayHeader();
        std::cout << "1. ����������� ���� ����������\n";
        std::cout << "2. ������ ������ �� �����������\n";
        std::cout << "3. �����\n";
        std::cin >> choice;

        switch (choice) {
            case 1: client.sendMessage("GET_SELF_INFO"); break;
            case 2: client.sendMessage("REQUEST_COMPENSATION"); break;
            case 3: return;
            default: std::cout << "������ �����.\n";
        }
    }
}
