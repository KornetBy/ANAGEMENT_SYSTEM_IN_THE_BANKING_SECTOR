#pragma once
#include <string>
#include <vector>

class Utils {
private:
    std::string logFileName;
public:
    // Конструктор, принимающий имя файла логов
    Utils(const std::string& logFile = "data/logs.txt");

    void Log(const std::string& level, const std::string& msg);
    bool FileExists(const std::string& fn);
    bool ValidateID(const std::string& id);
    bool CheckFieldsCount(const std::string& line, int required_count);
    std::vector<std::string> Split(const std::string& s, char d);
    std::string Trimr(const std::string& s);

    void PrintError(const std::string& msg);
    void PrintInfo(const std::string& msg);
};
