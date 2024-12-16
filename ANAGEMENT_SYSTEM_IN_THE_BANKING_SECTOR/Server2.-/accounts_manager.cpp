#include "accounts_manager.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

// �����������
AccountsManager::AccountsManager(const std::string& filename) : filename(filename) {}

// ��������������� ����� ��� �������� ������ �� �����
Account AccountsManager::parseAccount(const std::string& line) {
    Account account;
    std::istringstream iss(line);
    std::string id, userId, balance;

    // ���������� ������ �� ������� '|'
    if (!(std::getline(iss, id, '|') && std::getline(iss, userId, '|') && std::getline(iss, balance, '|'))) {
        throw std::runtime_error("������������ ������ ������.");
    }

    // �������������� ID � ������� � �����
    account.id = std::stoi(id);
    account.userId = std::stoi(userId);
    account.balance = std::stod(balance);

    // �������� �� ������������ ������
    if (account.id <= 0) {
        throw std::runtime_error("������������ ID �����: " + id);
    }
    if (account.userId <= 0) {
        throw std::runtime_error("������������ ID ������������: " + userId);
    }
    if (account.balance < 0) {
        throw std::runtime_error("������ �� ����� ���� �������������: " + balance);
    }

    return account;
}

// �������� ������ �� �����
void AccountsManager::loadAccounts() {
    accounts.clear();
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "������: �� ������� ������� ���� " << filename << std::endl;
        std::ofstream errorLog("logs/error.log", std::ios::app);
        errorLog << "������: ���� " << filename << " �� ������ ��� ����������." << std::endl;
        errorLog.close();
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        try {
            Account account = parseAccount(line);

            // �������� �� ������������ ID
            for (const auto& existingAccount : accounts) {
                if (existingAccount.id == account.id) {
                    throw std::runtime_error("������������� ID �����: " + std::to_string(account.id));
                }
            }

            accounts.push_back(account);
        }
        catch (const std::exception& e) {
            std::cerr << "������: " << e.what() << " ������: " << line << std::endl;
            std::ofstream errorLog("logs/error.log", std::ios::app);
            errorLog << "������ ��� �������� �����: " << e.what() << " ������: " << line << std::endl;
            errorLog.close();
        }
    }

    file.close();
}

// ���������� ������ � ����
void AccountsManager::saveAccounts() {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "������: �� ������� ������� ���� " << filename << " ��� ������." << std::endl;
        std::ofstream errorLog("logs/error.log", std::ios::app);
        errorLog << "������: �� ������� ������� ���� " << filename << " ��� ������." << std::endl;
        errorLog.close();
        return;
    }

    for (const auto& account : accounts) {
        file << account.id << " | " << account.userId << " | " << account.balance << std::endl;
    }
    file.close();
}

// ���������� ������ �����
void AccountsManager::addAccount(const Account& account) {
    // �������� ������������ ID
    for (const auto& existingAccount : accounts) {
        if (existingAccount.id == account.id) {
            std::cerr << "������: ���� � ID " << account.id << " ��� ����������." << std::endl;
            return;
        }
    }

    accounts.push_back(account);
    saveAccounts();
}

// �������� ����� �� ID
bool AccountsManager::removeAccount(int id) {
    for (auto it = accounts.begin(); it != accounts.end(); ++it) {
        if (it->id == id) {
            accounts.erase(it);
            saveAccounts();
            return true;
        }
    }
    std::cerr << "������: ���� � ID " << id << " �� ������." << std::endl;
    return false;
}

// ��������� ����� �� ID
Account* AccountsManager::getAccountById(int id) {
    for (auto& account : accounts) {
        if (account.id == id) {
            return &account;
        }
    }
    return nullptr;
}

// ��������� ���� ������ ��� ������������
std::vector<Account> AccountsManager::getAccountsByUserId(int userId) const {
    std::vector<Account> result;
    for (const auto& account : accounts) {
        if (account.userId == userId) {
            result.push_back(account);
        }
    }
    return result;
}

// ��������� ���� ������
const std::vector<Account>& AccountsManager::getAllAccounts() const {
    return accounts;
}

// ���������� ������� �����
bool AccountsManager::updateBalance(int accountId, double amount) {
    for (auto& account : accounts) {
        if (account.id == accountId) {
            if (account.balance + amount < 0) {
                std::cerr << "������: ������������ ������� �� ����� ID " << accountId << std::endl;
                return false;
            }
            account.balance += amount;
            saveAccounts();
            return true;
        }
    }
    std::cerr << "������: ���� � ID " << accountId << " �� ������." << std::endl;
    return false;
}

// �������� ������������� ����� �� ID
bool AccountsManager::accountExists(int accountId) const {
    for (const auto& account : accounts) {
        if (account.id == accountId) {
            return true;
        }
    }
    return false;
}

// ��������� ������ ������� ������������
double AccountsManager::getTotalBalanceByUserId(int userId) const {
    double total = 0.0;
    for (const auto& account : accounts) {
        if (account.userId == userId) {
            total += account.balance;
        }
    }
    return total;
}
