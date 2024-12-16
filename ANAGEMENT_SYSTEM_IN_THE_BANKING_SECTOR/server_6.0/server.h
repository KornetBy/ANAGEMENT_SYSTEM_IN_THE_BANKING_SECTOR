#pragma once
#ifndef SERVER_H
#define SERVER_H

#include <winsock2.h>
#include <string>
#include <iostream>

// Необходимо подключить библиотеку Winsock
#pragma comment(lib, "ws2_32.lib")

class Server {
private:
    WSADATA wsaData;
    SOCKET serverSocket;
    sockaddr_in serverAddr;
    int port;
    bool running;

    void handleClient(SOCKET clientSocket); // Обработка клиента

public:
    Server(int port);
    ~Server();

    void start(); // Запуск сервера
    void stop();  // Остановка сервера
};

#endif // SERVER_H
