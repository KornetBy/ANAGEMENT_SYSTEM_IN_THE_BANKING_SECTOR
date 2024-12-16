#include "Auth.h"
#include "Client.h"
#include <iostream>
#include <sstream>

bool Auth::login(std::string& username, std::string& role) {
    std::string password;

    // ���� ������ � ������
    std::cout << "������� �����: ";
    std::cin >> username;
    std::cout << "������� ������: ";
    std::cin >> password;

    // ������������ �������
    std::ostringstream request;
    request << "AUTH " << username << " " << password;
    Client::sendRequest(request.str());

    // ��������� ������ �� �������
    std::string response = Client::receiveResponse();
    if (response.find("SUCCESS") != std::string::npos) {
        // ���������� ���� �� ������
        role = response.substr(response.find(":") + 1);
        return true;
    }
    else {
        std::cerr << "������ �����������: " << response << "\n";
        return false;
    }
}
