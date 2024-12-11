#include "common.h"
#include <ctime>
#include <sstream>

std::mutex file_mutex;

std::string getCurrentTimestamp() {
    std::time_t now = std::time(nullptr);
    struct tm tm1; localtime_s(&tm1, &now);
    char buf[100];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm1);
    return std::string(buf);
}

std::vector<std::string> split(const std::string& s, char d) {
    std::vector<std::string>t; std::stringstream ss(s); std::string it;
    while (std::getline(ss, it, d))t.push_back(it);
    return t;
}
