#include "Client.h"

// �����������
Client::Client(const std::string& ip, int port) : serverIP(ip), port(port) {
    // ������������� Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "������ ������������� Winsock." << std::endl;
        exit(1);
    }

    // �������� ������
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "������ �������� ������." << std::endl;
        WSACleanup();
        exit(1);
    }

    // ��������� ������ �������
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(serverIP.c_str());
    serverAddr.sin_port = htons(port);
}

// ����������
Client::~Client() {
    closeConnection();
    WSACleanup();
}

// ����������� � �������
bool Client::connectToServer() {
    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "������ ����������� � �������." << std::endl;
        closesocket(clientSocket);
        return false;
    }
    std::cout << "����������� � ������� �����������." << std::endl;

    // ���� ��������������� ��������� �� �������
    char buffer[1024] = { 0 };
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesRead > 0) {
        buffer[bytesRead] = '\0';
        std::cout << "������: " << buffer << std::endl;
    }

    return true;
}

// �������� ��������� �������
void Client::sendMessage() {
    std::string userMessage;
    char buffer[1024] = { 0 };

    std::cout << "������� ��������� ��� ������� (��� 'exit' ��� ����������):" << std::endl;

    while (true) {
        std::cout << "��: ";
        std::getline(std::cin, userMessage);

        // ����� �� ����� �� ������� "exit"
        if (userMessage == "exit") {
            std::cout << "���������� ���������� � ��������." << std::endl;
            break;
        }

        // �������� ��������� �� ������
        send(clientSocket, userMessage.c_str(), userMessage.size(), 0);

        // ��������� ������ �� �������
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead > 0) {
            buffer[bytesRead] = '\0';
            std::cout << "������: " << buffer << std::endl;
        }
        else {
            std::cerr << "���������� � �������� ��������." << std::endl;
            break;
        }
    }
}

// �������� ����������
void Client::closeConnection() {
    closesocket(clientSocket);
    std::cout << "���������� �������." << std::endl;
}
