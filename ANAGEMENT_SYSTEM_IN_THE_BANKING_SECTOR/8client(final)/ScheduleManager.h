// ScheduleManager.h
#pragma once
#include <string>
#include "Client.h"

class ScheduleManager {
private:
    Client& client;
public:
    ScheduleManager(Client& cli);
    void manageWorkSchedules();
    void addWorkSchedule();
    void updateWorkSchedule();
    void deleteWorkSchedule();
    void viewWorkSchedule();
};
