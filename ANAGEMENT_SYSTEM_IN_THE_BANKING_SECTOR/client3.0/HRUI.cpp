#include "HRUI.h"
#include "Client.h"
#include <iostream>
#include <string>

void HRUI::showMenu() {
    while (true) {
        std::cout << "\n--- ���� HR-����������� ---\n";
        std::cout << "1. �������� ����������\n";
        std::cout << "2. ������� ����������\n";
        std::cout << "3. ������������� ������ ����������\n";
        std::cout << "0. �����\n";
        std::cout << "������� ��� �����: ";

        int choice;
        std::cin >> choice;

        switch (choice) {
        case 1: {
            std::string id, fullName, birthDate, position, contact, startDate, divisionId;
            std::cout << "������� ID ����������: ";
            std::cin >> id;
            std::cout << "������� ���: ";
            std::cin.ignore(); // ������� ������
            std::getline(std::cin, fullName);
            std::cout << "������� ���� �������� (YYYY-MM-DD): ";
            std::cin >> birthDate;
            std::cout << "������� ���������: ";
            std::cin.ignore();
            std::getline(std::cin, position);
            std::cout << "������� ���������� ������: ";
            std::getline(std::cin, contact);
            std::cout << "������� ���� ������ ������ (YYYY-MM-DD): ";
            std::cin >> startDate;
            std::cout << "������� ID �������������: ";
            std::cin >> divisionId;

            // ������������ �������
            std::string request = "ADD_EMPLOYEE " + id + " " + fullName + " " + birthDate + " " +
                position + " " + contact + " " + startDate + " " + divisionId;
            Client::sendRequest(request);

            // ��������� ������
            std::cout << Client::receiveResponse();
            break;
        }
        case 2: {
            std::string id;
            std::cout << "������� ID ���������� ��� ��������: ";
            std::cin >> id;

            // ������������ �������
            std::string request = "DELETE_EMPLOYEE " + id;
            Client::sendRequest(request);

            // ��������� ������
            std::cout << Client::receiveResponse();
            break;
        }
        case 3: {
            std::string id, fullName, birthDate, position, contact, startDate, divisionId;
            std::cout << "������� ID ���������� ��� ��������������: ";
            std::cin >> id;
            std::cout << "������� ����� ������ ���������� (�������� ���� ������ ��� ���������� �������� ��������):\n";

            std::cout << "���: ";
            std::cin.ignore();
            std::getline(std::cin, fullName);
            std::cout << "���� �������� (YYYY-MM-DD): ";
            std::getline(std::cin, birthDate);
            std::cout << "���������: ";
            std::getline(std::cin, position);
            std::cout << "���������� ������: ";
            std::getline(std::cin, contact);
            std::cout << "���� ������ ������ (YYYY-MM-DD): ";
            std::getline(std::cin, startDate);
            std::cout << "ID �������������: ";
            std::getline(std::cin, divisionId);

            // ������������ �������
            std::string request = "UPDATE_EMPLOYEE " + id + " " + fullName + " " + birthDate + " " +
                position + " " + contact + " " + startDate + " " + divisionId;
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
