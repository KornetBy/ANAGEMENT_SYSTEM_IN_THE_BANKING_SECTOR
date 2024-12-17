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
                std::cout << "Клиент отключился.\n";
                break;
            }

            std::string request(buffer, bytesReceived);
            std::string response;

            std::cout << "Получен запрос: " << request << "\n";

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

                    std::cout << "Попытка аутентификации пользователя: " << username << "\n";

                    User userAuth = requestHandler.getUserManager().getUser(username);
                    if (userAuth.username.empty()) {
                        response = "ERROR|Пользователь не найден.\n";
                        std::cout << "Пользователь не найден: " << username << "\n";
                    }
                    else if (userAuth.password != pwd) {
                        response = "ERROR|Неверный пароль.\n";
                        std::cout << "Неверный пароль для пользователя: " << username << "\n";
                    }
                    else if (userAuth.status != "active") {
                        response = "ERROR|Пользователь заблокирован.\n";
                        std::cout << "Пользователь заблокирован: " << username << "\n";
                    }
                    else {
                        role = userAuth.role;
                        authenticated = true;
                        response = "SUCCESS|" + role + "\n";
                        std::cout << "Пользователь аутентифицирован: " << username << " с ролью " << role << "\n";
                    }
                    send(clientSocket, response.c_str(), response.size(), 0);
                    continue;
                }
                else {
                    response = "ERROR|Сначала выполните аутентификацию.\n";
                    send(clientSocket, response.c_str(), response.size(), 0);
                    std::cout << "Попытка выполнить команду без аутентификации.\n";
                    continue;
                }
            }

            // Обработка остальных команд
            response = requestHandler.processRequest(request, username, role);
            std::cout << "Отправка ответа: " << response << "\n";
            response += "\n";
            send(clientSocket, response.c_str(), response.size(), 0);
        }

        closesocket(clientSocket);
    }
    catch (const std::exception& e) {
        std::cerr << "Исключение в handle: " << e.what() << "\n";
        closesocket(clientSocket);
    }
}



