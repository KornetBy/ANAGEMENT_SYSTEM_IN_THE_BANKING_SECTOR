// ScheduleManager.h
#pragma once
#include <string>
#include <vector>
#include <mutex>

struct Schedule {
    long scheduleID;
    long employeeID;
    long positionID;
    std::string scheduleType;   // сменный, фиксированный
    std::string shiftDetails;   // Утро/Вечер/Ночь
    std::string scheduleDates;  // "YYYY-MM-DD - YYYY-MM-DD"
};

class ScheduleManager {
private:
    std::vector<Schedule> schedules;
    std::string scheduleFileName;
    std::mutex mtx;
public:
    ScheduleManager(const std::string& filename);
    Schedule getSchedule(long scheduleID);
    bool addSchedule(const Schedule& schedule);
    bool updateSchedule(const Schedule& schedule);
    bool deleteSchedule(long scheduleID);
    std::vector<Schedule> getAllSchedules();
};
