#include "server.h"
#include "users.h"
#include "employees.h"
#include "transactions.h"
#include "credits.h"
#include "reports.h"
#include "audit.h"
#include "common.h"
#include "i18n.h"



int PORT = 8080;

std::string handleRequest(const std::string& request, int& authenticatedUserId, std::string& userRole) {
    // ��������� ������� �� ������
    auto tokens = split(request, ';');
    if (tokens.empty()) {
        auditLog("Empty request received");
        return "ERROR;" + tr("ERROR_GENERIC");
    }

    std::string command = tokens[0];
    auditLog("Request: " + request);

    // ������� LOGIN
    if (command == "LOGIN") {
        if (tokens.size() < 3) {
            auditLog("Login failed: insufficient parameters");
            return "ERROR;" + tr("LOGIN_FAILED");
        }
        std::string username = tokens[1];
        std::string password = tokens[2];

        // �������� �����������
        std::string resp = loginUser(username, password, authenticatedUserId, userRole);
        if (resp.find("SUCCESS") == 0) {
            auditLog("Login successful for user: " + username + ", role: " + userRole);
            return resp + ";" + userRole;  // ���������� SUCCESS � ����
        }
        else {
            auditLog("Login failed for user: " + username);
            return resp;  // ������� "ERROR;LOGIN_FAILED"
        }
    }

    // �������� �����������
    if (authenticatedUserId == -1) {
        auditLog("Unauthorized request: " + request);
        return "ERROR;" + tr("NOT_AUTHORIZED");
    }

    // ��������� ������ �� �����
    if (userRole == "hr") {
        return handleHRCommands(tokens);
    }
    else if (userRole == "people_manager") {
        return handlePeopleManagerCommands(tokens);
    }
    else if (userRole == "worker") {
        return handleWorkerCommands(tokens, authenticatedUserId);
    }
    else if (userRole == "user") {
        return handleUserCommands(tokens, authenticatedUserId);
    }

    // ����������� ����
    auditLog("Unknown role for user ID " + std::to_string(authenticatedUserId));
    return "ERROR;" + tr("ROLE_UNKNOWN");
}
