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
    auto tokens = split(request, ';');
    if (tokens.empty()) return "ERROR;" + tr("ERROR_GENERIC");
    std::string command = tokens[0];
    auditLog("Request: " + request);
    if (command == "LOGIN") {
        if (tokens.size() < 3) return "ERROR;" + tr("ERROR_GENERIC");
        std::string username = tokens[1];
        std::string password = tokens[2];
        auto resp = loginUser(username, password, authenticatedUserId, userRole);
        auditLog("Login attempt for " + username + " result: " + resp);
        return resp;
    }
    if (authenticatedUserId == -1) return "ERROR;" + tr("ERROR_GENERIC");
    if (userRole == "hr") return handleHRCommands(tokens);
    if (userRole == "people_manager") return handlePeopleManagerCommands(tokens);
    if (userRole == "worker") return handleWorkerCommands(tokens, authenticatedUserId);
    if (userRole == "user") return handleUserCommands(tokens, authenticatedUserId);
    return "ERROR;" + tr("ERROR_GENERIC");
}
