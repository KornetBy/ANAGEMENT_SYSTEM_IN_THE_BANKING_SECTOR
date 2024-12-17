// server.cpp
#include "RequestHandler.h"
#include "ClientHandler.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <thread>
#include <vector>
#include <sstream>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "27015"
#define BUFFER_SIZE 4096

void handleClient(SOCKET clientSocket, RequestHandler* handler, std::string clientAddress) {
    ClientHandler clientHandler(clientSocket, *handler);
    clientHandler.handle();
}



int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    WSADATA wsaData;
    int iResult;

    // Инициализация Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cerr << "WSAStartup failed: " << iResult << "\n";
        return 1;
    }

    struct addrinfo* result = NULL, hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;        // IPv4
    hints.ai_socktype = SOCK_STREAM;  // TCP
    hints.ai_protocol = IPPROTO_TCP;  // TCP
    hints.ai_flags = AI_PASSIVE;      // Для bind

    // Разрешение адреса и порта
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        std::cerr << "getaddrinfo failed: " << iResult << "\n";
        WSACleanup();
        return 1;
    }

    // Создание сокета
    SOCKET ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        std::cerr << "socket failed: " << WSAGetLastError() << "\n";
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Привязка сокета
    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        std::cerr << "bind failed: " << WSAGetLastError() << "\n";
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    // Прослушивание
    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        std::cerr << "listen failed: " << WSAGetLastError() << "\n";
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Сервер запущен. Ожидание подключений...\n";

    // Инициализация RequestHandler с необходимыми параметрами
    try {
        RequestHandler handler(
            "C:\\proj\\server\\server_logs.txt",
            "C:\\proj\\server\\users.txt",
            "C:\\proj\\server\\employees.txt",
            "C:\\proj\\server\\positions.txt",
            "C:\\proj\\server\\courses.txt",
            "C:\\proj\\server\\compensation_requests.txt",
            "C:\\proj\\server\\salaries.txt",
            "C:\\proj\\server\\schedules.txt",
            "C:\\proj\\server\\job_responsibilities.txt",
            "C:\\proj\\server\\departments.txt",
            "C:\\proj\\server\\performance_evaluations.txt",
            "C:\\proj\\server\\history.txt");

        std::cout << "RequestHandler успешно инициализирован!\n";

        // Запуск основного сервера
        while (true) {
            SOCKET ClientSocket = accept(ListenSocket, NULL, NULL);
            if (ClientSocket == INVALID_SOCKET) {
                std::cerr << "accept failed: " << WSAGetLastError() << "\n";
                closesocket(ListenSocket);
                WSACleanup();
                return 1;
            }

            // Получение адреса клиента
            sockaddr_in clientAddr;
            int clientAddrSize = sizeof(clientAddr);
            getpeername(ClientSocket, (sockaddr*)&clientAddr, &clientAddrSize);
            char clientIP[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(clientAddr.sin_addr), clientIP, INET_ADDRSTRLEN);
            std::string clientAddress(clientIP);

            std::cout << "Подключен клиент: " << clientAddress << "\n";

            std::thread t(handleClient, ClientSocket, &handler, clientAddress);
            t.detach();
        }

    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка при инициализации RequestHandler: " << e.what() << "\n";
        return 1;
    }
    catch (...) {
        std::cerr << "Произошло неизвестное исключение при инициализации RequestHandler!\n";
        return 1;
    }


    // Закрытие слушающего сокета
    closesocket(ListenSocket);
    WSACleanup();

    return 0;
}
