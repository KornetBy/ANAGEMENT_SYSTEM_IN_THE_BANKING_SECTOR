#include "ManagerUI.h"
#include "Client.h"
#include <iostream>
#include <string>

void ManagerUI::showMenu() {
    while (true) {
        std::cout << "\n--- ���� ������������ ������������� ---\n";
        std::cout << "1. ����������� ����������� �������������\n";
        std::cout << "2. ���������� ��������� ������\n";
        std::cout << "3. ������ ������������������ �����������\n";
        std::cout << "0. �����\n";
        std::cout << "������� ��� �����: ";

        int choice;
        std::cin >> choice;

        switch (choice) {
        case 1: {
            std::string divisionId;
            std::cout << "������� ID ������ �������������: ";
            std::cin >> divisionId;

            // ������������ �������
            std::string request = "GET_EMPLOYEES_BY_DIVISION " + divisionId;
            Client::sendRequest(request);

            // ��������� ������
            std::cout << Client::receiveResponse();
            break;
        }
        case 2: {
            std::string employeeId, date, startTime, endTime, shiftType;
            std::cout << "������� ID ����������: ";
            std::cin >> employeeId;
            std::cout << "������� ���� ����� (YYYY-MM-DD): ";
            std::cin >> date;
            std::cout << "������� ����� ������ ����� (HH:MM): ";
            std::cin >> startTime;
            std::cout << "������� ����� ��������� ����� (HH:MM): ";
            std::cin >> endTime;
            std::cout << "������� ��� ����� (Regular/Overtime/Leave/SickLeave): ";
            std::cin >> shiftType;

            // ������������ �������
            std::string request = "ADD_SHIFT " + employeeId + " " + date + " " +
                startTime + " " + endTime + " " + shiftType;
            Client::sendRequest(request);

            // ��������� ������
            std::cout << Client::receiveResponse();
            break;
        }
        case 3: {
            std::string employeeId, date, comments;
            int kpi;
            std::cout << "������� ID ����������: ";
            std::cin >> employeeId;
            std::cout << "������� ���� ������ (YYYY-MM-DD): ";
            std::cin >> date;
            std::cout << "������� KPI (0-100): ";
            std::cin >> kpi;
            std::cout << "������� �����������: ";
            std::cin.ignore();
            std::getline(std::cin, comments);

            // ������������ �������
            std::string request = "ADD_PERFORMANCE " + employeeId + " " + date + " " +
                std::to_string(kpi) + " " + comments;
            Client::sendRequest(request);

            // ��������� ������
            std::cout << Client::receiveResponse();
            break;
        }
        case 0:
            std::cout << "������� � ������� ����.\n";
            return;
        default:
            std::cerr << "������: �������� �����. ���������� �����.\n";
        }
    }
}
