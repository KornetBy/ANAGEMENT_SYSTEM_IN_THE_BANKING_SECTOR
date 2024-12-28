#include "Server.h"
#include <ws2tcpip.h>
#include <iostream>
#include "utils.h"

#pragma comment(lib, "ws2_32.lib")

Server::Server(const std::string& host, int port,
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
    const std::string& historyFilename)
    : host(host), port(port),
    handler(logFilename, userFilename, employeeFilename, positionFilename,
        courseFilename, compensationFilename, salaryFilename, scheduleFilename,
        responsibilityFilename, departmentFilename, evaluationFilename, historyFilename),
    running(true) {
    listening = INVALID_SOCKET;
}

bool Server::start() {
    WSADATA wsaData;
    int wsResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wsResult != 0) {
        std::cerr << "WSAStartup failed: " << wsResult << "\n";
        return false;
    }

    addrinfo hints, * addr = nullptr;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    std::string service = std::to_string(port);
    int result = getaddrinfo(NULL, service.c_str(), &hints, &addr);
    if (result != 0) {
        std::cerr << "getaddrinfo failed: " << result << "\n";
        WSACleanup();
        return false;
    }

    listening = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
    if (listening == INVALID_SOCKET) {
        std::cerr << "socket failed: " << WSAGetLastError() << "\n";
        freeaddrinfo(addr);
        WSACleanup();
        return false;
    }

    result = bind(listening, addr->ai_addr, (int)addr->ai_addrlen);
    if (result == SOCKET_ERROR) {
        std::cerr << "bind failed: " << WSAGetLastError() << "\n";
        freeaddrinfo(addr);
        closesocket(listening);
        WSACleanup();
        return false;
    }

    freeaddrinfo(addr);

    result = listen(listening, SOMAXCONN);
    if (result == SOCKET_ERROR) {
        std::cerr << "listen failed: " << WSAGetLastError() << "\n";
        closesocket(listening);
        WSACleanup();
        return false;
    }

    return true;
}

void Server::run() {
    std::cout << "Сервер запущен на порту " << port << "\n";
    while (running) {
        sockaddr_in clientAddr;
        int clientSize = sizeof(clientAddr);
        SOCKET clientSocket = accept(listening, (sockaddr*)&clientAddr, &clientSize);
        if (clientSocket == INVALID_SOCKET) {
            if (!running) break;
            std::cerr << "accept failed: " << WSAGetLastError() << "\n";
            continue;
        }

        char hostBuf[NI_MAXHOST];
        char serviceBuf[NI_MAXSERV];

        ZeroMemory(hostBuf, NI_MAXHOST);
        ZeroMemory(serviceBuf, NI_MAXSERV);

        std::string clientIP;
        if (getnameinfo((sockaddr*)&clientAddr, sizeof(clientAddr), hostBuf, NI_MAXHOST, serviceBuf, NI_MAXSERV, 0) == 0) {
            clientIP = hostBuf;
        }
        else {
            inet_ntop(AF_INET, &clientAddr.sin_addr, hostBuf, NI_MAXHOST);
            clientIP = hostBuf;
        }

        time_t now = time(nullptr);
        tm lt; // Создаём объект структуры tm
        localtime_s(&lt, &now); // Используем безопасную версию localtime_s
        char timeBuf[64];
        strftime(timeBuf, 64, "%Y-%m-%d %H:%M:%S", &lt); // Передаём адрес структуры lt
        long connID = handler.getConnectionManager().addConnection(clientIP, timeBuf, clientSocket);


        std::thread t(&Server::handleClient, this, clientSocket, connID);
        t.detach();
    }
}

void Server::handleClient(SOCKET clientSocket, long connID) {
    char buf[4096];
    ZeroMemory(buf, 4096);
    int bytesReceived = recv(clientSocket, buf, 4096, 0);
    if (bytesReceived <= 0) {
        std::cerr << "Авторизация не выполнена.\n";
        handler.getConnectionManager().removeConnection(connID);
        closesocket(clientSocket);
        return;
    }
    std::string auth(buf, 0, bytesReceived);
    auto parts = split(auth, '|');
    if (parts.size() < 3) {
        std::string res = "ERROR|Неверный формат авторизации";
        send(clientSocket, res.c_str(), (int)res.size(), 0);
        handler.getConnectionManager().removeConnection(connID);
        closesocket(clientSocket);
        return;
    }
    std::string uname = parts[1];
    std::string pass = parts[2];
    User u = handler.getUserManager().getUser(uname);
    if (u.username.empty() || u.password != pass || u.status != "active") {
        std::string res = "ERROR|Неверный логин или пароль";
        send(clientSocket, res.c_str(), (int)res.size(), 0);
        handler.getConnectionManager().removeConnection(connID);
        closesocket(clientSocket);
        return;
    }
    std::string res = "SUCCESS|" + u.role;
    send(clientSocket, res.c_str(), (int)res.size(), 0);

    while (true) {
        ZeroMemory(buf, 4096);
        bytesReceived = recv(clientSocket, buf, 4096, 0);
        if (bytesReceived <= 0) {
            std::cerr << "Клиент " << uname << " отключился.\n";
            handler.getConnectionManager().removeConnection(connID);
            break;
        }
        std::string req(buf, 0, bytesReceived);
        std::string response = handler.processRequest(req, uname, u.role);
        send(clientSocket, response.c_str(), (int)response.size(), 0);
    }

    closesocket(clientSocket);
}

void Server::stop() {
    running = false;
    closesocket(listening);
    WSACleanup();
}
