#include "Client.h"
#include <ws2tcpip.h>
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
    if (inet_pton(AF_INET, serverIP.c_str(), &serverAddr.sin_addr) <= 0) {
        std::cerr << "Ошибка конвертации IP-адреса." << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        exit(1);
    }
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
    std::cout << "Успешное подключение к серверу!" << std::endl;
    return true;
}

// Отправка сообщений
void Client::sendMessage(const std::string& message) {
    send(clientSocket, message.c_str(), message.size(), 0);
}

// Получение сообщений
std::string Client::receiveMessage() {
    char buffer[1024] = { 0 };
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead > 0) {
        buffer[bytesRead] = '\0';
        return std::string(buffer);
    }
    return "ERROR|Нет данных от сервера.";
}

// Закрытие соединения
void Client::closeConnection() {
    closesocket(clientSocket);
}
bool validateInput(const std::string& input) {
    if (input.empty()) {
        std::cerr << "Ошибка: введена пустая строка." << std::endl;
        return false;
    }

    // Дополнительная проверка: ввод не должен содержать пробелов или специальных символов
    for (char c : input) {
        if (!std::isalnum(c)) {
            std::cerr << "Ошибка: ввод содержит недопустимые символы." << std::endl;
            return false;
        }
    }

    return true;
}
bool Client::validateInput(const std::string& input) {
    if (input.empty()) {
        std::cerr << "Ошибка: введена пустая строка." << std::endl;
        return false;
    }

    for (char c : input) {
        if (!std::isalnum(c)) { // Проверка, что символы буквенно-цифровые
            std::cerr << "Ошибка: ввод содержит недопустимые символы." << std::endl;
            return false;
        }
    }

    return true;
}