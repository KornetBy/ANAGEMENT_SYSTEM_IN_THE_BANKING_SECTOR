#include "Client.h"

// Конструктор
Client::Client(const std::string& ip, int port) : serverIP(ip), port(port) {
    // Инициализация Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Ошибка инициализации Winsock." << std::endl;
        exit(1);
    }

    // Создание сокета
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Ошибка создания сокета." << std::endl;
        WSACleanup();
        exit(1);
    }

    // Настройка адреса сервера
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(serverIP.c_str());
    serverAddr.sin_port = htons(port);
}

// Деструктор
Client::~Client() {
    closeConnection();
    WSACleanup();
}

// Подключение к серверу
bool Client::connectToServer() {
    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Ошибка подключения к серверу." << std::endl;
        closesocket(clientSocket);
        return false;
    }
    std::cout << "Подключение к серверу установлено." << std::endl;

    // Приём приветственного сообщения от сервера
    char buffer[1024] = { 0 };
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesRead > 0) {
        buffer[bytesRead] = '\0';
        std::cout << "Сервер: " << buffer << std::endl;
    }

    return true;
}

// Отправка сообщений серверу
void Client::sendMessage() {
    std::string userMessage;
    char buffer[1024] = { 0 };

    std::cout << "Введите сообщение для сервера (или 'exit' для завершения):" << std::endl;

    while (true) {
        std::cout << "Вы: ";
        std::getline(std::cin, userMessage);

        // Выход из цикла по команде "exit"
        if (userMessage == "exit") {
            std::cout << "Завершение соединения с сервером." << std::endl;
            break;
        }

        // Отправка сообщения на сервер
        send(clientSocket, userMessage.c_str(), userMessage.size(), 0);

        // Получение ответа от сервера
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead > 0) {
            buffer[bytesRead] = '\0';
            std::cout << "Сервер: " << buffer << std::endl;
        }
        else {
            std::cerr << "Соединение с сервером потеряно." << std::endl;
            break;
        }
    }
}

// Закрытие соединения
void Client::closeConnection() {
    closesocket(clientSocket);
    std::cout << "Соединение закрыто." << std::endl;
}
