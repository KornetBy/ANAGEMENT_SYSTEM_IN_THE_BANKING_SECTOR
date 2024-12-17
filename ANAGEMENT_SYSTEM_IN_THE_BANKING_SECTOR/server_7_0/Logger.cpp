// Logger.cpp
#include "Logger.h"
#include <iostream>

// Конструктор открывает файл для записи логов
Logger::Logger(const std::string& filename) {
    logFile.open(filename, std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "Не удалось открыть файл логов: " << filename << std::endl;
    }
}

// Деструктор закрывает файл
Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

// Метод логирования
void Logger::log(const std::string& timestamp, const std::string& action, const std::string& description) {
    std::lock_guard<std::mutex> lock(mtx);
    if (logFile.is_open()) {
        logFile << timestamp << "|" << action << "|" << description << std::endl;
    }
    else {
        std::cerr << "Файл логов не открыт. Не удалось записать лог." << std::endl;
    }
}
