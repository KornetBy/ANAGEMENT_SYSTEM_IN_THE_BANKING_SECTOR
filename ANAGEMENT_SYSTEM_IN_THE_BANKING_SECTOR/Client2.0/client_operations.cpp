#include "client_operations.h"
#include <iostream>
#include <string>
#include <stdexcept>

// ���������� ������ ��������������
void executeAdminCommand(SOCKET clientSocket, int choice) {
    std::string request;
    switch (choice) {
    case 1: // �������� ������������
        request = "ADD_USER\n";
        break;
    case 2: // ������� ������������
        request = "DELETE_USER\n";
        break;
    case 3: // �������� ���� ������������
        request = "CHANGE_ROLE\n";
        break;
    case 4: // ����������� ���� �������������
        request = "LIST_USERS\n";
        break;
    case 5: // �����
        return;
    default:
        std::cerr << "�������� �������.\n";
        return;
    }
    send(clientSocket, request.c_str(), request.size(), 0);
    char response[1024] = { 0 };
    int bytesRead = recv(clientSocket, response, sizeof(response), 0);

    if (bytesRead <= 0) {
        std::cerr << "������: ������ �� ������� �� ������.\n";
        return;
    }

    std::cout << "����� �� �������: " << response << "\n";
}

// ���������� ������ �������
void executeClientCommand(SOCKET clientSocket, int choice) {
    std::string request;
    switch (choice) {
    case 1: // �������� ������
        request = "GET_ACCOUNTS\n";
        break;
    case 2: // ������� �������
        request = "TRANSFER\n";
        break;
    case 3: // �������� �������
        request = "ACCOUNT_STATEMENT\n";
        break;
    case 4: // �����
        return;
    default:
        std::cerr << "�������� �������.\n";
        return;
    }
    send(clientSocket, request.c_str(), request.size(), 0);
    char response[1024] = { 0 };
    recv(clientSocket, response, sizeof(response), 0);
    std::cout << "����� �� �������: " << response << "\n";
}

// ���������� ������ ���������� �����
void executeWorkerCommand(SOCKET clientSocket, int choice) {
    std::string request;
    switch (choice) {
    case 1: // ���������� �����
        request = "BLOCK_ACCOUNT\n";
        break;
    case 2: // ������������� �����
        request = "UNBLOCK_ACCOUNT\n";
        break;
    case 3: // ��������� ���������� �������
        request = "CREATE_LOAN_REQUEST\n";
        break;
    case 4: // ��������� ������
        request = "GENERATE_ACCOUNT_REPORT\n";
        break;
    case 5: // �����
        return;
    default:
        std::cerr << "�������� �������.\n";
        return;
    }
    send(clientSocket, request.c_str(), request.size(), 0);
    char response[1024] = { 0 };
    recv(clientSocket, response, sizeof(response), 0);
    std::cout << "����� �� �������: " << response << "\n";
}

// ���������� ������ ��������� �� ���������
void executeHRCommand(SOCKET clientSocket, int choice) {
    std::string request;
    switch (choice) {
    case 1: // ���������� ������������
        request = "MANAGE_EMPLOYEES\n";
        break;
    case 2: // ��������� ������
        request = "EMPLOYEE_REPORT\n";
        break;
    case 3: // �����
        return;
    default:
        std::cerr << "�������� �������.\n";
        return;
    }
    send(clientSocket, request.c_str(), request.size(), 0);
    char response[1024] = { 0 };
    recv(clientSocket, response, sizeof(response), 0);
    std::cout << "����� �� �������: " << response << "\n";
}
