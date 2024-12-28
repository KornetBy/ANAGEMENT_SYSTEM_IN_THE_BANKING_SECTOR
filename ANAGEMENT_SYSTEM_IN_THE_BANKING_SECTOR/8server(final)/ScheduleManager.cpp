// ScheduleManager.cpp
#include "ScheduleManager.h"
#include "utils.h"
#include <fstream>
#include <sstream>

ScheduleManager::ScheduleManager(const std::string& filename) : filename(filename) {
    load();
}

void ScheduleManager::load() {
    schedules.clear();
    if (!fileExists(filename)) return;
    std::string data = readAll(filename);
    std::istringstream iss(data);
    std::string line;
    while (std::getline(iss, line)) {
        line = trim(line);
        if (line.empty()) continue;
        auto parts = split(line, '|');
        // Формат: empID|dayOfWeek|startTime|endTime
        if (parts.size() < 4) continue;
        Schedule s;
        s.empID = std::stol(parts[0]);
        s.dayOfWeek = std::stoi(parts[1]);
        s.startTime = parts[2];
        s.endTime = parts[3];
        schedules.push_back(s);
    }
}

void ScheduleManager::save() {
    std::ostringstream oss;
    for (auto& s : schedules) {
        oss << s.empID << "|" << s.dayOfWeek << "|" << s.startTime << "|" << s.endTime << "\n";
    }
    writeAll(filename, oss.str());
}

bool ScheduleManager::addSchedule(long empID, int dayOfWeek, const std::string& start, const std::string& end) {
    std::lock_guard<std::mutex> lock(mtx);
    // Один график на сотрудника? Если нужно несколько, можно не проверять удаление
    // Здесь предположим, что можно иметь несколько записей для разных дней
    Schedule sch{ empID, dayOfWeek, start, end };
    schedules.push_back(sch);
    save();
    return true;
}

bool ScheduleManager::updateSchedule(long empID, int dayOfWeek, const std::string& start, const std::string& end) {
    std::lock_guard<std::mutex> lock(mtx);
    for (auto& s : schedules) {
        if (s.empID == empID && s.dayOfWeek == dayOfWeek) {
            s.startTime = start;
            s.endTime = end;
            save();
            return true;
        }
    }
    return false;
}

bool ScheduleManager::deleteSchedule(long empID) {
    std::lock_guard<std::mutex> lock(mtx);
    bool found = false;
    for (int i = (int)schedules.size() - 1; i >= 0; i--) {
        if (schedules[i].empID == empID) {
            schedules.erase(schedules.begin() + i);
            found = true;
        }
    }
    if (found) save();
    return found;
}

std::string ScheduleManager::viewSchedule(long empID) {
    std::lock_guard<std::mutex> lock(mtx);
    std::ostringstream oss;
    for (auto& s : schedules) {
        if (s.empID == empID) {
            oss << "День: " << s.dayOfWeek << ", " << s.startTime << "-" << s.endTime << "\n";
        }
    }
    return oss.str();
}
