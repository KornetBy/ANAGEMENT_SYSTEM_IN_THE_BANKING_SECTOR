#pragma once

#include <string>
#include <vector>
#include <mutex>

extern std::mutex file_mutex;
std::string getCurrentTimestamp();
std::vector<std::string> split(const std::string& s, char delimiter);

struct Account {
    int accountId;
    int userId;
    double balance;
    std::string currency;
};

std::vector<Account> readAccounts(const std::string& filename);
void writeAccounts(const std::string& filename, const std::vector<Account>& accounts);
