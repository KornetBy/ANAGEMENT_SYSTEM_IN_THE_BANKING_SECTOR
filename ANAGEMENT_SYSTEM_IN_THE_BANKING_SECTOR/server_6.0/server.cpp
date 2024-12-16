#include "Server.h"
#include "RequestHandler.h"

Server::Server(int port) : port(port), running(false) {
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Ошибка инициализации Winsock." << std::endl;
        exit(1);
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Ошибка создания сокета." << std::endl;
        WSACleanup();
        exit(1);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);
}

Server::~Server() {
    stop();
    WSACleanup();
}

void Server::start() {
    running = true;

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Ошибка привязки сокета к порту." << std::endl;
        stop();
        exit(1);
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Ошибка ожидания подключений." << std::endl;
        stop();
        exit(1);
    }

    std::cout << "Сервер запущен на порту " << port << std::endl;

    while (running) {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Ошибка подключения клиента." << std::endl;
            continue;
        }

        std::cout << "Клиент подключён." << std::endl;

        handleClient(clientSocket);
        closesocket(clientSocket);
    }
}

void Server::stop() {
    if (running) {
        running = false;
        closesocket(serverSocket);
        std::cout << "Сервер остановлен." << std::endl;
    }
}

void Server::handleClient(SOCKET clientSocket) {
    RequestHandler requestHandler;
    char buffer[1024] = { 0 };
    int bytesRead;

    std::cout << "Клиент подключён." << std::endl; // Сообщение о подключении клиента

    send(clientSocket, "Добро пожаловать на сервер!\n", 28, 0);

    while ((bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
        buffer[bytesRead] = '\0';
        std::string request(buffer);
        std::cout << "Получен запрос: " << request << std::endl;

        std::string response = requestHandler.processRequest(request, "username", "role");

        // Проверка на успешную авторизацию
        if (request.find("AUTH") == 0 && response.find("SUCCESS") == 0) {
            std::cout << "Пользователь успешно авторизован." << std::endl;
        }

        send(clientSocket, response.c_str(), response.size(), 0);
    }

    std::cout << "Клиент отключён." << std::endl;
}
