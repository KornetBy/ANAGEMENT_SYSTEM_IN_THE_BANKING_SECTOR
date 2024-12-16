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

// Создаём глобальные объекты, которые будут использованы для всех клиентов
// Можно было бы передавать их иначе, но для простоты сделаем так.
static Utils utils("data/logs.txt");
static DataAccess da(utils);
static AuthService auth(da, utils);

struct CI {
    SOCKET s;
    std::string r; // роль текущего пользователя, изначально пустая
};

static void client_thread(CI ci) {
    // Теперь создаём CommandHandler, передавая ссылки на utils, da, auth и role
    CommandHandler ch(ci.r, utils, da, auth);

    utils.Log("INFO", "Новый клиент подключен.");

    char buf[4096]; int n;
    for (;;) {
        n = recv(ci.s, buf, 4096, 0);
        if (n <= 0) {
            utils.Log("INFO", "Клиент отключен.");
            break;
        }
        std::string d(buf, buf + n);
        if (d == "EXIT") {
            utils.Log("INFO", "Клиент запросил отключение.");
            break;
        }
        std::string res = ch.Handle(d);
        // Если команда EXIT или Handle вернул пустую строку — разрываем соединение
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

    std::cout << "Сервер запущен, ожидаем подключения...\n";
    utils.Log("INFO", "Сервер запущен.");

    for (;;) {
        SOCKET c = accept(srv, NULL, NULL);
        if (c == INVALID_SOCKET) {
            utils.Log("ERROR", "Ошибка при accept");
            continue;
        }
        utils.Log("INFO", "Подключился новый клиент.");
        CI ci; ci.s = c;
        // Создаём поток для каждого клиента
        std::thread(client_thread, ci).detach();
    }
    WSACleanup();
}
