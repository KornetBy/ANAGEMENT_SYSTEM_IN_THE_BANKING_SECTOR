// RequestHandler.cpp
#include "RequestHandler.h"
#include "utils.h"
#include <sstream>
// RequestHandler.cpp
#include "ConnectionManager.h"
static bool isDateBeforeOrEqual(const std::string& d1, const std::string& d2) {
    // Примитивная проверка: сравним строки YYYY-MM-DD лексикографически
    // Предполагается, что формат всегда корректный
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
    if (parts.empty()) return "ERROR|Неизвестная команда";

    std::string command = parts[0];
    if (!roleValidator.isAllowed(role, command)) {
        return "ERROR|Недостаточно прав";
    }

    if (command == "ADD_EMPLOYEE") {
        if (parts.size() < 9) return "ERROR|Нехватка параметров";
        long empID = std::stol(parts[1]);
        long posID = std::stol(parts[2]);
        std::string fullName = parts[3];
        std::string birthDate = parts[4];
        std::string address = parts[5];
        std::string contactInfo = parts[6];
        std::string startDate = parts[7];
        std::string status = parts[8];
        if (!isDateBeforeOrEqual(birthDate, startDate)) {
            return "ERROR|Дата начала работы должна быть позже или совпадать с датой рождения.";
        }
        bool success = employeeManager.addEmployee(posID, fullName, birthDate, address, contactInfo, startDate, status);
        if (success) {
            historyManager.addHistory(username, command, "Добавлен сотрудник ID " + std::to_string(empID));
            logger.log("INFO", command, "Добавлен сотрудник ID " + std::to_string(empID));
            return "SUCCESS|Сотрудник добавлен.";
        }
        else return "ERROR|Сотрудник с таким ID уже существует.";
    }
    else if (command == "UPDATE_EMPLOYEE") {
        // Форматы:
        // UPDATE_EMPLOYEE|empID|fullName|address|contactInfo
        // или
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
            historyManager.addHistory(username, command, "Изменены данные сотрудника ID " + std::to_string(empID));
            logger.log("INFO", command, "Изменены данные сотрудника ID " + std::to_string(empID));
            return "SUCCESS|Данные обновлены.";
        }
        return "ERROR|Не удалось обновить данные.";
    }
    else if (command == "BLOCK_EMPLOYEE") {
        if (parts.size() < 2) return "ERROR|Нехватка параметров";
        long empID = std::stol(parts[1]);
        bool success = employeeManager.blockEmployee(empID);
        if (success) {
            historyManager.addHistory(username, command, "Заблокирован сотрудник ID " + std::to_string(empID));
            logger.log("INFO", command, "Заблокирован сотрудник ID " + std::to_string(empID));
            return "SUCCESS|Сотрудник заблокирован.";
        }
        return "ERROR|Не удалось заблокировать.";
    }
    else if (command == "VIEW_HISTORY") {
        std::string hist = historyManager.getHistory();
        return "SUCCESS|" + hist;
    }
    else if (command == "ADD_POSITION") {
        if (parts.size() < 6) return "ERROR|Нехватка параметров";
        long posID = std::stol(parts[1]);
        std::string posName = parts[2];
        double minSal = std::stod(parts[3]);
        double maxSal = std::stod(parts[4]);
        std::string qual = parts[5];
        bool success = positionManager.addPosition(posID, posName, minSal, maxSal, qual);
        if (success) {
            historyManager.addHistory(username, command, "Добавлена должность ID " + std::to_string(posID));
            logger.log("INFO", command, "Добавлена должность ID " + std::to_string(posID));
            return "SUCCESS|Должность добавлена.";
        }
        return "ERROR|Не удалось добавить должность.";
    }
    else if (command == "VIEW_ACTIVE_COURSES") {
        std::string courses = courseManager.getActiveCoursesFormatted();
        return "SUCCESS|" + courses;
    }
    else if (command == "VIEW_ACTIVE_CONNECTIONS") {
        // Предположим что активные соединения не реализованы, вернем пусто
        return "SUCCESS|Пока нет реализации.";
    }
    else if (command == "ADD_USER") {
        if (parts.size() < 5) return "ERROR|Нехватка параметров";
        std::string newUsername = parts[1];
        std::string newPassword = parts[2];
        std::string newRole = parts[3];
        std::string status = parts[4];
        User u{ newUsername,newPassword,newRole,status };
        bool success = userManager.addUser(u);
        if (success) {
            historyManager.addHistory(username, command, "Добавлен пользователь " + newUsername);
            logger.log("INFO", command, "Добавлен пользователь " + newUsername);
            return "SUCCESS|Пользователь добавлен.";
        }
        return "ERROR|Не удалось добавить пользователя.";
    }
    else if (command == "DELETE_USER") {
        if (parts.size() < 2) return "ERROR|Нехватка параметров";
        std::string delUser = parts[1];
        bool success = userManager.deleteUser(delUser);
        if (success) {
            historyManager.addHistory(username, command, "Удален пользователь " + delUser);
            logger.log("INFO", command, "Удален пользователь " + delUser);
            return "SUCCESS|Пользователь удален.";
        }
        return "ERROR|Не удалось удалить.";
    }
    else if (command == "VIEW_ALL_EMPLOYEES") {
        auto emps = employeeManager.getAllEmployees();
        std::ostringstream oss;
        for (auto& e : emps) {
            oss << "ID: " << e.id << ", ФИО: " << e.fullName << ", Должность: " << positionManager.getPositionName(e.posID)
                << ", Отдел: " << departmentManager.getDepartmentName(e.departmentID)
                << ", Зарплата: " << salaryManager.getSalary(e.id) << "\n";
        }
        historyManager.addHistory(username, command, "Просмотр всех сотрудников");
        logger.log("INFO", command, "Пользователь " + username + " просматривает всех сотрудников");
        return "SUCCESS|" + oss.str();
    }
    else if (command == "ADD_COURSE") {
        if (parts.size() < 5) return "ERROR|Нехватка параметров";
        std::string cid = parts[1];
        std::string topic = parts[2];
        std::string sDate = parts[3];
        std::string eDate = parts[4];
        bool success = courseManager.addCourse(cid, topic, sDate, eDate);
        if (success) {
            historyManager.addHistory(username, command, "Добавлен курс " + cid);
            logger.log("INFO", command, "Добавлен курс " + cid);
            return "SUCCESS|Курс добавлен.";
        }
        return "ERROR|Не удалось добавить курс.";
    }
    else if (command == "UPDATE_COURSE") {
        if (parts.size() < 5) return "ERROR|Нехватка параметров";
        std::string cid = parts[1];
        std::string topic = parts[2];
        std::string sDate = parts[3];
        std::string eDate = parts[4];
        bool success = courseManager.updateCourse(cid, topic, sDate, eDate);
        if (success) {
            historyManager.addHistory(username, command, "Обновлен курс " + cid);
            logger.log("INFO", command, "Обновлен курс " + cid);
            return "SUCCESS|Курс обновлен.";
        }
        return "ERROR|Не удалось обновить курс.";
    }
    else if (command == "DELETE_COURSE") {
        if (parts.size() < 2) return "ERROR|Нехватка параметров";
        std::string cid = parts[1];
        bool success = courseManager.deleteCourse(cid);
        if (success) {
            historyManager.addHistory(username, command, "Удален курс " + cid);
            logger.log("INFO", command, "Удален курс " + cid);
            return "SUCCESS|Курс удален.";
        }
        return "ERROR|Не удалось удалить курс.";
    }
    else if (command == "VIEW_LOGS") {
        std::string logs = logger.getLogs();
        return "SUCCESS|" + logs;
    }
    else if (command == "ADD_WORK_SCHEDULE") {
        if (parts.size() < 5) return "ERROR|Нехватка параметров";
        long empID = std::stol(parts[1]);
        int dayOfWeek = std::stoi(parts[2]);
        std::string st = parts[3];
        std::string et = parts[4];
        bool success = scheduleManager.addSchedule(empID, dayOfWeek, st, et);
        if (success) {
            historyManager.addHistory(username, command, "Добавлен график для сотрудника " + std::to_string(empID));
            logger.log("INFO", command, "Добавлен график для " + std::to_string(empID));
            return "SUCCESS|График добавлен.";
        }
        return "ERROR|Не удалось добавить график.";
    }
    else if (command == "UPDATE_WORK_SCHEDULE") {
        if (parts.size() < 5) return "ERROR|Нехватка параметров";
        long empID = std::stol(parts[1]);
        int dayOfWeek = std::stoi(parts[2]);
        std::string st = parts[3];
        std::string et = parts[4];
        bool success = scheduleManager.updateSchedule(empID, dayOfWeek, st, et);
        if (success) {
            historyManager.addHistory(username, command, "Обновлен график для " + std::to_string(empID));
            logger.log("INFO", command, "Обновлен график для " + std::to_string(empID));
            return "SUCCESS|График обновлен.";
        }
        return "ERROR|Не удалось обновить график.";
    }
    else if (command == "DELETE_WORK_SCHEDULE") {
        if (parts.size() < 2) return "ERROR|Нехватка параметров";
        long empID = std::stol(parts[1]);
        bool success = scheduleManager.deleteSchedule(empID);
        if (success) {
            historyManager.addHistory(username, command, "Удален график для " + std::to_string(empID));
            logger.log("INFO", command, "Удален график для " + std::to_string(empID));
            return "SUCCESS|График удален.";
        }
        return "ERROR|Не удалось удалить график.";
    }
    else if (command == "VIEW_WORK_SCHEDULE") {
        if (parts.size() < 2) return "ERROR|Нехватка параметров";
        long empID = std::stol(parts[1]);
        std::string sch = scheduleManager.viewSchedule(empID);
        return "SUCCESS|" + sch;
    }
    else if (command == "APPROVE_COMPENSATION") {
        if (parts.size() < 2) return "ERROR|Нехватка параметров";
        long cID = std::stol(parts[1]);
        bool success = compensationManager.approveRequest(cID);
        if (success) {
            historyManager.addHistory(username, command, "Одобрена компенсация " + std::to_string(cID));
            logger.log("INFO", command, "Одобрена компенсация " + std::to_string(cID));
            return "SUCCESS|Компенсация одобрена.";
        }
        return "ERROR|Не удалось одобрить.";
    }
    else if (command == "DECLINE_COMPENSATION") {
        if (parts.size() < 2) return "ERROR|Нехватка параметров";
        long cID = std::stol(parts[1]);
        bool success = compensationManager.declineRequest(cID);
        if (success) {
            historyManager.addHistory(username, command, "Отклонена компенсация " + std::to_string(cID));
            logger.log("INFO", command, "Отклонена компенсация " + std::to_string(cID));
            return "SUCCESS|Компенсация отклонена.";
        }
        return "ERROR|Не удалось отклонить.";
    }
    else if (command == "VIEW_SALARIES") {
        std::string sal = salaryManager.getAllSalariesFormatted();
        return "SUCCESS|" + sal;
    }
    else if (command == "PROCESS_COMPENSATION") {
        if (parts.size() < 3) return "ERROR|Нехватка параметров";
        long rID = std::stol(parts[1]);
        std::string ns = parts[2];
        bool success = compensationManager.processRequest(rID, ns);
        if (success) {
            historyManager.addHistory(username, command, "Обработка компенсации " + std::to_string(rID) + " статус " + ns);
            logger.log("INFO", command, "Обработана компенсация " + std::to_string(rID));
            return "SUCCESS|Компенсация обработана.";
        }
        return "ERROR|Не удалось обработать.";
    }
    else if (command == "EVALUATE_PERFORMANCE") {
        return "ERROR|Функционал еще не реализован.";
    }
    else if (command == "VIEW_OWN_INFO") {
        if (parts.size() < 2) return "ERROR|Нехватка параметров";
        std::string uname = parts[1];
        User u = userManager.getUser(uname);
        if (u.username.empty()) return "ERROR|Нет данных о пользователе.";
        // Предполагая, что employeeID хранится отдельно. Пока неизвестно как связать
        // Допустим employeeID == 1001 для примера - но реально надо хранить
        return "SUCCESS|Ваши данные: Логин: " + u.username + ", Роль: " + u.role + ", Статус: " + u.status;
    }
    else if (command == "VIEW_OWN_SCHEDULE") {
        if (parts.size() < 2) return "ERROR|Нехватка параметров";
        std::string uname = parts[1];
        // Связь username -> employeeID неизвестна. Предположим employeeID=1001
        // В реальном случае нужна таблица связей
        long empID = 1001;
        std::string sch = scheduleManager.viewSchedule(empID);
        return "SUCCESS|" + sch;
    }
    else if (command == "ENROLL_COURSE") {
        if (parts.size() < 3) return "ERROR|Нехватка параметров";
        std::string cid = parts[1];
        long empID = std::stol(parts[2]);
        // Предположим просто запись в history
        historyManager.addHistory(username, command, "Сотрудник " + std::to_string(empID) + " записался на курс " + cid);
        logger.log("INFO", command, "Запись на курс " + cid);
        return "SUCCESS|Вы записаны на курс.";
    }
    else if (command == "REQUEST_COMPENSATION") {
        if (parts.size() < 8) return "ERROR|Нехватка параметров";
        long reqID = std::stol(parts[1]);
        long empID = std::stol(parts[2]);
        long posID = std::stol(parts[3]);
        std::string ctype = parts[4];
        double amount = std::stod(parts[5]);
        std::string sdate = parts[6];
        std::string stat = parts[7];
        bool success = compensationManager.addRequest(reqID, empID, posID, ctype, amount, sdate, stat);
        if (success) {
            historyManager.addHistory(username, command, "Заявка на компенсацию ID " + std::to_string(reqID));
            logger.log("INFO", command, "Заявка на компенсацию ID " + std::to_string(reqID));
            return "SUCCESS|Заявка подана.";
        }
        return "ERROR|Не удалось подать заявку.";
    }
    if (command == "BROADCAST_MESSAGE") {
        // Только admin может это делать
        if (role != "admin") return "ERROR|Недостаточно прав";
        if (parts.size() < 2) return "ERROR|Нехватка параметров";
        std::string msg = parts[1];
        connectionManager.broadcastMessage(msg);
        return "SUCCESS|Сообщение отправлено всем.";
    }

    return "ERROR|Неизвестная команда";
}
