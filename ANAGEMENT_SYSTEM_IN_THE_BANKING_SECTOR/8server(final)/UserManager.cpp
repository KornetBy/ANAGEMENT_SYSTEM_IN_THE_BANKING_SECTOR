#include "UserManager.h"
#include "utils.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdexcept>
UserManager::UserManager(const std::string& filename) : filename(filename) {
    load();
}

void UserManager::load() {
    users.clear();
    if (!fileExists(filename)) return;
    std::string data = readAll(filename);
    std::istringstream iss(data);
    std::string line;
    while (std::getline(iss, line)) {
        line = trim(line);
        if (line.empty()) continue;
        auto parts = split(line, '|');
        if (parts.size() < 4) continue;
        User u{ parts[0], parts[1], parts[2], parts[3] };
        users.push_back(u);
    }
}

void UserManager::save() {
    std::ostringstream oss;
    for (auto& u : users) {
        oss << u.username << "|" << u.password << "|" << u.role << "|" << u.status << "\n";
    }
    writeAll(filename, oss.str());
}

User UserManager::getUser(const std::string& username) {
    std::lock_guard<std::mutex> lock(mtx);
    for (auto& u : users) {
        if (u.username == username) return u;
    }
    return User{ "","","","" };
}

bool UserManager::addUser(const User& user) {
    std::lock_guard<std::mutex> lock(mtx);
    for (auto& u : users) {
        if (u.username == user.username) return false;
    }
    users.push_back(user);
    save();
    return true;
}

bool UserManager::deleteUser(const std::string& username) {
    std::lock_guard<std::mutex> lock(mtx);
    for (size_t i = 0; i < users.size(); i++) {
        if (users[i].username == username) {
            users.erase(users.begin() + i);
            save();
            return true;
        }
    }
    return false;
}

bool UserManager::updateUserStatus(const std::string& username, const std::string& status) {
    std::lock_guard<std::mutex> lock(mtx);
    for (auto& u : users) {
        if (u.username == username) {
            u.status = status;
            save();
            return true;
        }
    }
    return false;
}

std::vector<User> UserManager::getAllUsers() {
    std::lock_guard<std::mutex> lock(mtx);
    return users;
}
