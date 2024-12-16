#pragma once
#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

class DataManager {
private:
    // ��������������� ����� ��� ���������� ������
    std::vector<std::string> split(const std::string& str, char delimiter);

public:
    DataManager();

    // ������ ������ �� �����
    std::vector<std::string> readFile(const std::string& filename);

    // ������ ������ � ����
    void writeFile(const std::string& filename, const std::string& data);

    // �������� �� ������������ �� �����
    bool isDuplicate(const std::string& filename, const std::string& key);

    // �������� ������ �� �����
    bool deleteRecord(const std::string& filename, const std::string& key);
};

#endif // DATA_MANAGER_H
