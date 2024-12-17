// Server/Server.h
#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>
#include <unordered_set>
#include "RequestHandler.h"

// �� �������� �������� ws2_32.lib � ��������� ��������
#pragma comment(lib, "Ws2_32.lib")

class Server {
private:
    SOCKET listenSocket;
    int port;
    std::atomic<bool> running;
    std::vector<std::thread> clientThreads;
    std::mutex clientsMtx;
    std::unordered_set<std::string> activeUsers; // ��� ������������ �������� �������������

    void handleClient(SOCKET clientSocket);
public:
    Server(int portNumber);
    ~Server();
    bool start();
    void stop();
    std::vector<std::string> getActiveUsers();
};
