// Logger.h
#pragma once
#include <string>
#include <fstream>
#include <mutex>

class Logger {
private:
    std::ofstream logFile;
    std::mutex mtx;
public:
    Logger(const std::string& filename);
    ~Logger();
    void log(const std::string& timestamp, const std::string& action, const std::string& description);
};
