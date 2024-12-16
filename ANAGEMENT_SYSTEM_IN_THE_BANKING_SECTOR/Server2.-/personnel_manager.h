#pragma once
#ifndef PERSONNEL_MANAGER_H
#define PERSONNEL_MANAGER_H

#include <string>
#include <vector>

// Структура для хранения информации о сотруднике
struct Employee {
    int id;                     // Уникальный идентификатор сотрудника
    std::string name;           // ФИО сотрудника
    std::string position;       // Должность
    std::string department;     // Отдел
    double salary;              // Зарплата
    std::string notes;          // Примечания
};

// Класс для управления сотрудниками
class PersonnelManager {
private:
    std::vector<Employee> employees; // Список сотрудников
    std::string filename;            // Имя файла с данными сотрудников

    // Вспомогательный метод для парсинга строки из файла
    Employee parseEmployee(const std::string& line);

    // Генерация уникального ID для нового сотрудника
    int generateEmployeeId() const;

    // Проверка корректности данных сотрудника
    bool validateEmployeeData(const Employee& employee) const;

public:
    // Конструктор с указанием файла данных
    PersonnelManager(const std::string& filename);

    // Загрузка данных из файла
    void loadEmployees();

    // Сохранение данных в файл
    void saveEmployees();

    // Добавление нового сотрудника
    void addEmployee(const Employee& employee);

    // Удаление сотрудника по ID
    bool removeEmployee(int id);

    // Изменение данных сотрудника
    bool updateEmployee(int id, const Employee& updatedEmployee);

    // Получение сотрудников по отделу
    std::vector<Employee> getEmployeesByDepartment(const std::string& department) const;

    // Получение сотрудников с фильтрацией по зарплате
    std::vector<Employee> getEmployeesBySalary(double minSalary, double maxSalary) const;

    // Генерация отчёта по отделу в файл
    void generateReportByDepartmentToFile(const std::string& department, const std::string& outputFilename) const;

    // Получение всех сотрудников
    const std::vector<Employee>& getAllEmployees() const;
};

#endif // PERSONNEL_MANAGER_H
