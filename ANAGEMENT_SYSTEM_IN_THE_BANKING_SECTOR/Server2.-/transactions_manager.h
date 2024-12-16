#ifndef TRANSACTIONS_MANAGER_H
#define TRANSACTIONS_MANAGER_H

#include <string>
#include <vector>
#include "accounts_manager.h" // ��� �������� ������������� ������

// ��������� ��� �������� ���������� � ����������
struct Transaction {
    int id;               // ���������� ������������� ����������
    int fromAccountId;    // ID ����� �����������
    int toAccountId;      // ID ����� ����������
    double amount;        // ����� ����������
    std::string timestamp; // ����� ������� (� ������� "YYYY-MM-DD HH:MM:SS")
};

// ����� ��� ���������� ������������
class TransactionsManager {
private:
    std::vector<Transaction> transactions; // ������ ����������
    std::string filename;                  // ��� ����� � ������� ����������

    // ��������������� ����� ��� �������� ������ �� �����
    Transaction parseTransaction(const std::string& line);

    // ��������� ����������� ID ��� ����� ����������
    int generateTransactionId() const;

public:
    // ����������� � ��������� ����� ������
    TransactionsManager(const std::string& filename);

    // �������� ������ �� �����
    void loadTransactions();

    // ���������� ������ � ����
    void saveTransactions();

    // ���������� ����� ���������� � ��������� ������
    bool addTransaction(const Transaction& transaction, const AccountsManager& accountsManager);

    // ��������� ���������� �� ID �����
    std::vector<Transaction> getTransactionsByAccountId(int accountId) const;

    // ��������� ���� ����������
    const std::vector<Transaction>& getAllTransactions() const;
};

#endif // TRANSACTIONS_MANAGER_H
