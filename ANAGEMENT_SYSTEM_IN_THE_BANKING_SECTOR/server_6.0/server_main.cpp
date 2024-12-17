// Server/Server_main.cpp
#include "Server.h"
#include <iostream>
#include <string>
#include <csignal>

// ���������� ��������� �� ������ ��� ��������� ��������
Server* globalServer = nullptr;

// ������� ��� ��������� ������� ���������� (��������, Ctrl+C)
void signalHandler(int signum) {
    if (globalServer != nullptr) {
        std::cout << "\n������� ������ ����������. ������������� ������..." << std::endl;
        globalServer->stop();
    }
    exit(signum);
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    int port = 5000; // ���� �������
    Server server(port);
    globalServer = &server;

    // ��������� ����������� ��������
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);

    if (!server.start()) {
        std::cerr << "�� ������� ��������� ������." << std::endl;
        return 1;
    }

    return 0;
}
