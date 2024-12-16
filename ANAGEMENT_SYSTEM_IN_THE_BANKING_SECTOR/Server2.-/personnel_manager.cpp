#include "personnel_manager.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

// �����������
PersonnelManager::PersonnelManager(const std::string& filename) : filename(filename) {}

// ��������������� ����� ��� �������� ������ �� �����
Employee PersonnelManager::parseEmployee(const std::string& line) {
    Employee employee;
    std::istringstream iss(line);
    std::string id, salary;

    // ���������� ������ �� ������� '|'
    if (!(std::getline(iss, id, '|') && std::getline(iss, employee.name, '|') &&
        std::getline(iss, employee.position, '|') && std::getline(iss, employee.department, '|') &&
        std::getline(iss, salary, '|') && std::getline(iss, employee.notes, '|'))) {
        throw std::runtime_error("������������ ������ ������.");
    }

    // �������������� ID � ��������
    employee.id = std::stoi(id);
    employee.salary = std::stod(salary);

    // �������� �� ������������ ������
    if (employee.id <= 0) {
        throw std::runtime_error("������������ ID ����������: " + id);
    }
    if (employee.salary < 0) {
        throw std::runtime_error("�������� �� ����� ���� �������������: " + salary);
    }

    return employee;
}

// ��������� ����������� ID ����������
int PersonnelManager::generateEmployeeId() const {
    int maxId = 0;
    for (const auto& employee : employees) {
        if (employee.id > maxId) {
            maxId = employee.id;
        }
    }
    return maxId + 1;
}

// �������� ������������ ������ ����������
bool PersonnelManager::validateEmployeeData(const Employee& employee) const {
    if (employee.salary < 10000) { // ����������� ��������
        std::cerr << "������: �������� ���������� �� ����� ���� ���� ������������ ��������." << std::endl;
        return false;
    }
    if (employee.name.empty() || employee.department.empty()) {
        std::cerr << "������: ��� ��� ����� �� ����� ���� �������." << std::endl;
        return false;
    }
    if (employee.name.find('|') != std::string::npos || employee.department.find('|') != std::string::npos) {
        std::cerr << "������: ��� ��� ����� �������� ������������ ������ '|'" << std::endl;
        return false;
    }
    return true;
}

// �������� ������ �� �����
void PersonnelManager::loadEmployees() {
    employees.clear();
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "������: �� ������� ������� ���� " << filename << std::endl;
        std::ofstream errorLog("logs/error.log", std::ios::app);
        errorLog << "������: ���� " << filename << " �� ������ ��� ����������." << std::endl;
        errorLog.close();
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        try {
            employees.push_back(parseEmployee(line));
        }
        catch (const std::exception& e) {
            std::cerr << "������: " << e.what() << " ������: " << line << std::endl;
            std::ofstream errorLog("logs/error.log", std::ios::app);
            errorLog << "������ ��� �������� ����������: " << e.what() << " ������: " << line << std::endl;
            errorLog.close();
        }
    }

    file.close();
}

// ���������� ������ � ����
void PersonnelManager::saveEmployees() {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "������: �� ������� ������� ���� " << filename << " ��� ������." << std::endl;
        std::ofstream errorLog("logs/error.log", std::ios::app);
        errorLog << "������: �� ������� ������� ���� " << filename << " ��� ������." << std::endl;
        errorLog.close();
        return;
    }

    for (const auto& employee : employees) {
        file << employee.id << " | " << employee.name << " | "
            << employee.position << " | " << employee.department << " | "
            << employee.salary << " | " << employee.notes << std::endl;
    }

    file.close();
}

// ���������� ������ ����������
void PersonnelManager::addEmployee(const Employee& employee) {
    if (!validateEmployeeData(employee)) {
        return;
    }

    Employee newEmployee = employee;
    if (newEmployee.id <= 0) {
        newEmployee.id = generateEmployeeId();
    }

    employees.push_back(newEmployee);
    saveEmployees();
}

// �������� ���������� �� ID
bool PersonnelManager::removeEmployee(int id) {
    for (auto it = employees.begin(); it != employees.end(); ++it) {
        if (it->id == id) {
            employees.erase(it);
            saveEmployees();
            return true;
        }
    }
    std::cerr << "������: ��������� � ID " << id << " �� ������." << std::endl;
    return false;
}

// ��������� ������ ����������
bool PersonnelManager::updateEmployee(int id, const Employee& updatedEmployee) {
    if (!validateEmployeeData(updatedEmployee)) {
        return false;
    }

    for (auto& employee : employees) {
        if (employee.id == id) {
            employee = updatedEmployee;
            saveEmployees();
            return true;
        }
    }
    std::cerr << "������: ��������� � ID " << id << " �� ������." << std::endl;
    return false;
}

// ��������� ����������� �� ������
std::vector<Employee> PersonnelManager::getEmployeesByDepartment(const std::string& department) const {
    std::vector<Employee> result;
    for (const auto& employee : employees) {
        if (employee.department == department) {
            result.push_back(employee);
        }
    }
    return result;
}

// ��������� ����������� � ����������� �� ��������
std::vector<Employee> PersonnelManager::getEmployeesBySalary(double minSalary, double maxSalary) const {
    std::vector<Employee> result;
    for (const auto& employee : employees) {
        if (employee.salary >= minSalary && employee.salary <= maxSalary) {
            result.push_back(employee);
        }
    }
    return result;
}

// ��������� ������ �� ������ � ����
void PersonnelManager::generateReportByDepartmentToFile(const std::string& department, const std::string& outputFilename) const {
    std::ofstream reportFile(outputFilename);
    if (!reportFile.is_open()) {
        std::cerr << "������: �� ������� ������� ���� " << outputFilename << " ��� ������." << std::endl;
        return;
    }

    reportFile << "����� �� ������: " << department << "\n\n";
    for (const auto& employee : employees) {
        if (employee.department == department) {
            reportFile << "ID: " << employee.id << ", ���: " << employee.name
                << ", ���������: " << employee.position << ", ��������: " << employee.salary
                << ", ����������: " << employee.notes << "\n";
        }
    }

    reportFile.close();
    std::cout << "����� ������� � ����: " << outputFilename << std::endl;
}

// ��������� ���� �����������
const std::vector<Employee>& PersonnelManager::getAllEmployees() const {
    return employees;
}
