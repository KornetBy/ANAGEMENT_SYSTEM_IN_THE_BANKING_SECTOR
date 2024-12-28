// CourseManager.cpp
#include "CourseManager.h"
#include <fstream>
#include <sstream>
#include <algorithm>

// �����������
CourseManager::CourseManager(const std::string& filename) : courseFileName(filename) {
    std::lock_guard<std::mutex> lock(mtx);
    std::ifstream ifs(courseFileName);
    if (!ifs.is_open()) return;

    std::string line;
    while (getline(ifs, line)) {
        std::istringstream iss(line);
        std::string token;
        Course course;

        // ������ ������ �� ������
        getline(iss, course.courseID, '|');
        getline(iss, course.topic, '|');
        getline(iss, course.startDate, '|');
        getline(iss, course.endDate, '|');
        getline(iss, course.status, '|'); // ������ ������� �����

        // ������ ������ ����������
        if (getline(iss, token, '|')) {
            std::istringstream empStream(token);
            std::string empIDStr;
            while (getline(empStream, empIDStr, ',')) {
                if (!empIDStr.empty()) {
                    course.enrolledEmployeeIDs.push_back(std::stol(empIDStr));
                }
            }
        }

        courses.push_back(course);
    }
    ifs.close();
}


// ����� ��������� ����� �� courseID
Course CourseManager::getCourse(const std::string& courseID) {
    std::lock_guard<std::mutex> lock(mtx);
    for (const auto& course : courses) {
        if (course.courseID == courseID) {
            return course;
        }
    }
    return Course(); // ���������� ������ ����, ���� �� ������
}

// ����� ���������� �����
bool CourseManager::addCourse(const Course& course) {
    std::lock_guard<std::mutex> lock(mtx);
    // �������� �� ������������ courseID
    auto it = std::find_if(courses.begin(), courses.end(),
        [&course](const Course& c) { return c.courseID == course.courseID; });
    if (it != courses.end()) {
        return false; // ���� � ����� ID ��� ����������
    }
    courses.push_back(course);

    // ������ � ����
    std::ofstream ofs(courseFileName, std::ios::app);
    if (!ofs.is_open()) return false;
    ofs << course.courseID << "|" << course.topic << "|"
        << course.startDate << "|" << course.endDate << "|";
    // ���������� ������ ���������� ����� �������
    for (size_t i = 0; i < course.enrolledEmployeeIDs.size(); ++i) {
        ofs << course.enrolledEmployeeIDs[i];
        if (i != course.enrolledEmployeeIDs.size() - 1) ofs << ",";
    }
    ofs << "\n";
    ofs.close();
    return true;
}

// ����� ���������� �����
bool CourseManager::updateCourse(const Course& course) {
    std::lock_guard<std::mutex> lock(mtx);
    for (auto& c : courses) {
        if (c.courseID == course.courseID) {
            c.topic = course.topic;
            c.startDate = course.startDate;
            c.endDate = course.endDate;
            c.enrolledEmployeeIDs = course.enrolledEmployeeIDs;

            // ��������� ����
            std::ofstream ofs(courseFileName, std::ios::trunc);
            if (!ofs.is_open()) return false;
            for (const auto& course : courses) {
                ofs << course.courseID << "|" << course.topic << "|"
                    << course.startDate << "|" << course.endDate << "|";
                // ���������� ������ ���������� ����� �������
                for (size_t i = 0; i < course.enrolledEmployeeIDs.size(); ++i) {
                    ofs << course.enrolledEmployeeIDs[i];
                    if (i != course.enrolledEmployeeIDs.size() - 1) ofs << ",";
                }
                ofs << "\n";
            }
            ofs.close();
            return true;
        }
    }
    return false; // ���� �� ������
}

// ����� �������� �����
bool CourseManager::deleteCourse(const std::string& courseID) {
    std::lock_guard<std::mutex> lock(mtx);
    auto it = std::remove_if(courses.begin(), courses.end(),
        [&courseID](const Course& c) { return c.courseID == courseID; });
    if (it != courses.end()) {
        courses.erase(it, courses.end());

        // ��������� ����
        std::ofstream ofs(courseFileName, std::ios::trunc);
        if (!ofs.is_open()) return false;
        for (const auto& course : courses) {
            ofs << course.courseID << "|" << course.topic << "|"
                << course.startDate << "|" << course.endDate << "|";
            // ���������� ������ ���������� ����� �������
            for (size_t i = 0; i < course.enrolledEmployeeIDs.size(); ++i) {
                ofs << course.enrolledEmployeeIDs[i];
                if (i != course.enrolledEmployeeIDs.size() - 1) ofs << ",";
            }
            ofs << "\n";
        }
        ofs.close();
        return true;
    }
    return false; // ���� �� ������
}

// ����� ��������� ���� ������
std::vector<Course> CourseManager::getAllCourses() {
    std::lock_guard<std::mutex> lock(mtx);
    return courses;
}

// ������������� ����� ��� ��������� ������ �� employeeID
std::vector<Course> CourseManager::getCoursesByEmployee(long employeeID) {
    std::lock_guard<std::mutex> lock(mtx);
    std::vector<Course> result;
    for (const auto& course : courses) {
        if (std::find(course.enrolledEmployeeIDs.begin(), course.enrolledEmployeeIDs.end(), employeeID) != course.enrolledEmployeeIDs.end()) {
            result.push_back(course);
        }
    }
    return result;
}

// ������������� ����� ��� ������ ���������� �� ����
bool CourseManager::enrollEmployeeToCourse(const std::string& courseID, long employeeID) {
    std::lock_guard<std::mutex> lock(mtx);
    for (auto& course : courses) {
        if (course.courseID == courseID) {
            if (std::find(course.enrolledEmployeeIDs.begin(), course.enrolledEmployeeIDs.end(), employeeID) == course.enrolledEmployeeIDs.end()) {
                course.enrolledEmployeeIDs.push_back(employeeID);

                // ��������� ����
                std::ofstream ofs(courseFileName, std::ios::trunc);
                if (!ofs.is_open()) return false;
                for (const auto& c : courses) {
                    ofs << c.courseID << "|" << c.topic << "|"
                        << c.startDate << "|" << c.endDate << "|";
                    // ���������� ������ ���������� ����� �������
                    for (size_t i = 0; i < c.enrolledEmployeeIDs.size(); ++i) {
                        ofs << c.enrolledEmployeeIDs[i];
                        if (i != c.enrolledEmployeeIDs.size() - 1) ofs << ",";
                    }
                    ofs << "\n";
                }
                ofs.close();
                return true;
            }
        }
    }
    return false; // ���� �� ������ ��� ��������� ��� �������
}
std::vector<Course> CourseManager::getActiveCourses() {
    std::vector<Course> activeCourses;
    std::ifstream infile("data/courses.txt");
    if (!infile.is_open()) return activeCourses;

    std::string line;
    while (getline(infile, line)) {
        std::istringstream iss(line);
        Course course;
        std::string token;

        // �������������� ������: courseID|topic|startDate|endDate|status
        getline(iss, course.courseID, '|');
        getline(iss, course.topic, '|');
        getline(iss, course.startDate, '|');
        getline(iss, course.endDate, '|');
        getline(iss, course.status, '|');

        if (course.status == "active") {
            activeCourses.push_back(course);
        }
    }
    infile.close();
    return activeCourses;
}

