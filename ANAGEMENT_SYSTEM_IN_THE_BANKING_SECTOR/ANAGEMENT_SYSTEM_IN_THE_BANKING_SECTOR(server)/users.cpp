#include "users.h"
#include "common.h"
#include "transactions.h"
#include "audit.h"
#include "i18n.h"
#include <fstream>
#include <mutex>
#include <sstream>
#include "credits.h"
#include "reports.h"
extern std::mutex file_mutex;

struct User {
    int id;
    std::string username;
    std::string password;
    double balance;
    std::string status;
    std::string role;
};

static std::vector<User> readUsers(const std::string& filename) {
    std::lock_guard<std::mutex> lock(file_mutex);
    std::vector<User> users;
    std::ifstream file(filename);
    std::string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        auto tokens = split(line, ';');
        if (tokens.size() < 6) continue;
        User u;
        u.id = std::stoi(tokens[0]);
        u.username = tokens[1];
        u.password = tokens[2];
        u.balance = std::stod(tokens[3]);
        u.status = tokens[4];
        u.role = tokens[5];
        users.push_back(u);
    }
    return users;
}

static void writeUsers(const std::string& filename, const std::vector<User>& users) {
    std::lock_guard<std::mutex> lock(file_mutex);
    std::ofstream file(filename, std::ios::trunc);
    for (auto& u : users) {
        file << u.id << ";" << u.username << ";" << u.password << ";"
            << u.balance << ";" << u.status << ";" << u.role << "\n";
    }
}

std::string loginUser(const std::string& username, const std::string& password, int& userId, std::string& userRole) {
    auto users = readUsers("users.txt");
    for (auto& u : users) {
        if (u.username == username && u.password == password) {
            if (u.status == "blocked") return "ERROR;" + tr("ERROR_BLOCKED");
            userId = u.id;
            userRole = u.role;
            return "SUCCESS;" + tr("WELCOME") + ", " + u.username + "!";
        }
    }
    return "ERROR;" + tr("ERROR_LOGIN");
}

bool updateUserBalance(int userId, double amount, const std::string& type, std::string& response) {
    auto users = readUsers("users.txt");
    for (auto& u : users) {
        if (u.id == userId) {
            if (type == "withdraw" && u.balance < amount) {
                response = "ERROR;" + tr("ERROR_GENERIC");
                return false;
            }
            if (type == "deposit") u.balance += amount;
            else if (type == "withdraw") u.balance -= amount;
            else { response = "ERROR;" + tr("ERROR_GENERIC"); return false; }
            writeUsers("users.txt", users);
            logTransaction(userId, type, amount, type);
            response = "SUCCESS;" + tr("SUCCESS_BALANCE") + " " + std::to_string(u.balance);
            return true;
        }
    }
    response = "ERROR;" + tr("ERROR_GENERIC");
    return false;
}

double getUserBalance(int userId, std::string& response) {
    auto users = readUsers("users.txt");
    for (auto& u : users) {
        if (u.id == userId) return u.balance;
    }
    response = "ERROR;" + tr("ERROR_GENERIC");
    return -1;
}

bool transferFunds(int fromUserId, int toUserId, double amount, std::string& response) {
    auto users = readUsers("users.txt");
    User* fromU = nullptr; User* toU = nullptr;
    for (auto& u : users) {
        if (u.id == fromUserId) fromU = &u;
        if (u.id == toUserId) toU = &u;
    }
    if (!fromU || !toU) { response = "ERROR;" + tr("ERROR_GENERIC"); return false; }
    if (fromU->status != "active" || toU->status != "active") { response = "ERROR;" + tr("ERROR_GENERIC"); return false; }
    if (fromU->balance < amount) { response = "ERROR;" + tr("ERROR_GENERIC"); return false; }
    fromU->balance -= amount;
    toU->balance += amount;
    writeUsers("users.txt", users);
    logTransaction(fromUserId, "transfer_out", amount, "transfer");
    logTransaction(toUserId, "transfer_in", amount, "transfer");
    response = "SUCCESS;transfer done";
    return true;
}

std::string createAccount(const std::string& username, const std::string& password, double balance, const std::string& status) {
    auto users = readUsers("users.txt");
    for (auto& u : users) {
        if (u.username == username) return "ERROR;" + tr("ERROR_GENERIC");
    }
    int newId = 1; for (auto& u : users)if (u.id >= newId)newId = u.id + 1;
    User nu{ newId,username,password,balance,status,"user" };
    users.push_back(nu);
    writeUsers("users.txt", users);
    {
        std::lock_guard<std::mutex> lock(file_mutex);
        std::ofstream f("user_" + std::to_string(newId) + "_transactions.txt");
    }
    auditLog("Created account " + username);
    return "SUCCESS;" + tr("ACCOUNT_CREATED");
}

std::string deleteAccount(int userId) {
    auto users = readUsers("users.txt");
    bool found = false;
    for (auto it = users.begin(); it != users.end(); ++it) {
        if (it->id == userId) { users.erase(it); found = true; break; }
    }
    if (!found)return "ERROR;" + tr("ERROR_GENERIC");
    writeUsers("users.txt", users);
    {
        std::lock_guard<std::mutex> lock(file_mutex);
        std::string fn = "user_" + std::to_string(userId) + "_transactions.txt";
        remove(fn.c_str());
    }
    auditLog("Deleted account " + std::to_string(userId));
    return "SUCCESS;ok";
}

std::string blockAccount(int userId) {
    auto users = readUsers("users.txt");
    for (auto& u : users) {
        if (u.id == userId) { u.status = "blocked"; writeUsers("users.txt", users); auditLog("Blocked account " + std::to_string(userId)); return "SUCCESS;ok"; }
    }
    return "ERROR;" + tr("ERROR_GENERIC");
}

std::string unblockAccount(int userId) {
    auto users = readUsers("users.txt");
    for (auto& u : users) {
        if (u.id == userId) { u.status = "active"; writeUsers("users.txt", users); auditLog("Unblocked " + std::to_string(userId)); return "SUCCESS;ok"; }
    }
    return "ERROR;" + tr("ERROR_GENERIC");
}

std::string applyInterest(double rate) {
    auto users = readUsers("users.txt");
    for (auto& u : users) {
        if (u.status == "active" && u.role == "user") {
            double interest = u.balance * (rate / 100.0);
            u.balance += interest;
            logTransaction(u.id, "interest", interest, "interest");
        }
    }
    writeUsers("users.txt", users);
    auditLog("Applied interest");
    return "SUCCESS;ok";
}

std::string viewClientTransactions(int userId) {
    std::lock_guard<std::mutex> lock(file_mutex);
    std::ifstream f("user_" + std::to_string(userId) + "_transactions.txt");
    if (!f.is_open())return "ERROR;" + tr("ERROR_GENERIC");
    std::stringstream ss; ss << "SUCCESS;\n";
    std::string line; while (getline(f, line))ss << line << "\n";
    return ss.str();
}

std::string searchAccountsByBalance(double amount) {
    auto users = readUsers("users.txt");
    std::stringstream ss; ss << "SUCCESS;\n";
    for (auto& u : users) {
        if (u.role == "user" && u.balance > amount) ss << "ID:" << u.id << " " << u.username << " " << u.balance << "\n";
    }
    return ss.str();
}

std::string generateAccountsReport() {
    auto users = readUsers("users.txt");
    std::stringstream ss; ss << "SUCCESS;\n";
    for (auto& u : users) {
        if (u.role == "user") ss << "ID:" << u.id << " " << u.username << " " << u.balance << " " << u.status << "\n";
    }
    return ss.str();
}

std::string handleWorkerCommands(const std::vector<std::string>& tokens, int authenticatedUserId) {
    std::string cmd = tokens[0];
    if (cmd == "CREATE_ACCOUNT") {
        if (tokens.size() < 5)return "ERROR;" + tr("ERROR_GENERIC");
        return createAccount(tokens[1], tokens[2], std::stod(tokens[3]), tokens[4]);
    }
    else if (cmd == "DELETE_ACCOUNT") {
        if (tokens.size() < 2)return "ERROR;" + tr("ERROR_GENERIC");
        return deleteAccount(std::stoi(tokens[1]));
    }
    else if (cmd == "BLOCK_ACCOUNT") {
        if (tokens.size() < 2)return "ERROR;" + tr("ERROR_GENERIC");
        return blockAccount(std::stoi(tokens[1]));
    }
    else if (cmd == "UNBLOCK_ACCOUNT") {
        if (tokens.size() < 2)return "ERROR;" + tr("ERROR_GENERIC");
        return unblockAccount(std::stoi(tokens[1]));
    }
    else if (cmd == "APPLY_INTEREST") {
        if (tokens.size() < 2)return "ERROR;" + tr("ERROR_GENERIC");
        return applyInterest(std::stod(tokens[1]));
    }
    else if (cmd == "VIEW_HISTORY") {
        if (tokens.size() < 2)return "ERROR;" + tr("ERROR_GENERIC");
        return viewClientTransactions(std::stoi(tokens[1]));
    }
    else if (cmd == "SEARCH_BALANCE") {
        if (tokens.size() < 2)return "ERROR;" + tr("ERROR_GENERIC");
        return searchAccountsByBalance(std::stod(tokens[1]));
    }
    else if (cmd == "GENERATE_REPORT") {
        if (tokens.size() < 5)return "ERROR;" + tr("ERROR_GENERIC");
        return generateAccountsReportFiltered(tokens[1], tokens[2], std::stod(tokens[3]), std::stod(tokens[4]));
    }
    else if (cmd == "REQUEST_CREDIT") {
        if (tokens.size() < 3)return "ERROR;" + tr("ERROR_GENERIC");
        return requestCredit(authenticatedUserId, std::stod(tokens[1]), std::stoi(tokens[2]));
    }
    else if (cmd == "APPROVE_CREDIT") {
        if (tokens.size() < 2)return "ERROR;" + tr("ERROR_GENERIC");
        return approveCredit(std::stoi(tokens[1]));
    }
    else if (cmd == "SET_CREDIT_TYPE") {
        if (tokens.size() < 3)return "ERROR;" + tr("ERROR_GENERIC");
        return setCreditType(std::stoi(tokens[1]), tokens[2]);
    }
    else if (cmd == "SET_CREDIT_TERM") {
        if (tokens.size() < 3)return "ERROR;" + tr("ERROR_GENERIC");
        return setCreditTerm(std::stoi(tokens[1]), std::stoi(tokens[2]));
    }

    return "ERROR;" + tr("ERROR_GENERIC");
}

std::string handleUserCommands(const std::vector<std::string>& tokens, int authenticatedUserId) {
    std::string cmd = tokens[0];
    if (cmd == "DEPOSIT") {
        if (tokens.size() < 2)return "ERROR;" + tr("ERROR_GENERIC");
        std::string resp; updateUserBalance(authenticatedUserId, std::stod(tokens[1]), "deposit", resp); return resp;
    }
    else if (cmd == "WITHDRAW") {
        if (tokens.size() < 2)return "ERROR;" + tr("ERROR_GENERIC");
        std::string resp; updateUserBalance(authenticatedUserId, std::stod(tokens[1]), "withdraw", resp); return resp;
    }
    else if (cmd == "TRANSFER") {
        if (tokens.size() < 3)return "ERROR;" + tr("ERROR_GENERIC");
        std::string resp; transferFunds(authenticatedUserId, std::stoi(tokens[1]), std::stod(tokens[2]), resp); return resp;
    }
    else if (cmd == "VIEW_BALANCE") {
        std::string r; double b = getUserBalance(authenticatedUserId, r);
        if (b < 0)return r;
        return "SUCCESS;" + tr("SUCCESS_BALANCE") + " " + std::to_string(b);
    }
    else if (cmd == "VIEW_HISTORY") {
        return viewClientTransactions(authenticatedUserId);
    }

    return "ERROR;" + tr("ERROR_GENERIC");
}
