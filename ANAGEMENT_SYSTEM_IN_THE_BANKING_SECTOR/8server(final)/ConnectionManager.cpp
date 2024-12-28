#include "ConnectionManager.h"
#include <sstream>

ConnectionManager::ConnectionManager() : nextID(1) {}

long ConnectionManager::addConnection(const std::string& ip, const std::string& time, SOCKET sock) {
    std::lock_guard<std::mutex> lock(mtx);
    long newID = nextID.fetch_add(1);
    ActiveConnection c{ newID, ip, time, sock };
    connections.push_back(c);
    return newID;
}

void ConnectionManager::removeConnection(long id) {
    std::lock_guard<std::mutex> lock(mtx);
    for (size_t i = 0; i < connections.size(); i++) {
        if (connections[i].id == id) {
            connections.erase(connections.begin() + i);
            break;
        }
    }
}

void ConnectionManager::broadcastMessage(const std::string& msg) {
    std::lock_guard<std::mutex> lock(mtx);
    std::string fullMsg = "BROADCAST|" + msg;
    for (auto& c : connections) {
        send(c.socket, fullMsg.c_str(), (int)fullMsg.size(), 0);
    }
}
