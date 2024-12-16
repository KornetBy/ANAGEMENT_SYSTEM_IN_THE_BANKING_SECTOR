#pragma once
#ifndef AUTH_H
#define AUTH_H

#include <string>
#include <iostream>
#include <conio.h> // ��� ������� getch() �� Windows

class Auth {
private:
    std::string login; // ����� ������������
    std::string password; // ������ ������������

public:
    Auth();                          // �����������
    void inputCredentials();         // ���� ������ � ������
    std::string getAuthRequest();    // ������������ ������ ������� ��� �������
    void setCredentials(const std::string& login, const std::string& password); // ����� �����
};

#endif // AUTH_H
