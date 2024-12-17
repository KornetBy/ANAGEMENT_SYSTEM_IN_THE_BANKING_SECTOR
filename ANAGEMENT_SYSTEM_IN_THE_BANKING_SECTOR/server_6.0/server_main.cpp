// Server/Server_main.cpp
#include "Server.h"
#include <iostream>
#include <string>
#include <csignal>

// Глобальный указатель на сервер для обработки сигналов
Server* globalServer = nullptr;

// Функция для обработки сигнала завершения (например, Ctrl+C)
void signalHandler(int signum) {
    if (globalServer != nullptr) {
        std::cout << "\nПолучен сигнал завершения. Останавливаем сервер..." << std::endl;
        globalServer->stop();
    }
    exit(signum);
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    int port = 5000; // Порт сервера
    Server server(port);
    globalServer = &server;

    // Установка обработчика сигналов
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);

    if (!server.start()) {
        std::cerr << "Не удалось запустить сервер." << std::endl;
        return 1;
    }

    return 0;
}
