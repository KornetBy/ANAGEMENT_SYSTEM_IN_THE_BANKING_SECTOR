#pragma once
#pragma once
#include <string>
extern int PORT;
std::string handleRequest(const std::string& request, int& authenticatedUserId, std::string& userRole);
