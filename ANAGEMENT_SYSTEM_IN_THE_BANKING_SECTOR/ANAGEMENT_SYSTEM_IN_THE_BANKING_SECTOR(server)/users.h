#pragma once
#pragma once
#include <string>
#include <vector>
std::string loginUser(const std::string& username, const std::string& password, int& userId, std::string& userRole);
std::string handleWorkerCommands(const std::vector<std::string>& tokens, int authenticatedUserId);
std::string handleUserCommands(const std::vector<std::string>& tokens, int authenticatedUserId);
bool updateUserBalance(int userId, double amount, const std::string& type, std::string& response);
double getUserBalance(int userId, std::string& response);
bool transferFunds(int fromUserId, int toUserId, double amount, std::string& response);
std::string createAccount(const std::string& username, const std::string& password, double balance, const std::string& status);
std::string deleteAccount(int userId);
std::string blockAccount(int userId);
std::string unblockAccount(int userId);
std::string applyInterest(double rate);
std::string viewClientTransactions(int userId);
std::string searchAccountsByBalance(double amount);
std::string generateAccountsReport();
