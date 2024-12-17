// PositionManager.h
#pragma once
#include <string>
#include <vector>
#include <mutex>

struct Position {
    long positionID;
    std::string positionName;
    double minSalary;
    double maxSalary;
    std::string qualificationRequirements;
};

class PositionManager {
private:
    std::vector<Position> positions;
    std::string positionFileName;
    std::mutex mtx;
public:
    PositionManager(const std::string& filename);
    Position getPosition(long positionID);
    bool addPosition(const Position& position);
    bool updatePosition(const Position& position);
    bool deletePosition(long positionID);
    std::vector<Position> getAllPositions();
};
