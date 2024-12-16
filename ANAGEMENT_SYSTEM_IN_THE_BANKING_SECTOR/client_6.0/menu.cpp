#include "Menu.h"

// Конструктор
Menu::Menu(Client& client, const std::string& role) : client(client), role(role) {}

// Вывод заголовка
void Menu::displayHeader() {
    std::cout << "\n=====================================\n";
    std::cout << "      Система управления персоналом   \n";
    std::cout << "=====================================\n";
    std::cout << "Текущая роль: " << role << "\n";
    std::cout << "-------------------------------------\n";
}

// Выбор меню в зависимости от роли
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

// Меню администратора
void Menu::adminMenu() {
    int choice = 0;
    while (true) {
        displayHeader();
        std::cout << "1. Добавить сотрудника\n";
        std::cout << "2. Блокировать сотрудника\n";
        std::cout << "3. Посмотреть историю изменений\n";
        std::cout << "4. Добавить должность\n";
        std::cout << "5. Выйти\n";
        std::cin >> choice;

        switch (choice) {
            case 1: client.sendMessage("ADD_EMPLOYEE"); break;
            case 2: client.sendMessage("BLOCK_USER"); break;
            case 3: client.sendMessage("GET_LOGS"); break;
            case 4: client.sendMessage("ADD_POSITION"); break;
            case 5: return;
            default: std::cout << "Ошибка ввода.\n";
        }
    }
}

// Меню менеджера HR
void Menu::hrMenu() {
    int choice = 0;
    while (true) {
        displayHeader();
        std::cout << "1. Добавить сотрудника\n";
        std::cout << "2. Изменить данные сотрудника\n";
        std::cout << "3. Добавить должность\n";
        std::cout << "4. Выйти\n";
        std::cin >> choice;

        switch (choice) {
            case 1: client.sendMessage("ADD_EMPLOYEE"); break;
            case 2: client.sendMessage("EDIT_EMPLOYEE"); break;
            case 3: client.sendMessage("ADD_POSITION"); break;
            case 4: return;
            default: std::cout << "Ошибка ввода.\n";
        }
    }
}

// Меню кассира/оператора
void Menu::cashierMenu() {
    int choice = 0;
    while (true) {
        displayHeader();
        std::cout << "1. Просмотреть зарплаты сотрудников\n";
        std::cout << "2. Обработать заявку на компенсацию\n";
        std::cout << "3. Выйти\n";
        std::cin >> choice;

        switch (choice) {
            case 1: client.sendMessage("GET_SALARIES"); break;
            case 2: client.sendMessage("PROCESS_COMPENSATION"); break;
            case 3: return;
            default: std::cout << "Ошибка ввода.\n";
        }
    }
}

// Меню руководителя подразделения
void Menu::managerMenu() {
    int choice = 0;
    while (true) {
        displayHeader();
        std::cout << "1. Управлять графиками работы\n";
        std::cout << "2. Оценить производительность сотрудников\n";
        std::cout << "3. Выйти\n";
        std::cin >> choice;

        switch (choice) {
            case 1: client.sendMessage("MANAGE_SCHEDULES"); break;
            case 2: client.sendMessage("EVALUATE_PERFORMANCE"); break;
            case 3: return;
            default: std::cout << "Ошибка ввода.\n";
        }
    }
}

// Меню обычного сотрудника
void Menu::employeeMenu() {
    int choice = 0;
    while (true) {
        displayHeader();
        std::cout << "1. Просмотреть свою информацию\n";
        std::cout << "2. Подать заявку на компенсацию\n";
        std::cout << "3. Выйти\n";
        std::cin >> choice;

        switch (choice) {
            case 1: client.sendMessage("GET_SELF_INFO"); break;
            case 2: client.sendMessage("REQUEST_COMPENSATION"); break;
            case 3: return;
            default: std::cout << "Ошибка ввода.\n";
        }
    }
}
