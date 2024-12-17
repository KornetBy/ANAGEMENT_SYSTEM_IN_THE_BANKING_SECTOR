// main.cpp
#include "Client.h"
#include "Menu.h"
#include <iostream>
#pragma comment(lib, "Ws2_32.lib")
int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    Client client;
    std::string serverIP = "127.0.0.1"; // IP сервера
    int serverPort = 27015;             // Порт сервера

    if (!client.connectToServer(serverIP, serverPort)) {
        std::cerr << "Не удалось подключиться к серверу.\n";
        return 1;
    }

    Menu menu(client);
    menu.showMenu();

    return 0;
}
