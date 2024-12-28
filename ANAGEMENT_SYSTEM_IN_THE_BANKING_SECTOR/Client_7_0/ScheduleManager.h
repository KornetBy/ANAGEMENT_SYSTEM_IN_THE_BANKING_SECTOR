#pragma once
#pragma once
#include "Client.h"
#include <string>

class ScheduleManager {
private:
    Client& client;
public:
    ScheduleManager(Client& cli);
    void addWorkSchedule();
    void updateWorkSchedule();
    void deleteWorkSchedule();
    void viewWorkSchedule();
};
