#ifndef CLIENT_H
#define CLIENT_H

#include <winsock2.h>
#include <string>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

class Client {
private:
    WSADATA wsaData;           // Инициализация Winsock
    SOCKET clientSocket;       // Клиентский сокет
    sockaddr_in serverAddr;    // Адрес сервера
    std::string serverIP;      // IP-адрес сервера
    int port;                  // Порт сервера

public:
    Client(const std::string& ip, int port);
    ~Client();
    bool validateInput(const std::string& input);

    bool connectToServer();                // Подключение к серверу
    void sendMessage(const std::string& message);  // Отправка сообщений серверу
    std::string receiveMessage();          // Получение сообщений от сервера
    void closeConnection();                // Закрытие соединения
};

#endif // CLIENT_H
