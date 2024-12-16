#include "Auth.h"
#include "RoleManager.h"
#include <iostream>
#include <string>
#include <Windows.h>

void displayWelcomeMessage() {
    std::cout << "=====================================\n";
    std::cout << " ���������� ����� ������� ���������� \n";
    std::cout << " ���������� �����\n";
    std::cout << "=====================================\n";
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    try {
        displayWelcomeMessage();

        std::string username, role;

        // ����������� ������������
        while (!Auth::login(username, role)) {
            std::cout << "������ �����������. ���������� �����.\n";
        }

        std::cout << "����� ����������, " << username << "! ���� ����: " << role << "\n";

        // �������� ���������� � ����������� �� ����
        RoleManager::handleRole(role);

    }
    catch (const std::exception& e) {
        std::cerr << "����������� ������: " << e.what() << "\n";
    }
    catch (...) {
        std::cerr << "��������� ����������� ������.\n";
    }

    std::cout << "���������� ������ �������.\n";
    return 0;
}
