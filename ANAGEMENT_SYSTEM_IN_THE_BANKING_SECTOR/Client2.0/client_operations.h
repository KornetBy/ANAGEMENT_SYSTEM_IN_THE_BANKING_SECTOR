#pragma once
#ifndef CLIENT_OPERATIONS_H
#define CLIENT_OPERATIONS_H

#include <string>
#include <winsock2.h>

// Функции для выполнения операций
void executeAdminCommand(SOCKET clientSocket, int choice);
void executeClientCommand(SOCKET clientSocket, int choice);
void executeWorkerCommand(SOCKET clientSocket, int choice);
void executeHRCommand(SOCKET clientSocket, int choice);

#endif // CLIENT_OPERATIONS_H
