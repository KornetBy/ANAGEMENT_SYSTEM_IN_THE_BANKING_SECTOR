#include "client_interface.h"
#include <iostream>

// Меню администратора
void showAdminMenu() {
    std::cout << "\n=== Меню администратора ===\n";
    std::cout << "1. Добавить пользователя\n";
    std::cout << "2. Удалить пользователя\n";
    std::cout << "3. Изменить роль пользователя\n";
    std::cout << "4. Просмотреть всех пользователей\n";
    std::cout << "5. Выход\n";
}

// Меню клиента
void showClientMenu() {
    std::cout << "\n=== Меню клиента ===\n";
    std::cout << "1. Просмотр счетов\n";
    std::cout << "2. Перевод средств\n";
    std::cout << "3. Просмотр выписки\n";
    std::cout << "4. Выход\n";
}

// Меню сотрудника банка
void showWorkerMenu() {
    std::cout << "\n=== Меню сотрудника банка ===\n";
    std::cout << "1. Блокировка счёта\n";
    std::cout << "2. Разблокировка счёта\n";
    std::cout << "3. Обработка кредитного запроса\n";
    std::cout << "4. Генерация отчёта\n";
    std::cout << "5. Выход\n";
}

// Меню менеджера по персоналу
void showHRMenu() {
    std::cout << "\n=== Меню менеджера по персоналу ===\n";
    std::cout << "1. Управление сотрудниками\n";
    std::cout << "2. Генерация отчёта\n";
    std::cout << "3. Выход\n";
}

// Получение выбора пользователя
int getUserChoice() {
    int choice;
    std::cout << "Введите номер команды: ";
    if (!(std::cin >> choice)) {
        std::cin.clear(); // Очистка состояния потока
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Пропуск некорректного ввода
        std::cerr << "Некорректный ввод. Попробуйте снова.\n";
        return -1; // Возвращаем код ошибки
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Пропуск оставшегося ввода
    return choice;
}

