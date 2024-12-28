// UserManager.cpp
#include "UserManager.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdexcept>

// �����������: ��������� ������������� �� ����� ��� �������������
UserManager::UserManager(const std::string& filename) : userFileName(filename) {
    std::lock_guard<std::mutex> lock(mtx);
    std::ifstream infile(userFileName);
    if (!infile.is_open()) {
        // ���� ���� �� ����������, ������� ���
        std::ofstream outfile(userFileName, std::ios::app);
        outfile.close();
        return;
    }

    std::string line;
    while (getline(infile, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        std::string token;
        User user;

        getline(iss, user.username, '|');
        getline(iss, user.password, '|');
        getline(iss, user.role, '|');
        getline(iss, user.status, '|');

        users.push_back(user);
    }
    infile.close();
}

// �������� ������������ �� �����
User UserManager::getUser(const std::string& username) {
    std::lock_guard<std::mutex> lock(mtx);
    for (const auto& user : users) {
        if (user.username == username) {
            return user;
        }
    }
    throw std::runtime_error("������������ �� ������");
}

// �������� ������ ������������
bool UserManager::addUser(const User& user) {
    std::lock_guard<std::mutex> lock(mtx);
    // �������� �� ������������� ������������
    auto it = std::find_if(users.begin(), users.end(),
        [&](const User& u) { return u.username == user.username; });
    if (it != users.end()) {
        return false; // ������������ ��� ����������
    }

    users.push_back(user);

    // ������ � ����
    std::ofstream outfile(userFileName, std::ios::app);
    if (!outfile.is_open()) {
        users.pop_back(); // ����� ���������
        return false;
    }
    outfile << user.username << "|" << user.password << "|"
        << user.role << "|" << user.status << "|\n";
    outfile.close();
    return true;
}

// ������� ������������ �� �����
bool UserManager::deleteUser(const std::string& username) {
    std::lock_guard<std::mutex> lock(mtx);
    auto it = std::remove_if(users.begin(), users.end(),
        [&](const User& u) { return u.username == username; });
    if (it == users.end()) {
        return false; // ������������ �� ������
    }
    users.erase(it, users.end());

    // ���������� ����� ��� ���������� ������������
    std::ofstream outfile(userFileName, std::ios::trunc);
    if (!outfile.is_open()) {
        return false;
    }
    for (const auto& user : users) {
        outfile << user.username << "|" << user.password << "|"
            << user.role << "|" << user.status << "|\n";
    }
    outfile.close();
    return true;
}

// �������� ������ ������������
bool UserManager::updateUserStatus(const std::string& username, const std::string& status) {
    std::lock_guard<std::mutex> lock(mtx);
    bool found = false;
    for (auto& user : users) {
        if (user.username == username) {
            user.status = status;
            found = true;
            break;
        }
    }
    if (!found) {
        return false; // ������������ �� ������
    }

    // ���������� ����� � ����������� ��������
    std::ofstream outfile(userFileName, std::ios::trunc);
    if (!outfile.is_open()) {
        return false;
    }
    for (const auto& user : users) {
        outfile << user.username << "|" << user.password << "|"
            << user.role << "|" << user.status << "|\n";
    }
    outfile.close();
    return true;
}
