#pragma once
#include "Client.h"
#include "ScheduleManager.h"
#include <string>

class Menu {
private:
    Client& client;
    std::string role;
    std::string username;
    ScheduleManager scheduleManager;

public:
    Menu(Client& cli);
    bool authenticate();
    void showMenu();
    void handleAdminMenu();
    void handleHRMenu();
    void handleCashierMenu();
    void handleManagerMenu();
    void handleEmployeeMenu();
};
