// PerformanceEvaluationManager.cpp
#include "PerformanceEvaluationManager.h"
#include "utils.h"
#include <fstream>
#include <sstream>
PerformanceEvaluationManager::PerformanceEvaluationManager(const std::string& filename) : filename(filename) {
    load();
}

void PerformanceEvaluationManager::load() {
    evaluations.clear();
    if (!fileExists(filename)) return;
    std::string data = readAll(filename);
    std::istringstream iss(data);
    std::string line;
    while (std::getline(iss, line)) {
        line = trim(line);
        if (line.empty()) continue;
        auto parts = split(line, '|');
        if (parts.size() < 2) continue;
        PerformanceEval p;
        p.employeeID = std::stol(parts[0]);
        p.evaluation = parts[1];
        evaluations.push_back(p);
    }
}

void PerformanceEvaluationManager::save() {
    std::ostringstream oss;
    for (auto& p : evaluations) {
        oss << p.employeeID << "|" << p.evaluation << "\n";
    }
    writeAll(filename, oss.str());
}

bool PerformanceEvaluationManager::addEvaluation(long empID, const std::string& eval) {
    std::lock_guard<std::mutex> lock(mtx);
    for (auto& p : evaluations) {
        if (p.employeeID == empID) {
            p.evaluation = eval;
            save();
            return true;
        }
    }
    PerformanceEval pe{ empID,eval };
    evaluations.push_back(pe);
    save();
    return true;
}

std::string PerformanceEvaluationManager::getEvaluation(long empID) {
    std::lock_guard<std::mutex> lock(mtx);
    for (auto& p : evaluations) {
        if (p.employeeID == empID) return p.evaluation;
    }
    return "Нет оценки";
}
