#include "client_interface.h"
#include "client_operations.h"
#include <winsock2.h>
#include <iostream>
#include <string>
#include <stdexcept>
#include <map>
#include <fstream>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib") // Линковка библиотеки Winsock

std::string userRole; // Хранит роль текущего пользователя

// Загрузка конфигурации клиента
std::map<std::string, std::string> loadClientConfig(const std::string& configFile) {
    std::map<std::string, std::string> config;
    std::ifstream file(configFile);
    if (!file.is_open()) {
        throw std::runtime_error("Ошибка: не удалось открыть файл конфигурации " + configFile);
    }
    std::string line;
    while (std::getline(file, line)) {
        size_t pos = line.find('=');
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            config[key] = value;
        }
    }
    return config;
}

// Установка соединения с сервером
void connectToServer(SOCKET& clientSocket, const std::string& serverIp, int serverPort) {
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        throw std::runtime_error("Ошибка создания сокета.");
    }

    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);

    // Преобразование IP-адреса из std::string в PCWSTR
    WCHAR wideIp[INET_ADDRSTRLEN];
    MultiByteToWideChar(CP_ACP, 0, serverIp.c_str(), -1, wideIp, INET_ADDRSTRLEN);

    // Используем InetPton
    if (InetPton(AF_INET, wideIp, &serverAddr.sin_addr) <= 0) {
        throw std::runtime_error("Ошибка преобразования IP-адреса.");
    }

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        throw std::runtime_error("Не удалось подключиться к серверу.");
    }

    std::cout << "Соединение с сервером установлено.\n";
}

// Авторизация пользователя
void login(SOCKET clientSocket) {
    std::string username, password;
    std::cout << "Введите имя пользователя: ";
    std::cin >> username;
    std::cout << "Введите пароль: ";
    std::cin >> password;

    std::string loginRequest = "LOGIN " + username + " " + password + "\n";
    std::cout << "Отправка запроса: " << loginRequest << std::endl;
    send(clientSocket, loginRequest.c_str(), loginRequest.size(), 0);

    char response[1024] = { 0 };
    int bytesRead = recv(clientSocket, response, sizeof(response), 0);

    if (bytesRead <= 0) {
        throw std::runtime_error("Ошибка: сервер не ответил на запрос авторизации.");
    }

    std::string loginResponse(response);
    if (loginResponse.find("SUCCESS") != std::string::npos) {
        size_t rolePos = loginResponse.find("Роль: ");
        if (rolePos != std::string::npos) {
            userRole = loginResponse.substr(rolePos + 6); // Извлекаем роль
            std::cout << "Авторизация успешна. Ваша роль: " << userRole << "\n";
        }
        else {
            throw std::runtime_error("Ошибка: сервер не вернул роль пользователя.");
        }
    }
    else {
        throw std::runtime_error("Ошибка авторизации: " + loginResponse);
    }
}

// Основной цикл работы клиента
void mainClientLoop(SOCKET clientSocket) {
    while (true) {
        if (userRole == "admin") {
            showAdminMenu();
            int choice = getUserChoice();
            if (choice == 5) break; // Выход
            executeAdminCommand(clientSocket, choice);
        }
        else if (userRole == "client") {
            showClientMenu();
            int choice = getUserChoice();
            if (choice == 4) break; // Выход
            executeClientCommand(clientSocket, choice);
        }
        else if (userRole == "worker") {
            showWorkerMenu();
            int choice = getUserChoice();
            if (choice == 5) break; // Выход
            executeWorkerCommand(clientSocket, choice);
        }
        else if (userRole == "hr") {
            showHRMenu();
            int choice = getUserChoice();
            if (choice == 3) break; // Выход
            executeHRCommand(clientSocket, choice);
        }
        else {
            std::cerr << "Неизвестная роль. Завершение работы.\n";
            break;
        }
    }
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    // Инициализация Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Ошибка инициализации Winsock.\n";
        return 1;
    }

    try {
        // Загрузка конфигурации клиента
        auto config = loadClientConfig("client/config.txt");
        std::string serverIp = config["SERVER_IP"];
        int serverPort = std::stoi(config["SERVER_PORT"]);

        // Подключение к серверу
        SOCKET clientSocket;
        connectToServer(clientSocket, serverIp, serverPort);

        // Авторизация
        login(clientSocket);

        // Основной цикл работы клиента
        mainClientLoop(clientSocket);

        closesocket(clientSocket);
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка клиента: " << e.what() << "\n";
    }

    WSACleanup();
    return 0;
}
