#ifndef CLIENT_INTERFACE_H
#define CLIENT_INTERFACE_H

#include <string>

// Отображение меню для каждой роли
void showAdminMenu();
void showClientMenu();
void showWorkerMenu();
void showHRMenu();

// Получение выбора пользователя
int getUserChoice();

#endif // CLIENT_INTERFACE_H
