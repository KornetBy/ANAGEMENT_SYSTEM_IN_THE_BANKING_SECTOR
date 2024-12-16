#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <iostream>
#include <vector>

class Logger {
private:
    std::string logFile; // ��� ����� ��� �����

    // ��������� �������� ������� � ������� [YYYY-MM-DD HH:MM:SS]
    std::string getCurrentTime();

public:
    Logger(const std::string& logFile = "data/history_log.txt");

    // ������ ���� � ������� ��������
    void log(const std::string& level, const std::string& action, const std::string& message);


    // ��������� ����������� �����
    std::string getLogs();

    // ��������� ����� �� ������ ��������
    std::string getLogsByLevel(const std::string& level);
};

#endif // LOGGER_H
