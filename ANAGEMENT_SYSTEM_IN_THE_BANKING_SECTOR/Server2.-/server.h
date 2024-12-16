#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <map>
#include "user_manager.h"
#include "accounts_manager.h"
#include "transactions_manager.h"
#include "personnel_manager.h"

// ������ ������ ��� ������ �������


// ������� ��� �������� ������������ �� �����
std::map<std::string, std::string> loadConfig(const std::string& configFile);

// ������� ��� ����������� �������� � �������
void logRequestResponse(const std::string& request, const std::string& response);

// ������� ��� ��������� �������
void handleClient(int clientSocket,
    UserManager& userManager,
    AccountsManager& accountsManager,
    TransactionsManager& transactionsManager,
    PersonnelManager& personnelManager);

#endif // SERVER_H
