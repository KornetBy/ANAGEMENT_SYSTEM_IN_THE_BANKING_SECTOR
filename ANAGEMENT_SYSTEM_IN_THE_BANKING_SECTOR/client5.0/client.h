#pragma once
#ifndef CLIENT_H
#define CLIENT_H

#include <winsock2.h>
#include <string>
#include <iostream>

// ���������� ���������� ���������� Winsock ��� ������ � ��������
#pragma comment(lib, "ws2_32.lib")

class Client {
private:
    WSADATA wsaData;           // ��������� ��� ������������� Winsock
    SOCKET clientSocket;       // ����� �������
    sockaddr_in serverAddr;    // ����� �������
    std::string serverIP;      // IP-����� �������
    int port;                  // ���� �������

public:
    Client(const std::string& ip, int port);  // ����������� � IP � ������
    ~Client();                               // ���������� ��� ������������ ��������

    bool connectToServer();      // ����������� � �������
    void sendMessage();          // �������� ��������� �������
    void closeConnection();      // �������� ����������
};

#endif // CLIENT_H
