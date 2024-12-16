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

    // ���������� �������
    bool addSchedule(const std::string& employeeId, const std::string& scheduleDetails);

    // ��������� �������
    bool editSchedule(const std::string& employeeId, const std::string& newScheduleDetails);

    // ��������� ������� ����������
    std::string getSchedule(const std::string& employeeId);

    // �������� �������
    bool deleteSchedule(const std::string& employeeId);
};

#endif // SCHEDULE_MANAGER_H
