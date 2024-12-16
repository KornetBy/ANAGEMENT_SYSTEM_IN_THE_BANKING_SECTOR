#include "accounts_manager.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

// Конструктор
AccountsManager::AccountsManager(const std::string& filename) : filename(filename) {}

// Вспомогательный метод для парсинга строки из файла
Account AccountsManager::parseAccount(const std::string& line) {
    Account account;
    std::istringstream iss(line);
    std::string id, userId, balance;

    // Разделение строки по символу '|'
    if (!(std::getline(iss, id, '|') && std::getline(iss, userId, '|') && std::getline(iss, balance, '|'))) {
        throw std::runtime_error("Некорректный формат строки.");
    }

    // Преобразование ID и баланса в числа
    account.id = std::stoi(id);
    account.userId = std::stoi(userId);
    account.balance = std::stod(balance);

    // Проверки на корректность данных
    if (account.id <= 0) {
        throw std::runtime_error("Некорректный ID счёта: " + id);
    }
    if (account.userId <= 0) {
        throw std::runtime_error("Некорректный ID пользователя: " + userId);
    }
    if (account.balance < 0) {
        throw std::runtime_error("Баланс не может быть отрицательным: " + balance);
    }

    return account;
}

// Загрузка данных из файла
void AccountsManager::loadAccounts() {
    accounts.clear();
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
            Account account = parseAccount(line);

            // Проверка на уникальность ID
            for (const auto& existingAccount : accounts) {
                if (existingAccount.id == account.id) {
                    throw std::runtime_error("Дублирующийся ID счёта: " + std::to_string(account.id));
                }
            }

            accounts.push_back(account);
        }
        catch (const std::exception& e) {
            std::cerr << "Ошибка: " << e.what() << " Строка: " << line << std::endl;
            std::ofstream errorLog("logs/error.log", std::ios::app);
            errorLog << "Ошибка при загрузке счёта: " << e.what() << " Строка: " << line << std::endl;
            errorLog.close();
        }
    }

    file.close();
}

// Сохранение данных в файл
void AccountsManager::saveAccounts() {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл " << filename << " для записи." << std::endl;
        std::ofstream errorLog("logs/error.log", std::ios::app);
        errorLog << "Ошибка: не удалось открыть файл " << filename << " для записи." << std::endl;
        errorLog.close();
        return;
    }

    for (const auto& account : accounts) {
        file << account.id << " | " << account.userId << " | " << account.balance << std::endl;
    }
    file.close();
}

// Добавление нового счёта
void AccountsManager::addAccount(const Account& account) {
    // Проверка уникальности ID
    for (const auto& existingAccount : accounts) {
        if (existingAccount.id == account.id) {
            std::cerr << "Ошибка: счёт с ID " << account.id << " уже существует." << std::endl;
            return;
        }
    }

    accounts.push_back(account);
    saveAccounts();
}

// Удаление счёта по ID
bool AccountsManager::removeAccount(int id) {
    for (auto it = accounts.begin(); it != accounts.end(); ++it) {
        if (it->id == id) {
            accounts.erase(it);
            saveAccounts();
            return true;
        }
    }
    std::cerr << "Ошибка: счёт с ID " << id << " не найден." << std::endl;
    return false;
}

// Получение счёта по ID
Account* AccountsManager::getAccountById(int id) {
    for (auto& account : accounts) {
        if (account.id == id) {
            return &account;
        }
    }
    return nullptr;
}

// Получение всех счетов для пользователя
std::vector<Account> AccountsManager::getAccountsByUserId(int userId) const {
    std::vector<Account> result;
    for (const auto& account : accounts) {
        if (account.userId == userId) {
            result.push_back(account);
        }
    }
    return result;
}

// Получение всех счетов
const std::vector<Account>& AccountsManager::getAllAccounts() const {
    return accounts;
}

// Обновление баланса счёта
bool AccountsManager::updateBalance(int accountId, double amount) {
    for (auto& account : accounts) {
        if (account.id == accountId) {
            if (account.balance + amount < 0) {
                std::cerr << "Ошибка: недостаточно средств на счёте ID " << accountId << std::endl;
                return false;
            }
            account.balance += amount;
            saveAccounts();
            return true;
        }
    }
    std::cerr << "Ошибка: счёт с ID " << accountId << " не найден." << std::endl;
    return false;
}

// Проверка существования счёта по ID
bool AccountsManager::accountExists(int accountId) const {
    for (const auto& account : accounts) {
        if (account.id == accountId) {
            return true;
        }
    }
    return false;
}

// Получение общего баланса пользователя
double AccountsManager::getTotalBalanceByUserId(int userId) const {
    double total = 0.0;
    for (const auto& account : accounts) {
        if (account.userId == userId) {
            total += account.balance;
        }
    }
    return total;
}
