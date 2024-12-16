#pragma once
#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include <string>

class RequestHandler {
public:
    // Создать заявку на компенсацию
    static void createRequest(const std::string& employeeId, const std::string& type, double amount);

    // Просмотреть все заявки по статусу
    static void viewRequestsByStatus(const std::string& status);

    // Обновить статус заявки
    static void updateRequestStatus(const std::string& requestId, const std::string& newStatus);
};

#endif // REQUESTHANDLER_H
