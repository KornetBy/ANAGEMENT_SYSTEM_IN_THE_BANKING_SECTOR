#include "FinanceUI.h"
#include "Client.h"
#include <iostream>
#include <string>

void FinanceUI::showMenu() {
    while (true) {
        std::cout << "\n--- ���� ����������� ---\n";
        std::cout << "1. �������� �������� ����������\n";
        std::cout << "2. �������� �������� ����������\n";
        std::cout << "3. ���������� ������ �� �����������\n";
        std::cout << "4. ����������� ����� �� ���������\n";
        std::cout << "5. ����������� ����� �� ������������\n";
        std::cout << "0. �����\n";
        std::cout << "������� ��� �����: ";

        int choice;
        std::cin >> choice;

        switch (choice) {
        case 1: {
            std::string employeeId, paymentDate;
            double baseSalary, bonuses, deductions;
            std::cout << "������� ID ����������: ";
            std::cin >> employeeId;
            std::cout << "������� ������� ��������: ";
            std::cin >> baseSalary;
            std::cout << "������� ����� �������: ";
            std::cin >> bonuses;
            std::cout << "������� ����� ���������: ";
            std::cin >> deductions;
            std::cout << "������� ���� ������� (YYYY-MM-DD): ";
            std::cin >> paymentDate;

            // ������������ �������
            std::string request = "ADD_SALARY " + employeeId + " " + std::to_string(baseSalary) + " " +
                std::to_string(bonuses) + " " + std::to_string(deductions) + " " + paymentDate;
            Client::sendRequest(request);

            // ��������� ������
            std::cout << Client::receiveResponse();
            break;
        }
        case 2: {
            std::string employeeId, paymentDate;
            double baseSalary, bonuses, deductions;
            std::cout << "������� ID ����������: ";
            std::cin >> employeeId;
            std::cout << "������� ����� ������� ��������: ";
            std::cin >> baseSalary;
            std::cout << "������� ����� ����� �������: ";
            std::cin >> bonuses;
            std::cout << "������� ����� ����� ���������: ";
            std::cin >> deductions;
            std::cout << "������� ����� ���� ������� (YYYY-MM-DD): ";
            std::cin >> paymentDate;

            // ������������ �������
            std::string request = "UPDATE_SALARY " + employeeId + " " + std::to_string(baseSalary) + " " +
                std::to_string(bonuses) + " " + std::to_string(deductions) + " " + paymentDate;
            Client::sendRequest(request);

            // ��������� ������
            std::cout << Client::receiveResponse();
            break;
        }
        case 3: {
            std::string requestId, newStatus;
            std::cout << "������� ID ������ �� �����������: ";
            std::cin >> requestId;
            std::cout << "������� ����� ������ (Approved/Rejected): ";
            std::cin >> newStatus;

            // ������������ �������
            std::string request = "UPDATE_COMPENSATION " + requestId + " " + newStatus;
            Client::sendRequest(request);

            // ��������� ������
            std::cout << Client::receiveResponse();
            break;
        }
        case 4: {
            Client::sendRequest("GET_REPORT SALARY");
            std::cout << Client::receiveResponse();
            break;
        }
        case 5: {
            Client::sendRequest("GET_REPORT COMPENSATION");
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
