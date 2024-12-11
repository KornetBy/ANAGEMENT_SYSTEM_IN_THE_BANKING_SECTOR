#include "audit.h"
#include <fstream>
#include <mutex>
#include <ctime>

static std::mutex audit_mutex;

static std::string currentTime() {
    std::time_t now = std::time(nullptr);
    struct tm timeInfo;
    if (localtime_s(&timeInfo, &now) != 0) {
        // Обработка ошибки при преобразовании времени
        return "";
    }
    char buf[50];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &timeInfo);
    return std::string(buf);
}


void auditLog(const std::string& message) {
    std::lock_guard<std::mutex> lock(audit_mutex);
    std::ofstream file("audit.log", std::ios::app);
    file << currentTime() << " " << message << "\n";
}
