#include "auth.h"
#include "data_access.h"
#include "utils.h"

AuthService::AuthService(DataAccess& dataAccess, Utils& u)
    : da(dataAccess), utils(u)
{}

bool AuthService::Authenticate(const std::string& user, const std::string& pass, std::string& role) {
    auto us = da.LoadData("users.txt");
    for (auto& x : us) {
        auto p1 = x.find(';');
        auto p2 = x.rfind(';');
        if (p1 == std::string::npos || p2 == std::string::npos || p1 == p2) continue;
        std::string uu = x.substr(0, p1);
        std::string pp = x.substr(p1 + 1, p2 - p1 - 1);
        std::string rr = x.substr(p2 + 1);
        if (uu == user && pp == pass) {
            role = rr;
            utils.Log("INFO", "Пользователь " + user + " авторизован как " + rr);
            return true;
        }
    }
    utils.Log("WARN", "Ошибка аутентификации для пользователя " + user);
    return false;
}
