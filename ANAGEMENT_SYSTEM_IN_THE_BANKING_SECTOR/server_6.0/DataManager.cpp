#include "DataManager.h"
#include <sstream>

// Конструктор
DataManager::DataManager() {}

// Разделение строки
std::vector<std::string> DataManager::split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// Чтение данных из файла
std::vector<std::string> DataManager::readFile(const std::string& filename) {
    std::vector<std::string> lines;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Ошибка открытия файла: " << filename << std::endl;
        return lines;
    }
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    file.close();
    return lines;
}

// Запись данных в файл
void DataManager::writeFile(const std::string& filename, const std::string& data) {
    std::ofstream file(filename, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Ошибка записи в файл: " << filename << std::endl;
        return;
    }
    file << data << std::endl;
    file.close();
}

// Проверка на дублирование по ключу
bool DataManager::isDuplicate(const std::string& filename, const std::string& key) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Ошибка открытия файла: " << filename << std::endl;
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

// Удаление записи из файла
bool DataManager::deleteRecord(const std::string& filename, const std::string& key) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Ошибка открытия файла: " << filename << std::endl;
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
        std::cerr << "Ошибка перезаписи файла: " << filename << std::endl;
        return false;
    }
    for (const auto& record : lines) {
        outFile << record << std::endl;
    }
    outFile.close();
    return true;
}
