#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <string>

struct Employee {
    long employeeID;
    long positionID;
    std::string fullName;
    std::string birthDate;
    std::string address;
    std::string contactInfo;
    std::string startDate;
    std::string status;
};

#endif
