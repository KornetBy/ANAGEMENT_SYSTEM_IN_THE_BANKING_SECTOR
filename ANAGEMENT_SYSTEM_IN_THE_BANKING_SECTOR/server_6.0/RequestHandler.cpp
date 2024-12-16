#include "RequestHandler.h"
#include <sstream>

RequestHandler::RequestHandler() : userManager("data/users.txt") {}

std::string RequestHandler::handleAuth(const std::vector<std::string>& parts) {
    if (parts.size() < 3) return "ERROR|Некорректные данные авторизации";
    std::string username = parts[1];
    std::string password = parts[2];
    if (userManager.isUserActive(username) && password == "password") {
        std::string role = userManager.getRole(username);
        if (!role.empty()) {
            logger.log("INFO", "AUTH", "Успешная авторизация пользователя: " + username);
            return "SUCCESS|" + role;
        }
    }
    logger.log("ERROR", "AUTH", "Ошибка авторизации для пользователя: " + username);
    return "ERROR|Неверный логин, пароль или пользователь заблокирован";
}

std::string RequestHandler::handleAddEmployee(const std::vector<std::string>& parts) {
    if (parts.size() < 3) return "ERROR|Недостаточно данных для добавления сотрудника";
    logger.log("INFO", "ADD_EMPLOYEE", "Добавлен сотрудник: " + parts[1]);
    return "SUCCESS|Сотрудник добавлен";
}

std::string RequestHandler::handleBlockUser(const std::vector<std::string>& parts) {
    if (parts.size() < 2) return "ERROR|Недостаточно данных для блокировки пользователя";
    if (userManager.blockUser(parts[1])) {
        logger.log("INFO", "BLOCK_USER", "Пользователь заблокирован: " + parts[1]);
        return "SUCCESS|Пользователь заблокирован";
    }
    logger.log("WARNING", "BLOCK_USER", "Попытка блокировки несуществующего пользователя: " + parts[1]);
    return "ERROR|Пользователь не найден";
}

std::string RequestHandler::handleGetLogs() {
    logger.log("INFO", "GET_LOGS", "Запрошены логи.");
    return logger.getLogs();
}

std::string RequestHandler::handleManageSchedules(const std::vector<std::string>& parts) {
    if (parts.size() < 3) return "ERROR|Недостаточно данных для управления графиком";
    if (scheduleManager.addSchedule(parts[1], parts[2])) {
        logger.log("INFO", "MANAGE_SCHEDULES", "График добавлен для сотрудника: " + parts[1]);
        return "SUCCESS|График добавлен";
    }
    logger.log("ERROR", "MANAGE_SCHEDULES", "Ошибка добавления графика для сотрудника: " + parts[1]);
    return "ERROR|Ошибка управления графиком";
}

std::string RequestHandler::handleRequestCompensation(const std::vector<std::string>& parts) {
    if (parts.size() < 3) return "ERROR|Недостаточно данных для подачи заявки";
    if (compensationManager.addCompensationRequest(parts[1], parts[2])) {
        logger.log("INFO", "REQUEST_COMPENSATION", "Заявка на компенсацию добавлена для сотрудника: " + parts[1]);
        return "SUCCESS|Заявка добавлена";
    }
    logger.log("ERROR", "REQUEST_COMPENSATION", "Ошибка добавления заявки для сотрудника: " + parts[1]);
    return "ERROR|Ошибка добавления заявки";
}

std::string RequestHandler::processRequest(const std::string& request, const std::string& username, const std::string& role) {
    std::vector<std::string> parts;
    std::istringstream stream(request);
    std::string part;
    while (std::getline(stream, part, '|')) {
        parts.push_back(part);
    }

    if (!roleValidator.isAllowed(role, parts[0])) {
        logger.log("WARNING", "ACCESS_DENIED", "Доступ запрещён для команды: " + parts[0] + " пользователем: " + username);
        return "ERROR|Доступ запрещён";
    }

    if (parts[0] == "AUTH") return handleAuth(parts);
    if (parts[0] == "ADD_EMPLOYEE") return handleAddEmployee(parts);
    if (parts[0] == "BLOCK_USER") return handleBlockUser(parts);
    if (parts[0] == "GET_LOGS") return handleGetLogs();
    if (parts[0] == "MANAGE_SCHEDULES") return handleManageSchedules(parts);
    if (parts[0] == "REQUEST_COMPENSATION") return handleRequestCompensation(parts);

    logger.log("ERROR", "UNKNOWN_COMMAND", "Неизвестная команда: " + parts[0]);
    return "ERROR|Неизвестная команда";
}
