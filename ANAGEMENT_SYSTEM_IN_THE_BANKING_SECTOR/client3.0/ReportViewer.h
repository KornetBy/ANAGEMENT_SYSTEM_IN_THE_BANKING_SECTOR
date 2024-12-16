#pragma once
#ifndef REPORTVIEWER_H
#define REPORTVIEWER_H

#include <string>

class ReportViewer {
public:
    // �������� ������ �� ���������
    static void viewSalaryReport();

    // �������� ������ �� ������������
    static void viewCompensationReport();

    // �������� �������� ������ �� �������������
    static void viewDivisionReport(const std::string& divisionId);
};

#endif // REPORTVIEWER_H
