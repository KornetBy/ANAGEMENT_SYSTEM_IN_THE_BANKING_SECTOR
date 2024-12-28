#pragma once
#include <string>
#include <vector>
#include <mutex>

struct User {
    std::string username;
    std::string password;
    std::string role;
    std::string status;
};

class UserManager {
private:
    std::string filename;
    std::vector<User> users;
    std::mutex mtx;
    void load();
    void save();
public:
    UserManager(const std::string& filename);
    User getUser(const std::string& username);
    bool addUser(const User& user);
    bool deleteUser(const std::string& username);
    bool updateUserStatus(const std::string& username, const std::string& status);
    std::vector<User> getAllUsers();
};
