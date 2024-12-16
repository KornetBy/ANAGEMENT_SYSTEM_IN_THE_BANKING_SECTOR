#pragma once
#ifndef ROLEMANAGER_H
#define ROLEMANAGER_H

#include <string>

class RoleManager {
public:
    // Обработка функционала в зависимости от роли
    static void handleRole(const std::string& role);
};

#endif // ROLEMANAGER_H
