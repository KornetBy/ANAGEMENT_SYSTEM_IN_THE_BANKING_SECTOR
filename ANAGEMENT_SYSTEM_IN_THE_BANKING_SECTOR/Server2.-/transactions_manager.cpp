#include "transactions_manager.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

// Конструктор
TransactionsManager::TransactionsManager(const std::string& filename) : filename(filename) {}

// Вспомогательный метод для парсинга строки из файла
Transaction TransactionsManager::parseTransaction(const std::string& line) {
    Transaction transaction;
    std::istringstream iss(line);
    std::string id, fromAccountId, toAccountId, amount, timestamp;

    // Разделение строки по символу '|'
    if (!(std::getline(iss, id, '|') && std::getline(iss, fromAccountId, '|') &&
        std::getline(iss, toAccountId, '|') && std::getline(iss, amount, '|') &&
        std::getline(iss, timestamp, '|'))) {
        throw std::runtime_error("Некорректный формат строки.");
    }

    // Преобразование полей в нужные типы
    transaction.id = std::stoi(id);
    transaction.fromAccountId = std::stoi(fromAccountId);
    transaction.toAccountId = std::stoi(toAccountId);
    transaction.amount = std::stod(amount);
    transaction.timestamp = timestamp;

    // Проверки на корректность данных
    if (transaction.id <= 0) {
        throw std::runtime_error("Некорректный ID транзакции: " + id);
    }
    if (transaction.amount <= 0) {
        throw std::runtime_error("Сумма транзакции должна быть положительной: " + amount);
    }

    return transaction;
}

// Генерация уникального ID транзакции
int TransactionsManager::generateTransactionId() const {
    int maxId = 0;
    for (const auto& transaction : transactions) {
        if (transaction.id > maxId) {
            maxId = transaction.id;
        }
    }
    return maxId + 1;
}

// Загрузка данных из файла
void TransactionsManager::loadTransactions() {
    transactions.clear();
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл " << filename << std::endl;
        std::ofstream errorLog("logs/error.log", std::ios::app);
        errorLog << "Ошибка: файл " << filename << " не найден или недоступен." << std::endl;
        errorLog.close();
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        try {
            transactions.push_back(parseTransaction(line));
        }
        catch (const std::exception& e) {
            std::cerr << "Ошибка: " << e.what() << " Строка: " << line << std::endl;
            std::ofstream errorLog("logs/error.log", std::ios::app);
            errorLog << "Ошибка при загрузке транзакции: " << e.what() << " Строка: " << line << std::endl;
            errorLog.close();
        }
    }

    file.close();
}

// Сохранение данных в файл
void TransactionsManager::saveTransactions() {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл " << filename << " для записи." << std::endl;
        std::ofstream errorLog("logs/error.log", std::ios::app);
        errorLog << "Ошибка: не удалось открыть файл " << filename << " для записи." << std::endl;
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

// Добавление новой транзакции с проверкой счетов
bool TransactionsManager::addTransaction(const Transaction& transaction, const AccountsManager& accountsManager) {
    // Проверка существования счетов отправителя и получателя
    if (!accountsManager.accountExists(transaction.fromAccountId)) {
        std::cerr << "Ошибка: счёт отправителя с ID " << transaction.fromAccountId << " не существует." << std::endl;
        return false;
    }
    if (!accountsManager.accountExists(transaction.toAccountId)) {
        std::cerr << "Ошибка: счёт получателя с ID " << transaction.toAccountId << " не существует." << std::endl;
        return false;
    }

    // Генерация уникального ID, если он не задан
    Transaction newTransaction = transaction;
    if (newTransaction.id <= 0) {
        newTransaction.id = generateTransactionId();
    }

    // Добавление транзакции
    transactions.push_back(newTransaction);
    saveTransactions();
    return true;
}

// Получение транзакций по ID счёта
std::vector<Transaction> TransactionsManager::getTransactionsByAccountId(int accountId) const {
    std::vector<Transaction> result;
    for (const auto& transaction : transactions) {
        if (transaction.fromAccountId == accountId || transaction.toAccountId == accountId) {
            result.push_back(transaction);
        }
    }
    return result;
}

// Получение всех транзакций
const std::vector<Transaction>& TransactionsManager::getAllTransactions() const {
    return transactions;
}
