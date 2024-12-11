#include "transactions.h"
#include "common.h"
#include "audit.h"
#include <fstream>
#include <mutex>

extern std::mutex file_mutex;

void logTransaction(int userId, const std::string& type, double amount, const std::string& details) {
    std::lock_guard<std::mutex> lock(file_mutex);
    std::ofstream f("user_" + std::to_string(userId) + "_transactions.txt", std::ios::app);
    f << getCurrentTimestamp() << ";" << type << ";" << amount << ";" << details << "\n";
    auditLog("Transaction user " + std::to_string(userId) + " " + type + " " + std::to_string(amount));
}
