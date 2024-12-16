#pragma once
#ifndef CLIENT_H
#define CLIENT_H

#include <winsock2.h>
#include <string>
#include <iostream>

// Необходимо подключить библиотеку Winsock для работы с сокетами
#pragma comment(lib, "ws2_32.lib")

class Client {
private:
    WSADATA wsaData;           // Структура для инициализации Winsock
    SOCKET clientSocket;       // Сокет клиента
    sockaddr_in serverAddr;    // Адрес сервера
    std::string serverIP;      // IP-адрес сервера
    int port;                  // Порт сервера

public:
    Client(const std::string& ip, int port);  // Конструктор с IP и портом
    ~Client();                               // Деструктор для освобождения ресурсов

    bool connectToServer();      // Подключение к серверу
    void sendMessage();          // Отправка сообщений серверу
    void closeConnection();      // Закрытие соединения
};

#endif // CLIENT_H
