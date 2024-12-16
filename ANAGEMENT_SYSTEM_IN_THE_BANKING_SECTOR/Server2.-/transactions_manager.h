#ifndef TRANSACTIONS_MANAGER_H
#define TRANSACTIONS_MANAGER_H

#include <string>
#include <vector>
#include "accounts_manager.h" // Для проверки существования счетов

// Структура для хранения информации о транзакции
struct Transaction {
    int id;               // Уникальный идентификатор транзакции
    int fromAccountId;    // ID счёта отправителя
    int toAccountId;      // ID счёта получателя
    double amount;        // Сумма транзакции
    std::string timestamp; // Метка времени (в формате "YYYY-MM-DD HH:MM:SS")
};

// Класс для управления транзакциями
class TransactionsManager {
private:
    std::vector<Transaction> transactions; // Список транзакций
    std::string filename;                  // Имя файла с данными транзакций

    // Вспомогательный метод для парсинга строки из файла
    Transaction parseTransaction(const std::string& line);

    // Генерация уникального ID для новой транзакции
    int generateTransactionId() const;

public:
    // Конструктор с указанием файла данных
    TransactionsManager(const std::string& filename);

    // Загрузка данных из файла
    void loadTransactions();

    // Сохранение данных в файл
    void saveTransactions();

    // Добавление новой транзакции с проверкой счетов
    bool addTransaction(const Transaction& transaction, const AccountsManager& accountsManager);

    // Получение транзакций по ID счёта
    std::vector<Transaction> getTransactionsByAccountId(int accountId) const;

    // Получение всех транзакций
    const std::vector<Transaction>& getAllTransactions() const;
};

#endif // TRANSACTIONS_MANAGER_H
