// Client.h
#pragma once
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <mutex>

class Client {
private:
    SOCKET sock;
    sockaddr_in serverAddr;
    bool connected;
    std::mutex mtx;
public:
    Client();
    ~Client();
    bool connectToServer(const std::string& ip, int port);
    bool sendMessage(const std::string& message);
    std::string receiveMessage();
    void disconnect();
};
