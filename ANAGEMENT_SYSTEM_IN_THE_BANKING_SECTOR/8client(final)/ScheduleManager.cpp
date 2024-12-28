// ScheduleManager.cpp
#include "ScheduleManager.h"
#include <iostream>
#include <sstream>
#include <regex>

ScheduleManager::ScheduleManager(Client& cli) : client(cli) {}

void ScheduleManager::manageWorkSchedules() {
    std::cout << "\n+====================================+\n"
        << "|      ���������� ��������� ������   |\n"
        << "+====================================+\n"
        << "| 1. �������� ������                 |\n"
        << "| 2. �������� ������                 |\n"
        << "| 3. ������� ������                  |\n"
        << "| 4. ����������� �������             |\n"
        << "| 5. �����                           |\n"
        << "+------------------------------------+\n"
        << "�������� ��������: ";
    int scheduleChoice;
    std::cin >> scheduleChoice;
    std::cin.ignore();
    if (scheduleChoice == 1) addWorkSchedule();
    else if (scheduleChoice == 2) updateWorkSchedule();
    else if (scheduleChoice == 3) deleteWorkSchedule();
    else if (scheduleChoice == 4) viewWorkSchedule();
}

void ScheduleManager::addWorkSchedule() {
    std::cout << "������� ID ����������: ";
    long empID;
    std::cin >> empID;
    std::cin.ignore();
    std::cout << "������� ���� ������ (1-7): ";
    int dayOfWeek;
    std::cin >> dayOfWeek;
    std::cin.ignore();
    std::cout << "������� ����� ������ (HH:MM): ";
    std::string startTime;
    getline(std::cin, startTime);
    if (!std::regex_match(startTime, std::regex(R"(\d{2}:\d{2})"))) return;
    std::cout << "������� ����� ��������� (HH:MM): ";
    std::string endTime;
    getline(std::cin, endTime);
    if (!std::regex_match(endTime, std::regex(R"(\d{2}:\d{2})"))) return;
    std::ostringstream oss;
    oss << "ADD_WORK_SCHEDULE|" << empID << "|" << dayOfWeek << "|" << startTime << "|" << endTime;
    client.sendMessage(oss.str());
    std::string response = client.receiveMessage();
    std::cout << response << "\n";
}

void ScheduleManager::updateWorkSchedule() {
    std::cout << "������� ID ����������: ";
    long empID;
    std::cin >> empID;
    std::cin.ignore();
    std::cout << "������� ����� ���� ������ (1-7): ";
    int dayOfWeek;
    std::cin >> dayOfWeek;
    std::cin.ignore();
    std::cout << "������� ����� ����� ������ (HH:MM): ";
    std::string startTime;
    getline(std::cin, startTime);
    if (!std::regex_match(startTime, std::regex(R"(\d{2}:\d{2})"))) return;
    std::cout << "������� ����� ����� ��������� (HH:MM): ";
    std::string endTime;
    getline(std::cin, endTime);
    if (!std::regex_match(endTime, std::regex(R"(\d{2}:\d{2})"))) return;
    std::ostringstream oss;
    oss << "UPDATE_WORK_SCHEDULE|" << empID << "|" << dayOfWeek << "|" << startTime << "|" << endTime;
    client.sendMessage(oss.str());
    std::string response = client.receiveMessage();
    std::cout << response << "\n";
}

void ScheduleManager::deleteWorkSchedule() {
    std::cout << "������� ID ����������: ";
    long empID;
    std::cin >> empID;
    std::cin.ignore();
    std::ostringstream oss;
    oss << "DELETE_WORK_SCHEDULE|" << empID;
    client.sendMessage(oss.str());
    std::string response = client.receiveMessage();
    std::cout << response << "\n";
}

void ScheduleManager::viewWorkSchedule() {
    std::cout << "������� ID ����������: ";
    long empID;
    std::cin >> empID;
    std::cin.ignore();
    std::ostringstream oss;
    oss << "VIEW_WORK_SCHEDULE|" << empID;
    client.sendMessage(oss.str());
    std::string response = client.receiveMessage();
    std::cout << response << "\n";
}
