// Server/UserManager.h
#pragma once
#include "User.h"
#include <vector>
#include <string>
#include <mutex>

class UserManager {
private:
    std::vector<User> users;
    std::string userFileName;
    std::mutex mtx;

    void loadUsers();
public:
    UserManager(const std::string& fileName);
    User getUser(const std::string& username);
    bool addUser(const User& user);
    bool deleteUser(const std::string& username);
    bool updateUserStatus(const std::string& username, const std::string& status);
};
