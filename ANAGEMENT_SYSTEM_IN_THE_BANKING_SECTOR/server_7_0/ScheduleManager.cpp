#include "ScheduleManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <mutex>
// �������� ���������� �� �����
void ScheduleManager::loadSchedulesFromFile() {
    std::ifstream ifs(scheduleFileName);
    if (!ifs.is_open()) {
        std::cerr << "�� ������� ������� ���� ��� ������: " << scheduleFileName << std::endl;
        return;
    }

    std::string line;
    while (std::getline(ifs, line)) {
        std::istringstream iss(line);
        std::string token;
        Schedule sched;
        std::getline(iss, token, '|');
        sched.scheduleID = std::stol(token);
        std::getline(iss, token, '|');
        sched.employeeID = std::stol(token);
        std::getline(iss, token, '|');
        sched.positionID = std::stol(token);
        std::getline(iss, sched.scheduleType, '|');
        std::getline(iss, sched.shiftDetails, '|');
        std::getline(iss, sched.scheduleDates, '|');
        schedules.push_back(sched);
        schedulesMap[sched.scheduleID] = sched;  // ����������� �� scheduleID ��� �������� ������
    }
    ifs.close();
}

// ���������� ���������� � ����
void ScheduleManager::saveSchedulesToFile() {
    std::ofstream ofs(scheduleFileName, std::ios::trunc);
    if (!ofs.is_open()) {
        std::cerr << "�� ������� ������� ���� ��� ������: " << scheduleFileName << std::endl;
        return;
    }
    for (const auto& sched : schedules) {
        ofs << sched.scheduleID << "|" << sched.employeeID << "|" << sched.positionID << "|"
            << sched.scheduleType << "|" << sched.shiftDetails << "|" << sched.scheduleDates << "\n";
    }
    ofs.close();
}

ScheduleManager::ScheduleManager(const std::string& filename) : scheduleFileName(filename) {
 
    loadSchedulesFromFile();
}

Schedule ScheduleManager::getSchedule(long scheduleID) const {

    auto it = schedulesMap.find(scheduleID);
    if (it != schedulesMap.end()) {
        return it->second;
    }
    return Schedule{ 0, 0, 0, "", "", "" };  // ���������� ������ ������, ���� �� �������
}

bool ScheduleManager::addSchedule(const Schedule& schedule) {
 
    if (schedulesMap.find(schedule.scheduleID) != schedulesMap.end()) {
        return false; // ������ � ����� ID ��� ����������
    }
    schedules.push_back(schedule);
    schedulesMap[schedule.scheduleID] = schedule;  // ��������� � ���-�������
    saveSchedulesToFile();
    return true;
}

bool ScheduleManager::updateSchedule(const Schedule& schedule) {
    std::lock_guard<std::mutex> lock(mtx);
    auto it = schedulesMap.find(schedule.scheduleID);
    if (it == schedulesMap.end()) {
        return false;  // ������ �� ������
    }
    // ��������� ������ � ���-�������
    for (auto& sched : schedules) {
        if (sched.scheduleID == schedule.scheduleID) {
            sched = schedule;
            break;
        }
    }
    schedulesMap[schedule.scheduleID] = schedule;  // ��������� ���-�������
    saveSchedulesToFile();
    return true;
}

bool ScheduleManager::deleteSchedule(long scheduleID) {
    std::lock_guard<std::mutex> lock(mtx);
    auto it = schedulesMap.find(scheduleID);
    if (it == schedulesMap.end()) {
        return false; // ������ �� ������
    }
    schedulesMap.erase(it); // ������� �� ���-�������
    schedules.erase(std::remove_if(schedules.begin(), schedules.end(),
        [scheduleID](const Schedule& sched) { return sched.scheduleID == scheduleID; }),
        schedules.end());  // ������� �� �������
    saveSchedulesToFile();
    return true;
}

std::vector<Schedule> ScheduleManager::getAllSchedules() const {

    return schedules;
}