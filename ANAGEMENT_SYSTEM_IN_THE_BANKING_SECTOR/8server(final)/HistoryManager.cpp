#include "HistoryManager.h"
#include "utils.h"
#include <fstream>
#include <ctime>

HistoryManager::HistoryManager(const std::string& filename) : filename(filename) {}

void HistoryManager::addHistory(const std::string& username, const std::string& command, const std::string& details) {
    std::ofstream ofs(filename, std::ios::app);
    time_t now = time(nullptr);

    // Используем localtime_s вместо localtime
    struct tm lt;
    localtime_s(&lt, &now);

    char buf[64];
    strftime(buf, 64, "%Y-%m-%d %H:%M:%S", &lt); // Используем &lt, а не lt
    ofs << buf << "|" << username << "|" << command << "|" << details << "\n";
}

std::string HistoryManager::getHistory() {
    return readAll(filename);
}
