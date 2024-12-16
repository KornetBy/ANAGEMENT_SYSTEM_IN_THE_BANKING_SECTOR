#pragma once
#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include <string>
#include <vector>

// ��������� ������������
struct User {
    int id;                 // ���������� ������������� ������������
    std::string username;   // ��� ������������
    std::string password;   // ������ ������������
    std::string role;       // ���� ������������ ("admin", "client", "worker", "hr")
};

// ����� ��� ���������� ��������������
class UserManager {
private:
    std::vector<User> users;    // ������ �������������
    std::string filename;       // ��� ����� � ������� �������������

    // ��������������� ����� ��� �������� ������ �� �����
    User parseUser(const std::string& line);

public:
    // ����������� � ��������� ����� ������
    UserManager(const std::string& filename);

    // �������� ������ �� �����
    void loadUsers();

    // ���������� ������ � ����
    void saveUsers();

    // ���������� ������ ������������
    void addUser(const User& user);

    // �������� ������������ �� ID
    bool removeUser(int id);

    // ��������� ������������ �� ������
    User* getUserByUsername(const std::string& username);

    // ��������� ���� �������������
    const std::vector<User>& getAllUsers() const;

    // ���������� ������ ������������
    bool updateUser(int id, const std::string& newPassword, const std::string& newRole);
};

#endif // USER_MANAGER_H

