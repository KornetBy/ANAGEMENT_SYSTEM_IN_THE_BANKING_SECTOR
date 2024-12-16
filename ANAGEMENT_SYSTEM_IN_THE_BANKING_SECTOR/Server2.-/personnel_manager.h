#pragma once
#ifndef PERSONNEL_MANAGER_H
#define PERSONNEL_MANAGER_H

#include <string>
#include <vector>

// ��������� ��� �������� ���������� � ����������
struct Employee {
    int id;                     // ���������� ������������� ����������
    std::string name;           // ��� ����������
    std::string position;       // ���������
    std::string department;     // �����
    double salary;              // ��������
    std::string notes;          // ����������
};

// ����� ��� ���������� ������������
class PersonnelManager {
private:
    std::vector<Employee> employees; // ������ �����������
    std::string filename;            // ��� ����� � ������� �����������

    // ��������������� ����� ��� �������� ������ �� �����
    Employee parseEmployee(const std::string& line);

    // ��������� ����������� ID ��� ������ ����������
    int generateEmployeeId() const;

    // �������� ������������ ������ ����������
    bool validateEmployeeData(const Employee& employee) const;

public:
    // ����������� � ��������� ����� ������
    PersonnelManager(const std::string& filename);

    // �������� ������ �� �����
    void loadEmployees();

    // ���������� ������ � ����
    void saveEmployees();

    // ���������� ������ ����������
    void addEmployee(const Employee& employee);

    // �������� ���������� �� ID
    bool removeEmployee(int id);

    // ��������� ������ ����������
    bool updateEmployee(int id, const Employee& updatedEmployee);

    // ��������� ����������� �� ������
    std::vector<Employee> getEmployeesByDepartment(const std::string& department) const;

    // ��������� ����������� � ����������� �� ��������
    std::vector<Employee> getEmployeesBySalary(double minSalary, double maxSalary) const;

    // ��������� ������ �� ������ � ����
    void generateReportByDepartmentToFile(const std::string& department, const std::string& outputFilename) const;

    // ��������� ���� �����������
    const std::vector<Employee>& getAllEmployees() const;
};

#endif // PERSONNEL_MANAGER_H
