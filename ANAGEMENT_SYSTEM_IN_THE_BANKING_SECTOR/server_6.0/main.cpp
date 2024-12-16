#include "Server.h"

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    const int port = 5000; // ���� ��� ����������� ��������
    Server server(port);

    try {
        std::cout << "������ ����������� �� ����� " << port << "..." << std::endl;
        server.start(); // ������ �������
    }
    catch (const std::exception& e) {
        std::cerr << "������ �������: " << e.what() << std::endl;
    }

    return 0;
}
