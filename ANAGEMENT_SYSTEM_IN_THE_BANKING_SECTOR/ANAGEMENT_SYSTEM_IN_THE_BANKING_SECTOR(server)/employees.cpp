#include "employees.h"
#include "common.h"
#include "audit.h"
#include "i18n.h"
#include <fstream>
#include <mutex>
#include <sstream>
#include "reports.h"
extern std::mutex file_mutex;

struct Employee {
    int id;
    std::string full_name;
    std::string position;
    std::string department;
    std::string hire_date;
    double salary;
    std::string note;
};

static std::vector<Employee> readEmployees(const std::string& filename) {
    std::lock_guard<std::mutex> lock(file_mutex);
    std::vector<Employee> employees;
    std::ifstream file(filename);
    std::string line;
    while (getline(file, line)) {
        if (line.empty())continue;
        auto t = split(line, ';');
        if (t.size() < 7)continue;
        Employee e;
        e.id = std::stoi(t[0]);
        e.full_name = t[1];
        e.position = t[2];
        e.department = t[3];
        e.hire_date = t[4];
        e.salary = std::stod(t[5]);
        e.note = t[6];
        employees.push_back(e);
    }
    return employees;
}

static void writeEmployees(const std::string& filename, const std::vector<Employee>& employees) {
    std::lock_guard<std::mutex> lock(file_mutex);
    std::ofstream file(filename, std::ios::trunc);
    for (auto& e : employees) {
        file << e.id << ";" << e.full_name << ";" << e.position << ";" << e.department << ";" << e.hire_date << ";" << e.salary << ";" << e.note << "\n";
    }
}

static std::string addEmployee(const std::string& fn, const std::string& pos, const std::string& dep, const std::string& hd, double sal, const std::string& note) {
    auto emps = readEmployees("employees.txt");
    int nid = 1; for (auto& e : emps)if (e.id >= nid)nid = e.id + 1;
    Employee E{ nid,fn,pos,dep,hd,sal,note };
    emps.push_back(E);
    writeEmployees("employees.txt", emps);
    auditLog("Added employee " + fn);
    return "SUCCESS;" + tr("EMPLOYEE_ADDED");
}

static std::string removeEmployee(int id) {
    auto emps = readEmployees("employees.txt");
    bool f = false;
    for (auto it = emps.begin(); it != emps.end(); ++it) {
        if (it->id == id) { emps.erase(it); f = true; break; }
    }
    if (!f)return "ERROR;" + tr("ERROR_GENERIC");
    writeEmployees("employees.txt", emps);
    auditLog("Removed employee " + std::to_string(id));
    return "SUCCESS;ok";
}

static std::string updateEmployeeField(int id, const std::string& field, const std::string& value) {
    auto emps = readEmployees("employees.txt");
    bool f = false;
    for (auto& e : emps) {
        if (e.id == id) {
            f = true;
            if (field == "department") e.department = value;
            else if (field == "position") e.position = value;
            else if (field == "note") e.note = value;
            else return "ERROR;" + tr("ERROR_GENERIC");
            break;
        }
    }
    if (!f)return "ERROR;" + tr("ERROR_GENERIC");
    writeEmployees("employees.txt", emps);
    auditLog("Updated emp " + std::to_string(id) + " field " + field);
    return "SUCCESS;ok";
}

static std::string updateEmployeeSalary(int id, double nsal) {
    auto emps = readEmployees("employees.txt");
    bool f = false;
    for (auto& e : emps) {
        if (e.id == id) { e.salary = nsal; f = true; break; }
    }
    if (!f)return "ERROR;" + tr("ERROR_GENERIC");
    writeEmployees("employees.txt", emps);
    auditLog("Updated salary emp " + std::to_string(id));
    return "SUCCESS;ok";
}

static std::string paySalary(int id, double amt) {
    auditLog("Paid salary " + std::to_string(amt) + " to emp " + std::to_string(id));
    return "SUCCESS;ok";
}

static std::string viewEmployees() {
    auto e = readEmployees("employees.txt");
    std::stringstream ss; ss << "SUCCESS;\n";
    for (auto& x : e) {
        ss << x.id << ";" << x.full_name << ";" << x.position << ";" << x.department << ";" << x.hire_date << ";" << x.salary << ";" << x.note << "\n";
    }
    return ss.str();
}

std::string handleHRCommands(const std::vector<std::string>& tokens) {
    std::string cmd = tokens[0];
    if (cmd == "ADD_EMPLOYEE") {
        if (tokens.size() < 7)return "ERROR;" + tr("ERROR_GENERIC");
        return addEmployee(tokens[1], tokens[2], tokens[3], tokens[4], std::stod(tokens[5]), tokens[6]);
    }
    else if (cmd == "REMOVE_EMPLOYEE") {
        if (tokens.size() < 2)return "ERROR;" + tr("ERROR_GENERIC");
        return removeEmployee(std::stoi(tokens[1]));
    }
    else if (cmd == "UPDATE_EMPLOYEE_FIELD") {
        if (tokens.size() < 4)return "ERROR;" + tr("ERROR_GENERIC");
        return updateEmployeeField(std::stoi(tokens[1]), tokens[2], tokens[3]);
    }
    else if (cmd == "UPDATE_EMPLOYEE_SALARY") {
        if (tokens.size() < 3)return "ERROR;" + tr("ERROR_GENERIC");
        return updateEmployeeSalary(std::stoi(tokens[1]), std::stod(tokens[2]));
    }
    else if (cmd == "VIEW_EMPLOYEES") {
        return viewEmployees();
    }
    else if (cmd == "PAY_SALARY") {
        if (tokens.size() < 3)return "ERROR;" + tr("ERROR_GENERIC");
        return paySalary(std::stoi(tokens[1]), std::stod(tokens[2]));
    }
    else if (cmd == "VIEW_EMPLOYEES_REPORT") {
        if (tokens.size() < 3)return "ERROR;" + tr("ERROR_GENERIC");
        return viewEmployeesReport(tokens[1], tokens[2]);
    }
    return "ERROR;" + tr("ERROR_GENERIC");
}

std::string handlePeopleManagerCommands(const std::vector<std::string>& tokens) {
    std::string cmd = tokens[0];
    if (cmd == "VIEW_EMPLOYEES") {
        return viewEmployees();
    }
    return "ERROR;" + tr("ERROR_GENERIC");
}
