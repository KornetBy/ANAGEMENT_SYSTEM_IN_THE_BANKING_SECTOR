#include "personnel_manager.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

// Конструктор
PersonnelManager::PersonnelManager(const std::string& filename) : filename(filename) {}

// Вспомогательный метод для парсинга строки из файла
Employee PersonnelManager::parseEmployee(const std::string& line) {
    Employee employee;
    std::istringstream iss(line);
    std::string id, salary;

    // Разделение строки по символу '|'
    if (!(std::getline(iss, id, '|') && std::getline(iss, employee.name, '|') &&
        std::getline(iss, employee.position, '|') && std::getline(iss, employee.department, '|') &&
        std::getline(iss, salary, '|') && std::getline(iss, employee.notes, '|'))) {
        throw std::runtime_error("Некорректный формат строки.");
    }

    // Преобразование ID и зарплаты
    employee.id = std::stoi(id);
    employee.salary = std::stod(salary);

    // Проверки на корректность данных
    if (employee.id <= 0) {
        throw std::runtime_error("Некорректный ID сотрудника: " + id);
    }
    if (employee.salary < 0) {
        throw std::runtime_error("Зарплата не может быть отрицательной: " + salary);
    }

    return employee;
}

// Генерация уникального ID сотрудника
int PersonnelManager::generateEmployeeId() const {
    int maxId = 0;
    for (const auto& employee : employees) {
        if (employee.id > maxId) {
            maxId = employee.id;
        }
    }
    return maxId + 1;
}

// Проверка корректности данных сотрудника
bool PersonnelManager::validateEmployeeData(const Employee& employee) const {
    if (employee.salary < 10000) { // Минимальная зарплата
        std::cerr << "Ошибка: зарплата сотрудника не может быть ниже минимального значения." << std::endl;
        return false;
    }
    if (employee.name.empty() || employee.department.empty()) {
        std::cerr << "Ошибка: имя или отдел не могут быть пустыми." << std::endl;
        return false;
    }
    if (employee.name.find('|') != std::string::npos || employee.department.find('|') != std::string::npos) {
        std::cerr << "Ошибка: имя или отдел содержат недопустимый символ '|'" << std::endl;
        return false;
    }
    return true;
}

// Загрузка данных из файла
void PersonnelManager::loadEmployees() {
    employees.clear();
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл " << filename << std::endl;
        std::ofstream errorLog("logs/error.log", std::ios::app);
        errorLog << "Ошибка: файл " << filename << " не найден или недоступен." << std::endl;
        errorLog.close();
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        try {
            employees.push_back(parseEmployee(line));
        }
        catch (const std::exception& e) {
            std::cerr << "Ошибка: " << e.what() << " Строка: " << line << std::endl;
            std::ofstream errorLog("logs/error.log", std::ios::app);
            errorLog << "Ошибка при загрузке сотрудника: " << e.what() << " Строка: " << line << std::endl;
            errorLog.close();
        }
    }

    file.close();
}

// Сохранение данных в файл
void PersonnelManager::saveEmployees() {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл " << filename << " для записи." << std::endl;
        std::ofstream errorLog("logs/error.log", std::ios::app);
        errorLog << "Ошибка: не удалось открыть файл " << filename << " для записи." << std::endl;
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

// Добавление нового сотрудника
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

// Удаление сотрудника по ID
bool PersonnelManager::removeEmployee(int id) {
    for (auto it = employees.begin(); it != employees.end(); ++it) {
        if (it->id == id) {
            employees.erase(it);
            saveEmployees();
            return true;
        }
    }
    std::cerr << "Ошибка: сотрудник с ID " << id << " не найден." << std::endl;
    return false;
}

// Изменение данных сотрудника
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
    std::cerr << "Ошибка: сотрудник с ID " << id << " не найден." << std::endl;
    return false;
}

// Получение сотрудников по отделу
std::vector<Employee> PersonnelManager::getEmployeesByDepartment(const std::string& department) const {
    std::vector<Employee> result;
    for (const auto& employee : employees) {
        if (employee.department == department) {
            result.push_back(employee);
        }
    }
    return result;
}

// Получение сотрудников с фильтрацией по зарплате
std::vector<Employee> PersonnelManager::getEmployeesBySalary(double minSalary, double maxSalary) const {
    std::vector<Employee> result;
    for (const auto& employee : employees) {
        if (employee.salary >= minSalary && employee.salary <= maxSalary) {
            result.push_back(employee);
        }
    }
    return result;
}

// Генерация отчёта по отделу в файл
void PersonnelManager::generateReportByDepartmentToFile(const std::string& department, const std::string& outputFilename) const {
    std::ofstream reportFile(outputFilename);
    if (!reportFile.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл " << outputFilename << " для записи." << std::endl;
        return;
    }

    reportFile << "Отчёт по отделу: " << department << "\n\n";
    for (const auto& employee : employees) {
        if (employee.department == department) {
            reportFile << "ID: " << employee.id << ", Имя: " << employee.name
                << ", Должность: " << employee.position << ", Зарплата: " << employee.salary
                << ", Примечания: " << employee.notes << "\n";
        }
    }

    reportFile.close();
    std::cout << "Отчёт сохранён в файл: " << outputFilename << std::endl;
}

// Получение всех сотрудников
const std::vector<Employee>& PersonnelManager::getAllEmployees() const {
    return employees;
}
