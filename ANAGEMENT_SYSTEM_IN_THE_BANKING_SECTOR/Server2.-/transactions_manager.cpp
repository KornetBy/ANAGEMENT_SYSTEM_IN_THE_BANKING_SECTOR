#include "transactions_manager.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

// �����������
TransactionsManager::TransactionsManager(const std::string& filename) : filename(filename) {}

// ��������������� ����� ��� �������� ������ �� �����
Transaction TransactionsManager::parseTransaction(const std::string& line) {
    Transaction transaction;
    std::istringstream iss(line);
    std::string id, fromAccountId, toAccountId, amount, timestamp;

    // ���������� ������ �� ������� '|'
    if (!(std::getline(iss, id, '|') && std::getline(iss, fromAccountId, '|') &&
        std::getline(iss, toAccountId, '|') && std::getline(iss, amount, '|') &&
        std::getline(iss, timestamp, '|'))) {
        throw std::runtime_error("������������ ������ ������.");
    }

    // �������������� ����� � ������ ����
    transaction.id = std::stoi(id);
    transaction.fromAccountId = std::stoi(fromAccountId);
    transaction.toAccountId = std::stoi(toAccountId);
    transaction.amount = std::stod(amount);
    transaction.timestamp = timestamp;

    // �������� �� ������������ ������
    if (transaction.id <= 0) {
        throw std::runtime_error("������������ ID ����������: " + id);
    }
    if (transaction.amount <= 0) {
        throw std::runtime_error("����� ���������� ������ ���� �������������: " + amount);
    }

    return transaction;
}

// ��������� ����������� ID ����������
int TransactionsManager::generateTransactionId() const {
    int maxId = 0;
    for (const auto& transaction : transactions) {
        if (transaction.id > maxId) {
            maxId = transaction.id;
        }
    }
    return maxId + 1;
}

// �������� ������ �� �����
void TransactionsManager::loadTransactions() {
    transactions.clear();
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
            transactions.push_back(parseTransaction(line));
        }
        catch (const std::exception& e) {
            std::cerr << "������: " << e.what() << " ������: " << line << std::endl;
            std::ofstream errorLog("logs/error.log", std::ios::app);
            errorLog << "������ ��� �������� ����������: " << e.what() << " ������: " << line << std::endl;
            errorLog.close();
        }
    }

    file.close();
}

// ���������� ������ � ����
void TransactionsManager::saveTransactions() {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "������: �� ������� ������� ���� " << filename << " ��� ������." << std::endl;
        std::ofstream errorLog("logs/error.log", std::ios::app);
        errorLog << "������: �� ������� ������� ���� " << filename << " ��� ������." << std::endl;
        errorLog.close();
        return;
    }

    for (const auto& transaction : transactions) {
        file << transaction.id << " | " << transaction.fromAccountId << " | "
            << transaction.toAccountId << " | " << transaction.amount << " | "
            << transaction.timestamp << std::endl;
    }

    file.close();
}

// ���������� ����� ���������� � ��������� ������
bool TransactionsManager::addTransaction(const Transaction& transaction, const AccountsManager& accountsManager) {
    // �������� ������������� ������ ����������� � ����������
    if (!accountsManager.accountExists(transaction.fromAccountId)) {
        std::cerr << "������: ���� ����������� � ID " << transaction.fromAccountId << " �� ����������." << std::endl;
        return false;
    }
    if (!accountsManager.accountExists(transaction.toAccountId)) {
        std::cerr << "������: ���� ���������� � ID " << transaction.toAccountId << " �� ����������." << std::endl;
        return false;
    }

    // ��������� ����������� ID, ���� �� �� �����
    Transaction newTransaction = transaction;
    if (newTransaction.id <= 0) {
        newTransaction.id = generateTransactionId();
    }

    // ���������� ����������
    transactions.push_back(newTransaction);
    saveTransactions();
    return true;
}

// ��������� ���������� �� ID �����
std::vector<Transaction> TransactionsManager::getTransactionsByAccountId(int accountId) const {
    std::vector<Transaction> result;
    for (const auto& transaction : transactions) {
        if (transaction.fromAccountId == accountId || transaction.toAccountId == accountId) {
            result.push_back(transaction);
        }
    }
    return result;
}

// ��������� ���� ����������
const std::vector<Transaction>& TransactionsManager::getAllTransactions() const {
    return transactions;
}
