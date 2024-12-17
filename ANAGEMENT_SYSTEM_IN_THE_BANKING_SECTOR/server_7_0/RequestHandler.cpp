// RequestHandler.cpp
#include "RequestHandler.h"
#include <sstream>

// ����������� � ������������� ��� ���� ����������� � ���������� ������
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
    roleValidator(),
    historyManager(historyFilename),
    employeeManager(employeeFilename),
    positionManager(positionFilename),
    courseManager(courseFilename),
    compensationManager(compensationFilename),
    salaryManager(salaryFilename),
    scheduleManager(scheduleFilename),
    jobResponsibilityManager(responsibilityFilename),
    departmentManager(departmentFilename),
    performanceEvaluationManager(evaluationFilename)
{}
std::string RequestHandler::processRequest(const std::string& request, const std::string& username, const std::string& role) {
    std::istringstream iss(request);
    std::string command;
    getline(iss, command, '|');

    // ������� AUTH �������������� �������� �� ������ ����������
    if (command == "AUTH") {
        return "SUCCESS|Authenticated";
    }

    // �������� ���� �������
    if (!roleValidator.isAllowed(role, command)) {
        return "ERROR|������������ ���� ��� ���������� ���� �������.";
    }

    // ��������� ������
    if (command == "ADD_EMPLOYEE") {
        // ������: ADD_EMPLOYEE|employeeID|positionID|fullName|birthDate|address|contactInfo|startDate|status
        std::string token;
        Employee emp;
        getline(iss, token, '|');
        emp.employeeID = std::stol(token);
        getline(iss, token, '|');
        emp.positionID = std::stol(token);
        getline(iss, emp.fullName, '|');
        getline(iss, emp.birthDate, '|');
        getline(iss, emp.address, '|');
        getline(iss, emp.contactInfo, '|');
        getline(iss, emp.startDate, '|');
        getline(iss, emp.status, '|');

        bool success = employeeManager.addEmployee(emp);
        if (success) {
            historyManager.addHistory(username, "ADD_EMPLOYEE", "�������� ���������: " + emp.fullName);
            logger.log("INFO", "ADD_EMPLOYEE", "�������� ���������: " + emp.fullName);
            return "SUCCESS|��������� ��������";
        }
        else {
            return "ERROR|�� ������� �������� ����������. ��������, ID ��� ����������.";
        }
    }

    if (command == "EDIT_EMPLOYEE") {
        // ������: EDIT_EMPLOYEE|employeeID|positionID|fullName|birthDate|address|contactInfo|startDate|status
        std::string token;
        Employee emp;
        getline(iss, token, '|');
        emp.employeeID = std::stol(token);
        getline(iss, token, '|');
        emp.positionID = std::stol(token);
        getline(iss, emp.fullName, '|');
        getline(iss, emp.birthDate, '|');
        getline(iss, emp.address, '|');
        getline(iss, emp.contactInfo, '|');
        getline(iss, emp.startDate, '|');
        getline(iss, emp.status, '|');

        bool success = employeeManager.editEmployee(emp);
        if (success) {
            historyManager.addHistory(username, "EDIT_EMPLOYEE", "�������� ������ ����������: " + emp.fullName);
            logger.log("INFO", "EDIT_EMPLOYEE", "�������� ������ ����������: " + emp.fullName);
            return "SUCCESS|������ ���������� ���������";
        }
        else {
            return "ERROR|�� ������� �������� ������ ����������. ��������, ID �� ����������.";
        }
    }

    if (command == "BLOCK_EMPLOYEE") {
        // ������: BLOCK_EMPLOYEE|employeeID
        std::string token;
        getline(iss, token, '|');
        long empID = std::stol(token);

        bool success = employeeManager.blockEmployee(empID);
        if (success) {
            historyManager.addHistory(username, "BLOCK_EMPLOYEE", "������������ ��������� � ID: " + std::to_string(empID));
            logger.log("INFO", "BLOCK_EMPLOYEE", "������������ ��������� � ID: " + std::to_string(empID));
            return "SUCCESS|��������� ������������";
        }
        else {
            return "ERROR|�� ������� ������������� ����������. ��������, ID �� ����������.";
        }
    }

    if (command == "VIEW_HISTORY") {
        // ������: VIEW_HISTORY
        std::string history = historyManager.getHistory();
        return "SUCCESS|" + history;
    }

    if (command == "ADD_POSITION") {
        // ������: ADD_POSITION|positionID|positionName|minSalary|maxSalary|qualificationRequirements
        std::string token;
        Position pos;
        getline(iss, token, '|');
        pos.positionID = std::stol(token);
        getline(iss, pos.positionName, '|');
        getline(iss, token, '|');
        pos.minSalary = std::stod(token);
        getline(iss, token, '|');
        pos.maxSalary = std::stod(token);
        getline(iss, pos.qualificationRequirements, '|');

        bool success = positionManager.addPosition(pos);
        if (success) {
            historyManager.addHistory(username, "ADD_POSITION", "��������� ���������: " + pos.positionName);
            logger.log("INFO", "ADD_POSITION", "��������� ���������: " + pos.positionName);
            return "SUCCESS|��������� ���������";
        }
        else {
            return "ERROR|�� ������� �������� ���������. ��������, ID ��� ����������.";
        }
    }

    if (command == "EDIT_POSITION") {
        // ������: EDIT_POSITION|positionID|positionName|minSalary|maxSalary|qualificationRequirements
        std::string token;
        Position pos;
        getline(iss, token, '|');
        pos.positionID = std::stol(token);
        getline(iss, pos.positionName, '|');
        getline(iss, token, '|');
        pos.minSalary = std::stod(token);
        getline(iss, token, '|');
        pos.maxSalary = std::stod(token);
        getline(iss, pos.qualificationRequirements, '|');

        bool success = positionManager.updatePosition(pos);
        if (success) {
            historyManager.addHistory(username, "EDIT_POSITION", "�������� ���������: " + pos.positionName);
            logger.log("INFO", "EDIT_POSITION", "�������� ���������: " + pos.positionName);
            return "SUCCESS|��������� ���������";
        }
        else {
            return "ERROR|�� ������� �������� ���������. ��������, ID �� ����������.";
        }
    }

    if (command == "DELETE_POSITION") {
        // ������: DELETE_POSITION|positionID
        std::string token;
        getline(iss, token, '|');
        long posID = std::stol(token);

        bool success = positionManager.deletePosition(posID);
        if (success) {
            historyManager.addHistory(username, "DELETE_POSITION", "������� ��������� � ID: " + std::to_string(posID));
            logger.log("INFO", "DELETE_POSITION", "������� ��������� � ID: " + std::to_string(posID));
            return "SUCCESS|��������� �������";
        }
        else {
            return "ERROR|�� ������� ������� ���������. ��������, ID �� ����������.";
        }
    }

    if (command == "ADD_COURSE") {
        // ������: ADD_COURSE|courseID|topic|startDate|endDate
        std::string courseID, topic, startDate, endDate;
        getline(iss, courseID, '|');
        getline(iss, topic, '|');
        getline(iss, startDate, '|');
        getline(iss, endDate, '|');

        Course course{ courseID, topic, startDate, endDate };
        bool success = courseManager.addCourse(course);
        if (success) {
            historyManager.addHistory(username, "ADD_COURSE", "�������� ����: " + course.topic);
            logger.log("INFO", "ADD_COURSE", "�������� ����: " + course.topic);
            return "SUCCESS|���� ��������";
        }
        else {
            return "ERROR|�� ������� �������� ����. ��������, ID ��� ����������.";
        }
    }

    if (command == "EDIT_COURSE") {
        // ������: EDIT_COURSE|courseID|topic|startDate|endDate
        std::string courseID, topic, startDate, endDate;
        getline(iss, courseID, '|');
        getline(iss, topic, '|');
        getline(iss, startDate, '|');
        getline(iss, endDate, '|');

        Course course{ courseID, topic, startDate, endDate };
        bool success = courseManager.updateCourse(course);
        if (success) {
            historyManager.addHistory(username, "EDIT_COURSE", "������� ����: " + course.topic);
            logger.log("INFO", "EDIT_COURSE", "������� ����: " + course.topic);
            return "SUCCESS|���� ��������";
        }
        else {
            return "ERROR|�� ������� �������� ����. ��������, ID �� ����������.";
        }
    }

    if (command == "DELETE_COURSE") {
        // ������: DELETE_COURSE|courseID
        std::string courseID;
        getline(iss, courseID, '|');

        bool success = courseManager.deleteCourse(courseID);
        if (success) {
            historyManager.addHistory(username, "DELETE_COURSE", "������ ���� � ID: " + courseID);
            logger.log("INFO", "DELETE_COURSE", "������ ���� � ID: " + courseID);
            return "SUCCESS|���� ������";
        }
        else {
            return "ERROR|�� ������� ������� ����. ��������, ID �� ����������.";
        }
    }

    if (command == "ENROLL_COURSE") {
        // ������: ENROLL_COURSE|courseID|employeeID
        std::string courseID, token;
        getline(iss, courseID, '|');
        getline(iss, token, '|');
        long employeeID = std::stol(token);

        // �������� ������������� �����
        Course course = courseManager.getCourse(courseID);
        if (course.courseID.empty()) {
            return "ERROR|���� � ����� ID �� ����������.";
        }

        // ����� ����� �������� ������ ������ �� ���� (��������, ������� ������ � �������������� �������)
        // ��� �������� �����������, ��� ������ �� ���� ������ �������

        historyManager.addHistory(username, "ENROLL_COURSE", "��������� ID " + std::to_string(employeeID) + " ��������� �� ����: " + course.topic);
        logger.log("INFO", "ENROLL_COURSE", "��������� ID " + std::to_string(employeeID) + " ��������� �� ����: " + course.topic);
        return "SUCCESS|�� ������� ���������� �� ���� \"" + course.topic + "\"";
    }

    if (command == "REQUEST_COMPENSATION") {
        // ������: REQUEST_COMPENSATION|requestID|employeeID|positionID|compensationType|amount|submissionDate|status
        std::string token;
        CompensationRequest req;
        getline(iss, token, '|');
        req.requestID = std::stol(token);
        getline(iss, token, '|');
        req.employeeID = std::stol(token);
        getline(iss, token, '|');
        req.positionID = std::stol(token);
        getline(iss, req.compensationType, '|');
        getline(iss, token, '|');
        req.amount = std::stod(token);
        getline(iss, req.submissionDate, '|');
        getline(iss, req.status, '|');

        bool success = compensationManager.addRequest(req);
        if (success) {
            historyManager.addHistory(username, "REQUEST_COMPENSATION", "������ ������ ID " + std::to_string(req.requestID));
            logger.log("INFO", "REQUEST_COMPENSATION", "������ ������ ID " + std::to_string(req.requestID));
            return "SUCCESS|������ �� ����������� ������ � ID: " + std::to_string(req.requestID);
        }
        else {
            return "ERROR|�� ������� ������ ������. ��������, ID ��� ����������.";
        }
    }

    if (command == "PROCESS_COMPENSATION") {
        // ������: PROCESS_COMPENSATION|requestID|newStatus
        std::string token, newStatus;
        getline(iss, token, '|');
        long requestID = std::stol(token);
        getline(iss, newStatus, '|');

        bool success = compensationManager.processRequest(requestID, newStatus);
        if (success) {
            historyManager.addHistory(username, "PROCESS_COMPENSATION", "���������� ������ ID " + std::to_string(requestID) + " ������: " + newStatus);
            logger.log("INFO", "PROCESS_COMPENSATION", "���������� ������ ID " + std::to_string(requestID) + " ������: " + newStatus);
            return "SUCCESS|������ ���������� � ������ ������� ��: " + newStatus;
        }
        else {
            return "ERROR|�� ������� ���������� ������. ��������, ID �� ����������.";
        }
    }

    if (command == "VIEW_SALARIES") {
        // ������: VIEW_SALARIES
        std::vector<Salary> salaries = salaryManager.getAllSalaries();
        std::string response = "";
        for (const auto& sal : salaries) {
            response += std::to_string(sal.salaryID) + "|" + std::to_string(sal.employeeID) + "|"
                + std::to_string(sal.positionID) + "|" + std::to_string(sal.baseSalary) + "|"
                + std::to_string(sal.bonuses) + "|" + std::to_string(sal.deductions) + "|"
                + std::to_string(sal.netPay) + "|" + sal.calculationDate + "\n";
        }
        return "SUCCESS|" + response;
    }

    if (command == "VIEW_LOGS") {
        // ������: VIEW_LOGS
        // ����������� �������� ����� ���������� ��������� �������
        // ��������������, ��� ���� �������� � ��������� �����, � ����� ����������� ��������� �������� ��� �����
        // ����� ���������� ���������� ���-�����
        std::ifstream ifs("logs/server_logs.txt");
        if (!ifs.is_open()) {
            return "ERROR|�� ������� ������� ���-����.";
        }
        std::ostringstream ss;
        ss << ifs.rdbuf();
        ifs.close();
        return "SUCCESS|" + ss.str();
    }

    if (command == "ADD_JOB_RESPONSIBILITY") {
        // ������: ADD_JOB_RESPONSIBILITY|responsibilityID|employeeID|positionID|responsibilityDescription
        std::string token, description;
        JobResponsibility resp;
        getline(iss, token, '|');
        resp.responsibilityID = std::stol(token);
        getline(iss, token, '|');
        resp.employeeID = std::stol(token);
        getline(iss, token, '|');
        resp.positionID = std::stol(token);
        getline(iss, resp.responsibilityDescription, '|');

        bool success = jobResponsibilityManager.addResponsibility(resp);
        if (success) {
            historyManager.addHistory(username, "ADD_JOB_RESPONSIBILITY", "��������� ����������� ID " + std::to_string(resp.responsibilityID));
            logger.log("INFO", "ADD_JOB_RESPONSIBILITY", "��������� ����������� ID " + std::to_string(resp.responsibilityID));
            return "SUCCESS|����������� ���������";
        }
        else {
            return "ERROR|�� ������� �������� �����������. ��������, ID ��� ����������.";
        }
    }

    if (command == "EDIT_JOB_RESPONSIBILITY") {
        // ������: EDIT_JOB_RESPONSIBILITY|responsibilityID|employeeID|positionID|responsibilityDescription
        std::string token, description;
        JobResponsibility resp;
        getline(iss, token, '|');
        resp.responsibilityID = std::stol(token);
        getline(iss, token, '|');
        resp.employeeID = std::stol(token);
        getline(iss, token, '|');
        resp.positionID = std::stol(token);
        getline(iss, resp.responsibilityDescription, '|');

        bool success = jobResponsibilityManager.updateResponsibility(resp);
        if (success) {
            historyManager.addHistory(username, "EDIT_JOB_RESPONSIBILITY", "�������� ����������� ID " + std::to_string(resp.responsibilityID));
            logger.log("INFO", "EDIT_JOB_RESPONSIBILITY", "�������� ����������� ID " + std::to_string(resp.responsibilityID));
            return "SUCCESS|����������� ���������";
        }
        else {
            return "ERROR|�� ������� �������� �����������. ��������, ID �� ����������.";
        }
    }

    if (command == "DELETE_JOB_RESPONSIBILITY") {
        // ������: DELETE_JOB_RESPONSIBILITY|responsibilityID
        std::string token;
        getline(iss, token, '|');
        long respID = std::stol(token);

        bool success = jobResponsibilityManager.deleteResponsibility(respID);
        if (success) {
            historyManager.addHistory(username, "DELETE_JOB_RESPONSIBILITY", "������� ����������� ID: " + std::to_string(respID));
            logger.log("INFO", "DELETE_JOB_RESPONSIBILITY", "������� ����������� ID: " + std::to_string(respID));
            return "SUCCESS|����������� �������";
        }
        else {
            return "ERROR|�� ������� ������� �����������. ��������, ID �� ����������.";
        }
    }

    if (command == "ADD_DEPARTMENT") {
        // ������: ADD_DEPARTMENT|departmentID|departmentName|location|positionID
        std::string token, deptName, location;
        long posID;
        Department dept;
        getline(iss, token, '|');
        dept.departmentID = std::stol(token);
        getline(iss, dept.departmentName, '|');
        getline(iss, dept.location, '|');
        getline(iss, token, '|');
        dept.positionID = std::stol(token);

        bool success = departmentManager.addDepartment(dept);
        if (success) {
            historyManager.addHistory(username, "ADD_DEPARTMENT", "�������� �����: " + dept.departmentName);
            logger.log("INFO", "ADD_DEPARTMENT", "�������� �����: " + dept.departmentName);
            return "SUCCESS|����� ��������";
        }
        else {
            return "ERROR|�� ������� �������� �����. ��������, ID ��� ����������.";
        }
    }

    if (command == "EDIT_DEPARTMENT") {
        // ������: EDIT_DEPARTMENT|departmentID|departmentName|location|positionID
        std::string token, deptName, location;
        long posID;
        Department dept;
        getline(iss, token, '|');
        dept.departmentID = std::stol(token);
        getline(iss, dept.departmentName, '|');
        getline(iss, dept.location, '|');
        getline(iss, token, '|');
        dept.positionID = std::stol(token);

        bool success = departmentManager.updateDepartment(dept);
        if (success) {
            historyManager.addHistory(username, "EDIT_DEPARTMENT", "������� �����: " + dept.departmentName);
            logger.log("INFO", "EDIT_DEPARTMENT", "������� �����: " + dept.departmentName);
            return "SUCCESS|����� ��������";
        }
        else {
            return "ERROR|�� ������� �������� �����. ��������, ID �� ����������.";
        }
    }

    if (command == "DELETE_DEPARTMENT") {
        // ������: DELETE_DEPARTMENT|departmentID
        std::string token;
        getline(iss, token, '|');
        long deptID = std::stol(token);

        bool success = departmentManager.deleteDepartment(deptID);
        if (success) {
            historyManager.addHistory(username, "DELETE_DEPARTMENT", "������ ����� � ID: " + std::to_string(deptID));
            logger.log("INFO", "DELETE_DEPARTMENT", "������ ����� � ID: " + std::to_string(deptID));
            return "SUCCESS|����� ������";
        }
        else {
            return "ERROR|�� ������� ������� �����. ��������, ID �� ����������.";
        }
    }

    if (command == "ADD_PERFORMANCE_EVALUATION") {
        // ������: ADD_PERFORMANCE_EVALUATION|evaluationID|employeeID|positionID|evaluationDate|evaluationResult|comments
        std::string token, evalDate, evalResult, comments;
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

        bool success = performanceEvaluationManager.addEvaluation(eval);
        if (success) {
            historyManager.addHistory(username, "ADD_PERFORMANCE_EVALUATION", "��������� ������ ID " + std::to_string(eval.evaluationID));
            logger.log("INFO", "ADD_PERFORMANCE_EVALUATION", "��������� ������ ID " + std::to_string(eval.evaluationID));
            return "SUCCESS|������ ���������";
        }
        else {
            return "ERROR|�� ������� �������� ������. ��������, ID ��� ����������.";
        }
    }

    if (command == "EDIT_PERFORMANCE_EVALUATION") {
        // ������: EDIT_PERFORMANCE_EVALUATION|evaluationID|employeeID|positionID|evaluationDate|evaluationResult|comments
        std::string token, evalDate, evalResult, comments;
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

        bool success = performanceEvaluationManager.updateEvaluation(eval);
        if (success) {
            historyManager.addHistory(username, "EDIT_PERFORMANCE_EVALUATION", "�������� ������ ID " + std::to_string(eval.evaluationID));
            logger.log("INFO", "EDIT_PERFORMANCE_EVALUATION", "�������� ������ ID " + std::to_string(eval.evaluationID));
            return "SUCCESS|������ ���������";
        }
        else {
            return "ERROR|�� ������� �������� ������. ��������, ID �� ����������.";
        }
    }

    if (command == "DELETE_PERFORMANCE_EVALUATION") {
        // ������: DELETE_PERFORMANCE_EVALUATION|evaluationID
        std::string token;
        getline(iss, token, '|');
        long evalID = std::stol(token);

        bool success = performanceEvaluationManager.deleteEvaluation(evalID);
        if (success) {
            historyManager.addHistory(username, "DELETE_PERFORMANCE_EVALUATION", "������� ������ ID: " + std::to_string(evalID));
            logger.log("INFO", "DELETE_PERFORMANCE_EVALUATION", "������� ������ ID: " + std::to_string(evalID));
            return "SUCCESS|������ �������";
        }
        else {
            return "ERROR|�� ������� ������� ������. ��������, ID �� ����������.";
        }
    }

    if (command == "VIEW_ACTIVE_CONNECTIONS") {
        // ������: VIEW_ACTIVE_CONNECTIONS
        // ���������� ������� �� ����, ��� �� ������� �������� ����������
        // � ������ ������� ��������������, ��� �������� ���������� �� �������� �� �������
        // ����� ����������� ������� ���������� �������� ��������
        // ��� �������� ���������� ��������
        return "SUCCESS|���������� �������� ����������: [��������]";
    }

    if (command == "VIEW_OWN_INFO") {
        // ������: VIEW_OWN_INFO|username
        std::string viewUsername;
        getline(iss, viewUsername, '|');

        // ��������� ������������
        User user = userManager.getUser(viewUsername);
        if (user.username.empty()) {
            return "ERROR|������������ �� ������.";
        }

        // ��������� ���������� � ����������
        // ��������������, ��� username ������������� employeeID ��� ���������� ����� ����� ����
        // � ������ ������� ���������� ����������� employeeID
        long employeeID = 1001; // ������, � �������� ������� ����� ������� username � employeeID
        Employee emp = employeeManager.getEmployee(employeeID);
        if (emp.employeeID == 0) {
            return "ERROR|���������� � ���������� �� �������.";
        }

        std::ostringstream oss;
        oss << "ID: " << emp.employeeID << "|��������� ID: " << emp.positionID << "|���: " << emp.fullName
            << "|���� ��������: " << emp.birthDate << "|�����: " << emp.address
            << "|���������� ����������: " << emp.contactInfo << "|���� ������ ������: " << emp.startDate
            << "|������: " << emp.status;

        return "SUCCESS|" + oss.str();
    }

    if (command == "VIEW_OWN_SCHEDULE") {
        // ������: VIEW_OWN_SCHEDULE|username
        std::string viewUsername;
        getline(iss, viewUsername, '|');

        // ��������� ������������
        User user = userManager.getUser(viewUsername);
        if (user.username.empty()) {
            return "ERROR|������������ �� ������.";
        }

        // ��������� ���������� � ����������
        long employeeID = 1001; // ������, � �������� ������� ����� ������� username � employeeID
        std::vector<Schedule> schedules = scheduleManager.getAllSchedules();
        std::string response = "";
        for (const auto& sched : schedules) {
            if (sched.employeeID == employeeID) {
                response += std::to_string(sched.scheduleID) + "|" + std::to_string(sched.positionID) + "|"
                    + sched.scheduleType + "|" + sched.shiftDetails + "|" + sched.scheduleDates + "\n";
            }
        }

        if (response.empty()) {
            return "SUCCESS|� ��� ��� ����������� �������� ������.";
        }

        return "SUCCESS|" + response;
    }

    if (command == "ADD_SCHEDULE") {
        // ������: ADD_SCHEDULE|scheduleID|employeeID|positionID|scheduleType|shiftDetails|scheduleDates
        std::string token, scheduleType, shiftDetails, scheduleDates;
        Schedule sched;
        getline(iss, token, '|');
        sched.scheduleID = std::stol(token);
        getline(iss, token, '|');
        sched.employeeID = std::stol(token);
        getline(iss, token, '|');
        sched.positionID = std::stol(token);
        getline(iss, sched.scheduleType, '|');
        getline(iss, sched.shiftDetails, '|');
        getline(iss, sched.scheduleDates, '|');

        bool success = scheduleManager.addSchedule(sched);
        if (success) {
            historyManager.addHistory(username, "ADD_SCHEDULE", "�������� ������ ID " + std::to_string(sched.scheduleID));
            logger.log("INFO", "ADD_SCHEDULE", "�������� ������ ID " + std::to_string(sched.scheduleID));
            return "SUCCESS|������ ������ ��������";
        }
        else {
            return "ERROR|�� ������� �������� ������ ������. ��������, ID ��� ����������.";
        }
    }

    if (command == "EDIT_SCHEDULE") {
        // ������: EDIT_SCHEDULE|scheduleID|employeeID|positionID|scheduleType|shiftDetails|scheduleDates
        std::string token, scheduleType, shiftDetails, scheduleDates;
        Schedule sched;
        getline(iss, token, '|');
        sched.scheduleID = std::stol(token);
        getline(iss, token, '|');
        sched.employeeID = std::stol(token);
        getline(iss, token, '|');
        sched.positionID = std::stol(token);
        getline(iss, sched.scheduleType, '|');
        getline(iss, sched.shiftDetails, '|');
        getline(iss, sched.scheduleDates, '|');

        bool success = scheduleManager.updateSchedule(sched);
        if (success) {
            historyManager.addHistory(username, "EDIT_SCHEDULE", "������� ������ ID " + std::to_string(sched.scheduleID));
            logger.log("INFO", "EDIT_SCHEDULE", "������� ������ ID " + std::to_string(sched.scheduleID));
            return "SUCCESS|������ ������ ��������";
        }
        else {
            return "ERROR|�� ������� �������� ������ ������. ��������, ID �� ����������.";
        }
    }

    if (command == "DELETE_SCHEDULE") {
        // ������: DELETE_SCHEDULE|scheduleID
        std::string token;
        getline(iss, token, '|');
        long schedID = std::stol(token);

        bool success = scheduleManager.deleteSchedule(schedID);
        if (success) {
            historyManager.addHistory(username, "DELETE_SCHEDULE", "������ ������ ID: " + std::to_string(schedID));
            logger.log("INFO", "DELETE_SCHEDULE", "������ ������ ID: " + std::to_string(schedID));
            return "SUCCESS|������ ������ ������";
        }
        else {
            return "ERROR|�� ������� ������� ������ ������. ��������, ID �� ����������.";
        }
    }

    if (command == "VIEW_SCHEDULES") {
        // ������: VIEW_SCHEDULES|employeeID (��� ��������� ������ �������) ��� VIEW_SCHEDULES|all (��� ��������� ���� ��������)
        std::string param;
        getline(iss, param, '|');

        std::vector<Schedule> schedules;
        if (param == "all") {
            schedules = scheduleManager.getAllSchedules();
        }
        else {
            long empID = std::stol(param);
            schedules = scheduleManager.getAllSchedules();
            // ���������� �� employeeID
            schedules.erase(
                std::remove_if(schedules.begin(), schedules.end(),
                    [empID](const Schedule& s) { return s.employeeID != empID; }),
                schedules.end());
        }

        std::string response = "";
        for (const auto& sched : schedules) {
            response += std::to_string(sched.scheduleID) + "|" + std::to_string(sched.employeeID) + "|"
                + std::to_string(sched.positionID) + "|" + sched.scheduleType + "|"
                + sched.shiftDetails + "|" + sched.scheduleDates + "\n";
        }

        if (response.empty()) {
            return "SUCCESS|��� �������� ������ ��� �����������.";
        }

        return "SUCCESS|" + response;
    }
    if (command == "VIEW_ALL_EMPLOYEES") {
        // ������: VIEW_ALL_EMPLOYEES
        std::vector<Employee> employees = employeeManager.getAllEmployees();
        std::string response = "";
        for (const auto& emp : employees) {
            response += std::to_string(emp.employeeID) + "|" + std::to_string(emp.positionID) + "|"
                + emp.fullName + "|" + emp.birthDate + "|" + emp.address + "|"
                + emp.contactInfo + "|" + emp.startDate + "|" + emp.status + "\n";
        }
        if (response.empty()) {
            return "SUCCESS|��� ����������� ��� �����������.";
        }
        return "SUCCESS|" + response;
    }
    if (command == "VIEW_ALL_POSITIONS") {
        // ������: VIEW_ALL_POSITIONS
        std::vector<Position> positions = positionManager.getAllPositions();
        std::string response = "";
        for (const auto& pos : positions) {
            response += std::to_string(pos.positionID) + "|" + pos.positionName + "|"
                + std::to_string(pos.minSalary) + "|" + std::to_string(pos.maxSalary)
                + "|" + pos.qualificationRequirements + "\n";
        }
        if (response.empty()) {
            return "SUCCESS|��� ���������� ��� �����������.";
        }
        return "SUCCESS|" + response;
    }
    if (command == "VIEW_ALL_DEPARTMENTS") {
        // ������: VIEW_ALL_DEPARTMENTS
        std::vector<Department> departments = departmentManager.getAllDepartments();
        std::string response = "";
        for (const auto& dept : departments) {
            response += std::to_string(dept.departmentID) + "|" + dept.departmentName + "|"
                + dept.location + "|" + std::to_string(dept.positionID) + "\n";
        }
        if (response.empty()) {
            return "SUCCESS|��� ������� ��� �����������.";
        }
        return "SUCCESS|" + response;
    }

    if (command == "VIEW_ALL_SCHEDULES") {
        // ������: VIEW_ALL_SCHEDULES
        std::vector<Schedule> schedules = scheduleManager.getAllSchedules();
        std::string response = "";
        for (const auto& sched : schedules) {
            response += std::to_string(sched.scheduleID) + "|" + std::to_string(sched.employeeID) + "|"
                + std::to_string(sched.positionID) + "|" + sched.scheduleType + "|"
                + sched.shiftDetails + "|" + sched.scheduleDates + "\n";
        }
        if (response.empty()) {
            return "SUCCESS|��� �������� ������ ��� �����������.";
        }
        return "SUCCESS|" + response;
    }
    if (command == "VIEW_ALL_COMPENSATIONS") {
        // ������: VIEW_ALL_COMPENSATIONS
        std::vector<CompensationRequest> requests = compensationManager.getAllRequests();
        std::string response = "";
        for (const auto& req : requests) {
            response += std::to_string(req.requestID) + "|" + std::to_string(req.employeeID) + "|"
                + std::to_string(req.positionID) + "|" + req.compensationType + "|"
                + std::to_string(req.amount) + "|" + req.submissionDate + "|"
                + req.status + "\n";
        }
        if (response.empty()) {
            return "SUCCESS|��� ������ �� ����������� ��� �����������.";
        }
        return "SUCCESS|" + response;
    }
    if (command == "VIEW_ALL_EVALUATIONS") {
        // ������: VIEW_ALL_EVALUATIONS
        std::vector<PerformanceEvaluation> evaluations = performanceEvaluationManager.getAllEvaluations();
        std::string response = "";
        for (const auto& eval : evaluations) {
            response += std::to_string(eval.evaluationID) + "|" + std::to_string(eval.employeeID) + "|"
                + std::to_string(eval.positionID) + "|" + eval.evaluationDate + "|"
                + eval.evaluationResult + "|" + eval.comments + "\n";
        }
        if (response.empty()) {
            return "SUCCESS|��� ������ ������������������ ��� �����������.";
        }
        return "SUCCESS|" + response;
    }
    if (command == "VIEW_ALL_RESPONSIBILITIES") {
        // ������: VIEW_ALL_RESPONSIBILITIES
        std::vector<JobResponsibility> responsibilities = jobResponsibilityManager.getAllResponsibilities();
        std::string response = "";
        for (const auto& resp : responsibilities) {
            response += std::to_string(resp.responsibilityID) + "|" + std::to_string(resp.employeeID) + "|"
                + std::to_string(resp.positionID) + "|" + resp.responsibilityDescription + "\n";
        }
        if (response.empty()) {
            return "SUCCESS|��� ������������ ��� �����������.";
        }
        return "SUCCESS|" + response;
    }
    if (command == "VIEW_ALL_COURSES") {
        // ������: VIEW_ALL_COURSES
        std::vector<Course> courses = courseManager.getAllCourses();
        std::string response = "";
        for (const auto& course : courses) {
            response += course.courseID + "|" + course.topic + "|" + course.startDate + "|" + course.endDate + "\n";
        }
        if (response.empty()) {
            return "SUCCESS|��� ������ ��� �����������.";
        }
        return "SUCCESS|" + response;
    }
    if (command == "VIEW_ALL_SALARIES") {
        // ������: VIEW_ALL_SALARIES
        std::vector<Salary> salaries = salaryManager.getAllSalaries();
        std::string response = "";
        for (const auto& sal : salaries) {
            response += std::to_string(sal.salaryID) + "|" + std::to_string(sal.employeeID) + "|"
                + std::to_string(sal.positionID) + "|" + std::to_string(sal.baseSalary) + "|"
                + std::to_string(sal.bonuses) + "|" + std::to_string(sal.deductions) + "|"
                + std::to_string(sal.netPay) + "|" + sal.calculationDate + "\n";
        }
        if (response.empty()) {
            return "SUCCESS|��� ������� ��� �����������.";
        }
        return "SUCCESS|" + response;
    }
    if (command == "VIEW_MY_RESPONSIBILITIES") {
        // ������: VIEW_MY_RESPONSIBILITIES|employeeID
        std::string token;
        getline(iss, token, '|');
        long employeeID = std::stol(token);

        std::vector<JobResponsibility> responsibilities = jobResponsibilityManager.getAllResponsibilities();
        std::string response = "";
        for (const auto& resp : responsibilities) {
            if (resp.employeeID == employeeID) {
                response += std::to_string(resp.responsibilityID) + "|"
                    + std::to_string(resp.positionID) + "|"
                    + resp.responsibilityDescription + "\n";
            }
        }

        if (response.empty()) {
            return "SUCCESS|� ��� ��� ����������� ������������.";
        }

        return "SUCCESS|" + response;
    }
    if (command == "VIEW_MY_COURSES") {
        // ������: VIEW_MY_COURSES|employeeID
        std::string token;
        getline(iss, token, '|');
        long employeeID = std::stol(token);

        // ��������������, ��� ������ �� ����� �������� � ��������� ��������� ��� �������
        // ��� ������� ���������� ����� courseManager.getCoursesByEmployee(employeeID)
        // ���������� ����������� ���� ����� � CourseManager

        std::vector<Course> myCourses = courseManager.getCoursesByEmployee(employeeID);
        std::string response = "";
        for (const auto& course : myCourses) {
            response += course.courseID + "|" + course.topic + "|" + course.startDate + "|" + course.endDate + "\n";
        }

        if (response.empty()) {
            return "SUCCESS|�� �� �������� �� �����.";
        }

        return "SUCCESS|" + response;
    }
    if (command == "PROCESS_COMPENSATION") {
        // ������: PROCESS_COMPENSATION|requestID|newStatus
        std::string token, newStatus;
        getline(iss, token, '|');
        long requestID = std::stol(token);
        getline(iss, newStatus, '|');

        bool success = compensationManager.processRequest(requestID, newStatus);
        if (success) {
            historyManager.addHistory(username, "PROCESS_COMPENSATION", "���������� ������ ID " + std::to_string(requestID) + " ������: " + newStatus);
            logger.log("INFO", "PROCESS_COMPENSATION", "���������� ������ ID " + std::to_string(requestID) + " ������: " + newStatus);
            return "SUCCESS|������ ���������� � ������ ������� ��: " + newStatus;
        }
        else {
            return "ERROR|�� ������� ���������� ������. ��������, ID �� ����������.";
        }
    }
    if (command == "ADD_SALARY") {
        // ������: ADD_SALARY|salaryID|employeeID|positionID|baseSalary|bonuses|deductions|netPay|calculationDate
        std::string token;
        Salary sal;
        getline(iss, token, '|');
        sal.salaryID = std::stol(token);
        getline(iss, token, '|');
        sal.employeeID = std::stol(token);
        getline(iss, token, '|');
        sal.positionID = std::stol(token);
        getline(iss, token, '|');
        sal.baseSalary = std::stod(token);
        getline(iss, token, '|');
        sal.bonuses = std::stod(token);
        getline(iss, token, '|');
        sal.deductions = std::stod(token);
        getline(iss, token, '|');
        sal.netPay = std::stod(token);
        getline(iss, sal.calculationDate, '|');

        bool success = salaryManager.addSalary(sal);
        if (success) {
            historyManager.addHistory(username, "ADD_SALARY", "��������� �������� ID " + std::to_string(sal.salaryID));
            logger.log("INFO", "ADD_SALARY", "��������� �������� ID " + std::to_string(sal.salaryID));
            return "SUCCESS|�������� ���������";
        }
        else {
            return "ERROR|�� ������� �������� ��������. ��������, ID ��� ����������.";
        }
    }
    if (command == "EDIT_SALARY") {
        // ������: EDIT_SALARY|salaryID|employeeID|positionID|baseSalary|bonuses|deductions|netPay|calculationDate
        std::string token;
        Salary sal;
        getline(iss, token, '|');
        sal.salaryID = std::stol(token);
        getline(iss, token, '|');
        sal.employeeID = std::stol(token);
        getline(iss, token, '|');
        sal.positionID = std::stol(token);
        getline(iss, token, '|');
        sal.baseSalary = std::stod(token);
        getline(iss, token, '|');
        sal.bonuses = std::stod(token);
        getline(iss, token, '|');
        sal.deductions = std::stod(token);
        getline(iss, token, '|');
        sal.netPay = std::stod(token);
        getline(iss, sal.calculationDate, '|');

        bool success = salaryManager.updateSalary(sal);
        if (success) {
            historyManager.addHistory(username, "EDIT_SALARY", "�������� �������� ID " + std::to_string(sal.salaryID));
            logger.log("INFO", "EDIT_SALARY", "�������� �������� ID " + std::to_string(sal.salaryID));
            return "SUCCESS|�������� ���������";
        }
        else {
            return "ERROR|�� ������� �������� ��������. ��������, ID �� ����������.";
        }
    }
    if (command == "DELETE_SALARY") {
        // ������: DELETE_SALARY|salaryID
        std::string token;
        getline(iss, token, '|');
        long salaryID = std::stol(token);

        bool success = salaryManager.deleteSalary(salaryID);
        if (success) {
            historyManager.addHistory(username, "DELETE_SALARY", "������� �������� ID: " + std::to_string(salaryID));
            logger.log("INFO", "DELETE_SALARY", "������� �������� ID: " + std::to_string(salaryID));
            return "SUCCESS|�������� �������";
        }
        else {
            return "ERROR|�� ������� ������� ��������. ��������, ID �� ����������.";
        }
    }
    if (command == "VIEW_ALL_SCHEDULES") {
        // ������: VIEW_ALL_SCHEDULES
        std::vector<Schedule> schedules = scheduleManager.getAllSchedules();
        std::string response = "";
        for (const auto& sched : schedules) {
            response += std::to_string(sched.scheduleID) + "|" + std::to_string(sched.employeeID) + "|"
                + std::to_string(sched.positionID) + "|" + sched.scheduleType + "|"
                + sched.shiftDetails + "|" + sched.scheduleDates + "\n";
        }
        if (response.empty()) {
            return "SUCCESS|��� �������� ������ ��� �����������.";
        }
        return "SUCCESS|" + response;
    }


    return "ERROR|����������� �������.";
}
