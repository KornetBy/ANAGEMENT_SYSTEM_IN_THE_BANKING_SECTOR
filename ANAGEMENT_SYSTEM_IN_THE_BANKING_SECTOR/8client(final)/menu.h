// Menu.h
#pragma once
#include <string>
#include "Client.h"
#include "ScheduleManager.h"

class Menu {
private:
    Client& client;
    std::string role;
    std::string username;
    ScheduleManager scheduleManager;
    bool authenticate();
    void handleAdminMenu();
    void handleHRMenu();
    void handleCashierMenu();
    void handleManagerMenu();
    void handleEmployeeMenu();
    void addEmployeeCommon();
    void updateEmployeeCommon(const std::string& cmd);
public:
    Menu(Client& cli);
    void showMenu();
};
