#include "server.h"
#include "user_manager.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <map>
#include <vector>

#pragma comment(lib, "ws2_32.lib")

// �������� ������������ �� �����
std::map<std::string, std::string> loadConfig(const std::string& configFile) {
    std::map<std::string, std::string> config;
    std::ifstream file(configFile);
    if (!file.is_open()) {
        throw std::runtime_error("������: �� ������� ������� ���� ������������: " + configFile);
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

// ����������� �������� � �������
void logRequestResponse(const std::string& request, const std::string& response) {
    std::ofstream logFile("logs/audit.log", std::ios::app);
    if (logFile.is_open()) {
        logFile << "������: " << request << "\n�����: " << response << "\n\n";
    }
}

// ��������� �������
void handleClient(SOCKET clientSocket, UserManager& userManager) {
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    // ������ ������� �� �������
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead <= 0) {
        std::cerr << "������ ������ ������ �� �������." << std::endl;
        return;
    }

    std::string request(buffer);
    std::istringstream iss(request);
    std::string command;
    iss >> command;

    std::ostringstream response;

    try {
        if (command == "ADD_USER") {
            User user;
            iss >> user.id >> user.username >> user.password >> user.role;
            userManager.addUser(user);
            response << "������������ ������� ��������.";
        }
        else if (command == "REMOVE_USER") {
            int userId;
            iss >> userId;
            if (userManager.removeUser(userId)) {
                response << "������������ ������� �����.";
            }
            else {
                response << "������: ������������ �� ������.";
            }
        }
        else if (command == "UPDATE_USER") {
            int userId;
            std::string newPassword, newRole;
            iss >> userId >> newPassword >> newRole;
            if (userManager.updateUser(userId, newPassword, newRole)) {
                response << "������ ������������ ������� ���������.";
            }
            else {
                response << "������: ������������ �� ������.";
            }
        }
        else if (command == "LOGIN") {
            std::string username, password;
            iss >> username >> password;

            std::cout << "������ �����������: " << username << ", ������: " << password << std::endl;

            User* user = userManager.getUserByUsername(username);
            if (user) {
                if (user->password == password) {
                    response << "SUCCESS ����: " << user->role;
                }
                else {
                    response << "ERROR �������� ������.";
                }
            }
            else {
                response << "ERROR ������������ �� ������.";
            }
        }
        else {
            response << "������: ����������� �������.";
        }
    }
    catch (const std::exception& e) {
        response << "������: " << e.what();
    }

    // ����������� � �������� ������
    logRequestResponse(request, response.str());
    send(clientSocket, response.str().c_str(), response.str().size(), 0);
    closesocket(clientSocket);
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "������ ������������� Winsock." << std::endl;
        return 1;
    }

    try {
        auto config = loadConfig("server/config.txt");
        int serverPort = std::stoi(config["SERVER_PORT"]);

        UserManager userManager(config["USERS_FILE"]);
        userManager.loadUsers();

        SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket == INVALID_SOCKET) {
            throw std::runtime_error("������ �������� ������.");
        }

        sockaddr_in serverAddr;
        memset(&serverAddr, 0, sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(serverPort);

        if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            throw std::runtime_error("������ �������� ������.");
        }

        if (listen(serverSocket, 5) == SOCKET_ERROR) {
            throw std::runtime_error("������ ��� ������������� ������.");
        }

        std::cout << "������ ������� �� ����� " << serverPort << std::endl;

        while (true) {
            sockaddr_in clientAddr;
            int clientLen = sizeof(clientAddr);
            SOCKET clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
            if (clientSocket == INVALID_SOCKET) {
                std::cerr << "������ ��� ����������� �������." << std::endl;
                continue;
            }

            std::cout << "����� ������ ���������." << std::endl;
            handleClient(clientSocket, userManager);
        }

        closesocket(serverSocket);
        WSACleanup();
    }
    catch (const std::exception& e) {
        std::cerr << "������ �������: " << e.what() << std::endl;
    }

    return 0;
}
