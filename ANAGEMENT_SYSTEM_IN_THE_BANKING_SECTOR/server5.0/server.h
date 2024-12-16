#pragma once
#ifndef SERVER_H
#define SERVER_H

#include <winsock2.h>
#include <string>
#include <iostream>

// Необходимо подключить библиотеку Winsock для работы с сокетами
#pragma comment(lib, "ws2_32.lib")

class Server {
private:
    WSADATA wsaData;          // Структура для инициализации Winsock
    SOCKET serverSocket;      // Сокет сервера
    sockaddr_in serverAddr;   // Адрес сервера
    int port;                 // Порт для подключения
    bool running;             // Флаг работы сервера

    void handleClient(SOCKET clientSocket); // Обработка подключения клиента

public:
    Server(int port);         // Конструктор с указанием порта
    ~Server();                // Деструктор для освобождения ресурсов

    void start();             // Запуск сервера
    void stop();              // Остановка сервера
};

#endif // SERVER_H
