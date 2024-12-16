#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <map>
#include "user_manager.h"
#include "accounts_manager.h"
#include "transactions_manager.h"
#include "personnel_manager.h"

// Размер буфера для обмена данными


// Функция для загрузки конфигурации из файла
std::map<std::string, std::string> loadConfig(const std::string& configFile);

// Функция для логирования запросов и ответов
void logRequestResponse(const std::string& request, const std::string& response);

// Функция для обработки клиента
void handleClient(int clientSocket,
    UserManager& userManager,
    AccountsManager& accountsManager,
    TransactionsManager& transactionsManager,
    PersonnelManager& personnelManager);

#endif // SERVER_H
