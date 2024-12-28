#include "utils.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <sys/stat.h>

std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::istringstream iss(str);
    std::string tok;
    while (std::getline(iss, tok, delimiter)) {
        tokens.push_back(tok);
    }
    return tokens;
}

std::string trim(const std::string& str) {
    if (str.empty()) return str;
    auto start = str.begin();
    while (start != str.end() && std::isspace((unsigned char)*start)) start++;
    auto end = str.end();
    do { end--; } while (std::distance(start, end) > 0 && std::isspace((unsigned char)*end));
    return std::string(start, end + 1);
}

bool fileExists(const std::string& filename) {
    struct stat buffer;
    return (stat(filename.c_str(), &buffer) == 0);
}

std::string readAll(const std::string& filename) {
    std::ifstream ifs(filename);
    if (!ifs.is_open()) return "";
    std::ostringstream oss;
    oss << ifs.rdbuf();
    return oss.str();
}

void writeAll(const std::string& filename, const std::string& data) {
    std::ofstream ofs(filename, std::ios::trunc);
    ofs << data;
}
