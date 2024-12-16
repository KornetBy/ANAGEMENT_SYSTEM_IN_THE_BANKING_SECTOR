#include "UserManager.h"
#include <fstream>
#include <sstream>

// �����������
UserManager::UserManager(const std::string& userFile) {
    loadUsers(userFile);
}

// �������� ������������� �� �����
void UserManager::loadUsers(const std::string& filename) {
    std::vector<std::string> lines = dataManager.readFile(filename);
    for (const auto& line : lines) {
        std::vector<std::string> parts = dataManager.split(line, '|');
        if (parts.size() >= 2) {
            std::string username = parts[0];
            std::string role = parts[1];
            userRoles[username] = role;
            userStatus[username] = true; // ��� ������������ �� ��������� �������
        }
    }
}

// ��������� ���� ������������
std::string UserManager::getRole(const std::string& username) {
    if (userRoles.find(username) != userRoles.end()) {
        return userRoles[username];
    }
    return "";
}

// ��������, ������� �� ������������
bool UserManager::isUserActive(const std::string& username) {
    if (userStatus.find(username) != userStatus.end()) {
        return userStatus[username];
    }
    return false;
}

// ���������� ������������
bool UserManager::blockUser(const std::string& username) {
    if (userStatus.find(username) != userStatus.end()) {
        userStatus[username] = false;
        return true;
    }
    return false;
}

// ���������� ������ ������������
bool UserManager::addUser(const std::string& username, const std::string& role) {
    if (userRoles.find(username) == userRoles.end()) {
        userRoles[username] = role;
        userStatus[username] = true;
        dataManager.writeFile("data/users.txt", username + "|" + role);
        return true;
    }
    return false;
}
