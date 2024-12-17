// Server/Logger.cpp
#include "Logger.h"

Logger::Logger(const std::string& fileName) : logFileName(fileName) {}

void Logger::log(LogLevel level, const std::string& command, const std::string& message) {
    std::lock_guard<std::mutex> lock(mtx);
    std::ofstream logFile(logFileName, std::ios::app);
    if (logFile.is_open()) {
        logFile << "[" << getCurrentTime() << "] "
            << "[" << logLevelToString(level) << "] "
            << "[" << command << "] "
            << message << std::endl;
        logFile.close();
    }
    else {
        std::cerr << "Не удалось открыть лог-файл: " << logFileName << std::endl;
    }
}

std::string Logger::getLogs() {
    std::lock_guard<std::mutex> lock(mtx);
    std::ifstream logFile(logFileName);
    if (!logFile.is_open()) {
        return "ERROR|Не удалось открыть лог-файл.";
    }
    std::string logs((std::istreambuf_iterator<char>(logFile)),
        std::istreambuf_iterator<char>());
    logFile.close();
    return logs.empty() ? "Логи пусты." : logs;
}
