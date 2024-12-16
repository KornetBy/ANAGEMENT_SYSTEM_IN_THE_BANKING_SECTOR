#include "server.h"
#include "utils.h"
#include "data_access.h"
#include "auth.h"
#include "command_handler.h"

#include <winsock2.h>
#include <windows.h>
#include <thread>
#include <string>
#include <iostream>
#pragma comment(lib,"ws2_32.lib")

// ������ ���������� �������, ������� ����� ������������ ��� ���� ��������
// ����� ���� �� ���������� �� �����, �� ��� �������� ������� ���.
static Utils utils("data/logs.txt");
static DataAccess da(utils);
static AuthService auth(da, utils);

struct CI {
    SOCKET s;
    std::string r; // ���� �������� ������������, ���������� ������
};

static void client_thread(CI ci) {
    // ������ ������ CommandHandler, ��������� ������ �� utils, da, auth � role
    CommandHandler ch(ci.r, utils, da, auth);

    utils.Log("INFO", "����� ������ ���������.");

    char buf[4096]; int n;
    for (;;) {
        n = recv(ci.s, buf, 4096, 0);
        if (n <= 0) {
            utils.Log("INFO", "������ ��������.");
            break;
        }
        std::string d(buf, buf + n);
        if (d == "EXIT") {
            utils.Log("INFO", "������ �������� ����������.");
            break;
        }
        std::string res = ch.Handle(d);
        // ���� ������� EXIT ��� Handle ������ ������ ������ � ��������� ����������
        if (d == "EXIT" || res.empty()) {
            send(ci.s, "", 0, 0);
            break;
        }
        send(ci.s, res.c_str(), (int)res.size(), 0);
    }
    closesocket(ci.s);
}

void Server::Run() {
    WSADATA w; WSAStartup(MAKEWORD(2, 2), &w);
    SOCKET srv = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in si; si.sin_family = AF_INET; si.sin_port = htons(5000); si.sin_addr.s_addr = INADDR_ANY;
    bind(srv, (sockaddr*)&si, sizeof(si));
    listen(srv, 5);

    std::cout << "������ �������, ������� �����������...\n";
    utils.Log("INFO", "������ �������.");

    for (;;) {
        SOCKET c = accept(srv, NULL, NULL);
        if (c == INVALID_SOCKET) {
            utils.Log("ERROR", "������ ��� accept");
            continue;
        }
        utils.Log("INFO", "����������� ����� ������.");
        CI ci; ci.s = c;
        // ������ ����� ��� ������� �������
        std::thread(client_thread, ci).detach();
    }
    WSACleanup();
}
