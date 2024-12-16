#pragma once
#include <string>
#include "data_access.h"
#include "auth.h"
#include "utils.h"

class CommandHandler {
private:
    Utils& utils;
    DataAccess& da;
    AuthService& auth;
    std::string& role;
    std::string user;

public:
    CommandHandler(std::string& r, Utils& u, DataAccess& d, AuthService& a);

    std::string Handle(const std::string& cmdline);

private:
    std::string cmdLogin(const std::string& rest);
    std::string cmdHelp();
    std::string cmdExit();
    std::string cmdAddEmployee(const std::string& rest);
    std::string cmdDelEmployee(const std::string& rest);
    std::string cmdAddRole(const std::string& rest);
    std::string cmdChangeUserRole(const std::string& rest);
    std::string cmdSubmitCompensation(const std::string& rest);
    std::string cmdApproveCompensation(const std::string& rest);
    std::string cmdEvalPerformance(const std::string& rest);
    std::string cmdGetReports();
    std::string cmdSubmitLoan(const std::string& rest);
    std::string cmdApproveLoan(const std::string& rest);
    std::string cmdGetLogs();
    std::string cmdSubmitTransaction(const std::string& rest);
    std::string cmdApproveTransaction(const std::string& rest);

    bool isAuthorized() const { return !role.empty(); }
};
