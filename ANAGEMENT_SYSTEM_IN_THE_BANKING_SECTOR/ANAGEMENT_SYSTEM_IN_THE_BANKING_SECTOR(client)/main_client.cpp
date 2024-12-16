#include "network.h"
#include "client.h"
#include "i18n.h"
#include <iostream>
#include <fstream>
#include <string>
#pragma comment(lib, "Ws2_32.lib")

int main() {
    // Установка UTF-8 для консоли
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    // Чтение конфигурации (язык)
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

    // Подключение к серверу
    SOCKET sock = connectToServer("127.0.0.1", 8080);
    if (sock == INVALID_SOCKET) {
        std::cerr << tr("ERROR_GENERIC") << ": " << tr("SERVER_UNAVAILABLE") << "\n";
        return 1;
    }

    // Авторизация пользователя
    std::string username, password;
    int attempts = 3; // Количество попыток
    while (attempts > 0) {
        std::cout << tr("WELCOME") << "\n";
        std::cout << tr("USERNAME_PROMPT") << ": ";
        std::cin >> username;
        std::cout << tr("PASSWORD_PROMPT") << ": ";
        std::cin >> password;

        // Отправка запроса на сервер
        std::string req = "LOGIN;" + username + ";" + password;
        send(sock, req.c_str(), (int)req.size(), 0);

        // Получение ответа от сервера
        char buf[4096];
        int br = recv(sock, buf, 4096, 0);
        if (br > 0) {
            buf[br] = '\0';
            std::string resp(buf);

            if (resp.find("SUCCESS;") == 0) {
                // Успешный вход
                std::cout << tr("LOGIN_SUCCESS") << "\n";
                std::string role = resp.substr(resp.find(";") + 1); // Роль из ответа сервера

                // Переход в меню в зависимости от роли
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
                // Ошибка авторизации
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

    // Слишком много неудачных попыток
    std::cout << tr("TOO_MANY_ATTEMPTS") << "\n";
    closesocket(sock);
    WSACleanup();
    return 1;
}
