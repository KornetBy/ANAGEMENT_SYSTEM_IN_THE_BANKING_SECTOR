#pragma once
#ifndef ACCOUNTS_MANAGER_H
#define ACCOUNTS_MANAGER_H

#include <string>
#include <vector>

// Структура для хранения информации о счёте
struct Account {
    int id;             // Уникальный идентификатор счёта
    int userId;         // ID пользователя, которому принадлежит счёт
    double balance;     // Баланс на счёте
};

// Класс для управления банковскими счетами
class AccountsManager {
private:
    std::vector<Account> accounts; // Список счетов
    std::string filename;          // Имя файла с данными счетов

    // Вспомогательный метод для парсинга строки из файла
    Account parseAccount(const std::string& line);

public:
    // Конструктор с указанием файла данных
    AccountsManager(const std::string& filename);

    // Загрузка данных из файла
    void loadAccounts();

    // Сохранение данных в файл
    void saveAccounts();

    // Добавление нового счёта
    void addAccount(const Account& account);

    // Удаление счёта по ID
    bool removeAccount(int id);

    // Получение счёта по ID
    Account* getAccountById(int id);

    // Получение всех счетов для пользователя
    std::vector<Account> getAccountsByUserId(int userId) const;

    // Получение всех счетов
    const std::vector<Account>& getAllAccounts() const;

    // Обновление баланса счёта
    bool updateBalance(int accountId, double amount);

    // Проверка существования счёта по ID
    bool accountExists(int accountId) const;

    // Получение общего баланса пользователя
    double getTotalBalanceByUserId(int userId) const;
};

#endif // ACCOUNTS_MANAGER_H
