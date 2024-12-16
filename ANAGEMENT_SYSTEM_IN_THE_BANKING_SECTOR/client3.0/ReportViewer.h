#pragma once
#ifndef REPORTVIEWER_H
#define REPORTVIEWER_H

#include <string>

class ReportViewer {
public:
    // Просмотр отчета по зарплатам
    static void viewSalaryReport();

    // Просмотр отчета по компенсациям
    static void viewCompensationReport();

    // Просмотр сводного отчета по подразделению
    static void viewDivisionReport(const std::string& divisionId);
};

#endif // REPORTVIEWER_H
