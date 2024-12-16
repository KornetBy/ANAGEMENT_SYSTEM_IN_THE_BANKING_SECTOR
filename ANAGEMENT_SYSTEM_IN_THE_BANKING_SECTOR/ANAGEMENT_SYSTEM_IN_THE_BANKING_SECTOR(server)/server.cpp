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
    // Разделяем команду на токены
    auto tokens = split(request, ';');
    if (tokens.empty()) {
        auditLog("Empty request received");
        return "ERROR;" + tr("ERROR_GENERIC");
    }

    std::string command = tokens[0];
    auditLog("Request: " + request);

    // Команда LOGIN
    if (command == "LOGIN") {
        if (tokens.size() < 3) {
            auditLog("Login failed: insufficient parameters");
            return "ERROR;" + tr("LOGIN_FAILED");
        }
        std::string username = tokens[1];
        std::string password = tokens[2];

        // Проверка авторизации
        std::string resp = loginUser(username, password, authenticatedUserId, userRole);
        if (resp.find("SUCCESS") == 0) {
            auditLog("Login successful for user: " + username + ", role: " + userRole);
            return resp + ";" + userRole;  // Возвращаем SUCCESS и роль
        }
        else {
            auditLog("Login failed for user: " + username);
            return resp;  // Вернётся "ERROR;LOGIN_FAILED"
        }
    }

    // Проверка авторизации
    if (authenticatedUserId == -1) {
        auditLog("Unauthorized request: " + request);
        return "ERROR;" + tr("NOT_AUTHORIZED");
    }

    // Обработка команд по ролям
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

    // Неизвестная роль
    auditLog("Unknown role for user ID " + std::to_string(authenticatedUserId));
    return "ERROR;" + tr("ROLE_UNKNOWN");
}
