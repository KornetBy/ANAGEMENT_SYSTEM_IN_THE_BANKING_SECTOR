// Server/ScheduleManager.h
#pragma once
#include <string>
#include <unordered_map>
#include <mutex>

class ScheduleManager {
private:
    std::unordered_map<std::string, std::string> schedules;
    std::string scheduleFileName;
    std::mutex mtx;

    void loadSchedules();
public:
    ScheduleManager(const std::string& fileName);
    bool updateSchedule(const std::string& employeeName, const std::string& scheduleData);
    std::string getSchedule(const std::string& employeeName);
};
