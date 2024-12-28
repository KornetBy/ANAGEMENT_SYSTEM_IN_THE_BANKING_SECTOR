// RequestHandler.cpp
#include "RequestHandler.h"
#include "utils.h"
#include <sstream>
// RequestHandler.cpp
#include "ConnectionManager.h"
static bool isDateBeforeOrEqual(const std::string& d1, const std::string& d2) {
    // ����������� ��������: ������� ������ YYYY-MM-DD �����������������
    // ��������������, ��� ������ ������ ����������
    return d1 <= d2;
}

RequestHandler::RequestHandler(const std::string& logFilename,
    const std::string& userFilename,
    const std::string& employeeFilename,
    const std::string& positionFilename,
    const std::string& courseFilename,
    const std::string& compensationFilename,
    const std::string& salaryFilename,
    const std::string& scheduleFilename,
    const std::string& responsibilityFilename,
    const std::string& departmentFilename,
    const std::string& evaluationFilename,
    const std::string& historyFilename)
    : logger(logFilename),
    userManager(userFilename),
    employeeManager(employeeFilename),
    positionManager(positionFilename),
    courseManager(courseFilename),
    compensationManager(compensationFilename),
    salaryManager(salaryFilename),
    scheduleManager(scheduleFilename),
    jobResponsibilityManager(responsibilityFilename),
    departmentManager(departmentFilename),
    performanceEvaluationManager(evaluationFilename),
    historyManager(historyFilename),
    roleValidator() {}

UserManager& RequestHandler::getUserManager() {
    return userManager;
}

std::string RequestHandler::processRequest(const std::string& request, const std::string& username, const std::string& role) {
    auto parts = split(request, '|');
    if (parts.empty()) return "ERROR|����������� �������";

    std::string command = parts[0];
    if (!roleValidator.isAllowed(role, command)) {
        return "ERROR|������������ ����";
    }

    if (command == "ADD_EMPLOYEE") {
        if (parts.size() < 9) return "ERROR|�������� ����������";
        long empID = std::stol(parts[1]);
        long posID = std::stol(parts[2]);
        std::string fullName = parts[3];
        std::string birthDate = parts[4];
        std::string address = parts[5];
        std::string contactInfo = parts[6];
        std::string startDate = parts[7];
        std::string status = parts[8];
        if (!isDateBeforeOrEqual(birthDate, startDate)) {
            return "ERROR|���� ������ ������ ������ ���� ����� ��� ��������� � ����� ��������.";
        }
        bool success = employeeManager.addEmployee(posID, fullName, birthDate, address, contactInfo, startDate, status);
        if (success) {
            historyManager.addHistory(username, command, "�������� ��������� ID " + std::to_string(empID));
            logger.log("INFO", command, "�������� ��������� ID " + std::to_string(empID));
            return "SUCCESS|��������� ��������.";
        }
        else return "ERROR|��������� � ����� ID ��� ����������.";
    }
    else if (command == "UPDATE_EMPLOYEE") {
        // �������:
        // UPDATE_EMPLOYEE|empID|fullName|address|contactInfo
        // ���
        // UPDATE_EMPLOYEE|empID|fullName|address|contactInfo|status
        long empID = std::stol(parts[1]);
        std::string fullName = parts[2];
        std::string address = parts[3];
        std::string contactInfo = parts[4];
        bool success = false;
        if (parts.size() == 6) {
            std::string status = parts[5];
            success = employeeManager.updateEmployeeFullData(empID, fullName, address, contactInfo, status);
        }
        else {
            success = employeeManager.updateEmployeeData(empID, fullName, address, contactInfo);
        }
        if (success) {
            historyManager.addHistory(username, command, "�������� ������ ���������� ID " + std::to_string(empID));
            logger.log("INFO", command, "�������� ������ ���������� ID " + std::to_string(empID));
            return "SUCCESS|������ ���������.";
        }
        return "ERROR|�� ������� �������� ������.";
    }
    else if (command == "BLOCK_EMPLOYEE") {
        if (parts.size() < 2) return "ERROR|�������� ����������";
        long empID = std::stol(parts[1]);
        bool success = employeeManager.blockEmployee(empID);
        if (success) {
            historyManager.addHistory(username, command, "������������ ��������� ID " + std::to_string(empID));
            logger.log("INFO", command, "������������ ��������� ID " + std::to_string(empID));
            return "SUCCESS|��������� ������������.";
        }
        return "ERROR|�� ������� �������������.";
    }
    else if (command == "VIEW_HISTORY") {
        std::string hist = historyManager.getHistory();
        return "SUCCESS|" + hist;
    }
    else if (command == "ADD_POSITION") {
        if (parts.size() < 6) return "ERROR|�������� ����������";
        long posID = std::stol(parts[1]);
        std::string posName = parts[2];
        double minSal = std::stod(parts[3]);
        double maxSal = std::stod(parts[4]);
        std::string qual = parts[5];
        bool success = positionManager.addPosition(posID, posName, minSal, maxSal, qual);
        if (success) {
            historyManager.addHistory(username, command, "��������� ��������� ID " + std::to_string(posID));
            logger.log("INFO", command, "��������� ��������� ID " + std::to_string(posID));
            return "SUCCESS|��������� ���������.";
        }
        return "ERROR|�� ������� �������� ���������.";
    }
    else if (command == "VIEW_ACTIVE_COURSES") {
        std::string courses = courseManager.getActiveCoursesFormatted();
        return "SUCCESS|" + courses;
    }
    else if (command == "VIEW_ACTIVE_CONNECTIONS") {
        // ����������� ��� �������� ���������� �� �����������, ������ �����
        return "SUCCESS|���� ��� ����������.";
    }
    else if (command == "ADD_USER") {
        if (parts.size() < 5) return "ERROR|�������� ����������";
        std::string newUsername = parts[1];
        std::string newPassword = parts[2];
        std::string newRole = parts[3];
        std::string status = parts[4];
        User u{ newUsername,newPassword,newRole,status };
        bool success = userManager.addUser(u);
        if (success) {
            historyManager.addHistory(username, command, "�������� ������������ " + newUsername);
            logger.log("INFO", command, "�������� ������������ " + newUsername);
            return "SUCCESS|������������ ��������.";
        }
        return "ERROR|�� ������� �������� ������������.";
    }
    else if (command == "DELETE_USER") {
        if (parts.size() < 2) return "ERROR|�������� ����������";
        std::string delUser = parts[1];
        bool success = userManager.deleteUser(delUser);
        if (success) {
            historyManager.addHistory(username, command, "������ ������������ " + delUser);
            logger.log("INFO", command, "������ ������������ " + delUser);
            return "SUCCESS|������������ ������.";
        }
        return "ERROR|�� ������� �������.";
    }
    else if (command == "VIEW_ALL_EMPLOYEES") {
        auto emps = employeeManager.getAllEmployees();
        std::ostringstream oss;
        for (auto& e : emps) {
            oss << "ID: " << e.id << ", ���: " << e.fullName << ", ���������: " << positionManager.getPositionName(e.posID)
                << ", �����: " << departmentManager.getDepartmentName(e.departmentID)
                << ", ��������: " << salaryManager.getSalary(e.id) << "\n";
        }
        historyManager.addHistory(username, command, "�������� ���� �����������");
        logger.log("INFO", command, "������������ " + username + " ������������� ���� �����������");
        return "SUCCESS|" + oss.str();
    }
    else if (command == "ADD_COURSE") {
        if (parts.size() < 5) return "ERROR|�������� ����������";
        std::string cid = parts[1];
        std::string topic = parts[2];
        std::string sDate = parts[3];
        std::string eDate = parts[4];
        bool success = courseManager.addCourse(cid, topic, sDate, eDate);
        if (success) {
            historyManager.addHistory(username, command, "�������� ���� " + cid);
            logger.log("INFO", command, "�������� ���� " + cid);
            return "SUCCESS|���� ��������.";
        }
        return "ERROR|�� ������� �������� ����.";
    }
    else if (command == "UPDATE_COURSE") {
        if (parts.size() < 5) return "ERROR|�������� ����������";
        std::string cid = parts[1];
        std::string topic = parts[2];
        std::string sDate = parts[3];
        std::string eDate = parts[4];
        bool success = courseManager.updateCourse(cid, topic, sDate, eDate);
        if (success) {
            historyManager.addHistory(username, command, "�������� ���� " + cid);
            logger.log("INFO", command, "�������� ���� " + cid);
            return "SUCCESS|���� ��������.";
        }
        return "ERROR|�� ������� �������� ����.";
    }
    else if (command == "DELETE_COURSE") {
        if (parts.size() < 2) return "ERROR|�������� ����������";
        std::string cid = parts[1];
        bool success = courseManager.deleteCourse(cid);
        if (success) {
            historyManager.addHistory(username, command, "������ ���� " + cid);
            logger.log("INFO", command, "������ ���� " + cid);
            return "SUCCESS|���� ������.";
        }
        return "ERROR|�� ������� ������� ����.";
    }
    else if (command == "VIEW_LOGS") {
        std::string logs = logger.getLogs();
        return "SUCCESS|" + logs;
    }
    else if (command == "ADD_WORK_SCHEDULE") {
        if (parts.size() < 5) return "ERROR|�������� ����������";
        long empID = std::stol(parts[1]);
        int dayOfWeek = std::stoi(parts[2]);
        std::string st = parts[3];
        std::string et = parts[4];
        bool success = scheduleManager.addSchedule(empID, dayOfWeek, st, et);
        if (success) {
            historyManager.addHistory(username, command, "�������� ������ ��� ���������� " + std::to_string(empID));
            logger.log("INFO", command, "�������� ������ ��� " + std::to_string(empID));
            return "SUCCESS|������ ��������.";
        }
        return "ERROR|�� ������� �������� ������.";
    }
    else if (command == "UPDATE_WORK_SCHEDULE") {
        if (parts.size() < 5) return "ERROR|�������� ����������";
        long empID = std::stol(parts[1]);
        int dayOfWeek = std::stoi(parts[2]);
        std::string st = parts[3];
        std::string et = parts[4];
        bool success = scheduleManager.updateSchedule(empID, dayOfWeek, st, et);
        if (success) {
            historyManager.addHistory(username, command, "�������� ������ ��� " + std::to_string(empID));
            logger.log("INFO", command, "�������� ������ ��� " + std::to_string(empID));
            return "SUCCESS|������ ��������.";
        }
        return "ERROR|�� ������� �������� ������.";
    }
    else if (command == "DELETE_WORK_SCHEDULE") {
        if (parts.size() < 2) return "ERROR|�������� ����������";
        long empID = std::stol(parts[1]);
        bool success = scheduleManager.deleteSchedule(empID);
        if (success) {
            historyManager.addHistory(username, command, "������ ������ ��� " + std::to_string(empID));
            logger.log("INFO", command, "������ ������ ��� " + std::to_string(empID));
            return "SUCCESS|������ ������.";
        }
        return "ERROR|�� ������� ������� ������.";
    }
    else if (command == "VIEW_WORK_SCHEDULE") {
        if (parts.size() < 2) return "ERROR|�������� ����������";
        long empID = std::stol(parts[1]);
        std::string sch = scheduleManager.viewSchedule(empID);
        return "SUCCESS|" + sch;
    }
    else if (command == "APPROVE_COMPENSATION") {
        if (parts.size() < 2) return "ERROR|�������� ����������";
        long cID = std::stol(parts[1]);
        bool success = compensationManager.approveRequest(cID);
        if (success) {
            historyManager.addHistory(username, command, "�������� ����������� " + std::to_string(cID));
            logger.log("INFO", command, "�������� ����������� " + std::to_string(cID));
            return "SUCCESS|����������� ��������.";
        }
        return "ERROR|�� ������� ��������.";
    }
    else if (command == "DECLINE_COMPENSATION") {
        if (parts.size() < 2) return "ERROR|�������� ����������";
        long cID = std::stol(parts[1]);
        bool success = compensationManager.declineRequest(cID);
        if (success) {
            historyManager.addHistory(username, command, "��������� ����������� " + std::to_string(cID));
            logger.log("INFO", command, "��������� ����������� " + std::to_string(cID));
            return "SUCCESS|����������� ���������.";
        }
        return "ERROR|�� ������� ���������.";
    }
    else if (command == "VIEW_SALARIES") {
        std::string sal = salaryManager.getAllSalariesFormatted();
        return "SUCCESS|" + sal;
    }
    else if (command == "PROCESS_COMPENSATION") {
        if (parts.size() < 3) return "ERROR|�������� ����������";
        long rID = std::stol(parts[1]);
        std::string ns = parts[2];
        bool success = compensationManager.processRequest(rID, ns);
        if (success) {
            historyManager.addHistory(username, command, "��������� ����������� " + std::to_string(rID) + " ������ " + ns);
            logger.log("INFO", command, "���������� ����������� " + std::to_string(rID));
            return "SUCCESS|����������� ����������.";
        }
        return "ERROR|�� ������� ����������.";
    }
    else if (command == "EVALUATE_PERFORMANCE") {
        return "ERROR|���������� ��� �� ����������.";
    }
    else if (command == "VIEW_OWN_INFO") {
        if (parts.size() < 2) return "ERROR|�������� ����������";
        std::string uname = parts[1];
        User u = userManager.getUser(uname);
        if (u.username.empty()) return "ERROR|��� ������ � ������������.";
        // �����������, ��� employeeID �������� ��������. ���� ���������� ��� �������
        // �������� employeeID == 1001 ��� ������� - �� ������� ���� �������
        return "SUCCESS|���� ������: �����: " + u.username + ", ����: " + u.role + ", ������: " + u.status;
    }
    else if (command == "VIEW_OWN_SCHEDULE") {
        if (parts.size() < 2) return "ERROR|�������� ����������";
        std::string uname = parts[1];
        // ����� username -> employeeID ����������. ����������� employeeID=1001
        // � �������� ������ ����� ������� ������
        long empID = 1001;
        std::string sch = scheduleManager.viewSchedule(empID);
        return "SUCCESS|" + sch;
    }
    else if (command == "ENROLL_COURSE") {
        if (parts.size() < 3) return "ERROR|�������� ����������";
        std::string cid = parts[1];
        long empID = std::stol(parts[2]);
        // ����������� ������ ������ � history
        historyManager.addHistory(username, command, "��������� " + std::to_string(empID) + " ��������� �� ���� " + cid);
        logger.log("INFO", command, "������ �� ���� " + cid);
        return "SUCCESS|�� �������� �� ����.";
    }
    else if (command == "REQUEST_COMPENSATION") {
        if (parts.size() < 8) return "ERROR|�������� ����������";
        long reqID = std::stol(parts[1]);
        long empID = std::stol(parts[2]);
        long posID = std::stol(parts[3]);
        std::string ctype = parts[4];
        double amount = std::stod(parts[5]);
        std::string sdate = parts[6];
        std::string stat = parts[7];
        bool success = compensationManager.addRequest(reqID, empID, posID, ctype, amount, sdate, stat);
        if (success) {
            historyManager.addHistory(username, command, "������ �� ����������� ID " + std::to_string(reqID));
            logger.log("INFO", command, "������ �� ����������� ID " + std::to_string(reqID));
            return "SUCCESS|������ ������.";
        }
        return "ERROR|�� ������� ������ ������.";
    }
    if (command == "BROADCAST_MESSAGE") {
        // ������ admin ����� ��� ������
        if (role != "admin") return "ERROR|������������ ����";
        if (parts.size() < 2) return "ERROR|�������� ����������";
        std::string msg = parts[1];
        connectionManager.broadcastMessage(msg);
        return "SUCCESS|��������� ���������� ����.";
    }

    return "ERROR|����������� �������";
}
