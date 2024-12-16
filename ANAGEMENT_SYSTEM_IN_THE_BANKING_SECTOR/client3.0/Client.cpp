#include "Client.h"
#include <fstream>
#include <stdexcept>

// ���� � ������ �������� � �������
const std::string requestFilePath = "request_queue.txt";
const std::string responseFilePath = "response_queue.txt";

// �������� ������� �� ������
void Client::sendRequest(const std::string& request) {
    std::ofstream requestFile(requestFilePath, std::ios::app); // �������� � ������ ����������
    if (!requestFile.is_open()) {
        throw std::runtime_error("������: �� ������� ������� ���� ��������.");
    }

    requestFile << request << "\n"; // ������ ������� � ����
    requestFile.close();
}

// ��������� ������ �� �������
std::string Client::receiveResponse() {
    std::ifstream responseFile(responseFilePath);
    if (!responseFile.is_open()) {
        throw std::runtime_error("������: �� ������� ������� ���� �������.");
    }

    std::string response;
    if (std::getline(responseFile, response)) {
        // ������� ����������� �����
        std::ofstream tempFile(responseFilePath, std::ios::trunc); // ������� �����
        tempFile.close();
        return response;
    }
    else {
        return "��� ������ �� �������.";
    }
}
