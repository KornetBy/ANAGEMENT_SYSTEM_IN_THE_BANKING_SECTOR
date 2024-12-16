#include "RequestHandler.h"
#include "Client.h"
#include <iostream>

void RequestHandler::createRequest(const std::string& employeeId, const std::string& type, double amount) {
    std::cout << "������� ���� ������ (YYYY-MM-DD): ";
    std::string date;
    std::cin >> date;

    // ������������ �������
    std::string request = "CREATE_REQUEST " + employeeId + " " + type + " " + std::to_string(amount) + " " + date;
    Client::sendRequest(request);

    // ��������� ������
    std::cout << Client::receiveResponse();
}

void RequestHandler::viewRequestsByStatus(const std::string& status) {
    // ������������ �������
    std::string request = "GET_REQUESTS_BY_STATUS " + status;
    Client::sendRequest(request);

    // ��������� ������
    std::cout << Client::receiveResponse();
}

void RequestHandler::updateRequestStatus(const std::string& requestId, const std::string& newStatus) {
    // ������������ �������
    std::string request = "UPDATE_REQUEST_STATUS " + requestId + " " + newStatus;
    Client::sendRequest(request);

    // ��������� ������
    std::cout << Client::receiveResponse();
}
