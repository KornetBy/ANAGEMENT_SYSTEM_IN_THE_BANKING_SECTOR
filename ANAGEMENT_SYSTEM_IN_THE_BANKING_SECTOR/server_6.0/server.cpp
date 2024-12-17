// Server/Server.cpp
#include "Server.h"
#include <iostream>
#include <sstream>
#include <algorithm>

Server::Server(int portNumber) : listenSocket(INVALID_SOCKET), port(portNumber), running(false) {
    // Инициализация Winsock
    WSADATA wsaData;
    int wsResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wsResult != 0) {
        std::cerr << "WSAStartup failed: " << wsResult << std::endl;
    }
}

Server::~Server() {
    stop();
    WSACleanup();
}

// Метод для запуска сервера
bool Server::start() {
    listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSocket == INVALID_SOCKET) {
        std::cerr << "Ошибка создания сокета: " << WSAGetLastError() << std::endl;
        return false;
    }

    // Настройка адреса сервера
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Принимает соединения на все IP-адреса

    // Привязка сокета
    if (bind(listenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Ошибка привязки сокета: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        return false;
    }

    // Начало прослушивания
    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Ошибка прослушивания: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        return false;
    }

    running = true;
    std::cout << "Сервер запущен и слушает порт " << port << "." << std::endl;

    // Основной цикл принятия клиентов
    while (running) {
        sockaddr_in clientAddr;
        int clientSize = sizeof(clientAddr);
        SOCKET clientSocket = accept(listenSocket, (sockaddr*)&clientAddr, &clientSize);
        if (clientSocket == INVALID_SOCKET) {
            if (running) { // Если сервер ещё работает
                std::cerr << "Ошибка принятия соединения: " << WSAGetLastError() << std::endl;
            }
            continue;
        }

        std::cout << "Принято новое соединение." << std::endl;
        // Запуск нового потока для обработки клиента
        clientThreads.emplace_back(&Server::handleClient, this, clientSocket);
    }

    return true;
}

// Метод для остановки сервера
void Server::stop() {
    if (running) {
        running = false;
        closesocket(listenSocket);
        std::cout << "Сервер остановлен." << std::endl;

        // Ожидание завершения всех потоков
        for (auto& t : clientThreads) {
            if (t.joinable()) {
                t.join();
            }
        }
    }
}

// Метод для получения списка активных пользователей
std::vector<std::string> Server::getActiveUsers() {
    std::lock_guard<std::mutex> lock(clientsMtx);
    std::vector<std::string> users(activeUsers.begin(), activeUsers.end());
    return users;
}

// Метод для обработки клиента
void Server::handleClient(SOCKET clientSocket) {
    RequestHandler requestHandler;
    char buffer[1024] = { 0 };
    int bytesRead;

    std::string username = ""; // Хранит имя пользователя после авторизации
    std::string role = "";     // Хранит роль пользователя после авторизации

    // Отправка приветственного сообщения
    std::string welcome = "Добро пожаловать на сервер!\n";
    send(clientSocket, welcome.c_str(), welcome.size(), 0);

    while (true) {
        bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0); // Уменьшено на 1 для безопасной записи '\0'
        if (bytesRead <= 0) {
            if (bytesRead == 0) {
                std::cout << "Клиент отключился." << std::endl;
            }
            else {
                std::cerr << "Ошибка при приёме данных от клиента: " << WSAGetLastError() << std::endl;
            }
            break;
        }

        buffer[bytesRead] = '\0';
        std::string request(buffer);
        std::cout << "Получен запрос: " << request << std::endl;

        std::string response;

        // Если запрос на авторизацию
        if (request.find("AUTH") == 0) {
            response = requestHandler.processRequest(request, "", "");

            // Проверка на успешную авторизацию
            if (response.find("SUCCESS") == 0) {
                size_t pos = response.find('|');
                if (pos != std::string::npos && pos + 1 < response.size()) {
                    role = response.substr(pos + 1);
                    std::istringstream iss(request);
                    std::string command, user, pass;
                    std::getline(iss, command, '|'); // "AUTH"
                    std::getline(iss, user, '|');    // Логин
                    username = user;

                    // Добавление пользователя в активные соединения
                    {
                        std::lock_guard<std::mutex> lock(clientsMtx);
                        activeUsers.insert(username);
                    }

                    std::cout << "Пользователь " << username << " авторизован с ролью " << role << "." << std::endl;
                }
                else {
                    std::cerr << "Некорректный формат ответа при аутентификации." << std::endl;
                    response = "ERROR|Некорректный формат ответа";
                }
            }
        }
        else {
            // Передаём реальные username и role для других запросов
            if (username.empty()) {
                response = "ERROR|Вы не авторизованы. Пожалуйста, выполните команду AUTH|логин|пароль.";
            }
            else {
                response = requestHandler.processRequest(request, username, role);
            }
        }

        std::cout << "Отправляется ответ: " << response << std::endl;
        // Отправка ответа клиенту
        if (send(clientSocket, response.c_str(), response.size(), 0) == SOCKET_ERROR) {
            std::cerr << "Ошибка при отправке данных клиенту: " << WSAGetLastError() << std::endl;
            break;
        }

        // Проверка, требует ли запрос отключения соединения
        if (request.find("EXIT") == 0) {
            std::cout << "Клиент запросил отключение." << std::endl;
            break;
        }
    }

    // Удаление пользователя из активных соединений
    if (!username.empty()) {
        std::lock_guard<std::mutex> lock(clientsMtx);
        activeUsers.erase(username);
    }

    closesocket(clientSocket);
    std::cout << "Соединение с клиентом закрыто." << std::endl;
}
