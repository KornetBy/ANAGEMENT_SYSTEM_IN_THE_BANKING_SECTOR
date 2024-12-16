#pragma once
#include <string>

class DataAccess;
class Utils;

class AuthService {
private:
    DataAccess& da;
    Utils& utils;
public:
    AuthService(DataAccess& dataAccess, Utils& u);

    bool Authenticate(const std::string& user, const std::string& pass, std::string& role);
};
