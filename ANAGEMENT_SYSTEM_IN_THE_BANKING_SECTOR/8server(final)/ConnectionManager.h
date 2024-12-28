#pragma once
#include <string>
#include <vector>
#include <mutex>
#include <atomic>
#include <winsock2.h>

struct ActiveConnection {
    long id;
    std::string ip;
    std::string connectTime;
    SOCKET socket;
};

class ConnectionManager {
private:
    std::vector<ActiveConnection> connections;
    std::mutex mtx;
    std::atomic<long> nextID;
public:
    ConnectionManager();
    long addConnection(const std::string& ip, const std::string& time, SOCKET sock);
    void removeConnection(long id);
    void broadcastMessage(const std::string& msg);
};
