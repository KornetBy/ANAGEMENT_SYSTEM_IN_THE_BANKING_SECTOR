#ifndef POSITION_H
#define POSITION_H

#include <string>

struct Position {
    long positionID;
    std::string positionName;
    double minSalary;
    double maxSalary;
    std::string qualificationRequirements;
};

#endif
