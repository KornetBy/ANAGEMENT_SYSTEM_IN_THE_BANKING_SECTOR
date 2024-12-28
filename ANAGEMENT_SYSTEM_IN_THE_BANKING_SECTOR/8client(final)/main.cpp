
// main.cpp
#include <iostream>
#include "Client.h"
#include "Menu.h"
#include "Windows.h"
#pragma comment(lib, "Ws2_32.lib")
int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    Client client("127.0.0.1", 54000);
    if (!client.connectToServer()) {
        std::cout << "Не удалось подключиться к серверу.\n";
        return 1;
    }

    Menu menu(client);
    menu.showMenu();

    return 0;
}
