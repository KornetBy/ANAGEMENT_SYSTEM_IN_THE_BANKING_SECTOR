#include "DirectorUI.h"
#include "Client.h"
#include <iostream>
#include <string>

void DirectorUI::showMenu() {
    while (true) {
        std::cout << "\n--- ���� ��������� ---\n";
        std::cout << "1. �������� ������ �� ���������\n";
        std::cout << "2. �������� �������� ������ �� �������������\n";
        std::cout << "0. �����\n";
        std::cout << "������� ��� �����: ";

        int choice;
        std::cin >> choice;

        switch (choice) {
        case 1: {
            Client::sendRequest("GET_REPORT SALARY");
            std::cout << Client::receiveResponse();
            break;
        }
        case 2: {
            std::cout << "������� ID �������������: ";
            std::string divisionId;
            std::cin >> divisionId;
            Client::sendRequest("GET_DIVISION_REPORT " + divisionId);
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
