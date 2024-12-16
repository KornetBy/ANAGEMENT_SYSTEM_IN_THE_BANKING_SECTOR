#include "network.h"
#include "client.h"
#include "i18n.h"
#include <iostream>
#include <fstream>
#include <string>
#pragma comment(lib, "Ws2_32.lib")

int main() {
    // ��������� UTF-8 ��� �������
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    // ������ ������������ (����)
    std::ifstream cf("config.txt");
    std::string line;
    Language lang = Language::EN;
    while (std::getline(cf, line)) {
        if (line.find("language=") == 0) {
            std::string val = line.substr(9);
            if (val == "ru") lang = Language::RU;
            else if (val == "be") lang = Language::BE;
            else lang = Language::EN;
        }
    }
    setLanguage(lang);

    // ����������� � �������
    SOCKET sock = connectToServer("127.0.0.1", 8080);
    if (sock == INVALID_SOCKET) {
        std::cerr << tr("ERROR_GENERIC") << ": " << tr("SERVER_UNAVAILABLE") << "\n";
        return 1;
    }

    // ����������� ������������
    std::string username, password;
    int attempts = 3; // ���������� �������
    while (attempts > 0) {
        std::cout << tr("WELCOME") << "\n";
        std::cout << tr("USERNAME_PROMPT") << ": ";
        std::cin >> username;
        std::cout << tr("PASSWORD_PROMPT") << ": ";
        std::cin >> password;

        // �������� ������� �� ������
        std::string req = "LOGIN;" + username + ";" + password;
        send(sock, req.c_str(), (int)req.size(), 0);

        // ��������� ������ �� �������
        char buf[4096];
        int br = recv(sock, buf, 4096, 0);
        if (br > 0) {
            buf[br] = '\0';
            std::string resp(buf);

            if (resp.find("SUCCESS;") == 0) {
                // �������� ����
                std::cout << tr("LOGIN_SUCCESS") << "\n";
                std::string role = resp.substr(resp.find(";") + 1); // ���� �� ������ �������

                // ������� � ���� � ����������� �� ����
                if (role == "hr") hrMenu(sock);
                else if (role == "user") userMenu(sock);
                else if (role == "worker") workerMenu(sock);
                else if (role == "people_manager") peopleManagerMenu(sock);
                else std::cerr << tr("ERROR_GENERIC") << ": " << tr("ROLE_UNKNOWN") << "\n";

                closesocket(sock);
                WSACleanup();
                return 0;
            }
            else {
                // ������ �����������
                std::cout << tr("LOGIN_FAILED") << " (" << --attempts << " " << tr("ATTEMPTS_LEFT") << ")\n";
            }
        }
        else {
            std::cerr << tr("ERROR_GENERIC") << ": " << tr("SERVER_RESPONSE_ERROR") << "\n";
            closesocket(sock);
            WSACleanup();
            return 1;
        }
    }

    // ������� ����� ��������� �������
    std::cout << tr("TOO_MANY_ATTEMPTS") << "\n";
    closesocket(sock);
    WSACleanup();
    return 1;
}
