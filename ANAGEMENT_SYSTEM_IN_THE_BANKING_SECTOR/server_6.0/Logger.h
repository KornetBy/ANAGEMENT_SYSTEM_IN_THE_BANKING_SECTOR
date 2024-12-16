#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <iostream>
#include <vector>

class Logger {
private:
    std::string logFile; // Имя файла для логов

    // Получение текущего времени в формате [YYYY-MM-DD HH:MM:SS]
    std::string getCurrentTime();

public:
    Logger(const std::string& logFile = "data/history_log.txt");

    // Запись лога с уровнем важности
    void log(const std::string& level, const std::string& action, const std::string& message);


    // Получение содержимого логов
    std::string getLogs();

    // Получение логов по уровню важности
    std::string getLogsByLevel(const std::string& level);
};

#endif // LOGGER_H
