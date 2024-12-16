#include "network.h"
#include <ws2tcpip.h>
#include <iostream>

SOCKET connectToServer(const std::string& ip, int port) {
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cerr << "WSAStartup ошибка.\n";
        return INVALID_SOCKET;
    }

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Ошибка создания сокета.\n";
        WSACleanup();
        return INVALID_SOCKET;
    }

    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    InetPtonA(AF_INET, ip.c_str(), &server.sin_addr);

    if (connect(sock, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        std::cerr << "Ошибка подключения к серверу.\n";
        closesocket(sock);
        WSACleanup();
        return INVALID_SOCKET;
    }

    return sock;
}
