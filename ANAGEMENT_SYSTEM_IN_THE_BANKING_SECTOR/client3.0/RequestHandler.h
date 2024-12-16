#pragma once
#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include <string>

class RequestHandler {
public:
    // ������� ������ �� �����������
    static void createRequest(const std::string& employeeId, const std::string& type, double amount);

    // ����������� ��� ������ �� �������
    static void viewRequestsByStatus(const std::string& status);

    // �������� ������ ������
    static void updateRequestStatus(const std::string& requestId, const std::string& newStatus);
};

#endif // REQUESTHANDLER_H
