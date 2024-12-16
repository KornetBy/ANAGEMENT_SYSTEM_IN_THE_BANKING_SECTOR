#include "Logger.h"
#include <sstream>
#include <ctime>

// Конструктор
Logger::Logger(const std::string& logFile) : logFile(logFile) {}

// Получение текущего времени
std::string Logger::getCurrentTime() {
    std::time_t now = std::time(nullptr);
    char buf[20];
    struct tm timeInfo;
    localtime_s(&timeInfo, &now); // Безопасная версия localtime
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &timeInfo);
    return std::string(buf);
}

// Запись лога с уровнем важности
void Logger::log(const std::string& level, const std::string& action, const std::string& message) {
    std::ofstream file(logFile, std::ios::app);
    if (file.is_open()) {
        file << "[" << getCurrentTime() << "] [" << level << "] [" << action << "] " << message << std::endl;
        file.close();
    }
    else {
        std::cerr << "Ошибка открытия файла для логирования: " << logFile << std::endl;
    }
}

// Получение всех логов
std::string Logger::getLogs() {
    std::ifstream file(logFile);
    if (!file.is_open()) {
        return "ERROR|Не удалось открыть файл логов";
    }

    std::ostringstream logs;
    logs << file.rdbuf();
    file.close();
    return logs.str();
}

// Получение логов по уровню важности
std::string Logger::getLogsByLevel(const std::string& level) {
    std::ifstream file(logFile);
    if (!file.is_open()) {
        return "ERROR|Не удалось открыть файл логов";
    }

    std::ostringstream filteredLogs;
    std::string line;

    while (std::getline(file, line)) {
        if (line.find("[" + level + "]") != std::string::npos) {
            filteredLogs << line << std::endl;
        }
    }

    file.close();
    return filteredLogs.str();
}
