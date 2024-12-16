#pragma once
#ifndef SCHEDULE_MANAGER_H
#define SCHEDULE_MANAGER_H

#include <string>
#include <vector>
#include "DataManager.h"

class ScheduleManager {
private:
    DataManager dataManager;

public:
    ScheduleManager();

    // Добавление графика
    bool addSchedule(const std::string& employeeId, const std::string& scheduleDetails);

    // Изменение графика
    bool editSchedule(const std::string& employeeId, const std::string& newScheduleDetails);

    // Получение графика сотрудника
    std::string getSchedule(const std::string& employeeId);

    // Удаление графика
    bool deleteSchedule(const std::string& employeeId);
};

#endif // SCHEDULE_MANAGER_H
