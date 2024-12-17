// Logger.cpp
#include "Logger.h"
#include <iostream>

// ����������� ��������� ���� ��� ������ �����
Logger::Logger(const std::string& filename) {
    logFile.open(filename, std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "�� ������� ������� ���� �����: " << filename << std::endl;
    }
}

// ���������� ��������� ����
Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

// ����� �����������
void Logger::log(const std::string& timestamp, const std::string& action, const std::string& description) {
    std::lock_guard<std::mutex> lock(mtx);
    if (logFile.is_open()) {
        logFile << timestamp << "|" << action << "|" << description << std::endl;
    }
    else {
        std::cerr << "���� ����� �� ������. �� ������� �������� ���." << std::endl;
    }
}
