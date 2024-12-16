#include "CashierUI.h"
#include "Client.h"
#include <iostream>
#include <string>

void CashierUI::showMenu() {
    while (true) {
        std::cout << "\n--- ���� ������� ---\n";
        std::cout << "1. ����������� ������ ������\n";
        std::cout << "0. �����\n";
        std::cout << "������� ��� �����: ";

        int choice;
        std::cin >> choice;

        switch (choice) {
        case 1: {
            std::string employeeId;
            std::cout << "������� ��� ID: ";
            std::cin >> employeeId;

            // ������������ �������
            std::string request = "GET_SCHEDULE " + employeeId;
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
