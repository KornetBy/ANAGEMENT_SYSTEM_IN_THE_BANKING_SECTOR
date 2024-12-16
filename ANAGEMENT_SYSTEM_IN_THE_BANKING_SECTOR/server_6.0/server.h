#pragma once
#ifndef SERVER_H
#define SERVER_H

#include <winsock2.h>
#include <string>
#include <iostream>

// ���������� ���������� ���������� Winsock
#pragma comment(lib, "ws2_32.lib")

class Server {
private:
    WSADATA wsaData;
    SOCKET serverSocket;
    sockaddr_in serverAddr;
    int port;
    bool running;

    void handleClient(SOCKET clientSocket); // ��������� �������

public:
    Server(int port);
    ~Server();

    void start(); // ������ �������
    void stop();  // ��������� �������
};

#endif // SERVER_H
