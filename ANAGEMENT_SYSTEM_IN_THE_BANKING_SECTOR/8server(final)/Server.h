#pragma once
#include "RequestHandler.h"
#include <string>
#include <winsock2.h>
#include <thread>
#include <atomic>

class Server {
private:
    std::string host;
    int port;
    SOCKET listening;
    RequestHandler handler;
    std::atomic<bool> running;

    void handleClient(SOCKET clientSocket, long connID);

public:
    Server(const std::string& host, int port,
        const std::string& logFilename,
        const std::string& userFilename,
        const std::string& employeeFilename,
        const std::string& positionFilename,
        const std::string& courseFilename,
        const std::string& compensationFilename,
        const std::string& salaryFilename,
        const std::string& scheduleFilename,
        const std::string& responsibilityFilename,
        const std::string& departmentFilename,
        const std::string& evaluationFilename,
        const std::string& historyFilename);
    bool start();
    void run();
    void stop();
    RequestHandler& getHandler() { return handler; }
};
