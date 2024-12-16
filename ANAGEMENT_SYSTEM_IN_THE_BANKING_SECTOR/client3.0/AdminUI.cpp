#include "AdminUI.h"
#include "Client.h"
#include <iostream>
#include <string>

void AdminUI::showMenu() {
    while (true) {
        std::cout << "\n--- ���� �������������� ---\n";
        std::cout << "1. �������� ������������\n";
        std::cout << "2. ������� ������������\n";
        std::cout << "0. �����\n";
        std::cout << "������� ��� �����: ";

        int choice;
        std::cin >> choice;

        switch (choice) {
        case 1: {
            std::string id, username, password, role;
            std::cout << "������� ID ������������: ";
            std::cin >> id;
            std::cout << "������� �����: ";
            std::cin >> username;
            std::cout << "������� ������: ";
            std::cin >> password;
            std::cout << "������� ����: ";
            std::cin >> role;

            // ������������ �������
            std::string request = "ADD_USER " + id + " " + username + " " + password + " " + role;
            Client::sendRequest(request);

            // ��������� ������
            std::cout << Client::receiveResponse();
            break;
        }
        case 2: {
            std::string username;
            std::cout << "������� ����� ������������ ��� ��������: ";
            std::cin >> username;

            // ������������ �������
            std::string request = "DELETE_USER " + username;
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
