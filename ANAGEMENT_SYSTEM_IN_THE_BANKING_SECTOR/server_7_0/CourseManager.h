// CourseManager.h
#pragma once
#include <string>
#include <vector>
#include <mutex>

struct Course {
    std::string courseID;
    std::string topic;
    std::string startDate; // Формат YYYY-MM-DD
    std::string endDate;   // Формат YYYY-MM-DD
    // Список участников курса
    std::vector<long> enrolledEmployeeIDs;
};

class CourseManager {
private:
    std::vector<Course> courses;
    std::string courseFileName;
    std::mutex mtx;
public:
    CourseManager(const std::string& filename);
    Course getCourse(const std::string& courseID);
    bool addCourse(const Course& course);
    bool updateCourse(const Course& course);
    bool deleteCourse(const std::string& courseID);
    std::vector<Course> getAllCourses();

    // Добавленный метод для получения курсов по employeeID
    std::vector<Course> getCoursesByEmployee(long employeeID);

    // Добавленный метод для записи сотрудника на курс
    bool enrollEmployeeToCourse(const std::string& courseID, long employeeID);
};
