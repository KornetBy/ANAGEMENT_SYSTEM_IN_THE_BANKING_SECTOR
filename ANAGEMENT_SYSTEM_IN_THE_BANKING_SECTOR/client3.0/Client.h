#pragma once
#ifndef CLIENT_H
#define CLIENT_H

#include <string>

class Client {
public:
    // Отправка запроса на сервер
    static void sendRequest(const std::string& request);

    // Получение ответа от сервера
    static std::string receiveResponse();
};

#endif // CLIENT_H
