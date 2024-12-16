#include "RequestHandler.h"
#include "Client.h"
#include <iostream>

void RequestHandler::createRequest(const std::string& employeeId, const std::string& type, double amount) {
    std::cout << "Введите дату заявки (YYYY-MM-DD): ";
    std::string date;
    std::cin >> date;

    // Формирование запроса
    std::string request = "CREATE_REQUEST " + employeeId + " " + type + " " + std::to_string(amount) + " " + date;
    Client::sendRequest(request);

    // Получение ответа
    std::cout << Client::receiveResponse();
}

void RequestHandler::viewRequestsByStatus(const std::string& status) {
    // Формирование запроса
    std::string request = "GET_REQUESTS_BY_STATUS " + status;
    Client::sendRequest(request);

    // Получение ответа
    std::cout << Client::receiveResponse();
}

void RequestHandler::updateRequestStatus(const std::string& requestId, const std::string& newStatus) {
    // Формирование запроса
    std::string request = "UPDATE_REQUEST_STATUS " + requestId + " " + newStatus;
    Client::sendRequest(request);

    // Получение ответа
    std::cout << Client::receiveResponse();
}
