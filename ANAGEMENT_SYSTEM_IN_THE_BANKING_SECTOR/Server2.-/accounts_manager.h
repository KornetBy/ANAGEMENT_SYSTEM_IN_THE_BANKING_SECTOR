#pragma once
#ifndef ACCOUNTS_MANAGER_H
#define ACCOUNTS_MANAGER_H

#include <string>
#include <vector>

// ��������� ��� �������� ���������� � �����
struct Account {
    int id;             // ���������� ������������� �����
    int userId;         // ID ������������, �������� ����������� ����
    double balance;     // ������ �� �����
};

// ����� ��� ���������� ����������� �������
class AccountsManager {
private:
    std::vector<Account> accounts; // ������ ������
    std::string filename;          // ��� ����� � ������� ������

    // ��������������� ����� ��� �������� ������ �� �����
    Account parseAccount(const std::string& line);

public:
    // ����������� � ��������� ����� ������
    AccountsManager(const std::string& filename);

    // �������� ������ �� �����
    void loadAccounts();

    // ���������� ������ � ����
    void saveAccounts();

    // ���������� ������ �����
    void addAccount(const Account& account);

    // �������� ����� �� ID
    bool removeAccount(int id);

    // ��������� ����� �� ID
    Account* getAccountById(int id);

    // ��������� ���� ������ ��� ������������
    std::vector<Account> getAccountsByUserId(int userId) const;

    // ��������� ���� ������
    const std::vector<Account>& getAllAccounts() const;

    // ���������� ������� �����
    bool updateBalance(int accountId, double amount);

    // �������� ������������� ����� �� ID
    bool accountExists(int accountId) const;

    // ��������� ������ ������� ������������
    double getTotalBalanceByUserId(int userId) const;
};

#endif // ACCOUNTS_MANAGER_H
