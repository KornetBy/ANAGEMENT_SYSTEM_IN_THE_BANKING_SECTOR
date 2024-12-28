#include "ScheduleManager.h"
#include <iostream>
#include <sstream>
#include <regex>
#include "Validation.h"

ScheduleManager::ScheduleManager(Client& cli) : client(cli) {}

void ScheduleManager::addWorkSchedule() {
    std::cout << "Введите ID сотрудника: ";
    long empID;
    std::cin >> empID;
    std::cin.ignore();

    std::cout << "Введите день недели (1-7): ";
    int dayOfWeek;
    std::cin >> dayOfWeek;
    std::cin.ignore();

    std::cout << "Введите время начала (HH:MM): ";
    std::string startTime;
    getline(std::cin, startTime);
    if (!std::regex_match(startTime, std::regex(R"(\d{2}:\d{2})"))) {
        std::cout << "Некорректный формат времени начала.\n";
        return;
    }

    std::cout << "Введите время окончания (HH:MM): ";
    std::string endTime;
    getline(std::cin, endTime);
    if (!std::regex_match(endTime, std::regex(R"(\d{2}:\d{2})"))) {
        std::cout << "Некорректный формат времени окончания.\n";
        return;
    }

    std::ostringstream oss;
    oss << "ADD_WORK_SCHEDULE|" << empID << "|" << dayOfWeek << "|" << startTime << "|" << endTime;
    client.sendMessage(oss.str());
    std::string response = client.receiveMessage();
    if (response.find("SUCCESS|") == 0) {
        std::cout << response.substr(8) << "\n";
    }
    else {
        std::cout << "Ошибка: " << response.substr(6) << "\n";
    }
}

void ScheduleManager::updateWorkSchedule() {
    std::cout << "Введите ID сотрудника для изменения графика: ";
    long empID;
    std::cin >> empID;
    std::cin.ignore();

    std::cout << "Введите новый день недели (1-7): ";
    int dayOfWeek;
    std::cin >> dayOfWeek;
    std::cin.ignore();

    std::cout << "Введите новое время начала (HH:MM): ";
    std::string startTime;
    getline(std::cin, startTime);
    if (!std::regex_match(startTime, std::regex(R"(\d{2}:\d{2})"))) {
        std::cout << "Некорректный формат времени начала.\n";
        return;
    }

    std::cout << "Введите новое время окончания (HH:MM): ";
    std::string endTime;
    getline(std::cin, endTime);
    if (!std::regex_match(endTime, std::regex(R"(\d{2}:\d{2})"))) {
        std::cout << "Некорректный формат времени окончания.\n";
        return;
    }

    std::ostringstream oss;
    oss << "UPDATE_WORK_SCHEDULE|" << empID << "|" << dayOfWeek << "|" << startTime << "|" << endTime;
    client.sendMessage(oss.str());
    std::string response = client.receiveMessage();
    if (response.find("SUCCESS|") == 0) {
        std::cout << response.substr(8) << "\n";
    }
    else {
        std::cout << "Ошибка: " << response.substr(6) << "\n";
    }
}

void ScheduleManager::deleteWorkSchedule() {
    std::cout << "Введите ID сотрудника для удаления графика: ";
    long empID;
    std::cin >> empID;
    std::cin.ignore();

    std::ostringstream oss;
    oss << "DELETE_WORK_SCHEDULE|" << empID;
    client.sendMessage(oss.str());
    std::string response = client.receiveMessage();
    if (response.find("SUCCESS|") == 0) {
        std::cout << response.substr(8) << "\n";
    }
    else {
        std::cout << "Ошибка: " << response.substr(6) << "\n";
    }
}

void ScheduleManager::viewWorkSchedule() {
    std::cout << "Введите ID сотрудника для просмотра графиков: ";
    long empID;
    std::cin >> empID;
    std::cin.ignore();

    std::ostringstream oss;
    oss << "VIEW_WORK_SCHEDULE|" << empID;
    client.sendMessage(oss.str());
    std::string response = client.receiveMessage();
    if (response.find("SUCCESS|") == 0) {
        std::cout << "Графики работы сотрудника:\n" << response.substr(8) << "\n";
    }
    else {
        std::cout << "Ошибка: " << response.substr(6) << "\n";
    }
}
