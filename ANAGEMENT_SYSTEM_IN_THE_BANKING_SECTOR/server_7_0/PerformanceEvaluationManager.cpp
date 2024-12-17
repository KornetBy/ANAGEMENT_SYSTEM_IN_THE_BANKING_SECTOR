// PerformanceEvaluationManager.cpp
#include "PerformanceEvaluationManager.h"
#include <fstream>
#include <sstream>

PerformanceEvaluationManager::PerformanceEvaluationManager(const std::string& filename) : evaluationFileName(filename) {
    std::lock_guard<std::mutex> lock(mtx);
    std::ifstream ifs(evaluationFileName);
    if (!ifs.is_open()) return;

    std::string line;
    while (getline(ifs, line)) {
        std::istringstream iss(line);
        std::string token;
        PerformanceEvaluation eval;
        getline(iss, token, '|');
        eval.evaluationID = std::stol(token);
        getline(iss, token, '|');
        eval.employeeID = std::stol(token);
        getline(iss, token, '|');
        eval.positionID = std::stol(token);
        getline(iss, eval.evaluationDate, '|');
        getline(iss, eval.evaluationResult, '|');
        getline(iss, eval.comments, '|');
        evaluations.push_back(eval);
    }
    ifs.close();
}

PerformanceEvaluation PerformanceEvaluationManager::getEvaluation(long evaluationID) {
    std::lock_guard<std::mutex> lock(mtx);
    for (const auto& eval : evaluations) {
        if (eval.evaluationID == evaluationID) return eval;
    }
    return PerformanceEvaluation{ 0, 0, 0, "", "", "" };
}

bool PerformanceEvaluationManager::addEvaluation(const PerformanceEvaluation& evaluation) {
    std::lock_guard<std::mutex> lock(mtx);
    for (const auto& eval : evaluations) {
        if (eval.evaluationID == evaluation.evaluationID) return false; // Оценка уже существует
    }
    evaluations.push_back(evaluation);

    std::ofstream ofs(evaluationFileName, std::ios::app);
    if (!ofs.is_open()) return false;
    ofs << evaluation.evaluationID << "|" << evaluation.employeeID << "|"
        << evaluation.positionID << "|" << evaluation.evaluationDate << "|"
        << evaluation.evaluationResult << "|" << evaluation.comments << "\n";
    ofs.close();
    return true;
}

bool PerformanceEvaluationManager::updateEvaluation(const PerformanceEvaluation& evaluation) {
    std::lock_guard<std::mutex> lock(mtx);
    bool found = false;
    for (auto& eval : evaluations) {
        if (eval.evaluationID == evaluation.evaluationID) {
            eval = evaluation;
            found = true;
            break;
        }
    }
    if (!found) return false;

    // Перезапись файла
    std::ofstream ofs(evaluationFileName, std::ios::trunc);
    if (!ofs.is_open()) return false;
    for (const auto& eval : evaluations) {
        ofs << eval.evaluationID << "|" << eval.employeeID << "|"
            << eval.positionID << "|" << eval.evaluationDate << "|"
            << eval.evaluationResult << "|" << eval.comments << "\n";
    }
    ofs.close();
    return true;
}

bool PerformanceEvaluationManager::deleteEvaluation(long evaluationID) {
    std::lock_guard<std::mutex> lock(mtx);
    bool found = false;
    std::vector<PerformanceEvaluation> updatedEvaluations;
    for (const auto& eval : evaluations) {
        if (eval.evaluationID != evaluationID) {
            updatedEvaluations.push_back(eval);
        }
        else {
            found = true;
        }
    }
    if (!found) return false;
    evaluations = updatedEvaluations;

    // Перезапись файла
    std::ofstream ofs(evaluationFileName, std::ios::trunc);
    if (!ofs.is_open()) return false;
    for (const auto& eval : evaluations) {
        ofs << eval.evaluationID << "|" << eval.employeeID << "|"
            << eval.positionID << "|" << eval.evaluationDate << "|"
            << eval.evaluationResult << "|" << eval.comments << "\n";
    }
    ofs.close();
    return true;
}

std::vector<PerformanceEvaluation> PerformanceEvaluationManager::getAllEvaluations() {
    std::lock_guard<std::mutex> lock(mtx);
    return evaluations;
}
