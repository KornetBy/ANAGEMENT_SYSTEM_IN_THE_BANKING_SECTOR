#include "utils.h"
#include <fstream>
#include <ctime>
#include <iostream>

Utils::Utils(const std::string& logFile)
    : logFileName(logFile)
{}

void Utils::Log(const std::string& level, const std::string& msg) {
    std::ofstream f(logFileName, std::ios::app);
    time_t t = time(NULL);
    tm lt;
    localtime_s(&lt, &t);
    char buf[64]; strftime(buf, 64, "%Y-%m-%d %H:%M:%S", &lt);
    f << buf << ";" << level << ";" << msg << "\n";
}

bool Utils::FileExists(const std::string& fn) {
    std::ifstream f(fn);
    return f.good();
}

bool Utils::ValidateID(const std::string& id) {
    if (id.empty())return false;
    for (auto c : id)if (!isdigit((unsigned char)c))return false;
    return true;
}

bool Utils::CheckFieldsCount(const std::string& line, int required_count) {
    auto p = Split(line, ';');
    return (int)p.size() >= required_count;
}

std::vector<std::string> Utils::Split(const std::string& s, char d) {
    std::vector<std::string>v; std::string tmp;
    for (auto c : s) {
        if (c == d) { v.push_back(tmp); tmp.clear(); }
        else tmp.push_back(c);
    }
    v.push_back(tmp);
    return v;
}

std::string Utils::Trimr(const std::string& s) {
    std::string r = s; while (!r.empty() && (r.back() == '\n' || r.back() == '\r'))r.pop_back(); return r;
}

void Utils::PrintError(const std::string& msg) {
    std::cerr << "Îøèáêà: " << msg << "\n";
}

void Utils::PrintInfo(const std::string& msg) {
    std::cout << msg << "\n";
}
