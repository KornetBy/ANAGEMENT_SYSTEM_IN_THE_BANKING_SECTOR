#pragma once
#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include <string>
#include <unordered_map>
#include <vector>
#include "DataManager.h"

class UserManager {
private:
    std::unordered_map<std::string, std::string> userRoles; // ����� ������� � �����
    std::unordered_map<std::string, bool> userStatus;      // ����� ������� � �������� (�������/������������)
    DataManager dataManager;                               // ������ � ���������� �������

    void loadUsers(const std::string& filename);           // �������� ������������� �� �����

public:
    UserManager(const std::string& userFile);

    std::string getRole(const std::string& username);      // ��������� ���� ������������
    bool isUserActive(const std::string& username);        // ��������, ������� �� ������������
    bool blockUser(const std::string& username);           // ���������� ������������
    bool addUser(const std::string& username, const std::string& role); // ���������� ������ ������������
};

#endif // USER_MANAGER_H
