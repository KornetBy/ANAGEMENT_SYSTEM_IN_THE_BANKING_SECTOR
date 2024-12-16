#include "Auth.h"

// �����������
Auth::Auth() : login(""), password("") {}

// ���� ������ � ������
void Auth::inputCredentials() {
    std::cout << "������� �����: ";
    std::getline(std::cin, login);

    std::cout << "������� ������: ";
    char ch;
    password.clear();

    // ���� ������ ������ �� �������� � ������������ ���������
    while ((ch = _getch()) != '\r') { // '\r' � Enter
        if (ch == '\b') { // ��������� Backspace
            if (!password.empty()) {
                password.pop_back();
                std::cout << "\b \b"; // �������� ���������� ������� � ������
            }
        }
        else if (isprint(ch)) { // ������ �������� �������
            password += ch;
            std::cout << '*';
        }
    }
    std::cout << std::endl;
}
// ��������� ������ � ������ ����� ���������
void Auth::setCredentials(const std::string& login, const std::string& password) {
    this->login = login;
    this->password = password;
}
// ������������ ������ ��� �������� ������� �� ������
std::string Auth::getAuthRequest() {
    return "AUTH|" + login + "|" + password;
}
