// Client.h
#pragma once
#include <string>

class Client {
private:
    bool connected;
    int sock;
    std::string host;
    int port;
public:
    Client(const std::string& host, int port);
    bool connectToServer();
    void sendMessage(const std::string& msg);
    std::string receiveMessage();
    void disconnect();
};
