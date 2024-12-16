#include "ScheduleManager.h"

// Конструктор
ScheduleManager::ScheduleManager() {}

// Добавление графика
bool ScheduleManager::addSchedule(const std::string& employeeId, const std::string& scheduleDetails) {
    if (dataManager.isDuplicate("data/графики_работы.txt", employeeId)) {
        return false;
    }
    dataManager.writeFile("data/графики_работы.txt", employeeId + "|" + scheduleDetails);
    return true;
}

// Изменение графика
bool ScheduleManager::editSchedule(const std::string& employeeId, const std::string& newScheduleDetails) {
    if (!dataManager.deleteRecord("data/графики_работы.txt", employeeId)) {
        return false;
    }
    dataManager.writeFile("data/графики_работы.txt", employeeId + "|" + newScheduleDetails);
    return true;
}

// Получение графика сотрудника
std::string ScheduleManager::getSchedule(const std::string& employeeId) {
    std::vector<std::string> schedules = dataManager.readFile("data/графики_работы.txt");
    for (const auto& line : schedules) {
        if (line.find(employeeId + "|") == 0) {
            return line.substr(employeeId.length() + 1); // Убираем ID и разделитель
        }
    }
    return "";
}

// Удаление графика
bool ScheduleManager::deleteSchedule(const std::string& employeeId) {
    return dataManager.deleteRecord("data/графики_работы.txt", employeeId);
}
