// HistoryManager.cpp
#include "HistoryManager.h"
#include <ctime>
#include <iomanip>
#include <sstream>

void HistoryManager::addHistory(const std::string& username, const std::string& action, const std::string& details) {
    std::lock_guard<std::mutex> lock(mtx);
    std::ofstream ofs(historyFileName, std::ios::app);
    if (!ofs.is_open()) return;

    // Получение текущего времени
    std::time_t t = std::time(nullptr);
    std::tm bt;
    localtime_s(&bt, &t);
    std::ostringstream timestamp;
    timestamp << "[" << std::put_time(&bt, "%Y-%m-%d %H:%M:%S") << "]";


    ofs << timestamp.str() << username << " " << action << " " << details << "\n";
    ofs.close();
}

std::string HistoryManager::getHistory() {
    std::lock_guard<std::mutex> lock(mtx);
    std::ifstream ifs(historyFileName);
    if (!ifs.is_open()) return "";

    std::ostringstream ss;
    ss << ifs.rdbuf();
    ifs.close();
    return ss.str();
}
