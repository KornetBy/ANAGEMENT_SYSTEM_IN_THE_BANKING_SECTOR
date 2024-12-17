// Server/RequestHandler.cpp
#include "RequestHandler.h"
#include <sstream>

// Конструктор
RequestHandler::RequestHandler()
    : logger("logs/server_logs.txt"), userManager("data/users.txt"), roleValidator() {}

// Обработчик AUTH
std::string RequestHandler::handleAuth(const std::vector<std::string>& parts) {
    if (parts.size() < 3) return "ERROR|Некорректные данные авторизации";

    std::string username = parts[1];
    std::string password = parts[2];

    // Получаем данные пользователя из UserManager
    User user = userManager.getUser(username);

    if (user.username.empty()) {
        logger.log(LogLevel::ERR, "AUTH", "Пользователь не найден: " + username);
        return "ERROR|Пользователь не найден";
    }

    // Проверяем пароль и статус
    if (user.password == password) {
        if (user.status == "active") {
            logger.log(LogLevel::INFO, "AUTH", "Успешная авторизация пользователя: " + username);
            return "SUCCESS|" + user.role;
        }
        else {
            logger.log(LogLevel::WARNING, "AUTH", "Попытка входа заблокированного пользователя: " + username);
            return "ERROR|Пользователь заблокирован";
        }
    }

    logger.log(LogLevel::ERR, "AUTH", "Неверный пароль для пользователя: " + username);
    return "ERROR|Неверный пароль";
}

// Обработчик ADD_EMPLOYEE
std::string RequestHandler::handleAddEmployee(const std::vector<std::string>& parts) {
    if (parts.size() < 3) return "ERROR|Некорректные данные для добавления сотрудника";

    std::string employeeName = parts[1];
    std::string employeeData = parts[2];

    // Создаём нового пользователя с ролью employee
    User newUser{ employeeName, "password123", "employee", "active" };
    if (userManager.addUser(newUser)) {
        logger.log(LogLevel::INFO, "ADD_EMPLOYEE", "Добавлен сотрудник: " + employeeName + " с данными: " + employeeData);
        // Здесь можно добавить запись в schedules.txt через ScheduleManager (не реализован)
        return "SUCCESS|Сотрудник добавлен";
    }
    else {
        logger.log(LogLevel::ERR, "ADD_EMPLOYEE", "Не удалось добавить сотрудника: " + employeeName);
        return "ERROR|Не удалось добавить сотрудника";
    }
}

// Обработчик EDIT_EMPLOYEE
std::string RequestHandler::handleEditEmployee(const std::vector<std::string>& parts) {
    if (parts.size() < 3) return "ERROR|Некорректные данные для редактирования сотрудника";

    std::string employeeName = parts[1];
    std::string newData = parts[2];

    // Для упрощения, предполагаем, что редактируется только пароль
    // В реальной системе нужно предоставить более гибкие возможности
    // Здесь мы просто изменим статус пользователя как пример
    if (userManager.updateUserStatus(employeeName, "active")) {
        logger.log(LogLevel::INFO, "EDIT_EMPLOYEE", "Изменены данные сотрудника: " + employeeName + " на " + newData);
        return "SUCCESS|Данные сотрудника обновлены";
    }
    else {
        logger.log(LogLevel::ERR, "EDIT_EMPLOYEE", "Не удалось изменить данные сотрудника: " + employeeName);
        return "ERROR|Не удалось изменить данные сотрудника";
    }
}

// Обработчик BLOCK_USER
std::string RequestHandler::handleBlockUser(const std::vector<std::string>& parts) {
    if (parts.size() < 2) return "ERROR|Некорректные данные для блокировки пользователя";

    std::string username = parts[1];

    if (userManager.updateUserStatus(username, "blocked")) {
        logger.log(LogLevel::INFO, "BLOCK_USER", "Пользователь заблокирован: " + username);
        return "SUCCESS|Пользователь заблокирован";
    }
    else {
        logger.log(LogLevel::ERR, "BLOCK_USER", "Не удалось заблокировать пользователя: " + username);
        return "ERROR|Не удалось заблокировать пользователя";
    }
}

// Обработчик GET_LOGS
std::string RequestHandler::handleGetLogs() {
    std::string logs = logger.getLogs();
    if (logs.find("ERROR|") == 0 || logs.find("WARNING|") == 0 || logs.find("INFO|") == 0) {
        return logs;
    }
    else {
        return "Логи пусты.";
    }
}

// Обработчик ADD_POSITION
std::string RequestHandler::handleAddPosition(const std::vector<std::string>& parts) {
    // Для упрощения, функция будет просто логировать добавление должности
    if (parts.size() < 2) return "ERROR|Некорректные данные для добавления должности";

    std::string positionName = parts[1];

    logger.log(LogLevel::INFO, "ADD_POSITION", "Добавлена должность: " + positionName);
    return "SUCCESS|Должность добавлена";
}

// Обработчик GET_ACTIVE_USERS
std::string RequestHandler::handleGetActiveUsers() {
    // Для упрощения, функция будет просто возвращать заглушку
    // В реальной системе нужно реализовать отслеживание активных пользователей
    return "active_user1, active_user2, active_user3";
}

// Обработчик MANAGE_SCHEDULES
std::string RequestHandler::handleManageSchedules(const std::vector<std::string>& parts) {
    // Для упрощения, функция будет просто логировать управление графиками
    if (parts.size() < 3) return "ERROR|Некорректные данные для управления графиками";

    std::string employeeName = parts[1];
    std::string scheduleData = parts[2];

    logger.log(LogLevel::INFO, "MANAGE_SCHEDULES", "Изменён график сотрудника: " + employeeName + " на " + scheduleData);
    return "SUCCESS|График сотрудника изменён";
}

// Обработчик EVALUATE_PERFORMANCE
std::string RequestHandler::handleEvaluatePerformance(const std::vector<std::string>& parts) {
    // Для упрощения, функция будет просто логировать оценку производительности
    if (parts.size() < 3) return "ERROR|Некорректные данные для оценки производительности";

    std::string employeeName = parts[1];
    std::string performanceData = parts[2];

    logger.log(LogLevel::INFO, "EVALUATE_PERFORMANCE", "Оценка производительности сотрудника: " + employeeName + " - " + performanceData);
    return "SUCCESS|Производительность оценена";
}

// Обработчик GET_SALARIES
std::string RequestHandler::handleGetSalaries() {
    // Для упрощения, функция будет просто возвращать заглушку
    // В реальной системе нужно реализовать получение зарплат
    return "John Doe: 50000, Jane Smith: 55000, Alice Johnson: 60000";
}

// Обработчик PROCESS_COMPENSATION
std::string RequestHandler::handleProcessCompensation(const std::vector<std::string>& parts) {
    if (parts.size() < 2) return "ERROR|Некорректные данные для обработки компенсации";

    std::string requestId = parts[1];

    // Для упрощения, функция будет просто логировать обработку компенсации
    logger.log(LogLevel::INFO, "PROCESS_COMPENSATION", "Обработана заявка на компенсацию с ID: " + requestId);
    return "SUCCESS|Заявка на компенсацию обработана";
}

// Обработчик GET_SELF_INFO
std::string RequestHandler::handleGetSelfInfo(const std::string& username) {
    // Для упрощения, функция будет просто возвращать заглушку
    // В реальной системе нужно реализовать получение информации о пользователе
    return "Username: " + username + ", Role: employee, Status: active";
}

// Обработчик REQUEST_COMPENSATION
std::string RequestHandler::handleRequestCompensation(const std::vector<std::string>& parts) {
    if (parts.size() < 2) return "ERROR|Некорректные данные для подачи заявки на компенсацию";

    std::string compensationData = parts[1];
    // Генерация уникального ID заявки (на основе времени)
    std::time_t now = std::time(nullptr);
    std::string requestId = std::to_string(now);

    // Добавление заявки в файл compensation_requests.txt
    std::ofstream file("data/compensation_requests.txt", std::ios::app);
    if (file.is_open()) {
        file << requestId << "|" << "current_user" << "|" << compensationData << "|" << "pending" << std::endl;
        file.close();
        logger.log(LogLevel::INFO, "REQUEST_COMPENSATION", "Подача заявки на компенсацию с ID: " + requestId);
        return "SUCCESS|Заявка на компенсацию подана с ID: " + requestId;
    }
    else {
        logger.log(LogLevel::ERR, "REQUEST_COMPENSATION", "Не удалось записать заявку на компенсацию");
        return "ERROR|Не удалось подать заявку на компенсацию";
    }
}

// Обработчик ADD_USER
std::string RequestHandler::handleAddUser(const std::vector<std::string>& parts) {
    if (parts.size() < 5) return "ERROR|Некорректные данные для добавления пользователя";

    std::string username = parts[1];
    std::string password = parts[2];
    std::string role = parts[3];
    std::string status = parts[4];

    User newUser{ username, password, role, status };
    if (userManager.addUser(newUser)) {
        logger.log(LogLevel::INFO, "ADD_USER", "Добавлен пользователь: " + username + " с ролью: " + role + " и статусом: " + status);
        return "SUCCESS|Пользователь добавлен";
    }
    else {
        logger.log(LogLevel::ERR, "ADD_USER", "Не удалось добавить пользователя: " + username);
        return "ERROR|Не удалось добавить пользователя";
    }
}

// Обработчик DELETE_USER
std::string RequestHandler::handleDeleteUser(const std::vector<std::string>& parts) {
    if (parts.size() < 2) return "ERROR|Некорректные данные для удаления пользователя";

    std::string username = parts[1];

    if (userManager.deleteUser(username)) {
        logger.log(LogLevel::INFO, "DELETE_USER", "Удалён пользователь: " + username);
        return "SUCCESS|Пользователь удалён";
    }
    else {
        logger.log(LogLevel::ERR, "DELETE_USER", "Не удалось удалить пользователя: " + username);
        return "ERROR|Не удалось удалить пользователя";
    }
}

// Метод обработки запроса
std::string RequestHandler::processRequest(const std::string& request, const std::string& username, const std::string& role) {
    std::vector<std::string> parts;
    std::istringstream stream(request);
    std::string part;
    while (std::getline(stream, part, '|')) {
        parts.push_back(part);
    }

    if (parts.empty()) {
        logger.log(LogLevel::ERR, "UNKNOWN_COMMAND", "Пустой запрос");
        return "ERROR|Пустой запрос";
    }

    // Обработка команды AUTH перед проверкой роли
    if (parts[0] == "AUTH") {
        return handleAuth(parts);
    }

    // Проверка доступа для остальных команд
    if (!roleValidator.isAllowed(role, parts[0])) {
        logger.log(LogLevel::WARNING, "ACCESS_DENIED", "Доступ запрещён для команды: " + parts[0] + " пользователем: " + username);
        return "ERROR|Доступ запрещён";
    }

    // Обработка остальных команд
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

    logger.log(LogLevel::ERR, "UNKNOWN_COMMAND", "Неизвестная команда: " + parts[0]);
    return "ERROR|Неизвестная команда";
}
