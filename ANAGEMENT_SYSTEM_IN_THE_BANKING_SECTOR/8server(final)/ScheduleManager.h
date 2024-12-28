// ScheduleManager.h
#pragma once
#include <string>
#include <vector>
#include <mutex>

struct Schedule {
    long empID;
    int dayOfWeek;
    std::string startTime;
    std::string endTime;
};

class ScheduleManager {
private:
    std::string filename;
    std::vector<Schedule> schedules;
    std::mutex mtx;
    void load();
    void save();
public:
    ScheduleManager(const std::string& filename);
    bool addSchedule(long empID, int dayOfWeek, const std::string& start, const std::string& end);
    bool updateSchedule(long empID, int dayOfWeek, const std::string& start, const std::string& end);
    bool deleteSchedule(long empID);
    std::string viewSchedule(long empID);
};
