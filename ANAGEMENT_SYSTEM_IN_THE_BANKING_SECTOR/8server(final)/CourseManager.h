#pragma once
#include <string>
#include <vector>
#include <mutex>

struct Course {
    std::string id;
    std::string topic;
    std::string startDate;
    std::string endDate;
};

class CourseManager {
private:
    std::string filename;
    std::vector<Course> courses;
    std::mutex mtx;
    void load();
    void save();
public:
    CourseManager(const std::string& filename);
    bool addCourse(const std::string& id, const std::string& topic, const std::string& startDate, const std::string& endDate);
    bool updateCourse(const std::string& id, const std::string& topic, const std::string& startDate, const std::string& endDate);
    bool deleteCourse(const std::string& id);
    std::string getActiveCoursesFormatted();
};
