#include "DataManager.h"
#include <sstream>

// �����������
DataManager::DataManager() {}

// ���������� ������
std::vector<std::string> DataManager::split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// ������ ������ �� �����
std::vector<std::string> DataManager::readFile(const std::string& filename) {
    std::vector<std::string> lines;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "������ �������� �����: " << filename << std::endl;
        return lines;
    }
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    file.close();
    return lines;
}

// ������ ������ � ����
void DataManager::writeFile(const std::string& filename, const std::string& data) {
    std::ofstream file(filename, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "������ ������ � ����: " << filename << std::endl;
        return;
    }
    file << data << std::endl;
    file.close();
}

// �������� �� ������������ �� �����
bool DataManager::isDuplicate(const std::string& filename, const std::string& key) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "������ �������� �����: " << filename << std::endl;
        return false;
    }
    std::string line;
    while (std::getline(file, line)) {
        if (line.find(key) != std::string::npos) {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

// �������� ������ �� �����
bool DataManager::deleteRecord(const std::string& filename, const std::string& key) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "������ �������� �����: " << filename << std::endl;
        return false;
    }
    std::vector<std::string> lines;
    std::string line;
    bool found = false;

    while (std::getline(file, line)) {
        if (line.find(key) == std::string::npos) {
            lines.push_back(line);
        }
        else {
            found = true;
        }
    }
    file.close();

    if (!found) {
        return false;
    }

    std::ofstream outFile(filename, std::ios::trunc);
    if (!outFile.is_open()) {
        std::cerr << "������ ���������� �����: " << filename << std::endl;
        return false;
    }
    for (const auto& record : lines) {
        outFile << record << std::endl;
    }
    outFile.close();
    return true;
}
