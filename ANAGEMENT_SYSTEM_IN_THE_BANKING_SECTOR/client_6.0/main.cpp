#include "Client.h"
#include "Auth.h"
#include "Menu.h"
#include <windows.h>

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    const std::string serverIP = "127.0.0.1"; // IP-����� �������
    const int port = 5000;                    // ���� �������

    Client client(serverIP, port);

    if (client.connectToServer()) { // ������������� ����������
        std::cout << "����������� �������!" << std::endl;

        // �����������
        Auth auth;
        std::string login, password;

        // ���� ������ � ����������
        do {
            std::cout << "������� �����: ";
            std::getline(std::cin, login);
        } while (!client.validateInput(login));

        // ���� ������ � ����������
        do {
            std::cout << "������� ������: ";
            std::getline(std::cin, password);
        } while (!client.validateInput(password));

        // ��������� ������ ��� �����������
        auth.setCredentials(login, password);
        std::string authRequest = auth.getAuthRequest();

        // �������� ������� �����������
        client.sendMessage(authRequest);
        std::string serverResponse = client.receiveMessage();

        if (serverResponse.find("SUCCESS") != std::string::npos) {
            std::cout << "����������� �������!" << std::endl;

            // ��������� ���� ������������
            std::string role = serverResponse.substr(serverResponse.find('|') + 1);

            // ����������� ���� � ����������� �� ����
            Menu menu(client, role);
            menu.showMenu();
        }
        else {
            std::cerr << "������ �����������: " << serverResponse << std::endl;
        }
    }
    else {
        std::cerr << "�� ������� ������������ � �������." << std::endl;
    }

    return 0;
}
