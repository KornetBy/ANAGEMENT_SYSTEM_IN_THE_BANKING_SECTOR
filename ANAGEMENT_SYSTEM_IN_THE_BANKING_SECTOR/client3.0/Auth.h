#pragma once
#ifndef AUTH_H
#define AUTH_H

#include <string>

class Auth {
public:
    // ����������� ������������
    static bool login(std::string& username, std::string& role);
};

#endif // AUTH_H
