// Client.cpp
#include "Client.h"
#include <iostream>

Client::Client() : sock(INVALID_SOCKET), connected(false) {
    // Инициализация Winsock
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cerr << "WSAStartup failed: " << iResult << "\n";
    }
}

Client::~Client() {
    disconnect();
    WSACleanup();
}

bool Client::connectToServer(const std::string& ip, int port) {
    std::lock_guard<std::mutex> lock(mtx);
    if (connected) return true;

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "socket failed: " << WSAGetLastError() << "\n";
        return false;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr);

    int iResult = connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (iResult == SOCKET_ERROR) {
        std::cerr << "connect failed: " << WSAGetLastError() << "\n";
        closesocket(sock);
        sock = INVALID_SOCKET;
        return false;
    }

    connected = true;
    return true;
}

bool Client::sendMessage(const std::string& message) {
    std::lock_guard<std::mutex> lock(mtx);
    if (!connected) return false;

    int iResult = send(sock, message.c_str(), message.length(), 0);
    if (iResult == SOCKET_ERROR) {
        std::cerr << "send failed: " << WSAGetLastError() << "\n";
        disconnect();
        return false;
    }
    return true;
}

std::string Client::receiveMessage() {
    std::lock_guard<std::mutex> lock(mtx);
    if (!connected) return "";

    char recvbuf[4096];
    int iResult = recv(sock, recvbuf, 4096, 0);
    if (iResult > 0) {
        return std::string(recvbuf, iResult);
    }
    else if (iResult == 0) {
        std::cout << "Соединение закрыто сервером.\n";
        disconnect();
    }
    else {
        std::cerr << "recv failed: " << WSAGetLastError() << "\n";
        disconnect();
    }
    return "";
}

void Client::disconnect() {
    std::lock_guard<std::mutex> lock(mtx);
    if (connected) {
        closesocket(sock);
        sock = INVALID_SOCKET;
        connected = false;
    }
}
