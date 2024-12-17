// ClientHandler.cpp
#include "ClientHandler.h"
#include <cstring>
#include <sstream>
#include <iostream>

ClientHandler::ClientHandler(SOCKET clientSocket, RequestHandler& handler)
    : clientSocket(clientSocket), requestHandler(handler), activeUsers(), username(""), role("") {}

void ClientHandler::handle() {
    try {
        char buffer[1024];
        int bytesReceived;

        bool authenticated = false;

        while (true) {
            std::memset(buffer, 0, sizeof(buffer));
            bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
            if (bytesReceived == SOCKET_ERROR) {
                std::cerr << "recv failed: " << WSAGetLastError() << "\n";
                break;
            }
            if (bytesReceived == 0) {
                std::cout << "������ ����������.\n";
                break;
            }

            std::string request(buffer, bytesReceived);
            std::string response;

            std::cout << "������� ������: " << request << "\n";

            // ��������� ��������������
            if (!authenticated) {
                std::istringstream iss(request);
                std::string command;
                getline(iss, command, '|');

                if (command == "AUTH") {
                    std::string pwd;
                    // ��������������, ��� ������ ������� AUTH|username|password
                    getline(iss, username, '|');
                    getline(iss, pwd, '|');

                    std::cout << "������� �������������� ������������: " << username << "\n";

                    User userAuth = requestHandler.getUserManager().getUser(username);
                    if (userAuth.username.empty()) {
                        response = "ERROR|������������ �� ������.\n";
                        std::cout << "������������ �� ������: " << username << "\n";
                    }
                    else if (userAuth.password != pwd) {
                        response = "ERROR|�������� ������.\n";
                        std::cout << "�������� ������ ��� ������������: " << username << "\n";
                    }
                    else if (userAuth.status != "active") {
                        response = "ERROR|������������ ������������.\n";
                        std::cout << "������������ ������������: " << username << "\n";
                    }
                    else {
                        role = userAuth.role;
                        authenticated = true;
                        response = "SUCCESS|" + role + "\n";
                        std::cout << "������������ ����������������: " << username << " � ����� " << role << "\n";
                    }
                    send(clientSocket, response.c_str(), response.size(), 0);
                    continue;
                }
                else {
                    response = "ERROR|������� ��������� ��������������.\n";
                    send(clientSocket, response.c_str(), response.size(), 0);
                    std::cout << "������� ��������� ������� ��� ��������������.\n";
                    continue;
                }
            }

            // ��������� ��������� ������
            response = requestHandler.processRequest(request, username, role);
            std::cout << "�������� ������: " << response << "\n";
            response += "\n";
            send(clientSocket, response.c_str(), response.size(), 0);
        }

        closesocket(clientSocket);
    }
    catch (const std::exception& e) {
        std::cerr << "���������� � handle: " << e.what() << "\n";
        closesocket(clientSocket);
    }
}



