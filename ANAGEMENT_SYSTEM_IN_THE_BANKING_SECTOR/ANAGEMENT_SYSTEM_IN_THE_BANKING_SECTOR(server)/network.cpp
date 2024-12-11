#include "network.h"
#include "server.h"
#include <iostream>
#include <thread>

void clientHandler(SOCKET clientSocket) {
    int authenticatedUserId = -1;
    std::string userRole = "";
    char buffer[4096];
    int bytes;
    while ((bytes = recv(clientSocket, buffer, 4096, 0)) > 0) {
        buffer[bytes] = '\0';
        std::string req(buffer);
        std::string resp = handleRequest(req, authenticatedUserId, userRole);
        send(clientSocket, resp.c_str(), (int)resp.size(), 0);
    }
    closesocket(clientSocket);
}

SOCKET createServerSocket(int port) {
    WSADATA wsa; WSAStartup(MAKEWORD(2, 2), &wsa);
    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serv; serv.sin_family = AF_INET; serv.sin_port = htons(port); serv.sin_addr.s_addr = INADDR_ANY;
    bind(s, (sockaddr*)&serv, sizeof(serv));
    listen(s, 3);
    return s;
}
