#pragma once
#include <string>
#include <vector>
#include <mutex>

struct Position {
    long id;
    std::string name;
    double minSalary;
    double maxSalary;
    std::string qualifications;
};

class PositionManager {
private:
    std::string filename;
    std::vector<Position> positions;
    std::mutex mtx;
    void load();
    void save();
public:
    PositionManager(const std::string& filename);
    bool addPosition(long id, const std::string& name, double minSal, double maxSal, const std::string& qual);
    Position getPosition(long id);
    std::string getPositionName(long id);
};
