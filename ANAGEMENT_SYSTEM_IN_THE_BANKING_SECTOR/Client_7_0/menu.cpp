// Menu.cpp
#include "Menu.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <locale>
#include <regex>
#include "Validation.h"
#include "ScheduleManager.h"
// ���������� Validation::isValidFullName � �.�.

Menu::Menu(Client& cli) : client(cli), role(""), username(""), scheduleManager(cli) {}

std::string trim(const std::string& str) {
    auto start = str.begin();
    while (start != str.end() && std::isspace(*start)) {
        start++;
    }

    auto end = str.end();
    do {
        end--;
    } while (std::distance(start, end) > 0 && std::isspace(*end));

    return std::string(start, end + 1);
}
bool Menu::authenticate() {
    std::cout << "������� �����: ";
    std::cin >> username;
    std::cout << "������� ������: ";
    std::string password;
    std::cin >> password;

    std::string authCommand = "AUTH|" + username + "|" + password;
    client.sendMessage(authCommand);
    std::string response = client.receiveMessage();

    if (response.find("SUCCESS|") == 0) {
        role = trim(response.substr(8)); // ������� ��������
        std::cout << "�������� �����������. ����: " << role << "\n";
        return true;
    }
    else if (response.find("ERROR|") == 0) {
        std::string errorMessage = (response.size() > 6) ? response.substr(6) : "����������� ������";
        std::cout << "������ �����������: " << errorMessage << "\n";
        return false;
    }
    else {
        std::cout << "������������ ����� �������: " << response << "\n";
        return false;
    }
}




void Menu::showMenu() {
    if (!authenticate()) return;

    while (true) {
        if (role == "admin") {
            handleAdminMenu();
        }
        else if (role == "hr") {
            handleHRMenu();
        }
        else if (role == "cashier") {
            handleCashierMenu();
        }
        else if (role == "manager") {
            handleManagerMenu();
        }
        else if (role == "employee") {
            handleEmployeeMenu();
        }
        else {
            std::cout << "����������� ����.\n";
            break;
        }
    }
}

void Menu::handleAdminMenu() {
    while (true) { // ��������� ���� ��� ���������� ����������� ���� ����� ���������� ��������
        std::cout << "\n+====================================+\n"
            << "|      ������� ���������� ���������� |\n"
            << "+====================================+\n"
            << "| ����: Admin                        |\n"
            << "+------------------------------------+\n"
            << "| 1. �������� ����������             |\n"
            << "| 2. �������� ������ ����������      |\n"
            << "| 3. ����������� ����������          |\n"
            << "| 4. ���������� ������� ���������    |\n"
            << "| 5. �������� ���������              |\n"
            << "| 6. ����������� �������� �����      |\n"
            << "| 7. ����������� �������� ���������� |\n"
            << "| 8. �������� ������������           |\n"
            << "| 9. ������� ������������            |\n"
            << "| 10. ����������� ���� ����������    |\n" // ����� �����
            << "| 11. �����                           |\n"
            << "+------------------------------------+\n"
            << "�������� ��������: ";
        int choice;
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear(); // ������� ����� ������
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ������������� ��������� �����
            std::cout << "������������ ����. ����������, ������� ����� �� 1 �� 11.\n";
            continue;
        }
        std::cin.ignore(); // ������� ������ ����� ����� �����

        switch (choice) {
        case 1: { // �������� ����������
            std::cout << "������� ID ����������: ";
            long empID;
            std::cin >> empID;
            std::cin.ignore();

            std::cout << "�������� ���������:\n"
                << "1. ��������\n"
                << "2. ������\n"
                << "3. ������\n"
                << "�������� ��������: ";
            int posChoice;
            std::cin >> posChoice;
            std::cin.ignore();
            long posID;
            if (posChoice == 1) posID = 2001;
            else if (posChoice == 2) posID = 2002;
            else { std::cout << "������ ���������� ����������.\n"; break; }

            std::string fullName, birthDate, address, contactInfo, startDate, status;
            do {
                std::cout << "������� ��� ����������: ";
                getline(std::cin, fullName);
                if (!Validation::isValidFullName(fullName)) {
                    std::cout << "������������ ���. ���������� �����.\n";
                }
            } while (!Validation::isValidFullName(fullName));

            std::cout << "������� ���� �������� (YYYY-MM-DD): ";
            getline(std::cin, birthDate);
            if (!Validation::isValidDate(birthDate)) {
                std::cout << "������������ ������ ���� ��������. ��������� YYYY-MM-DD.\n";
                break;
            }

            std::cout << "������� �����: ";
            getline(std::cin, address);
            std::cout << "������� ���������� ����������: ";
            getline(std::cin, contactInfo);
            std::cout << "������� ���� ������ ������ (YYYY-MM-DD): ";
            getline(std::cin, startDate);
            if (!Validation::isValidDate(startDate)) {
                std::cout << "������������ ������ ���� ������ ������. ��������� YYYY-MM-DD.\n";
                break;
            }
            std::cout << "������� ������ (active/blocked): ";
            getline(std::cin, status);
            if (status != "active" && status != "blocked") {
                std::cout << "������������ ������. ����������� 'active' ��� 'blocked'.\n";
                break;
            }

            std::ostringstream oss;
            oss << "ADD_EMPLOYEE|" << empID << "|" << posID << "|" << fullName << "|"
                << birthDate << "|" << address << "|" << contactInfo << "|"
                << startDate << "|" << status;
            client.sendMessage(oss.str());
            std::string response = client.receiveMessage();
            if (response.find("SUCCESS|") == 0) {
                std::cout << response.substr(8) << "\n"; // ����� ��������� ����� SUCCESS|
            }
            else {
                std::cout << "������: " << response.substr(6) << "\n"; // ����� ��������� ����� ERROR|
            }
            break;
        }
        case 2: { // �������� ������ ����������
            std::cout << "������� ID ����������: ";
            long empID;
            std::cin >> empID;
            std::cin.ignore();

            std::string fullName, address, contactInfo;
            do {
                std::cout << "������� ����� ��� ����������: ";
                getline(std::cin, fullName);
                if (!Validation::isValidFullName(fullName)) {
                    std::cout << "������������ ���. ���������� �����.\n";
                }
            } while (!Validation::isValidFullName(fullName));

            std::cout << "������� ����� �����: ";
            getline(std::cin, address);
            std::cout << "������� ����� ���������� ������: ";
            getline(std::cin, contactInfo);

            std::ostringstream oss;
            oss << "UPDATE_EMPLOYEE|" << empID << "|" << fullName << "|"
                << address << "|" << contactInfo;
            client.sendMessage(oss.str());
            std::string response = client.receiveMessage();
            if (response.find("SUCCESS|") == 0) {
                std::cout << response.substr(8) << "\n";
            }
            else {
                std::cout << "������: " << response.substr(6) << "\n";
            }
            break;
        }
        case 3: { // ����������� ����������
            std::cout << "������� ID ���������� ��� ����������: ";
            long empID;
            std::cin >> empID;
            std::cin.ignore();

            std::ostringstream oss;
            oss << "BLOCK_EMPLOYEE|" << empID;
            client.sendMessage(oss.str());
            std::string response = client.receiveMessage();
            if (response.find("SUCCESS|") == 0) {
                std::cout << response.substr(8) << "\n";
            }
            else {
                std::cout << "������: " << response.substr(6) << "\n";
            }
            break;
        }

        case 4: { // ���������� ������� ���������
            std::string command = "VIEW_HISTORY";
            client.sendMessage(command);
            std::string response = client.receiveMessage();
            if (response.find("SUCCESS|") == 0) {
                std::cout << "������� ���������:\n" << response.substr(8) << "\n";
            }
            else {
                std::cout << "������: " << response.substr(6) << "\n";
            }
            break;
        }
        case 5: { // �������� ���������
            std::cout << "������� ID ���������: ";
            long posID;
            std::cin >> posID;
            std::cin.ignore();
            std::cout << "������� �������� ���������: ";
            std::string posName;
            getline(std::cin, posName);
            std::cout << "������� ����������� ��������: ";
            double minSal;
            std::cin >> minSal;
            std::cin.ignore();
            std::cout << "������� ������������ ��������: ";
            double maxSal;
            std::cin >> maxSal;
            std::cin.ignore();
            std::cout << "������� ���������������� ����������: ";
            std::string qualReq;
            getline(std::cin, qualReq);

            std::ostringstream oss;
            oss << "ADD_POSITION|" << posID << "|" << posName << "|" << minSal << "|"
                << maxSal << "|" << qualReq;
            client.sendMessage(oss.str());
            std::string response = client.receiveMessage();
            if (response.find("SUCCESS|") == 0) {
                std::cout << response.substr(8) << "\n"; // ����� ��������� ����� SUCCESS|
            }
            else {
                std::cout << "������: " << response.substr(6) << "\n"; // ����� ��������� ����� ERROR|
            }
            break;
        }
        case 6: { // ����������� �������� �����
            std::string command = "VIEW_ACTIVE_COURSES";
            client.sendMessage(command);
            std::string response = client.receiveMessage();
            if (response.find("SUCCESS|") == 0) {
                std::cout << "�������� �����:\n" << response.substr(8) << "\n";
            }
            else {
                std::cout << "������: " << response.substr(6) << "\n";
            }
            break;
        }
        case 7: { // ����������� ���-�����
            std::string command = "VIEW_LOGS";
            client.sendMessage(command);
            std::string response = client.receiveMessage();
            if (response.find("SUCCESS|") == 0) {
                std::cout << "���� �������:\n" << response.substr(8) << "\n";
            }
            else {
                std::cout << "������: " << response.substr(6) << "\n";
            }
            break;
        }

        case 8: { // ��������� ��������� ������
            scheduleManager.manageWorkSchedules();
            break;
        }
        case 9: { // ���������� ������ �� �����������
            std::cout << "������� ID ������ �� �����������: ";
            long compensationID;
            std::cin >> compensationID;
            std::cin.ignore();

            std::cout << "�������� ��������:\n"
                << "1. �������� ������\n"
                << "2. ��������� ������\n"
                << "�������� ��������: ";

            int actionChoice;
            std::cin >> actionChoice;
            std::cin.ignore();

            if (actionChoice == 1) {
                std::ostringstream oss;
                oss << "APPROVE_COMPENSATION|" << compensationID;
                client.sendMessage(oss.str());
                std::string response = client.receiveMessage();
                if (response.find("SUCCESS|") == 0) {
                    std::cout << response.substr(8) << "\n";
                }
                else {
                    std::cout << "������: " << response.substr(6) << "\n";
                }
            }
            else if (actionChoice == 2) {
                std::ostringstream oss;
                oss << "DECLINE_COMPENSATION|" << compensationID;
                client.sendMessage(oss.str());
                std::string response = client.receiveMessage();
                if (response.find("SUCCESS|") == 0) {
                    std::cout << response.substr(8) << "\n";
                }
                else {
                    std::cout << "������: " << response.substr(6) << "\n";
                }
            }
            else {
                std::cout << "������������ �����.\n";
            }
            break;
        }
        case 10: { // ����������� ���� ����������
            std::string command = "VIEW_ALL_EMPLOYEES";
            client.sendMessage(command);
            std::string response = client.receiveMessage();
            if (response.find("SUCCESS|") == 0) {
                std::cout << "��� ���������:\n" << response.substr(8) << "\n";
            }
            else {
                std::cout << "������: " << response.substr(6) << "\n";
            }
            break;
        }
        case 11: { // �����
            client.disconnect();
            std::cout << "����� �� �������.\n";
            exit(0);
        }
        default:
            std::cout << "������������ �����.\n";
        }
    }
}

void Menu::handleHRMenu() {
    std::cout << "\n+====================================+\n"
        << "|      ������� ���������� ���������� |\n"
        << "+====================================+\n"
        << "| ����: HR                           |\n"
        << "+------------------------------------+\n"
        << "| 1. �������� ����������             |\n"
        << "| 2. �������� ������ ����������      |\n"
        << "| 3. �������� ���������� ����        |\n"
        << "| 4. �������� ���������� ����        |\n"
        << "| 5. ������� ���������� ����         |\n"
        << "| 6. ����������� �������� �����      |\n"
        << "| 7. ����������� ���-�����           |\n"
        << "| 8. ��������� ��������� ������      |\n"  // ���������� ����� 8
        << "| 9. ���������� ������ �� �����������|\n"  // ���������� ����� 9
        << "| 10. �����                           |\n"
        << "+------------------------------------+\n"
        << "�������� ��������: ";
    int choice;
    std::cin >> choice;
    std::cin.ignore(); // ������� ������

    switch (choice) {
    case 1: { // �������� ����������
        std::cout << "������� ID ����������: ";
        long empID;
        std::cin >> empID;
        std::cin.ignore();

        std::cout << "�������� ���������:\n"
            << "1. ��������\n"
            << "2. ������\n"
            << "3. ������\n"
            << "�������� ��������: ";
        int posChoice;
        std::cin >> posChoice;
        std::cin.ignore();
        long posID;
        if (posChoice == 1) posID = 2001;
        else if (posChoice == 2) posID = 2002;
        else { std::cout << "������ ���������� ����������.\n"; return; }

        std::string fullName, birthDate, address, contactInfo, startDate, status;
        std::cout << "������� ��� ����������: ";
        getline(std::cin, fullName);
        std::cout << "������� ���� �������� (YYYY-MM-DD): ";
        getline(std::cin, birthDate);
        std::cout << "������� �����: ";
        getline(std::cin, address);
        std::cout << "������� ���������� ����������: ";
        getline(std::cin, contactInfo);
        std::cout << "������� ���� ������ ������ (YYYY-MM-DD): ";
        getline(std::cin, startDate);
        std::cout << "������� ������ (active/blocked): ";
        getline(std::cin, status);

        std::ostringstream oss;
        oss << "ADD_EMPLOYEE|" << empID << "|" << posID << "|" << fullName << "|"
            << birthDate << "|" << address << "|" << contactInfo << "|"
            << startDate << "|" << status;
        client.sendMessage(oss.str());
        std::string response = client.receiveMessage();
        std::cout << response.substr(8) << "\n"; // ����� ��������� ����� SUCCESS|
        break;
    }
    case 2: { // �������� ������ ����������
        std::cout << "������� ID ���������� ��� ��������� ������: ";
        long empID;
        std::cin >> empID;
        std::cin.ignore(); // ������� ������

        std::cout << "������� ����� ��� ����������: ";
        std::string fullName;
        getline(std::cin, fullName);

        std::cout << "������� ����� �����: ";
        std::string address;
        getline(std::cin, address);

        std::cout << "������� ����� ���������� �������: ";
        std::string contactInfo;
        getline(std::cin, contactInfo);

        std::cout << "������� ����� ������ (active/blocked): ";
        std::string status;
        getline(std::cin, status);

        std::ostringstream oss;
        oss << "UPDATE_EMPLOYEE|" << empID << "|" << fullName << "|" << address << "|" << contactInfo << "|" << status;
        client.sendMessage(oss.str());
        std::string response = client.receiveMessage();
        std::cout << response.substr(8) << "\n"; // ����� ��������� ����� SUCCESS|
        break;
    }

    case 3: { // �������� ���������� ����
        std::cout << "������� ID �����: ";
        std::string courseID;
        getline(std::cin, courseID);

        // �������� �� ������ ID �����
        if (courseID.empty() || courseID.find_first_not_of(" \t") == std::string::npos) {
            std::cout << "������: ID ����� �� ����� ���� ������ ��� ��������� ������ �������.\n";
            break;
        }

        std::cout << "������� ���� �����: ";
        std::string topic;
        getline(std::cin, topic);

        // �������� �� ������ ���� �����
        if (topic.empty()) {
            std::cout << "������: ���� ����� �� ����� ���� ������.\n";
            break;
        }

        std::cout << "������� ���� ������ ����� (YYYY-MM-DD): ";
        std::string startDate;
        getline(std::cin, startDate);

        // �������� �� ������������ ������� ���� ������
        if (startDate.empty() || !std::regex_match(startDate, std::regex(R"(\d{4}-\d{2}-\d{2})"))) {
            std::cout << "������: ������������ ������ ���� ������. ��������� YYYY-MM-DD.\n";
            break;
        }

        std::cout << "������� ���� ��������� ����� (YYYY-MM-DD): ";
        std::string endDate;
        getline(std::cin, endDate);

        // �������� �� ������������ ������� ���� ���������
        if (endDate.empty() || !std::regex_match(endDate, std::regex(R"(\d{4}-\d{2}-\d{2})"))) {
            std::cout << "������: ������������ ������ ���� ���������. ��������� YYYY-MM-DD.\n";
            break;
        }

        // ������������ ������ ��� ��������
        std::ostringstream oss;
        oss << "ADD_COURSE|" << courseID << "|" << topic << "|" << startDate << "|" << endDate;

        // �������� ���������
        client.sendMessage(oss.str());

        // ��������� ������
        std::string response = client.receiveMessage();

        // ����� ��������� ����� SUCCESS|
        if (response.find("SUCCESS") == 0) {
            std::cout << "���� ������� ��������: " << response.substr(8) << "\n";
        }
        else {
            std::cout << "������ ��� ���������� �����: " << response.substr(7) << "\n";
        }
        break;
    }

    case 4: { // �������� ���������� ����
        std::cout << "������� ID ����� ��� ���������: ";
        std::string courseID;
        getline(std::cin, courseID);
        std::cout << "������� ����� ���� �����: ";
        std::string topic;
        getline(std::cin, topic);
        std::cout << "������� ����� ���� ������ ����� (YYYY-MM-DD): ";
        std::string startDate;
        getline(std::cin, startDate);
        std::cout << "������� ����� ���� ��������� ����� (YYYY-MM-DD): ";
        std::string endDate;
        getline(std::cin, endDate);

        std::ostringstream oss;
        oss << "UPDATE_COURSE|" << courseID << "|" << topic << "|" << startDate << "|" << endDate;
        client.sendMessage(oss.str());
        std::string response = client.receiveMessage();
        std::cout << response.substr(8) << "\n";
        break;
    }
    case 5: { // ������� ���������� ����
        std::cout << "������� ID ����� ��� ��������: ";
        std::string courseID;
        getline(std::cin, courseID);

        std::ostringstream oss;
        oss << "DELETE_COURSE|" << courseID;
        client.sendMessage(oss.str());
        std::string response = client.receiveMessage();
        std::cout << response.substr(8) << "\n";
        break;
    }
    case 6: { // ����������� �������� �����
        std::string command = "VIEW_ACTIVE_COURSES";
        client.sendMessage(command);
        std::string response = client.receiveMessage();
        if (response.find("SUCCESS|") == 0) {
            std::cout << "�������� �����:\n" << response.substr(8) << "\n";
        }
        else {
            std::cout << response.substr(6) << "\n";
        }
        break;
    }

    case 7: { // ����������� ���-�����
        std::string command = "VIEW_LOGS";
        client.sendMessage(command);
        std::string response = client.receiveMessage();
        if (response.find("SUCCESS|") == 0) {
            std::cout << "���� �������:\n" << response.substr(8) << "\n";
        }
        else {
            std::cout << response.substr(6) << "\n";
        }
        break;
    }
    case 8: { // ��������� ��������� ������
        std::cout << "���������� ��������� ������:\n"
            << "1. �������� ����� ������\n"
            << "2. �������� ������ ������\n"
            << "3. ������� ������ ������\n"
            << "4. ����������� ������� ������\n"
            << "�������� ��������: ";

        int scheduleChoice;
        std::cin >> scheduleChoice;
        std::cin.ignore(); // ������� ������

        switch (scheduleChoice) {
        case 1: {
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

            std::cout << "������� ����� ��������� (HH:MM): ";
            std::string endTime;
            getline(std::cin, endTime);

            std::ostringstream oss;
            oss << "ADD_WORK_SCHEDULE|" << empID << "|" << dayOfWeek << "|" << startTime << "|" << endTime;
            client.sendMessage(oss.str());
            std::string response = client.receiveMessage();
            std::cout << response.substr(8) << "\n";
            break;
        }
        case 2: {
            std::cout << "������� ID ���������� ��� ��������� �������: ";
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

            std::cout << "������� ����� ����� ��������� (HH:MM): ";
            std::string endTime;
            getline(std::cin, endTime);

            std::ostringstream oss;
            oss << "UPDATE_WORK_SCHEDULE|" << empID << "|" << dayOfWeek << "|" << startTime << "|" << endTime;
            client.sendMessage(oss.str());
            std::string response = client.receiveMessage();
            std::cout << response.substr(8) << "\n";
            break;
        }
        case 3: {
            std::cout << "������� ID ���������� ��� �������� �������: ";
            long empID;
            std::cin >> empID;
            std::cin.ignore();

            std::ostringstream oss;
            oss << "DELETE_WORK_SCHEDULE|" << empID;
            client.sendMessage(oss.str());
            std::string response = client.receiveMessage();
            std::cout << response.substr(8) << "\n";
            break;
        }
        case 4: {
            std::cout << "������� ID ���������� ��� ��������� ��������: ";
            long empID;
            std::cin >> empID;
            std::cin.ignore();

            std::ostringstream oss;
            oss << "VIEW_WORK_SCHEDULE|" << empID;
            client.sendMessage(oss.str());
            std::string response = client.receiveMessage();
            std::cout << response.substr(8) << "\n";
            break;
        }
        default:
            std::cout << "������������ �����.\n";
        }
        break;
    }
    case 9: { // ���������� ������ �� �����������
        std::cout << "������� ID ������ �� �����������: ";
        long compensationID;
        std::cin >> compensationID;
        std::cin.ignore();

        std::cout << "�������� ��������:\n"
            << "1. �������� ������\n"
            << "2. ��������� ������\n"
            << "�������� ��������: ";

        int actionChoice;
        std::cin >> actionChoice;
        std::cin.ignore();

        if (actionChoice == 1) {
            std::ostringstream oss;
            oss << "APPROVE_COMPENSATION|" << compensationID;
            client.sendMessage(oss.str());
            std::string response = client.receiveMessage();
            std::cout << response.substr(8) << "\n";
        }
        else if (actionChoice == 2) {
            std::ostringstream oss;
            oss << "DECLINE_COMPENSATION|" << compensationID;
            client.sendMessage(oss.str());
            std::string response = client.receiveMessage();
            std::cout << response.substr(8) << "\n";
        }
        else {
            std::cout << "������������ �����.\n";
        }
        break;
    }
    case 10: { // �����
        client.disconnect();
        std::cout << "����� �� �������.\n";
        exit(0);
    }
    default:
        std::cout << "������������ �����.\n";
    }
}


void Menu::handleCashierMenu() {
    std::cout << "\n+====================================+\n"
        << "|      ������� ���������� ���������� |\n"
        << "+====================================+\n"
        << "| ����: Cashier                      |\n"
        << "+------------------------------------+\n"
        << "| 1. ����������� �������� ����������� |\n"
        << "| 2. ���������� ������ �� ����������� |\n"
        << "| 3. ����������� �������� �����      |\n"
        << "| 4. �����                           |\n"
        << "+------------------------------------+\n"
        << "�������� ��������: ";
    int choice;
    std::cin >> choice;
    std::cin.ignore(); // ������� ������

    switch (choice) {
    case 1: { // ����������� �������� �����������
        std::string command = "VIEW_SALARIES";
        client.sendMessage(command);
        std::string response = client.receiveMessage();
        if (response.find("SUCCESS|") == 0) {
            std::cout << "�������� �����������:\n" << response.substr(8) << "\n";
        }
        else {
            std::cout << response.substr(6) << "\n";
        }
        break;
    }
    case 2: { // ���������� ������ �� �����������
        std::cout << "������� ID ������ ��� ���������: ";
        long requestID;
        std::cin >> requestID;
        std::cin.ignore();
        std::cout << "������� ����� ������ ������ (��������/���������): ";
        std::string newStatus;
        getline(std::cin, newStatus);

        std::ostringstream oss;
        oss << "PROCESS_COMPENSATION|" << requestID << "|" << newStatus;
        client.sendMessage(oss.str());
        std::string response = client.receiveMessage();
        std::cout << response.substr(8) << "\n";
        break;
    }
    case 3: { // ����������� �������� �����
        std::string command = "VIEW_ACTIVE_COURSES";
        client.sendMessage(command);
        std::string response = client.receiveMessage();
        if (response.find("SUCCESS|") == 0) {
            std::cout << "�������� �����:\n" << response.substr(8) << "\n";
        }
        else {
            std::cout << response.substr(6) << "\n";
        }
        break;
    }
    case 4: { // �����
        client.disconnect();
        std::cout << "����� �� �������.\n";
        exit(0);
    }
    default:
        std::cout << "������������ �����.\n";
    }
}

void Menu::handleManagerMenu() {
    std::cout << "\n+====================================+\n"
        << "|      ������� ���������� ���������� |\n"
        << "+====================================+\n"
        << "| ����: Manager                      |\n"
        << "+------------------------------------+\n"
        << "| 1. ��������� ��������� ������      |\n"
        << "| 2. ������� ������������������      |\n"
        << "| 3. ����������� �������� �����      |\n"
        << "| 4. �����                           |\n"
        << "+------------------------------------+\n"
        << "�������� ��������: ";
    int choice;
    std::cin >> choice;
    std::cin.ignore(); // ������� ������

    switch (choice) {
    case 1: { // ��������� ��������� ������
        std::cout << "���������� ��������� ������:\n"
            << "1. �������� ����� ������\n"
            << "2. �������� ������ ������\n"
            << "3. ������� ������ ������\n"
            << "4. ����������� ������� ������\n"
            << "�������� ��������: ";

        int scheduleChoice;
        std::cin >> scheduleChoice;
        std::cin.ignore(); // ������� ������

        switch (scheduleChoice) {
        case 1: {
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

            std::cout << "������� ����� ��������� (HH:MM): ";
            std::string endTime;
            getline(std::cin, endTime);

            std::ostringstream oss;
            oss << "ADD_WORK_SCHEDULE|" << empID << "|" << dayOfWeek << "|" << startTime << "|" << endTime;
            client.sendMessage(oss.str());
            std::string response = client.receiveMessage();
            std::cout << response.substr(8) << "\n";
            break;
        }
        case 2: {
            std::cout << "������� ID ���������� ��� ��������� �������: ";
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

            std::cout << "������� ����� ����� ��������� (HH:MM): ";
            std::string endTime;
            getline(std::cin, endTime);

            std::ostringstream oss;
            oss << "UPDATE_WORK_SCHEDULE|" << empID << "|" << dayOfWeek << "|" << startTime << "|" << endTime;
            client.sendMessage(oss.str());
            std::string response = client.receiveMessage();
            std::cout << response.substr(8) << "\n";
            break;
        }
        case 3: {
            std::cout << "������� ID ���������� ��� �������� �������: ";
            long empID;
            std::cin >> empID;
            std::cin.ignore();

            std::ostringstream oss;
            oss << "DELETE_WORK_SCHEDULE|" << empID;
            client.sendMessage(oss.str());
            std::string response = client.receiveMessage();
            std::cout << response.substr(8) << "\n";
            break;
        }
        case 4: {
            std::cout << "������� ID ���������� ��� ��������� ��������: ";
            long empID;
            std::cin >> empID;
            std::cin.ignore();

            std::ostringstream oss;
            oss << "VIEW_WORK_SCHEDULE|" << empID;
            client.sendMessage(oss.str());
            std::string response = client.receiveMessage();
            std::cout << response.substr(8) << "\n";
            break;
        }
        default:
            std::cout << "������������ �����.\n";
        }
        break;
    }
    case 2: { // ������� ������������������
        // ���������� ������ ������������������
        std::cout << "���������� ������ ������������������ ��� �� ����������.\n";
        break;
    }
    case 3: { // ����������� �������� �����
        std::string command = "VIEW_ACTIVE_COURSES";
        client.sendMessage(command);
        std::string response = client.receiveMessage();
        if (response.find("SUCCESS|") == 0) {
            std::cout << "�������� �����:\n" << response.substr(8) << "\n";
        }
        else {
            std::cout << response.substr(6) << "\n";
        }
        break;
    }
    case 4: { // �����
        client.disconnect();
        std::cout << "����� �� �������.\n";
        exit(0);
    }
    default:
        std::cout << "������������ �����.\n";
    }
}


void Menu::handleEmployeeMenu() {
    std::cout << "\n+====================================+\n"
        << "|      ������� ���������� ���������� |\n"
        << "+====================================+\n"
        << "| ����: Employee                     |\n"
        << "+------------------------------------+\n"
        << "| 1. ����������� ���� ����������     |\n"
        << "| 2. ����������� ���� ������ ������  |\n"
        << "| 3. ������� � ���������� �� ����    |\n"
        << "| 4. ������ ������ �� �����������    |\n"
        << "| 5. �����                           |\n"
        << "+------------------------------------+\n"
        << "�������� ��������: ";
    int choice;
    std::cin >> choice;
    std::cin.ignore(); // ������� ������

    switch (choice) {
    case 1: { // ����������� ���� ����������
        std::string command = "VIEW_OWN_INFO|" + username;
        client.sendMessage(command);
        std::string response = client.receiveMessage();
        if (response.find("SUCCESS|") == 0) {
            std::cout << "���� ����������:\n" << response.substr(8) << "\n";
        }
        else {
            std::cout << response.substr(6) << "\n";
        }
        break;
    }
    case 2: { // ����������� ���� ������ ������
        std::string command = "VIEW_OWN_SCHEDULE|" + username;
        client.sendMessage(command);
        std::string response = client.receiveMessage();
        if (response.find("SUCCESS|") == 0) {
            std::cout << "��� ������ ������:\n" << response.substr(8) << "\n";
        }
        else {
            std::cout << response.substr(6) << "\n";
        }
        break;
    }
    case 3: { // ������� � ���������� �� ����
        std::string command = "VIEW_ACTIVE_COURSES";
        client.sendMessage(command);
        std::string response = client.receiveMessage();
        if (response.find("SUCCESS|") == 0) {
            std::cout << "��������� �����:\n" << response.substr(8) << "\n";
            std::cout << "������� ID ����� ��� ������: ";
            std::string courseID;
            getline(std::cin, courseID);
            // ������������, ��� � ���������� ���� ���������� employeeID, ��������, 1001
            // � �������� ������� employeeID ����� ������� ����� ��������������
            long employeeID = 1001; // ������
            std::ostringstream oss;
            oss << "ENROLL_COURSE|" << courseID << "|" << employeeID;
            client.sendMessage(oss.str());
            std::string enrollResponse = client.receiveMessage();
            std::cout << enrollResponse.substr(8) << "\n";
        }
        else {
            std::cout << response.substr(6) << "\n";
        }
        break;
    }
    case 4: { // ������ ������ �� �����������
        std::cout << "������� ID ������: ";
        long requestID;
        std::cin >> requestID;
        std::cin.ignore();
        std::cout << "������� ��� �����������: ";
        std::string compensationType;
        getline(std::cin, compensationType);
        std::cout << "������� �����: ";
        double amount;
        std::cin >> amount;
        std::cin.ignore();
        std::cout << "������� ���� ������ ������ (YYYY-MM-DD): ";
        std::string submissionDate;
        getline(std::cin, submissionDate);
        std::string status = "�� ������������";

        std::ostringstream oss;
        oss << "REQUEST_COMPENSATION|" << requestID << "|" << 1001 << "|" << 2001 << "|"
            << compensationType << "|" << amount << "|" << submissionDate << "|" << status;
        client.sendMessage(oss.str());
        std::string response = client.receiveMessage();
        std::cout << response.substr(8) << "\n";
        break;
    }
    case 5: { // �����
        client.disconnect();
        std::cout << "����� �� �������.\n";
        exit(0);
    }
    default:
        std::cout << "������������ �����.\n";
    }
}
