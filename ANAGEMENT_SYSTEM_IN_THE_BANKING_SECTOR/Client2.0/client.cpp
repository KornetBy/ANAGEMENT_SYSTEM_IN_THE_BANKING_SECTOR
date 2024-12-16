#include "client_interface.h"
#include "client_operations.h"
#include <winsock2.h>
#include <iostream>
#include <string>
#include <stdexcept>
#include <map>
#include <fstream>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib") // �������� ���������� Winsock

std::string userRole; // ������ ���� �������� ������������

// �������� ������������ �������
std::map<std::string, std::string> loadClientConfig(const std::string& configFile) {
    std::map<std::string, std::string> config;
    std::ifstream file(configFile);
    if (!file.is_open()) {
        throw std::runtime_error("������: �� ������� ������� ���� ������������ " + configFile);
    }
    std::string line;
    while (std::getline(file, line)) {
        size_t pos = line.find('=');
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            config[key] = value;
        }
    }
    return config;
}

// ��������� ���������� � ��������
void connectToServer(SOCKET& clientSocket, const std::string& serverIp, int serverPort) {
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        throw std::runtime_error("������ �������� ������.");
    }

    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);

    // �������������� IP-������ �� std::string � PCWSTR
    WCHAR wideIp[INET_ADDRSTRLEN];
    MultiByteToWideChar(CP_ACP, 0, serverIp.c_str(), -1, wideIp, INET_ADDRSTRLEN);

    // ���������� InetPton
    if (InetPton(AF_INET, wideIp, &serverAddr.sin_addr) <= 0) {
        throw std::runtime_error("������ �������������� IP-������.");
    }

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        throw std::runtime_error("�� ������� ������������ � �������.");
    }

    std::cout << "���������� � �������� �����������.\n";
}

// ����������� ������������
void login(SOCKET clientSocket) {
    std::string username, password;
    std::cout << "������� ��� ������������: ";
    std::cin >> username;
    std::cout << "������� ������: ";
    std::cin >> password;

    std::string loginRequest = "LOGIN " + username + " " + password + "\n";
    std::cout << "�������� �������: " << loginRequest << std::endl;
    send(clientSocket, loginRequest.c_str(), loginRequest.size(), 0);

    char response[1024] = { 0 };
    int bytesRead = recv(clientSocket, response, sizeof(response), 0);

    if (bytesRead <= 0) {
        throw std::runtime_error("������: ������ �� ������� �� ������ �����������.");
    }

    std::string loginResponse(response);
    if (loginResponse.find("SUCCESS") != std::string::npos) {
        size_t rolePos = loginResponse.find("����: ");
        if (rolePos != std::string::npos) {
            userRole = loginResponse.substr(rolePos + 6); // ��������� ����
            std::cout << "����������� �������. ���� ����: " << userRole << "\n";
        }
        else {
            throw std::runtime_error("������: ������ �� ������ ���� ������������.");
        }
    }
    else {
        throw std::runtime_error("������ �����������: " + loginResponse);
    }
}

// �������� ���� ������ �������
void mainClientLoop(SOCKET clientSocket) {
    while (true) {
        if (userRole == "admin") {
            showAdminMenu();
            int choice = getUserChoice();
            if (choice == 5) break; // �����
            executeAdminCommand(clientSocket, choice);
        }
        else if (userRole == "client") {
            showClientMenu();
            int choice = getUserChoice();
            if (choice == 4) break; // �����
            executeClientCommand(clientSocket, choice);
        }
        else if (userRole == "worker") {
            showWorkerMenu();
            int choice = getUserChoice();
            if (choice == 5) break; // �����
            executeWorkerCommand(clientSocket, choice);
        }
        else if (userRole == "hr") {
            showHRMenu();
            int choice = getUserChoice();
            if (choice == 3) break; // �����
            executeHRCommand(clientSocket, choice);
        }
        else {
            std::cerr << "����������� ����. ���������� ������.\n";
            break;
        }
    }
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    // ������������� Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "������ ������������� Winsock.\n";
        return 1;
    }

    try {
        // �������� ������������ �������
        auto config = loadClientConfig("client/config.txt");
        std::string serverIp = config["SERVER_IP"];
        int serverPort = std::stoi(config["SERVER_PORT"]);

        // ����������� � �������
        SOCKET clientSocket;
        connectToServer(clientSocket, serverIp, serverPort);

        // �����������
        login(clientSocket);

        // �������� ���� ������ �������
        mainClientLoop(clientSocket);

        closesocket(clientSocket);
    }
    catch (const std::exception& e) {
        std::cerr << "������ �������: " << e.what() << "\n";
    }

    WSACleanup();
    return 0;
}
