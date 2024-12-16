#include "user_manager.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

// �����������
UserManager::UserManager(const std::string& filename) : filename(filename) {}

// ��������������� ����� ��� �������� ������ �� �����
User UserManager::parseUser(const std::string& line) {
    User user;
    std::istringstream iss(line);
    std::string id, username, password, role;

    // ���������� ������ �� ������� '|'
    if (!(std::getline(iss, id, '|') && std::getline(iss, username, '|') &&
        std::getline(iss, password, '|') && std::getline(iss, role, '|'))) {
        throw std::runtime_error("������������ ������ ������.");
    }

    // �������������� ID � ����� � ��������
    user.id = std::stoi(id);
    if (user.id <= 0) {
        throw std::runtime_error("������������ ID ������������: " + id);
    }

    // ���������� ���������� ��������
    user.username = username;
    user.password = password;
    user.role = role;
    return user;
}

// �������� ������ �� �����
void UserManager::loadUsers() {
    users.clear();
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "������: �� ������� ������� ���� " << filename << std::endl;
        std::ofstream errorLog("logs/error.log", std::ios::app);
        errorLog << "������: ���� " << filename << " �� ������ ��� ����������." << std::endl;
        errorLog.close();
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        try {
            // ��������� ���������� ������������
            if (std::count(line.begin(), line.end(), '|') != 3) {
                throw std::runtime_error("������������ ������ ������: " + line);
            }

            User user = parseUser(line);

            // �������� �� ������������ ID
            for (const auto& existingUser : users) {
                if (existingUser.id == user.id) {
                    throw std::runtime_error("������������� ID ������������: " + std::to_string(user.id));
                }
            }

            // ��������� ������������ ������ ���� �������� �������
            users.push_back(user);
        }
        catch (const std::exception& e) {
            std::cerr << "������: " << e.what() << " ������: " << line << std::endl;
            std::ofstream errorLog("logs/error.log", std::ios::app);
            errorLog << "������ ��� �������� ������������: " << e.what() << " ������: " << line << std::endl;
            errorLog.close();
        }
    }

    if (users.empty()) {
        std::cerr << "��������������: ���� " << filename << " ������. ������ ������������� �� ��������." << std::endl;
    }

    file.close();
}


// ���������� ������ � ����
void UserManager::saveUsers() {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "������: �� ������� ������� ���� " << filename << " ��� ������." << std::endl;
        std::ofstream errorLog("logs/error.log", std::ios::app);
        errorLog << "������: �� ������� ������� ���� " << filename << " ��� ������." << std::endl;
        errorLog.close();
        return;
    }

    for (const auto& user : users) {
        file << user.id << " | " << user.username << " | "
            << user.password << " | " << user.role << std::endl;
    }
    file.close();
}

// ���������� ������ ������������
void UserManager::addUser(const User& user) {
    // �������� ������������ ID
    for (const auto& existingUser : users) {
        if (existingUser.id == user.id) {
            std::cerr << "������: ������������ � ID " << user.id << " ��� ����������." << std::endl;
            return;
        }
        if (existingUser.username == user.username) {
            std::cerr << "������: ������������ � ������� " << user.username << " ��� ����������." << std::endl;
            return;
        }
    }

    // �������� �� ������������ ������� � ������ � ����
    if (user.username.find('|') != std::string::npos || user.role.find('|') != std::string::npos) {
        std::cerr << "������: ����� ��� ���� �������� ������������ ������ '|'" << std::endl;
        return;
    }

    users.push_back(user);
    saveUsers();
}

// �������� ������������ �� ID
bool UserManager::removeUser(int id) {
    for (auto it = users.begin(); it != users.end(); ++it) {
        if (it->id == id) {
            users.erase(it);
            saveUsers();
            return true;
        }
    }
    std::cerr << "������: ������������ � ID " << id << " �� ������." << std::endl;
    return false;
}

// ��������� ������������ �� ������
User* UserManager::getUserByUsername(const std::string& username) {
    for (auto& user : users) {
        if (user.username == username) {
            return &user;
        }
    }
    return nullptr;
}

// ��������� ���� �������������
const std::vector<User>& UserManager::getAllUsers() const {
    return users;
}

// ���������� ������ ������������
bool UserManager::updateUser(int id, const std::string& newPassword, const std::string& newRole) {
    for (auto& user : users) {
        if (user.id == id) {
            // �������� �� ������������ �������
            if (newPassword.find('|') != std::string::npos || newRole.find('|') != std::string::npos) {
                std::cerr << "������: ������ ��� ���� �������� ������������ ������ '|'" << std::endl;
                return false;
            }

            user.password = newPassword;
            user.role = newRole;
            saveUsers();
            return true;
        }
    }
    std::cerr << "������: ������������ � ID " << id << " �� ������." << std::endl;
    return false;
}
