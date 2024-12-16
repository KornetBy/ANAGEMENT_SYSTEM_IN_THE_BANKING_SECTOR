#include "client_interface.h"
#include <iostream>

// ���� ��������������
void showAdminMenu() {
    std::cout << "\n=== ���� �������������� ===\n";
    std::cout << "1. �������� ������������\n";
    std::cout << "2. ������� ������������\n";
    std::cout << "3. �������� ���� ������������\n";
    std::cout << "4. ����������� ���� �������������\n";
    std::cout << "5. �����\n";
}

// ���� �������
void showClientMenu() {
    std::cout << "\n=== ���� ������� ===\n";
    std::cout << "1. �������� ������\n";
    std::cout << "2. ������� �������\n";
    std::cout << "3. �������� �������\n";
    std::cout << "4. �����\n";
}

// ���� ���������� �����
void showWorkerMenu() {
    std::cout << "\n=== ���� ���������� ����� ===\n";
    std::cout << "1. ���������� �����\n";
    std::cout << "2. ������������� �����\n";
    std::cout << "3. ��������� ���������� �������\n";
    std::cout << "4. ��������� ������\n";
    std::cout << "5. �����\n";
}

// ���� ��������� �� ���������
void showHRMenu() {
    std::cout << "\n=== ���� ��������� �� ��������� ===\n";
    std::cout << "1. ���������� ������������\n";
    std::cout << "2. ��������� ������\n";
    std::cout << "3. �����\n";
}

// ��������� ������ ������������
int getUserChoice() {
    int choice;
    std::cout << "������� ����� �������: ";
    if (!(std::cin >> choice)) {
        std::cin.clear(); // ������� ��������� ������
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ������� ������������� �����
        std::cerr << "������������ ����. ���������� �����.\n";
        return -1; // ���������� ��� ������
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ������� ����������� �����
    return choice;
}

