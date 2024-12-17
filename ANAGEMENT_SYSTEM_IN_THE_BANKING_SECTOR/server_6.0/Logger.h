// Server/Logger.h
#pragma once
#include <string>
#include <fstream>
#include <mutex>
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>

// ������������ ������� �����������
enum class LogLevel {
    INFO,
    WARNING,
    ERR
};

class Logger {
private:
    std::string logFileName;
    std::mutex mtx;

    // ��������� �������� ������� � ������� "YYYY-MM-DD HH:MM:SS"
    std::string getCurrentTime() {
        std::time_t now = std::time(nullptr);
        std::tm tm_obj;
        // ������������� localtime_s ������ localtime
        if (localtime_s(&tm_obj, &now) != 0) {
            return "1970-01-01 00:00:00"; // ���������� ��������� �������� � ������ ������
        }
        std::ostringstream oss;
        oss << std::put_time(&tm_obj, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

    // �������������� ������ ����������� � ������
    std::string logLevelToString(LogLevel level) {
        switch (level) {
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERR: return "ERROR";
        default: return "UNKNOWN";
        }
    }

public:
    Logger(const std::string& fileName);
    void log(LogLevel level, const std::string& command, const std::string& message);
    std::string getLogs();
};
