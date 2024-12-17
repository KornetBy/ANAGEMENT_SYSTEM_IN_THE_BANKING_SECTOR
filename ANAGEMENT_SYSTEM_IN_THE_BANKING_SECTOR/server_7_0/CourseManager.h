// CourseManager.h
#pragma once
#include <string>
#include <vector>
#include <mutex>

struct Course {
    std::string courseID;
    std::string topic;
    std::string startDate; // ������ YYYY-MM-DD
    std::string endDate;   // ������ YYYY-MM-DD
    // ������ ���������� �����
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

    // ����������� ����� ��� ��������� ������ �� employeeID
    std::vector<Course> getCoursesByEmployee(long employeeID);

    // ����������� ����� ��� ������ ���������� �� ����
    bool enrollEmployeeToCourse(const std::string& courseID, long employeeID);
};
