#pragma once

#include <string>
#include <vector>
#include <mutex>

extern std::mutex file_mutex;
std::string getCurrentTimestamp();
std::vector<std::string> split(const std::string& s, char delimiter);
