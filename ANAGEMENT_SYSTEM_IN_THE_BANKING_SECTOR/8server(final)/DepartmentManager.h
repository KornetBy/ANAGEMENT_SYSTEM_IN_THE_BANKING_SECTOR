// DepartmentManager.h
#pragma once
#include <string>
#include <vector>
#include <mutex>

struct Department {
    long id;
    std::string name;
};

class DepartmentManager {
private:
    std::string filename;
    std::vector<Department> departments;
    std::mutex mtx;
    void load();
    void save();
public:
    DepartmentManager(const std::string& filename);
    bool addDepartment(long id, const std::string& name);
    std::string getDepartmentName(long id);
};
