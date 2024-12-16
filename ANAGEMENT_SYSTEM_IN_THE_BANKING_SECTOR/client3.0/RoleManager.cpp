#include "RoleManager.h"
#include "DirectorUI.h"
#include "AdminUI.h"
#include "HRUI.h"
#include "FinanceUI.h"
#include "ManagerUI.h"
#include "CashierUI.h"
#include "OperatorUI.h"
#include <iostream>

void RoleManager::handleRole(const std::string& role) {
    if (role == "Director") {
        DirectorUI::showMenu();
    }
    else if (role == "Administrator") {
        AdminUI::showMenu();
    }
    else if (role == "HR-Specialist") {
        HRUI::showMenu();
    }
    else if (role == "Finance") {
        FinanceUI::showMenu();
    }
    else if (role == "Manager") {
        ManagerUI::showMenu();
    }
    else if (role == "Cashier") {
        CashierUI::showMenu();
    }
    else if (role == "Operator") {
        OperatorUI::showMenu();
    }
    else {
        std::cerr << "Ошибка: Роль " << role << " не поддерживается.\n";
    }
}
