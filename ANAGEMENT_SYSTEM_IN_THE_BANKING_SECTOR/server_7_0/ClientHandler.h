// ClientHandler.h
#pragma once
#include <string>
#include <vector>
#include "RequestHandler.h"
#include <winsock2.h>

class ClientHandler {
private:
    SOCKET clientSocket;
    RequestHandler& requestHandler; // ���������� ������
    std::vector<std::string> activeUsers;
    std::string username;
    std::string role;
public:
    ClientHandler(SOCKET clientSocket, RequestHandler& handler); // ���������� �����������
    void handle();
};
