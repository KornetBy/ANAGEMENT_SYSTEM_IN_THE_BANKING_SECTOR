#include "Server.h"

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    const int port = 5000; // Порт для подключения клиентов
    Server server(port);

    try {
        std::cout << "Сервер запускается на порту " << port << "..." << std::endl;
        server.start(); // Запуск сервера
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка сервера: " << e.what() << std::endl;
    }

    return 0;
}
