#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <iostream>
#include <ctime>

class Logger {
private:
    std::string logFile;  // Имя файла для логов

    // Получение текущего времени в формате [YYYY-MM-DD HH:MM:SS]
    std::string getCurrentTime();

public:
    Logger(const std::string& logFile = "data/history_log.txt"); // Конструктор с указанием файла
    void log(const std::string& action, const std::string& message); // Запись лога
};

#endif // LOGGER_H
