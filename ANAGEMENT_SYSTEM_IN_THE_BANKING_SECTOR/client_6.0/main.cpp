// Client/main.cpp
#include "Client.h"
#include "Menu.h"
#include <iostream>
#include <string>

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    Client client;
    std::string serverIP = "127.0.0.1"; // IP �������
    int serverPort = 5000; // ���� �������

    // ����������� � �������
    if (!client.connectToServer(serverIP, serverPort)) {
        std::cerr << "�� ������� ������������ � �������. ���������� ���������." << std::endl;
        return 1;
    }
    std::cout << "������� ���������� � �������.\n";

    // ��������� ��������������� ��������� �� �������
    std::string welcome = client.receiveMessage();
    std::cout << "������: " << welcome << std::endl;

    // ��������������
    std::string username, password;
    std::cout << "������� �����: ";
    std::getline(std::cin, username);
    std::cout << "������� ������: ";
    std::getline(std::cin, password);

    // ��������� ������� AUTH|username|password
    std::string authCommand = "AUTH|" + username + "|" + password;
    client.sendMessage(authCommand);

    // ��������� ������
    std::string authResponse = client.receiveMessage();
    if (authResponse.find("SUCCESS") == 0) {
        size_t pos = authResponse.find('|');
        std::string role = authResponse.substr(pos + 1);
        std::cout << "�������� �����������. ����: " << role << std::endl;

        // �������� � ������ ����
        Menu menu(client, role);
        menu.showMenu();
    }
    else {
        std::cerr << "������ ��������������: " << authResponse << std::endl;
    }

    // ���������� �� �������
    client.disconnect();
    return 0;
}
