#pragma once
#ifndef CLIENT_H
#define CLIENT_H

#include <string>

class Client {
public:
    // �������� ������� �� ������
    static void sendRequest(const std::string& request);

    // ��������� ������ �� �������
    static std::string receiveResponse();
};

#endif // CLIENT_H
