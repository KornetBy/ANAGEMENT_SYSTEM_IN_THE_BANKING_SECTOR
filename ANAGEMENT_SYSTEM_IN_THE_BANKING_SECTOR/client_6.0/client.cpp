#include "Client.h"
#include <ws2tcpip.h>
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
    if (inet_pton(AF_INET, serverIP.c_str(), &serverAddr.sin_addr) <= 0) {
        std::cerr << "������ ����������� IP-������." << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        exit(1);
    }
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
    std::cout << "�������� ����������� � �������!" << std::endl;
    return true;
}

// �������� ���������
void Client::sendMessage(const std::string& message) {
    send(clientSocket, message.c_str(), message.size(), 0);
}

// ��������� ���������
std::string Client::receiveMessage() {
    char buffer[1024] = { 0 };
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead > 0) {
        buffer[bytesRead] = '\0';
        return std::string(buffer);
    }
    return "ERROR|��� ������ �� �������.";
}

// �������� ����������
void Client::closeConnection() {
    closesocket(clientSocket);
}
bool validateInput(const std::string& input) {
    if (input.empty()) {
        std::cerr << "������: ������� ������ ������." << std::endl;
        return false;
    }

    // �������������� ��������: ���� �� ������ ��������� �������� ��� ����������� ��������
    for (char c : input) {
        if (!std::isalnum(c)) {
            std::cerr << "������: ���� �������� ������������ �������." << std::endl;
            return false;
        }
    }

    return true;
}
bool Client::validateInput(const std::string& input) {
    if (input.empty()) {
        std::cerr << "������: ������� ������ ������." << std::endl;
        return false;
    }

    for (char c : input) {
        if (!std::isalnum(c)) { // ��������, ��� ������� ��������-��������
            std::cerr << "������: ���� �������� ������������ �������." << std::endl;
            return false;
        }
    }

    return true;
}