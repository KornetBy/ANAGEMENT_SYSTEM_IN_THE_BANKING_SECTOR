// Client.cpp
#include "Client.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

Client::Client(const std::string& host, int port) : connected(false), sock(INVALID_SOCKET), host(host), port(port) {}

bool Client::connectToServer() {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << "\n";
        return false;
    }

    addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    std::string service = std::to_string(port);
    addrinfo* addr = nullptr;
    result = getaddrinfo(host.c_str(), service.c_str(), &hints, &addr);
    if (result != 0) {
        std::cerr << "getaddrinfo failed: " << result << "\n";
        WSACleanup();
        return false;
    }

    sock = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
    if (sock == INVALID_SOCKET) {
        std::cerr << "socket failed: " << WSAGetLastError() << "\n";
        freeaddrinfo(addr);
        WSACleanup();
        return false;
    }

    result = ::connect(sock, addr->ai_addr, (int)addr->ai_addrlen);
    freeaddrinfo(addr);
    if (result == SOCKET_ERROR) {
        std::cerr << "connect failed: " << WSAGetLastError() << "\n";
        closesocket(sock);
        WSACleanup();
        return false;
    }

    connected = true;
    return true;
}

void Client::sendMessage(const std::string& msg) {
    if (!connected) return;
    int sendResult = send(sock, msg.c_str(), (int)msg.size(), 0);
    if (sendResult == SOCKET_ERROR) {
        std::cerr << "send failed: " << WSAGetLastError() << "\n";
        disconnect();
    }
}

std::string Client::receiveMessage() {
    if (!connected) return "ERROR|Нет соединения";
    char buf[4096];
    ZeroMemory(buf, 4096);
    int bytesReceived = recv(sock, buf, 4096, 0);
    if (bytesReceived > 0) {
        return std::string(buf, 0, bytesReceived);
    }
    else if (bytesReceived == 0) {
        disconnect();
        return "ERROR|Соединение закрыто сервером";
    }
    else {
        std::cerr << "recv failed: " << WSAGetLastError() << "\n";
        disconnect();
        return "ERROR|Ошибка получения данных";
    }
}

void Client::disconnect() {
    if (connected) {
        closesocket(sock);
        sock = INVALID_SOCKET;
        connected = false;
        WSACleanup();
    }
}
