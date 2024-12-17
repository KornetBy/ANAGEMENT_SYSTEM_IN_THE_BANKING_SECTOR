#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include <unordered_set>
#include "RequestHandler.h"
#include "ClientHandler.h"
#include <winsock2.h>

// Не забудьте добавить линковку с Ws2_32.lib в проект

class Server {
public:
    Server(int port);
    void start();
    void stop();
private:
    int port;
    SOCKET listenSocket;
    bool running;
    std::vector<std::thread> clientThreads;
    std::mutex clientsMtx;
    std::unordered_set<std::string> activeUsers;
    RequestHandler requestHandler;
    void handleClient(SOCKET clientSocket);
};

#endif
