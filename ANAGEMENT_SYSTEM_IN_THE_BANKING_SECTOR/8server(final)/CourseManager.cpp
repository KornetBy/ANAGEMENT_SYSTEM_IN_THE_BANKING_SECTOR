#include "CourseManager.h"
#include "utils.h"
#include <fstream>
#include <sstream>
CourseManager::CourseManager(const std::string& filename) : filename(filename) {
    load();
}

void CourseManager::load() {
    courses.clear();
    if (!fileExists(filename)) return;
    std::string data = readAll(filename);
    std::istringstream iss(data);
    std::string line;
    while (std::getline(iss, line)) {
        line = trim(line);
        if (line.empty()) continue;
        auto parts = split(line, '|');
        if (parts.size() < 4) continue;
        Course c{ parts[0], parts[1], parts[2], parts[3] };
        courses.push_back(c);
    }
}

void CourseManager::save() {
    std::ostringstream oss;
    for (auto& c : courses) {
        oss << c.id << "|" << c.topic << "|" << c.startDate << "|" << c.endDate << "\n";
    }
    writeAll(filename, oss.str());
}

bool CourseManager::addCourse(const std::string& id, const std::string& topic, const std::string& startDate, const std::string& endDate) {
    std::lock_guard<std::mutex> lock(mtx);
    for (auto& c : courses) {
        if (c.id == id) return false;
    }
    Course cr{ id,topic,startDate,endDate };
    courses.push_back(cr);
    save();
    return true;
}

bool CourseManager::updateCourse(const std::string& id, const std::string& topic, const std::string& startDate, const std::string& endDate) {
    std::lock_guard<std::mutex> lock(mtx);
    for (auto& c : courses) {
        if (c.id == id) {
            c.topic = topic;
            c.startDate = startDate;
            c.endDate = endDate;
            save();
            return true;
        }
    }
    return false;
}

bool CourseManager::deleteCourse(const std::string& id) {
    std::lock_guard<std::mutex> lock(mtx);
    for (size_t i = 0; i < courses.size(); i++) {
        if (courses[i].id == id) {
            courses.erase(courses.begin() + i);
            save();
            return true;
        }
    }
    return false;
}

std::string CourseManager::getActiveCoursesFormatted() {
    // Считаем все курсы активными. Можно добавить логику проверки дат
    std::lock_guard<std::mutex> lock(mtx);
    std::ostringstream oss;
    for (auto& c : courses) {
        oss << "ID: " << c.id << ", Тема: " << c.topic << ", Начало: " << c.startDate << ", Конец: " << c.endDate << "\n";
    }
    return oss.str();
}
