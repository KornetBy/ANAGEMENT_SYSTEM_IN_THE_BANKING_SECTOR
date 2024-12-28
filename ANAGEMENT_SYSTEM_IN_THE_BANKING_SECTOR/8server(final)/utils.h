#pragma once
#include <string>
#include <vector>

std::vector<std::string> split(const std::string& str, char delimiter);
std::string trim(const std::string& str);
bool fileExists(const std::string& filename);
std::string readAll(const std::string& filename);
void writeAll(const std::string& filename, const std::string& data);
