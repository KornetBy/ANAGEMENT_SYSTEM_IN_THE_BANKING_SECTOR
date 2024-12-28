#pragma once
#include <string>

class Logger {
private:
    std::string filename;
public:
    Logger(const std::string& filename);
    void log(const std::string& level, const std::string& command, const std::string& message);
    std::string getLogs();
};
