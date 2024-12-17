// HistoryManager.h
#pragma once
#include <string>
#include <mutex>
#include <fstream>

class HistoryManager {
private:
    std::string historyFileName;
    std::mutex mtx;
public:
    HistoryManager(const std::string& filename) : historyFileName(filename) {}
    void addHistory(const std::string& username, const std::string& action, const std::string& details);
    std::string getHistory();
};
