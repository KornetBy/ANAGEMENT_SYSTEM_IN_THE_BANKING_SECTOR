#pragma once
#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

class DataManager {

    

public:
    DataManager();
    std::vector<std::string> split(const std::string& str, char delimiter);
    // Чтение данных из файла
    std::vector<std::string> readFile(const std::string& filename);

    // Запись данных в файл
    void writeFile(const std::string& filename, const std::string& data);

    // Проверка на дублирование по ключу
    bool isDuplicate(const std::string& filename, const std::string& key);

    // Удаление записи из файла
    bool deleteRecord(const std::string& filename, const std::string& key);
};

#endif // DATA_MANAGER_H
