#include "network.h"
#include "server.h"
#include "i18n.h"
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#pragma comment(lib, "Ws2_32.lib")

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    std::ifstream cf("config.txt");
    std::string line; Language lang = Language::EN;
    while (std::getline(cf, line)) {
        if (line.find("language=") == 0) {
            std::string val = line.substr(9);
            if (val == "ru")lang = Language::RU;
            else if (val == "be")lang = Language::BE;
            else lang = Language::EN;
        }
    }
    setLanguage(lang);
    SOCKET serverSocket = createServerSocket(PORT);
    std::cout << "Server started.\n";
    sockaddr_in client; int c = sizeof(sockaddr_in);
    SOCKET cs;
    while ((cs = accept(serverSocket, (sockaddr*)&client, &c)) != INVALID_SOCKET) {
        std::cout << "Client connected.\n";
        std::thread(clientHandler, cs).detach();
    }
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
