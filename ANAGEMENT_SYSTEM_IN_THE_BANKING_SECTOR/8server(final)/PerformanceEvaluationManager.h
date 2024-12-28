// PerformanceEvaluationManager.h
#pragma once
#include <string>
#include <vector>
#include <mutex>

struct PerformanceEval {
    long employeeID;
    std::string evaluation;
};

class PerformanceEvaluationManager {
private:
    std::string filename;
    std::vector<PerformanceEval> evaluations;
    std::mutex mtx;
    void load();
    void save();
public:
    PerformanceEvaluationManager(const std::string& filename);
    bool addEvaluation(long empID, const std::string& eval);
    std::string getEvaluation(long empID);
};
