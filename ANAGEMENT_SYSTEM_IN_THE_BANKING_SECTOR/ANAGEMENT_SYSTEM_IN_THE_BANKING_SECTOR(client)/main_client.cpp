#include "network.h"
#include "client.h"
#include "i18n.h"
#include <iostream>
#include <fstream>
#include <string>
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
    SOCKET sock = connectToServer("127.0.0.1", 8080);
    if (sock == INVALID_SOCKET)return 1;
    std::string username, password;
    std::cout << tr("WELCOME") << "\n";
    std::cout << "Username:"; std::cin >> username;
    std::cout << "Password:"; std::cin >> password;
    std::string req = "LOGIN;" + username + ";" + password;
    send(sock, req.c_str(), (int)req.size(), 0);
    char buf[4096]; int br = recv(sock, buf, 4096, 0);
    if (br > 0) {
        buf[br] = '\0';
        std::string resp(buf);
        if (resp.find("SUCCESS;") == 0) {
            std::string role = "worker";
            if (role == "hr")hrMenu(sock);
            else if (role == "user")userMenu(sock);
            else if (role == "worker")workerMenu(sock);
            else if (role == "people_manager")peopleManagerMenu(sock);
        }
        else std::cout << resp << "\n";
    }
    closesocket(sock);
    WSACleanup();
    return 0;
}
