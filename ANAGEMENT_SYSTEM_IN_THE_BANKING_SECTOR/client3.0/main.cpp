#include "Auth.h"
#include "RoleManager.h"
#include <iostream>
#include <string>
#include <Windows.h>

void displayWelcomeMessage() {
    std::cout << "=====================================\n";
    std::cout << " Клиентская часть системы управления \n";
    std::cout << " персоналом банка\n";
    std::cout << "=====================================\n";
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    try {
        displayWelcomeMessage();

        std::string username, role;

        // Авторизация пользователя
        while (!Auth::login(username, role)) {
            std::cout << "Ошибка авторизации. Попробуйте снова.\n";
        }

        std::cout << "Добро пожаловать, " << username << "! Ваша роль: " << role << "\n";

        // Передача управления в зависимости от роли
        RoleManager::handleRole(role);

    }
    catch (const std::exception& e) {
        std::cerr << "Критическая ошибка: " << e.what() << "\n";
    }
    catch (...) {
        std::cerr << "Произошла неизвестная ошибка.\n";
    }

    std::cout << "Завершение работы клиента.\n";
    return 0;
}
