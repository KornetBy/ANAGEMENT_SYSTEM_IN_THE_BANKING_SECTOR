#include "Logger.h"
#include "utils.h"
#include <fstream>
#include <ctime>
#include <sstream>

static const size_t MAX_LOG_SIZE = 1024 * 1024; // 1MB

Logger::Logger(const std::string& filename) : filename(filename) {}

void rotateLogs(const std::string& filename) {
    // CHANGED: Простейшая ротация: переименуем старый лог
    std::string rotatedName = filename + ".old";
    remove(rotatedName.c_str());
    rename(filename.c_str(), rotatedName.c_str());
}

void Logger::log(const std::string& level, const std::string& command, const std::string& message) {
    {
        std::ifstream in(filename, std::ios::ate | std::ios::binary);
        if (in.is_open()) {
            auto size = in.tellg();
            if (size > MAX_LOG_SIZE) {
                in.close();
                rotateLogs(filename);  // CHANGED: вызываем ротацию перед записью
            }
        }
    }

    std::ofstream ofs(filename, std::ios::app);
    time_t now = time(nullptr);

    // Используем localtime_s вместо localtime
    struct tm lt;
    localtime_s(&lt, &now);  // Используем безопасную версию

    char buf[64];
    strftime(buf, 64, "%Y-%m-%d %H:%M:%S", &lt);  // Передаем ссылку на структуру tm
    ofs << buf << "|" << level << "|" << command << "|" << message << "\n"; // CHANGED: без изменений, но теперь ротация работает
}

std::string Logger::getLogs() {
    return readAll(filename);
}
