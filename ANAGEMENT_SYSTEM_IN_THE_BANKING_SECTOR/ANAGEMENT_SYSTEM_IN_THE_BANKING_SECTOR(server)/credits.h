#pragma once
#pragma once
#include <string>
std::string requestCredit(int userId, double amount, int duration);
std::string approveCredit(int creditId);
std::string setCreditType(int creditId, const std::string& type);
std::string setCreditTerm(int creditId, int term);
