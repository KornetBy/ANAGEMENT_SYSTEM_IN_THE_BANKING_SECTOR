// Server/RequestHandler.cpp
#include "RequestHandler.h"
#include <sstream>

// �����������
RequestHandler::RequestHandler()
    : logger("logs/server_logs.txt"), userManager("data/users.txt"), roleValidator() {}

// ���������� AUTH
std::string RequestHandler::handleAuth(const std::vector<std::string>& parts) {
    if (parts.size() < 3) return "ERROR|������������ ������ �����������";

    std::string username = parts[1];
    std::string password = parts[2];

    // �������� ������ ������������ �� UserManager
    User user = userManager.getUser(username);

    if (user.username.empty()) {
        logger.log(LogLevel::ERR, "AUTH", "������������ �� ������: " + username);
        return "ERROR|������������ �� ������";
    }

    // ��������� ������ � ������
    if (user.password == password) {
        if (user.status == "active") {
            logger.log(LogLevel::INFO, "AUTH", "�������� ����������� ������������: " + username);
            return "SUCCESS|" + user.role;
        }
        else {
            logger.log(LogLevel::WARNING, "AUTH", "������� ����� ���������������� ������������: " + username);
            return "ERROR|������������ ������������";
        }
    }

    logger.log(LogLevel::ERR, "AUTH", "�������� ������ ��� ������������: " + username);
    return "ERROR|�������� ������";
}

// ���������� ADD_EMPLOYEE
std::string RequestHandler::handleAddEmployee(const std::vector<std::string>& parts) {
    if (parts.size() < 3) return "ERROR|������������ ������ ��� ���������� ����������";

    std::string employeeName = parts[1];
    std::string employeeData = parts[2];

    // ������ ������ ������������ � ����� employee
    User newUser{ employeeName, "password123", "employee", "active" };
    if (userManager.addUser(newUser)) {
        logger.log(LogLevel::INFO, "ADD_EMPLOYEE", "�������� ���������: " + employeeName + " � �������: " + employeeData);
        // ����� ����� �������� ������ � schedules.txt ����� ScheduleManager (�� ����������)
        return "SUCCESS|��������� ��������";
    }
    else {
        logger.log(LogLevel::ERR, "ADD_EMPLOYEE", "�� ������� �������� ����������: " + employeeName);
        return "ERROR|�� ������� �������� ����������";
    }
}

// ���������� EDIT_EMPLOYEE
std::string RequestHandler::handleEditEmployee(const std::vector<std::string>& parts) {
    if (parts.size() < 3) return "ERROR|������������ ������ ��� �������������� ����������";

    std::string employeeName = parts[1];
    std::string newData = parts[2];

    // ��� ���������, ������������, ��� ������������� ������ ������
    // � �������� ������� ����� ������������ ����� ������ �����������
    // ����� �� ������ ������� ������ ������������ ��� ������
    if (userManager.updateUserStatus(employeeName, "active")) {
        logger.log(LogLevel::INFO, "EDIT_EMPLOYEE", "�������� ������ ����������: " + employeeName + " �� " + newData);
        return "SUCCESS|������ ���������� ���������";
    }
    else {
        logger.log(LogLevel::ERR, "EDIT_EMPLOYEE", "�� ������� �������� ������ ����������: " + employeeName);
        return "ERROR|�� ������� �������� ������ ����������";
    }
}

// ���������� BLOCK_USER
std::string RequestHandler::handleBlockUser(const std::vector<std::string>& parts) {
    if (parts.size() < 2) return "ERROR|������������ ������ ��� ���������� ������������";

    std::string username = parts[1];

    if (userManager.updateUserStatus(username, "blocked")) {
        logger.log(LogLevel::INFO, "BLOCK_USER", "������������ ������������: " + username);
        return "SUCCESS|������������ ������������";
    }
    else {
        logger.log(LogLevel::ERR, "BLOCK_USER", "�� ������� ������������� ������������: " + username);
        return "ERROR|�� ������� ������������� ������������";
    }
}

// ���������� GET_LOGS
std::string RequestHandler::handleGetLogs() {
    std::string logs = logger.getLogs();
    if (logs.find("ERROR|") == 0 || logs.find("WARNING|") == 0 || logs.find("INFO|") == 0) {
        return logs;
    }
    else {
        return "���� �����.";
    }
}

// ���������� ADD_POSITION
std::string RequestHandler::handleAddPosition(const std::vector<std::string>& parts) {
    // ��� ���������, ������� ����� ������ ���������� ���������� ���������
    if (parts.size() < 2) return "ERROR|������������ ������ ��� ���������� ���������";

    std::string positionName = parts[1];

    logger.log(LogLevel::INFO, "ADD_POSITION", "��������� ���������: " + positionName);
    return "SUCCESS|��������� ���������";
}

// ���������� GET_ACTIVE_USERS
std::string RequestHandler::handleGetActiveUsers() {
    // ��� ���������, ������� ����� ������ ���������� ��������
    // � �������� ������� ����� ����������� ������������ �������� �������������
    return "active_user1, active_user2, active_user3";
}

// ���������� MANAGE_SCHEDULES
std::string RequestHandler::handleManageSchedules(const std::vector<std::string>& parts) {
    // ��� ���������, ������� ����� ������ ���������� ���������� ���������
    if (parts.size() < 3) return "ERROR|������������ ������ ��� ���������� ���������";

    std::string employeeName = parts[1];
    std::string scheduleData = parts[2];

    logger.log(LogLevel::INFO, "MANAGE_SCHEDULES", "������ ������ ����������: " + employeeName + " �� " + scheduleData);
    return "SUCCESS|������ ���������� ������";
}

// ���������� EVALUATE_PERFORMANCE
std::string RequestHandler::handleEvaluatePerformance(const std::vector<std::string>& parts) {
    // ��� ���������, ������� ����� ������ ���������� ������ ������������������
    if (parts.size() < 3) return "ERROR|������������ ������ ��� ������ ������������������";

    std::string employeeName = parts[1];
    std::string performanceData = parts[2];

    logger.log(LogLevel::INFO, "EVALUATE_PERFORMANCE", "������ ������������������ ����������: " + employeeName + " - " + performanceData);
    return "SUCCESS|������������������ �������";
}

// ���������� GET_SALARIES
std::string RequestHandler::handleGetSalaries() {
    // ��� ���������, ������� ����� ������ ���������� ��������
    // � �������� ������� ����� ����������� ��������� �������
    return "John Doe: 50000, Jane Smith: 55000, Alice Johnson: 60000";
}

// ���������� PROCESS_COMPENSATION
std::string RequestHandler::handleProcessCompensation(const std::vector<std::string>& parts) {
    if (parts.size() < 2) return "ERROR|������������ ������ ��� ��������� �����������";

    std::string requestId = parts[1];

    // ��� ���������, ������� ����� ������ ���������� ��������� �����������
    logger.log(LogLevel::INFO, "PROCESS_COMPENSATION", "���������� ������ �� ����������� � ID: " + requestId);
    return "SUCCESS|������ �� ����������� ����������";
}

// ���������� GET_SELF_INFO
std::string RequestHandler::handleGetSelfInfo(const std::string& username) {
    // ��� ���������, ������� ����� ������ ���������� ��������
    // � �������� ������� ����� ����������� ��������� ���������� � ������������
    return "Username: " + username + ", Role: employee, Status: active";
}

// ���������� REQUEST_COMPENSATION
std::string RequestHandler::handleRequestCompensation(const std::vector<std::string>& parts) {
    if (parts.size() < 2) return "ERROR|������������ ������ ��� ������ ������ �� �����������";

    std::string compensationData = parts[1];
    // ��������� ����������� ID ������ (�� ������ �������)
    std::time_t now = std::time(nullptr);
    std::string requestId = std::to_string(now);

    // ���������� ������ � ���� compensation_requests.txt
    std::ofstream file("data/compensation_requests.txt", std::ios::app);
    if (file.is_open()) {
        file << requestId << "|" << "current_user" << "|" << compensationData << "|" << "pending" << std::endl;
        file.close();
        logger.log(LogLevel::INFO, "REQUEST_COMPENSATION", "������ ������ �� ����������� � ID: " + requestId);
        return "SUCCESS|������ �� ����������� ������ � ID: " + requestId;
    }
    else {
        logger.log(LogLevel::ERR, "REQUEST_COMPENSATION", "�� ������� �������� ������ �� �����������");
        return "ERROR|�� ������� ������ ������ �� �����������";
    }
}

// ���������� ADD_USER
std::string RequestHandler::handleAddUser(const std::vector<std::string>& parts) {
    if (parts.size() < 5) return "ERROR|������������ ������ ��� ���������� ������������";

    std::string username = parts[1];
    std::string password = parts[2];
    std::string role = parts[3];
    std::string status = parts[4];

    User newUser{ username, password, role, status };
    if (userManager.addUser(newUser)) {
        logger.log(LogLevel::INFO, "ADD_USER", "�������� ������������: " + username + " � �����: " + role + " � ��������: " + status);
        return "SUCCESS|������������ ��������";
    }
    else {
        logger.log(LogLevel::ERR, "ADD_USER", "�� ������� �������� ������������: " + username);
        return "ERROR|�� ������� �������� ������������";
    }
}

// ���������� DELETE_USER
std::string RequestHandler::handleDeleteUser(const std::vector<std::string>& parts) {
    if (parts.size() < 2) return "ERROR|������������ ������ ��� �������� ������������";

    std::string username = parts[1];

    if (userManager.deleteUser(username)) {
        logger.log(LogLevel::INFO, "DELETE_USER", "����� ������������: " + username);
        return "SUCCESS|������������ �����";
    }
    else {
        logger.log(LogLevel::ERR, "DELETE_USER", "�� ������� ������� ������������: " + username);
        return "ERROR|�� ������� ������� ������������";
    }
}

// ����� ��������� �������
std::string RequestHandler::processRequest(const std::string& request, const std::string& username, const std::string& role) {
    std::vector<std::string> parts;
    std::istringstream stream(request);
    std::string part;
    while (std::getline(stream, part, '|')) {
        parts.push_back(part);
    }

    if (parts.empty()) {
        logger.log(LogLevel::ERR, "UNKNOWN_COMMAND", "������ ������");
        return "ERROR|������ ������";
    }

    // ��������� ������� AUTH ����� ��������� ����
    if (parts[0] == "AUTH") {
        return handleAuth(parts);
    }

    // �������� ������� ��� ��������� ������
    if (!roleValidator.isAllowed(role, parts[0])) {
        logger.log(LogLevel::WARNING, "ACCESS_DENIED", "������ �������� ��� �������: " + parts[0] + " �������������: " + username);
        return "ERROR|������ ��������";
    }

    // ��������� ��������� ������
    if (parts[0] == "ADD_EMPLOYEE") return handleAddEmployee(parts);
    if (parts[0] == "EDIT_EMPLOYEE") return handleEditEmployee(parts);
    if (parts[0] == "BLOCK_USER") return handleBlockUser(parts);
    if (parts[0] == "GET_LOGS") return handleGetLogs();
    if (parts[0] == "ADD_POSITION") return handleAddPosition(parts);
    if (parts[0] == "GET_ACTIVE_USERS") return handleGetActiveUsers();
    if (parts[0] == "MANAGE_SCHEDULES") return handleManageSchedules(parts);
    if (parts[0] == "EVALUATE_PERFORMANCE") return handleEvaluatePerformance(parts);
    if (parts[0] == "GET_SALARIES") return handleGetSalaries();
    if (parts[0] == "PROCESS_COMPENSATION") return handleProcessCompensation(parts);
    if (parts[0] == "GET_SELF_INFO") return handleGetSelfInfo(username);
    if (parts[0] == "REQUEST_COMPENSATION") return handleRequestCompensation(parts);
    if (parts[0] == "ADD_USER") return handleAddUser(parts);
    if (parts[0] == "DELETE_USER") return handleDeleteUser(parts);

    logger.log(LogLevel::ERR, "UNKNOWN_COMMAND", "����������� �������: " + parts[0]);
    return "ERROR|����������� �������";
}
