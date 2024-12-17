// Server/ScheduleManager.cpp
#include "ScheduleManager.h"
#include <fstream>
#include <sstream>

ScheduleManager::ScheduleManager(const std::string& fileName) : scheduleFileName(fileName) {
    loadSchedules();
}

void ScheduleManager::loadSchedules() {
    std::lock_guard<std::mutex> lock(mtx);
    schedules.clear();
    std::ifstream file(scheduleFileName);
    if (!file.is_open()) {
        // Файл не существует, создадим пустой
        std::ofstream outfile(scheduleFileName);
        outfile.close();
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string employeeName, scheduleData;
        if (std::getline(iss, employeeName, '|') &&
            std::getline(iss, scheduleData, '|')) {
            schedules[employeeName] = scheduleData;
        }
    }
    file.close();
}

bool ScheduleManager::updateSchedule(const std::string& employeeName, const std::string& scheduleData) {
    std::lock_guard<std::mutex> lock(mtx);
    schedules[employeeName] = scheduleData;
    // Перезаписываем файл
    std::ofstream file(scheduleFileName, std::ios::trunc);
    if (file.is_open()) {
        for (const auto& pair : schedules) {
            file << pair.first << "|" << pair.second << std::endl;
        }
        file.close();
        return true;
    }
    return false;
}

std::string ScheduleManager::getSchedule(const std::string& employeeName) {
    std::lock_guard<std::mutex> lock(mtx);
    auto it = schedules.find(employeeName);
    if (it != schedules.end()) {
        return it->second;
    }
    return "График не найден.";
}
