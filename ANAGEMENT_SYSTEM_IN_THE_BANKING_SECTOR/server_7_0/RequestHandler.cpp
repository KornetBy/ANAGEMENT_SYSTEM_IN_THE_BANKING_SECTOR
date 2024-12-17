// RequestHandler.cpp
#include "RequestHandler.h"
#include <sstream>

// Конструктор и инициализация уже были реализованы в предыдущем ответе
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

    // Команда AUTH обрабатывается отдельно на уровне соединения
    if (command == "AUTH") {
        return "SUCCESS|Authenticated";
    }

    // Проверка прав доступа
    if (!roleValidator.isAllowed(role, command)) {
        return "ERROR|Недостаточно прав для выполнения этой команды.";
    }

    // Обработка команд
    if (command == "ADD_EMPLOYEE") {
        // Формат: ADD_EMPLOYEE|employeeID|positionID|fullName|birthDate|address|contactInfo|startDate|status
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
            historyManager.addHistory(username, "ADD_EMPLOYEE", "Добавлен сотрудник: " + emp.fullName);
            logger.log("INFO", "ADD_EMPLOYEE", "Добавлен сотрудник: " + emp.fullName);
            return "SUCCESS|Сотрудник добавлен";
        }
        else {
            return "ERROR|Не удалось добавить сотрудника. Возможно, ID уже существует.";
        }
    }

    if (command == "EDIT_EMPLOYEE") {
        // Формат: EDIT_EMPLOYEE|employeeID|positionID|fullName|birthDate|address|contactInfo|startDate|status
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
            historyManager.addHistory(username, "EDIT_EMPLOYEE", "Изменены данные сотрудника: " + emp.fullName);
            logger.log("INFO", "EDIT_EMPLOYEE", "Изменены данные сотрудника: " + emp.fullName);
            return "SUCCESS|Данные сотрудника обновлены";
        }
        else {
            return "ERROR|Не удалось обновить данные сотрудника. Возможно, ID не существует.";
        }
    }

    if (command == "BLOCK_EMPLOYEE") {
        // Формат: BLOCK_EMPLOYEE|employeeID
        std::string token;
        getline(iss, token, '|');
        long empID = std::stol(token);

        bool success = employeeManager.blockEmployee(empID);
        if (success) {
            historyManager.addHistory(username, "BLOCK_EMPLOYEE", "Заблокирован сотрудник с ID: " + std::to_string(empID));
            logger.log("INFO", "BLOCK_EMPLOYEE", "Заблокирован сотрудник с ID: " + std::to_string(empID));
            return "SUCCESS|Сотрудник заблокирован";
        }
        else {
            return "ERROR|Не удалось заблокировать сотрудника. Возможно, ID не существует.";
        }
    }

    if (command == "VIEW_HISTORY") {
        // Формат: VIEW_HISTORY
        std::string history = historyManager.getHistory();
        return "SUCCESS|" + history;
    }

    if (command == "ADD_POSITION") {
        // Формат: ADD_POSITION|positionID|positionName|minSalary|maxSalary|qualificationRequirements
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
            historyManager.addHistory(username, "ADD_POSITION", "Добавлена должность: " + pos.positionName);
            logger.log("INFO", "ADD_POSITION", "Добавлена должность: " + pos.positionName);
            return "SUCCESS|Должность добавлена";
        }
        else {
            return "ERROR|Не удалось добавить должность. Возможно, ID уже существует.";
        }
    }

    if (command == "EDIT_POSITION") {
        // Формат: EDIT_POSITION|positionID|positionName|minSalary|maxSalary|qualificationRequirements
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
            historyManager.addHistory(username, "EDIT_POSITION", "Изменена должность: " + pos.positionName);
            logger.log("INFO", "EDIT_POSITION", "Изменена должность: " + pos.positionName);
            return "SUCCESS|Должность обновлена";
        }
        else {
            return "ERROR|Не удалось обновить должность. Возможно, ID не существует.";
        }
    }

    if (command == "DELETE_POSITION") {
        // Формат: DELETE_POSITION|positionID
        std::string token;
        getline(iss, token, '|');
        long posID = std::stol(token);

        bool success = positionManager.deletePosition(posID);
        if (success) {
            historyManager.addHistory(username, "DELETE_POSITION", "Удалена должность с ID: " + std::to_string(posID));
            logger.log("INFO", "DELETE_POSITION", "Удалена должность с ID: " + std::to_string(posID));
            return "SUCCESS|Должность удалена";
        }
        else {
            return "ERROR|Не удалось удалить должность. Возможно, ID не существует.";
        }
    }

    if (command == "ADD_COURSE") {
        // Формат: ADD_COURSE|courseID|topic|startDate|endDate
        std::string courseID, topic, startDate, endDate;
        getline(iss, courseID, '|');
        getline(iss, topic, '|');
        getline(iss, startDate, '|');
        getline(iss, endDate, '|');

        Course course{ courseID, topic, startDate, endDate };
        bool success = courseManager.addCourse(course);
        if (success) {
            historyManager.addHistory(username, "ADD_COURSE", "Добавлен курс: " + course.topic);
            logger.log("INFO", "ADD_COURSE", "Добавлен курс: " + course.topic);
            return "SUCCESS|Курс добавлен";
        }
        else {
            return "ERROR|Не удалось добавить курс. Возможно, ID уже существует.";
        }
    }

    if (command == "EDIT_COURSE") {
        // Формат: EDIT_COURSE|courseID|topic|startDate|endDate
        std::string courseID, topic, startDate, endDate;
        getline(iss, courseID, '|');
        getline(iss, topic, '|');
        getline(iss, startDate, '|');
        getline(iss, endDate, '|');

        Course course{ courseID, topic, startDate, endDate };
        bool success = courseManager.updateCourse(course);
        if (success) {
            historyManager.addHistory(username, "EDIT_COURSE", "Изменен курс: " + course.topic);
            logger.log("INFO", "EDIT_COURSE", "Изменен курс: " + course.topic);
            return "SUCCESS|Курс обновлен";
        }
        else {
            return "ERROR|Не удалось обновить курс. Возможно, ID не существует.";
        }
    }

    if (command == "DELETE_COURSE") {
        // Формат: DELETE_COURSE|courseID
        std::string courseID;
        getline(iss, courseID, '|');

        bool success = courseManager.deleteCourse(courseID);
        if (success) {
            historyManager.addHistory(username, "DELETE_COURSE", "Удален курс с ID: " + courseID);
            logger.log("INFO", "DELETE_COURSE", "Удален курс с ID: " + courseID);
            return "SUCCESS|Курс удален";
        }
        else {
            return "ERROR|Не удалось удалить курс. Возможно, ID не существует.";
        }
    }

    if (command == "ENROLL_COURSE") {
        // Формат: ENROLL_COURSE|courseID|employeeID
        std::string courseID, token;
        getline(iss, courseID, '|');
        getline(iss, token, '|');
        long employeeID = std::stol(token);

        // Проверка существования курса
        Course course = courseManager.getCourse(courseID);
        if (course.courseID.empty()) {
            return "ERROR|Курс с таким ID не существует.";
        }

        // Здесь можно добавить логику записи на курс (например, создать запись в дополнительной таблице)
        // Для простоты предположим, что запись на курс всегда успешна

        historyManager.addHistory(username, "ENROLL_COURSE", "Сотрудник ID " + std::to_string(employeeID) + " записался на курс: " + course.topic);
        logger.log("INFO", "ENROLL_COURSE", "Сотрудник ID " + std::to_string(employeeID) + " записался на курс: " + course.topic);
        return "SUCCESS|Вы успешно записались на курс \"" + course.topic + "\"";
    }

    if (command == "REQUEST_COMPENSATION") {
        // Формат: REQUEST_COMPENSATION|requestID|employeeID|positionID|compensationType|amount|submissionDate|status
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
            historyManager.addHistory(username, "REQUEST_COMPENSATION", "Подача заявки ID " + std::to_string(req.requestID));
            logger.log("INFO", "REQUEST_COMPENSATION", "Подача заявки ID " + std::to_string(req.requestID));
            return "SUCCESS|Заявка на компенсацию подана с ID: " + std::to_string(req.requestID);
        }
        else {
            return "ERROR|Не удалось подать заявку. Возможно, ID уже существует.";
        }
    }

    if (command == "PROCESS_COMPENSATION") {
        // Формат: PROCESS_COMPENSATION|requestID|newStatus
        std::string token, newStatus;
        getline(iss, token, '|');
        long requestID = std::stol(token);
        getline(iss, newStatus, '|');

        bool success = compensationManager.processRequest(requestID, newStatus);
        if (success) {
            historyManager.addHistory(username, "PROCESS_COMPENSATION", "Обработана заявка ID " + std::to_string(requestID) + " статус: " + newStatus);
            logger.log("INFO", "PROCESS_COMPENSATION", "Обработана заявка ID " + std::to_string(requestID) + " статус: " + newStatus);
            return "SUCCESS|Заявка обработана и статус изменен на: " + newStatus;
        }
        else {
            return "ERROR|Не удалось обработать заявку. Возможно, ID не существует.";
        }
    }

    if (command == "VIEW_SALARIES") {
        // Формат: VIEW_SALARIES
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
        // Формат: VIEW_LOGS
        // Реализовать просмотр логов аналогично просмотру истории
        // Предполагается, что логи хранятся в отдельном файле, и можно реализовать отдельный менеджер для логов
        // Здесь возвращаем содержимое лог-файла
        std::ifstream ifs("logs/server_logs.txt");
        if (!ifs.is_open()) {
            return "ERROR|Не удалось открыть лог-файл.";
        }
        std::ostringstream ss;
        ss << ifs.rdbuf();
        ifs.close();
        return "SUCCESS|" + ss.str();
    }

    if (command == "ADD_JOB_RESPONSIBILITY") {
        // Формат: ADD_JOB_RESPONSIBILITY|responsibilityID|employeeID|positionID|responsibilityDescription
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
            historyManager.addHistory(username, "ADD_JOB_RESPONSIBILITY", "Добавлена обязанность ID " + std::to_string(resp.responsibilityID));
            logger.log("INFO", "ADD_JOB_RESPONSIBILITY", "Добавлена обязанность ID " + std::to_string(resp.responsibilityID));
            return "SUCCESS|Обязанность добавлена";
        }
        else {
            return "ERROR|Не удалось добавить обязанность. Возможно, ID уже существует.";
        }
    }

    if (command == "EDIT_JOB_RESPONSIBILITY") {
        // Формат: EDIT_JOB_RESPONSIBILITY|responsibilityID|employeeID|positionID|responsibilityDescription
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
            historyManager.addHistory(username, "EDIT_JOB_RESPONSIBILITY", "Изменена обязанность ID " + std::to_string(resp.responsibilityID));
            logger.log("INFO", "EDIT_JOB_RESPONSIBILITY", "Изменена обязанность ID " + std::to_string(resp.responsibilityID));
            return "SUCCESS|Обязанность обновлена";
        }
        else {
            return "ERROR|Не удалось обновить обязанность. Возможно, ID не существует.";
        }
    }

    if (command == "DELETE_JOB_RESPONSIBILITY") {
        // Формат: DELETE_JOB_RESPONSIBILITY|responsibilityID
        std::string token;
        getline(iss, token, '|');
        long respID = std::stol(token);

        bool success = jobResponsibilityManager.deleteResponsibility(respID);
        if (success) {
            historyManager.addHistory(username, "DELETE_JOB_RESPONSIBILITY", "Удалена обязанность ID: " + std::to_string(respID));
            logger.log("INFO", "DELETE_JOB_RESPONSIBILITY", "Удалена обязанность ID: " + std::to_string(respID));
            return "SUCCESS|Обязанность удалена";
        }
        else {
            return "ERROR|Не удалось удалить обязанность. Возможно, ID не существует.";
        }
    }

    if (command == "ADD_DEPARTMENT") {
        // Формат: ADD_DEPARTMENT|departmentID|departmentName|location|positionID
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
            historyManager.addHistory(username, "ADD_DEPARTMENT", "Добавлен отдел: " + dept.departmentName);
            logger.log("INFO", "ADD_DEPARTMENT", "Добавлен отдел: " + dept.departmentName);
            return "SUCCESS|Отдел добавлен";
        }
        else {
            return "ERROR|Не удалось добавить отдел. Возможно, ID уже существует.";
        }
    }

    if (command == "EDIT_DEPARTMENT") {
        // Формат: EDIT_DEPARTMENT|departmentID|departmentName|location|positionID
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
            historyManager.addHistory(username, "EDIT_DEPARTMENT", "Изменен отдел: " + dept.departmentName);
            logger.log("INFO", "EDIT_DEPARTMENT", "Изменен отдел: " + dept.departmentName);
            return "SUCCESS|Отдел обновлен";
        }
        else {
            return "ERROR|Не удалось обновить отдел. Возможно, ID не существует.";
        }
    }

    if (command == "DELETE_DEPARTMENT") {
        // Формат: DELETE_DEPARTMENT|departmentID
        std::string token;
        getline(iss, token, '|');
        long deptID = std::stol(token);

        bool success = departmentManager.deleteDepartment(deptID);
        if (success) {
            historyManager.addHistory(username, "DELETE_DEPARTMENT", "Удален отдел с ID: " + std::to_string(deptID));
            logger.log("INFO", "DELETE_DEPARTMENT", "Удален отдел с ID: " + std::to_string(deptID));
            return "SUCCESS|Отдел удален";
        }
        else {
            return "ERROR|Не удалось удалить отдел. Возможно, ID не существует.";
        }
    }

    if (command == "ADD_PERFORMANCE_EVALUATION") {
        // Формат: ADD_PERFORMANCE_EVALUATION|evaluationID|employeeID|positionID|evaluationDate|evaluationResult|comments
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
            historyManager.addHistory(username, "ADD_PERFORMANCE_EVALUATION", "Добавлена оценка ID " + std::to_string(eval.evaluationID));
            logger.log("INFO", "ADD_PERFORMANCE_EVALUATION", "Добавлена оценка ID " + std::to_string(eval.evaluationID));
            return "SUCCESS|Оценка добавлена";
        }
        else {
            return "ERROR|Не удалось добавить оценку. Возможно, ID уже существует.";
        }
    }

    if (command == "EDIT_PERFORMANCE_EVALUATION") {
        // Формат: EDIT_PERFORMANCE_EVALUATION|evaluationID|employeeID|positionID|evaluationDate|evaluationResult|comments
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
            historyManager.addHistory(username, "EDIT_PERFORMANCE_EVALUATION", "Изменена оценка ID " + std::to_string(eval.evaluationID));
            logger.log("INFO", "EDIT_PERFORMANCE_EVALUATION", "Изменена оценка ID " + std::to_string(eval.evaluationID));
            return "SUCCESS|Оценка обновлена";
        }
        else {
            return "ERROR|Не удалось обновить оценку. Возможно, ID не существует.";
        }
    }

    if (command == "DELETE_PERFORMANCE_EVALUATION") {
        // Формат: DELETE_PERFORMANCE_EVALUATION|evaluationID
        std::string token;
        getline(iss, token, '|');
        long evalID = std::stol(token);

        bool success = performanceEvaluationManager.deleteEvaluation(evalID);
        if (success) {
            historyManager.addHistory(username, "DELETE_PERFORMANCE_EVALUATION", "Удалена оценка ID: " + std::to_string(evalID));
            logger.log("INFO", "DELETE_PERFORMANCE_EVALUATION", "Удалена оценка ID: " + std::to_string(evalID));
            return "SUCCESS|Оценка удалена";
        }
        else {
            return "ERROR|Не удалось удалить оценку. Возможно, ID не существует.";
        }
    }

    if (command == "VIEW_ACTIVE_CONNECTIONS") {
        // Формат: VIEW_ACTIVE_CONNECTIONS
        // Реализация зависит от того, как вы храните активные соединения
        // В данном примере предполагается, что активные соединения не хранятся на сервере
        // Можно реализовать подсчет количества активных клиентов
        // Для простоты возвращаем заглушку
        return "SUCCESS|Количество активных соединений: [Заглушка]";
    }

    if (command == "VIEW_OWN_INFO") {
        // Формат: VIEW_OWN_INFO|username
        std::string viewUsername;
        getline(iss, viewUsername, '|');

        // Получение пользователя
        User user = userManager.getUser(viewUsername);
        if (user.username.empty()) {
            return "ERROR|Пользователь не найден.";
        }

        // Получение информации о сотруднике
        // Предполагается, что username соответствует employeeID или существует связь между ними
        // В данном примере используем статический employeeID
        long employeeID = 1001; // Пример, в реальной системе нужно связать username и employeeID
        Employee emp = employeeManager.getEmployee(employeeID);
        if (emp.employeeID == 0) {
            return "ERROR|Информация о сотруднике не найдена.";
        }

        std::ostringstream oss;
        oss << "ID: " << emp.employeeID << "|Должность ID: " << emp.positionID << "|ФИО: " << emp.fullName
            << "|Дата рождения: " << emp.birthDate << "|Адрес: " << emp.address
            << "|Контактная информация: " << emp.contactInfo << "|Дата начала работы: " << emp.startDate
            << "|Статус: " << emp.status;

        return "SUCCESS|" + oss.str();
    }

    if (command == "VIEW_OWN_SCHEDULE") {
        // Формат: VIEW_OWN_SCHEDULE|username
        std::string viewUsername;
        getline(iss, viewUsername, '|');

        // Получение пользователя
        User user = userManager.getUser(viewUsername);
        if (user.username.empty()) {
            return "ERROR|Пользователь не найден.";
        }

        // Получение информации о сотруднике
        long employeeID = 1001; // Пример, в реальной системе нужно связать username и employeeID
        std::vector<Schedule> schedules = scheduleManager.getAllSchedules();
        std::string response = "";
        for (const auto& sched : schedules) {
            if (sched.employeeID == employeeID) {
                response += std::to_string(sched.scheduleID) + "|" + std::to_string(sched.positionID) + "|"
                    + sched.scheduleType + "|" + sched.shiftDetails + "|" + sched.scheduleDates + "\n";
            }
        }

        if (response.empty()) {
            return "SUCCESS|У вас нет назначенных графиков работы.";
        }

        return "SUCCESS|" + response;
    }

    if (command == "ADD_SCHEDULE") {
        // Формат: ADD_SCHEDULE|scheduleID|employeeID|positionID|scheduleType|shiftDetails|scheduleDates
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
            historyManager.addHistory(username, "ADD_SCHEDULE", "Добавлен график ID " + std::to_string(sched.scheduleID));
            logger.log("INFO", "ADD_SCHEDULE", "Добавлен график ID " + std::to_string(sched.scheduleID));
            return "SUCCESS|График работы добавлен";
        }
        else {
            return "ERROR|Не удалось добавить график работы. Возможно, ID уже существует.";
        }
    }

    if (command == "EDIT_SCHEDULE") {
        // Формат: EDIT_SCHEDULE|scheduleID|employeeID|positionID|scheduleType|shiftDetails|scheduleDates
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
            historyManager.addHistory(username, "EDIT_SCHEDULE", "Изменен график ID " + std::to_string(sched.scheduleID));
            logger.log("INFO", "EDIT_SCHEDULE", "Изменен график ID " + std::to_string(sched.scheduleID));
            return "SUCCESS|График работы обновлен";
        }
        else {
            return "ERROR|Не удалось обновить график работы. Возможно, ID не существует.";
        }
    }

    if (command == "DELETE_SCHEDULE") {
        // Формат: DELETE_SCHEDULE|scheduleID
        std::string token;
        getline(iss, token, '|');
        long schedID = std::stol(token);

        bool success = scheduleManager.deleteSchedule(schedID);
        if (success) {
            historyManager.addHistory(username, "DELETE_SCHEDULE", "Удален график ID: " + std::to_string(schedID));
            logger.log("INFO", "DELETE_SCHEDULE", "Удален график ID: " + std::to_string(schedID));
            return "SUCCESS|График работы удален";
        }
        else {
            return "ERROR|Не удалось удалить график работы. Возможно, ID не существует.";
        }
    }

    if (command == "VIEW_SCHEDULES") {
        // Формат: VIEW_SCHEDULES|employeeID (для просмотра своего графика) или VIEW_SCHEDULES|all (для просмотра всех графиков)
        std::string param;
        getline(iss, param, '|');

        std::vector<Schedule> schedules;
        if (param == "all") {
            schedules = scheduleManager.getAllSchedules();
        }
        else {
            long empID = std::stol(param);
            schedules = scheduleManager.getAllSchedules();
            // Фильтрация по employeeID
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
            return "SUCCESS|Нет графиков работы для отображения.";
        }

        return "SUCCESS|" + response;
    }
    if (command == "VIEW_ALL_EMPLOYEES") {
        // Формат: VIEW_ALL_EMPLOYEES
        std::vector<Employee> employees = employeeManager.getAllEmployees();
        std::string response = "";
        for (const auto& emp : employees) {
            response += std::to_string(emp.employeeID) + "|" + std::to_string(emp.positionID) + "|"
                + emp.fullName + "|" + emp.birthDate + "|" + emp.address + "|"
                + emp.contactInfo + "|" + emp.startDate + "|" + emp.status + "\n";
        }
        if (response.empty()) {
            return "SUCCESS|Нет сотрудников для отображения.";
        }
        return "SUCCESS|" + response;
    }
    if (command == "VIEW_ALL_POSITIONS") {
        // Формат: VIEW_ALL_POSITIONS
        std::vector<Position> positions = positionManager.getAllPositions();
        std::string response = "";
        for (const auto& pos : positions) {
            response += std::to_string(pos.positionID) + "|" + pos.positionName + "|"
                + std::to_string(pos.minSalary) + "|" + std::to_string(pos.maxSalary)
                + "|" + pos.qualificationRequirements + "\n";
        }
        if (response.empty()) {
            return "SUCCESS|Нет должностей для отображения.";
        }
        return "SUCCESS|" + response;
    }
    if (command == "VIEW_ALL_DEPARTMENTS") {
        // Формат: VIEW_ALL_DEPARTMENTS
        std::vector<Department> departments = departmentManager.getAllDepartments();
        std::string response = "";
        for (const auto& dept : departments) {
            response += std::to_string(dept.departmentID) + "|" + dept.departmentName + "|"
                + dept.location + "|" + std::to_string(dept.positionID) + "\n";
        }
        if (response.empty()) {
            return "SUCCESS|Нет отделов для отображения.";
        }
        return "SUCCESS|" + response;
    }

    if (command == "VIEW_ALL_SCHEDULES") {
        // Формат: VIEW_ALL_SCHEDULES
        std::vector<Schedule> schedules = scheduleManager.getAllSchedules();
        std::string response = "";
        for (const auto& sched : schedules) {
            response += std::to_string(sched.scheduleID) + "|" + std::to_string(sched.employeeID) + "|"
                + std::to_string(sched.positionID) + "|" + sched.scheduleType + "|"
                + sched.shiftDetails + "|" + sched.scheduleDates + "\n";
        }
        if (response.empty()) {
            return "SUCCESS|Нет графиков работы для отображения.";
        }
        return "SUCCESS|" + response;
    }
    if (command == "VIEW_ALL_COMPENSATIONS") {
        // Формат: VIEW_ALL_COMPENSATIONS
        std::vector<CompensationRequest> requests = compensationManager.getAllRequests();
        std::string response = "";
        for (const auto& req : requests) {
            response += std::to_string(req.requestID) + "|" + std::to_string(req.employeeID) + "|"
                + std::to_string(req.positionID) + "|" + req.compensationType + "|"
                + std::to_string(req.amount) + "|" + req.submissionDate + "|"
                + req.status + "\n";
        }
        if (response.empty()) {
            return "SUCCESS|Нет заявок на компенсацию для отображения.";
        }
        return "SUCCESS|" + response;
    }
    if (command == "VIEW_ALL_EVALUATIONS") {
        // Формат: VIEW_ALL_EVALUATIONS
        std::vector<PerformanceEvaluation> evaluations = performanceEvaluationManager.getAllEvaluations();
        std::string response = "";
        for (const auto& eval : evaluations) {
            response += std::to_string(eval.evaluationID) + "|" + std::to_string(eval.employeeID) + "|"
                + std::to_string(eval.positionID) + "|" + eval.evaluationDate + "|"
                + eval.evaluationResult + "|" + eval.comments + "\n";
        }
        if (response.empty()) {
            return "SUCCESS|Нет оценок производительности для отображения.";
        }
        return "SUCCESS|" + response;
    }
    if (command == "VIEW_ALL_RESPONSIBILITIES") {
        // Формат: VIEW_ALL_RESPONSIBILITIES
        std::vector<JobResponsibility> responsibilities = jobResponsibilityManager.getAllResponsibilities();
        std::string response = "";
        for (const auto& resp : responsibilities) {
            response += std::to_string(resp.responsibilityID) + "|" + std::to_string(resp.employeeID) + "|"
                + std::to_string(resp.positionID) + "|" + resp.responsibilityDescription + "\n";
        }
        if (response.empty()) {
            return "SUCCESS|Нет обязанностей для отображения.";
        }
        return "SUCCESS|" + response;
    }
    if (command == "VIEW_ALL_COURSES") {
        // Формат: VIEW_ALL_COURSES
        std::vector<Course> courses = courseManager.getAllCourses();
        std::string response = "";
        for (const auto& course : courses) {
            response += course.courseID + "|" + course.topic + "|" + course.startDate + "|" + course.endDate + "\n";
        }
        if (response.empty()) {
            return "SUCCESS|Нет курсов для отображения.";
        }
        return "SUCCESS|" + response;
    }
    if (command == "VIEW_ALL_SALARIES") {
        // Формат: VIEW_ALL_SALARIES
        std::vector<Salary> salaries = salaryManager.getAllSalaries();
        std::string response = "";
        for (const auto& sal : salaries) {
            response += std::to_string(sal.salaryID) + "|" + std::to_string(sal.employeeID) + "|"
                + std::to_string(sal.positionID) + "|" + std::to_string(sal.baseSalary) + "|"
                + std::to_string(sal.bonuses) + "|" + std::to_string(sal.deductions) + "|"
                + std::to_string(sal.netPay) + "|" + sal.calculationDate + "\n";
        }
        if (response.empty()) {
            return "SUCCESS|Нет зарплат для отображения.";
        }
        return "SUCCESS|" + response;
    }
    if (command == "VIEW_MY_RESPONSIBILITIES") {
        // Формат: VIEW_MY_RESPONSIBILITIES|employeeID
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
            return "SUCCESS|У вас нет назначенных обязанностей.";
        }

        return "SUCCESS|" + response;
    }
    if (command == "VIEW_MY_COURSES") {
        // Формат: VIEW_MY_COURSES|employeeID
        std::string token;
        getline(iss, token, '|');
        long employeeID = std::stol(token);

        // Предполагается, что запись на курсы хранится в отдельном менеджере или таблице
        // Для примера используем метод courseManager.getCoursesByEmployee(employeeID)
        // Необходимо реализовать этот метод в CourseManager

        std::vector<Course> myCourses = courseManager.getCoursesByEmployee(employeeID);
        std::string response = "";
        for (const auto& course : myCourses) {
            response += course.courseID + "|" + course.topic + "|" + course.startDate + "|" + course.endDate + "\n";
        }

        if (response.empty()) {
            return "SUCCESS|Вы не записаны на курсы.";
        }

        return "SUCCESS|" + response;
    }
    if (command == "PROCESS_COMPENSATION") {
        // Формат: PROCESS_COMPENSATION|requestID|newStatus
        std::string token, newStatus;
        getline(iss, token, '|');
        long requestID = std::stol(token);
        getline(iss, newStatus, '|');

        bool success = compensationManager.processRequest(requestID, newStatus);
        if (success) {
            historyManager.addHistory(username, "PROCESS_COMPENSATION", "Обработана заявка ID " + std::to_string(requestID) + " статус: " + newStatus);
            logger.log("INFO", "PROCESS_COMPENSATION", "Обработана заявка ID " + std::to_string(requestID) + " статус: " + newStatus);
            return "SUCCESS|Заявка обработана и статус изменен на: " + newStatus;
        }
        else {
            return "ERROR|Не удалось обработать заявку. Возможно, ID не существует.";
        }
    }
    if (command == "ADD_SALARY") {
        // Формат: ADD_SALARY|salaryID|employeeID|positionID|baseSalary|bonuses|deductions|netPay|calculationDate
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
            historyManager.addHistory(username, "ADD_SALARY", "Добавлена зарплата ID " + std::to_string(sal.salaryID));
            logger.log("INFO", "ADD_SALARY", "Добавлена зарплата ID " + std::to_string(sal.salaryID));
            return "SUCCESS|Зарплата добавлена";
        }
        else {
            return "ERROR|Не удалось добавить зарплату. Возможно, ID уже существует.";
        }
    }
    if (command == "EDIT_SALARY") {
        // Формат: EDIT_SALARY|salaryID|employeeID|positionID|baseSalary|bonuses|deductions|netPay|calculationDate
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
            historyManager.addHistory(username, "EDIT_SALARY", "Изменена зарплата ID " + std::to_string(sal.salaryID));
            logger.log("INFO", "EDIT_SALARY", "Изменена зарплата ID " + std::to_string(sal.salaryID));
            return "SUCCESS|Зарплата обновлена";
        }
        else {
            return "ERROR|Не удалось обновить зарплату. Возможно, ID не существует.";
        }
    }
    if (command == "DELETE_SALARY") {
        // Формат: DELETE_SALARY|salaryID
        std::string token;
        getline(iss, token, '|');
        long salaryID = std::stol(token);

        bool success = salaryManager.deleteSalary(salaryID);
        if (success) {
            historyManager.addHistory(username, "DELETE_SALARY", "Удалена зарплата ID: " + std::to_string(salaryID));
            logger.log("INFO", "DELETE_SALARY", "Удалена зарплата ID: " + std::to_string(salaryID));
            return "SUCCESS|Зарплата удалена";
        }
        else {
            return "ERROR|Не удалось удалить зарплату. Возможно, ID не существует.";
        }
    }
    if (command == "VIEW_ALL_SCHEDULES") {
        // Формат: VIEW_ALL_SCHEDULES
        std::vector<Schedule> schedules = scheduleManager.getAllSchedules();
        std::string response = "";
        for (const auto& sched : schedules) {
            response += std::to_string(sched.scheduleID) + "|" + std::to_string(sched.employeeID) + "|"
                + std::to_string(sched.positionID) + "|" + sched.scheduleType + "|"
                + sched.shiftDetails + "|" + sched.scheduleDates + "\n";
        }
        if (response.empty()) {
            return "SUCCESS|Нет графиков работы для отображения.";
        }
        return "SUCCESS|" + response;
    }


    return "ERROR|Неизвестная команда.";
}
