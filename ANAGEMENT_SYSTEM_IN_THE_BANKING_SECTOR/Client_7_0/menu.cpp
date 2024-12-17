// Menu.cpp
#include "Menu.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <locale>
Menu::Menu(Client& cli) : client(cli), role(""), username("") {}

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
    case 4: { // ���������� ������� ���������
        std::string command = "VIEW_HISTORY";
        client.sendMessage(command);
        std::string response = client.receiveMessage();
        if (response.find("SUCCESS|") == 0) {
            std::cout << "������� ���������:\n" << response.substr(8) << "\n";
        }
        else {
            std::cout << response.substr(6) << "\n";
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
        std::cout << response.substr(8) << "\n"; // ����� ��������� ����� SUCCESS|
        break;
    }
    case 8: { // �������� ������������
        std::cout << "������� �����: ";
        std::string newUsername;
        getline(std::cin, newUsername);
        std::cout << "������� ������: ";
        std::string newPassword;
        getline(std::cin, newPassword);
        std::cout << "�������� ���� ������������:\n"
            << "1. HR\n"
            << "2. Cashier\n"
            << "3. Manager\n"
            << "4. Employee\n"
            << "5. ������\n"
            << "�������� ��������: ";
        int roleChoice;
        std::cin >> roleChoice;
        std::cin.ignore();
        std::string newRole;
        if (roleChoice == 1) newRole = "hr";
        else if (roleChoice == 2) newRole = "cashier";
        else if (roleChoice == 3) newRole = "manager";
        else if (roleChoice == 4) newRole = "employee";
        else { std::cout << "������ ���������� ������������.\n"; return; }

        std::string status = "active";

        std::ostringstream oss;
        oss << "ADD_USER|" << newUsername << "|" << newPassword << "|"
            << newRole << "|" << status;
        client.sendMessage(oss.str());
        std::string response = client.receiveMessage();
        std::cout << response.substr(8) << "\n"; // ����� ��������� ����� SUCCESS|
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
        << "| 8. ��������� ��������� ������      |\n"
        << "| 9. ���������� ������ �� �����������|\n"
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
    case 3: { // �������� ���������� ����
        std::cout << "������� ID �����: ";
        std::string courseID;
        getline(std::cin, courseID);
        std::cout << "������� ���� �����: ";
        std::string topic;
        getline(std::cin, topic);
        std::cout << "������� ���� ������ ����� (YYYY-MM-DD): ";
        std::string startDate;
        getline(std::cin, startDate);
        std::cout << "������� ���� ��������� ����� (YYYY-MM-DD): ";
        std::string endDate;
        getline(std::cin, endDate);

        std::ostringstream oss;
        oss << "ADD_COURSE|" << courseID << "|" << topic << "|" << startDate << "|" << endDate;
        client.sendMessage(oss.str());
        std::string response = client.receiveMessage();
        std::cout << response.substr(8) << "\n"; // ����� ��������� ����� SUCCESS|
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
        // ���������� ���������� ����������/�������������� ��������
        std::cout << "���������� ���������� ��������� ������ ��� �� ����������.\n";
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
