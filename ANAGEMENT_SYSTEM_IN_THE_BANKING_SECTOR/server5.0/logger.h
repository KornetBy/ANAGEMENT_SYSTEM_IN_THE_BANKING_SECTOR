#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <iostream>
#include <ctime>

class Logger {
private:
    std::string logFile;  // ��� ����� ��� �����

    // ��������� �������� ������� � ������� [YYYY-MM-DD HH:MM:SS]
    std::string getCurrentTime();

public:
    Logger(const std::string& logFile = "data/history_log.txt"); // ����������� � ��������� �����
    void log(const std::string& action, const std::string& message); // ������ ����
};

#endif // LOGGER_H
