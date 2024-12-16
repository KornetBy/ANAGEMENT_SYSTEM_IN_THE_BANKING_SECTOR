#include "RequestHandler.h"
#include <sstream>

RequestHandler::RequestHandler() : userManager("data/users.txt") {}

std::string RequestHandler::handleAuth(const std::vector<std::string>& parts) {
    if (parts.size() < 3) return "ERROR|������������ ������ �����������";
    std::string username = parts[1];
    std::string password = parts[2];
    if (userManager.isUserActive(username) && password == "password") {
        std::string role = userManager.getRole(username);
        if (!role.empty()) {
            logger.log("INFO", "AUTH", "�������� ����������� ������������: " + username);
            return "SUCCESS|" + role;
        }
    }
    logger.log("ERROR", "AUTH", "������ ����������� ��� ������������: " + username);
    return "ERROR|�������� �����, ������ ��� ������������ ������������";
}

std::string RequestHandler::handleAddEmployee(const std::vector<std::string>& parts) {
    if (parts.size() < 3) return "ERROR|������������ ������ ��� ���������� ����������";
    logger.log("INFO", "ADD_EMPLOYEE", "�������� ���������: " + parts[1]);
    return "SUCCESS|��������� ��������";
}

std::string RequestHandler::handleBlockUser(const std::vector<std::string>& parts) {
    if (parts.size() < 2) return "ERROR|������������ ������ ��� ���������� ������������";
    if (userManager.blockUser(parts[1])) {
        logger.log("INFO", "BLOCK_USER", "������������ ������������: " + parts[1]);
        return "SUCCESS|������������ ������������";
    }
    logger.log("WARNING", "BLOCK_USER", "������� ���������� ��������������� ������������: " + parts[1]);
    return "ERROR|������������ �� ������";
}

std::string RequestHandler::handleGetLogs() {
    logger.log("INFO", "GET_LOGS", "��������� ����.");
    return logger.getLogs();
}

std::string RequestHandler::handleManageSchedules(const std::vector<std::string>& parts) {
    if (parts.size() < 3) return "ERROR|������������ ������ ��� ���������� ��������";
    if (scheduleManager.addSchedule(parts[1], parts[2])) {
        logger.log("INFO", "MANAGE_SCHEDULES", "������ �������� ��� ����������: " + parts[1]);
        return "SUCCESS|������ ��������";
    }
    logger.log("ERROR", "MANAGE_SCHEDULES", "������ ���������� ������� ��� ����������: " + parts[1]);
    return "ERROR|������ ���������� ��������";
}

std::string RequestHandler::handleRequestCompensation(const std::vector<std::string>& parts) {
    if (parts.size() < 3) return "ERROR|������������ ������ ��� ������ ������";
    if (compensationManager.addCompensationRequest(parts[1], parts[2])) {
        logger.log("INFO", "REQUEST_COMPENSATION", "������ �� ����������� ��������� ��� ����������: " + parts[1]);
        return "SUCCESS|������ ���������";
    }
    logger.log("ERROR", "REQUEST_COMPENSATION", "������ ���������� ������ ��� ����������: " + parts[1]);
    return "ERROR|������ ���������� ������";
}

std::string RequestHandler::processRequest(const std::string& request, const std::string& username, const std::string& role) {
    std::vector<std::string> parts;
    std::istringstream stream(request);
    std::string part;
    while (std::getline(stream, part, '|')) {
        parts.push_back(part);
    }

    if (!roleValidator.isAllowed(role, parts[0])) {
        logger.log("WARNING", "ACCESS_DENIED", "������ �������� ��� �������: " + parts[0] + " �������������: " + username);
        return "ERROR|������ ��������";
    }

    if (parts[0] == "AUTH") return handleAuth(parts);
    if (parts[0] == "ADD_EMPLOYEE") return handleAddEmployee(parts);
    if (parts[0] == "BLOCK_USER") return handleBlockUser(parts);
    if (parts[0] == "GET_LOGS") return handleGetLogs();
    if (parts[0] == "MANAGE_SCHEDULES") return handleManageSchedules(parts);
    if (parts[0] == "REQUEST_COMPENSATION") return handleRequestCompensation(parts);

    logger.log("ERROR", "UNKNOWN_COMMAND", "����������� �������: " + parts[0]);
    return "ERROR|����������� �������";
}
