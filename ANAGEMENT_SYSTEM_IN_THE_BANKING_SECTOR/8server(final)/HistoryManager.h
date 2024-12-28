#pragma once
#include <string>

class HistoryManager {
private:
    std::string filename;
public:
    HistoryManager(const std::string& filename);
    void addHistory(const std::string& username, const std::string& command, const std::string& details);
    std::string getHistory();
};
