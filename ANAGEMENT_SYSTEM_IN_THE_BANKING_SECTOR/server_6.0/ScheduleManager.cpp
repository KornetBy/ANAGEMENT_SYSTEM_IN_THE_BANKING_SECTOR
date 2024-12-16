#include "ScheduleManager.h"

// �����������
ScheduleManager::ScheduleManager() {}

// ���������� �������
bool ScheduleManager::addSchedule(const std::string& employeeId, const std::string& scheduleDetails) {
    if (dataManager.isDuplicate("data/�������_������.txt", employeeId)) {
        return false;
    }
    dataManager.writeFile("data/�������_������.txt", employeeId + "|" + scheduleDetails);
    return true;
}

// ��������� �������
bool ScheduleManager::editSchedule(const std::string& employeeId, const std::string& newScheduleDetails) {
    if (!dataManager.deleteRecord("data/�������_������.txt", employeeId)) {
        return false;
    }
    dataManager.writeFile("data/�������_������.txt", employeeId + "|" + newScheduleDetails);
    return true;
}

// ��������� ������� ����������
std::string ScheduleManager::getSchedule(const std::string& employeeId) {
    std::vector<std::string> schedules = dataManager.readFile("data/�������_������.txt");
    for (const auto& line : schedules) {
        if (line.find(employeeId + "|") == 0) {
            return line.substr(employeeId.length() + 1); // ������� ID � �����������
        }
    }
    return "";
}

// �������� �������
bool ScheduleManager::deleteSchedule(const std::string& employeeId) {
    return dataManager.deleteRecord("data/�������_������.txt", employeeId);
}
