// ClientHandler.cpp
#include "ClientHandler.h"
#include <cstring>
#include <sstream>

ClientHandler::ClientHandler(SOCKET clientSocket, RequestHandler& handler)
    : clientSocket(clientSocket), requestHandler(handler), activeUsers(), username(""), role("") {}

void ClientHandler::handle() {
    char buffer[1024];
    int bytesReceived;

    bool authenticated = false;

    while (true) {
        std::memset(buffer, 0, sizeof(buffer));
        bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived == SOCKET_ERROR || bytesReceived == 0) {
            break;
        }

        std::string request(buffer, bytesReceived);
        std::string response;

        // Обработка аутентификации
        if (!authenticated) {
            std::istringstream iss(request);
            std::string command;
            getline(iss, command, '|');

            if (command == "AUTH") {
                std::string pwd;
                // Предполагается, что формат команды AUTH|username|password
                getline(iss, username, '|');
                getline(iss, pwd, '|');

                User userAuth = requestHandler.getUserManager().getUser(username);
                if (userAuth.username.empty()) {
                    response = "ERROR|Пользователь не найден.\n";
                }
                else if (userAuth.password != pwd) {
                    response = "ERROR|Неверный пароль.\n";
                }
                else if (userAuth.status != "active") {
                    response = "ERROR|Пользователь заблокирован.\n";
                }
                else {
                    role = userAuth.role;
                    authenticated = true;
                    response = "SUCCESS|" + role + "\n";
                }
                send(clientSocket, response.c_str(), response.size(), 0);
                continue;
            }
            else {
                response = "ERROR|Сначала выполните аутентификацию.\n";
                send(clientSocket, response.c_str(), response.size(), 0);
                continue;
            }
        }

        // Обработка остальных команд
        response = requestHandler.processRequest(request, username, role);
        response += "\n";
        send(clientSocket, response.c_str(), response.size(), 0);
    }

    closesocket(clientSocket);
}

