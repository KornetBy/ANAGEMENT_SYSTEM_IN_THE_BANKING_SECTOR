// ScheduleManager.cpp
#include "ScheduleManager.h"
#include <fstream>
#include <sstream>

ScheduleManager::ScheduleManager(const std::string& filename) : scheduleFileName(filename) {
    std::lock_guard<std::mutex> lock(mtx);
    std::ifstream ifs(scheduleFileName);
    if (!ifs.is_open()) return;

    std::string line;
    while (getline(ifs, line)) {
        std::istringstream iss(line);
        std::string token;
        Schedule sched;
        getline(iss, token, '|');
        sched.scheduleID = std::stol(token);
        getline(iss, token, '|');
        sched.employeeID = std::stol(token);
        getline(iss, token, '|');
        sched.positionID = std::stol(token);
        getline(iss, sched.scheduleType, '|');
        getline(iss, sched.shiftDetails, '|');
        getline(iss, sched.scheduleDates, '|');
        schedules.push_back(sched);
    }
    ifs.close();
}

Schedule ScheduleManager::getSchedule(long scheduleID) {
    std::lock_guard<std::mutex> lock(mtx);
    for (const auto& sched : schedules) {
        if (sched.scheduleID == scheduleID) return sched;
    }
    return Schedule{ 0, 0, 0, "", "", "" };
}

bool ScheduleManager::addSchedule(const Schedule& schedule) {
    std::lock_guard<std::mutex> lock(mtx);
    for (const auto& sched : schedules) {
        if (sched.scheduleID == schedule.scheduleID) return false; // График уже существует
    }
    schedules.push_back(schedule);

    std::ofstream ofs(scheduleFileName, std::ios::app);
    if (!ofs.is_open()) return false;
    ofs << schedule.scheduleID << "|" << schedule.employeeID << "|" << schedule.positionID << "|"
        << schedule.scheduleType << "|" << schedule.shiftDetails << "|" << schedule.scheduleDates << "\n";
    ofs.close();
    return true;
}

bool ScheduleManager::updateSchedule(const Schedule& schedule) {
    std::lock_guard<std::mutex> lock(mtx);
    bool found = false;
    for (auto& sched : schedules) {
        if (sched.scheduleID == schedule.scheduleID) {
            sched = schedule;
            found = true;
            break;
        }
    }
    if (!found) return false;

    // Перезапись файла
    std::ofstream ofs(scheduleFileName, std::ios::trunc);
    if (!ofs.is_open()) return false;
    for (const auto& sched : schedules) {
        ofs << sched.scheduleID << "|" << sched.employeeID << "|" << sched.positionID << "|"
            << sched.scheduleType << "|" << sched.shiftDetails << "|" << sched.scheduleDates << "\n";
    }
    ofs.close();
    return true;
}

bool ScheduleManager::deleteSchedule(long scheduleID) {
    std::lock_guard<std::mutex> lock(mtx);
    bool found = false;
    std::vector<Schedule> updatedSchedules;
    for (const auto& sched : schedules) {
        if (sched.scheduleID != scheduleID) {
            updatedSchedules.push_back(sched);
        }
        else {
            found = true;
        }
    }
    if (!found) return false;
    schedules = updatedSchedules;

    // Перезапись файла
    std::ofstream ofs(scheduleFileName, std::ios::trunc);
    if (!ofs.is_open()) return false;
    for (const auto& sched : schedules) {
        ofs << sched.scheduleID << "|" << sched.employeeID << "|" << sched.positionID << "|"
            << sched.scheduleType << "|" << sched.shiftDetails << "|" << sched.scheduleDates << "\n";
    }
    ofs.close();
    return true;
}

std::vector<Schedule> ScheduleManager::getAllSchedules() {
    std::lock_guard<std::mutex> lock(mtx);
    return schedules;
}
