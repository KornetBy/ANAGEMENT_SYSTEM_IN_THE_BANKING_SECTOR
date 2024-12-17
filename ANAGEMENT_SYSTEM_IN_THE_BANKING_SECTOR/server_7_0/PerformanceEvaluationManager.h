#pragma once
// PerformanceEvaluationManager.h
#pragma once
#include <string>
#include <vector>
#include <mutex>

struct PerformanceEvaluation {
    long evaluationID;
    long employeeID;
    long positionID;
    std::string evaluationDate;     // Формат YYYY-MM-DD
    std::string evaluationResult;   // Отлично, Хорошо и пр.
    std::string comments;
};

class PerformanceEvaluationManager {
private:
    std::vector<PerformanceEvaluation> evaluations;
    std::string evaluationFileName;
    std::mutex mtx;
public:
    PerformanceEvaluationManager(const std::string& filename);
    PerformanceEvaluation getEvaluation(long evaluationID);
    bool addEvaluation(const PerformanceEvaluation& evaluation);
    bool updateEvaluation(const PerformanceEvaluation& evaluation);
    bool deleteEvaluation(long evaluationID);
    std::vector<PerformanceEvaluation> getAllEvaluations();
};
