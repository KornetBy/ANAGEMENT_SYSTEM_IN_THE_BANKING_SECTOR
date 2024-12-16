#include "command_handler.h"
#include "utils.h"

CommandHandler::CommandHandler(std::string& r, Utils& u, DataAccess& d, AuthService& a)
    : role(r), utils(u), da(d), auth(a)
{
    // Конструктор, ссылки инициализированы в списке инициализации
}

std::string CommandHandler::Handle(const std::string& cmdline) {
    auto p = cmdline.find(' ');
    std::string cmd = (p == std::string::npos ? cmdline : cmdline.substr(0, p));
    std::string rest = (p == std::string::npos ? "" : cmdline.substr(p + 1));

    // Если пользователь не авторизован, разрешаем только LOGIN, EXIT, HELP
    if (!isAuthorized()) {
        if (cmd == "LOGIN") return cmdLogin(rest);
        if (cmd == "EXIT") return cmdExit();
        if (cmd == "HELP") {
            return "Для начала работы необходимо авторизоваться.\nКоманды:\nLOGIN <логин> <пароль>\nEXIT - выход\nHELP - помощь";
        }
        return "Вы не авторизованы! Пожалуйста, используйте команду LOGIN или HELP.";
    }

    // Пользователь авторизован
    if (cmd == "HELP") return cmdHelp();
    else if (cmd == "EXIT") return cmdExit();
    else if (cmd == "ADD_EMPLOYEE") return cmdAddEmployee(rest);
    else if (cmd == "DEL_EMPLOYEE") return cmdDelEmployee(rest);
    else if (cmd == "ADD_ROLE") return cmdAddRole(rest);
    else if (cmd == "CHANGE_USER_ROLE") return cmdChangeUserRole(rest);
    else if (cmd == "SUBMIT_COMPENSATION") return cmdSubmitCompensation(rest);
    else if (cmd == "APPROVE_COMPENSATION") return cmdApproveCompensation(rest);
    else if (cmd == "EVAL_PERFORMANCE") return cmdEvalPerformance(rest);
    else if (cmd == "GET_REPORTS") return cmdGetReports();
    else if (cmd == "SUBMIT_LOAN") return cmdSubmitLoan(rest);
    else if (cmd == "APPROVE_LOAN") return cmdApproveLoan(rest);
    else if (cmd == "GET_LOGS") return cmdGetLogs();
    else if (cmd == "SUBMIT_TRANSACTION") return cmdSubmitTransaction(rest);
    else if (cmd == "APPROVE_TRANSACTION") return cmdApproveTransaction(rest);
    return "Неизвестная команда. Введите HELP для списка доступных команд.";
}

std::string CommandHandler::cmdLogin(const std::string& rest) {
    if (isAuthorized()) return "Вы уже авторизованы как " + role + ".";
    auto q = rest.find(' ');
    if (q == std::string::npos)return "Неверный формат. Используйте: LOGIN <логин> <пароль>";
    std::string u = rest.substr(0, q), pw = rest.substr(q + 1), rr;
    if (auth.Authenticate(u, pw, rr)) {
        role = rr; user = u;
        return "Вы успешно вошли в систему как " + rr + ". Введите HELP для списка доступных команд.";
    }
    return "Неверный логин или пароль.";
}

std::string CommandHandler::cmdHelp() {
    std::string base = "Доступные команды:\n"
        "EXIT - выход\n"
        "HELP - помощь\n";
    if (role == "Администратор системы") {
        base += "ADD_EMPLOYEE <данные> - добавить сотрудника\n"
            "DEL_EMPLOYEE <ID> - удалить сотрудника\n"
            "ADD_ROLE <роль> - добавить новую роль\n"
            "CHANGE_USER_ROLE <логин;роль> - изменить роль пользователя\n";
    }
    else if (role == "HR-специалист") {
        base += "ADD_EMPLOYEE <данные> - добавить сотрудника\n"
            "DEL_EMPLOYEE <ID> - удалить сотрудника\n"
            "SUBMIT_COMPENSATION <данные> - подать компенсацию\n"
            "APPROVE_COMPENSATION <ID;Статус> - одобрить/отклонить компенсацию\n";
    }
    else if (role == "Директор банка" || role == "Аналитик") {
        base += "GET_REPORTS - получить сводный отчет\n";
        if (role == "Аналитик")
            base += "APPROVE_TRANSACTION <ID;Статус> - одобрить/отклонить транзакцию\n";
    }
    else if (role == "Финансовый специалист") {
        base += "APPROVE_COMPENSATION <ID;Статус> - одобрить/отклонить компенсацию\n"
            "APPROVE_TRANSACTION <ID;Статус> - одобрить/отклонить транзакцию\n";
    }
    else if (role == "Менеджер подразделения") {
        base += "EVAL_PERFORMANCE <ID;ID_сотр;ID_должн;Оценка;Дата;Коммент> - оценить производительность\n";
    }
    else if (role == "Кассир" || role == "Операционист" || role == "Клиент") {
        base += "SUBMIT_COMPENSATION <данные> - подать компенсацию\n"
            "SUBMIT_LOAN <данные> - подать заявку на кредит\n"
            "SUBMIT_TRANSACTION <данные> - подать транзакцию\n";
    }
    else if (role == "Кредитный менеджер") {
        base += "APPROVE_LOAN <ID;Статус> - одобрить/отклонить кредит\n";
    }
    else if (role == "Аудитор") {
        base += "GET_LOGS - просмотр логов\n";
    }
    return base;
}

std::string CommandHandler::cmdExit() {
    // Пустая строка будет означать закрытие соединения
    return "";
}

std::string CommandHandler::cmdAddEmployee(const std::string& rest) {
    if (role == "Администратор системы" || role == "HR-специалист") {
        if (!utils.CheckFieldsCount(rest, 8))
            return "Недостаточно полей для добавления сотрудника. Нужно 8 полей разделенных ';'.";
        return da.AddLine("employees.txt", rest);
    }
    return "У вас нет прав для этой операции.";
}

std::string CommandHandler::cmdDelEmployee(const std::string& rest) {
    if (role == "Администратор системы" || role == "HR-специалист") {
        if (!utils.ValidateID(rest))return "Неверный формат ID.";
        return da.DelLineByID("employees.txt", rest);
    }
    return "У вас нет прав для этой операции.";
}

std::string CommandHandler::cmdAddRole(const std::string& rest) {
    if (role == "Администратор системы") {
        if (rest.empty())return "Роль не может быть пустой.";
        return da.AddLine("roles.txt", rest);
    }
    return "У вас нет прав для этой операции.";
}

std::string CommandHandler::cmdChangeUserRole(const std::string& rest) {
    if (role == "Администратор системы") {
        auto sp = utils.Split(rest, ';');
        if (sp.size() < 2)return "Нужно указать логин;роль";
        if (!da.RoleExists(sp[1]))return "Указанной роли не существует.";
        bool ok = da.UpdateUserRole(sp[0], sp[1]);
        return ok ? "OK" : "Пользователь не найден.";
    }
    return "У вас нет прав для этой операции.";
}

std::string CommandHandler::cmdSubmitCompensation(const std::string& rest) {
    // Для подачи компенсации роли: Кассир, Операционист, Менеджер подразделения, HR-специалист
    if (role == "Кассир" || role == "Операционист" || role == "Менеджер подразделения" || role == "HR-специалист") {
        if (!utils.CheckFieldsCount(rest, 7))return "Недостаточно полей. Нужно 7 полей через ';'.";
        return da.AddLine("compensations.txt", rest);
    }
    return "У вас нет прав для этой операции.";
}

std::string CommandHandler::cmdApproveCompensation(const std::string& rest) {
    if (role == "HR-специалист" || role == "Финансовый специалист") {
        auto sp = utils.Split(rest, ';');
        if (sp.size() < 2)return "Формат: ID;Статус(Одобрено/Отклонено)";
        std::string id = sp[0], st = sp[1];
        if (!utils.ValidateID(id))return "Неверный ID.";
        if (st != "Одобрено" && st != "Отклонено")return "Статус должен быть 'Одобрено' или 'Отклонено'.";
        auto v = da.LoadData("compensations.txt");
        std::vector<std::string>nv; bool done = false;
        for (auto& x : v) {
            auto pp = utils.Split(x, ';');
            if (pp.size() > 0 && pp[0] == id && pp.size() > 6) {
                pp[6] = st; done = true; x = "";
                for (size_t i = 0; i < pp.size(); i++) { x += pp[i]; if (i < pp.size() - 1)x += ";"; }
            }
            nv.push_back(x);
        }
        if (done) { da.SaveData("compensations.txt", nv); return "OK"; }
        return "Не найдена компенсация с таким ID.";
    }
    return "У вас нет прав для этой операции.";
}

std::string CommandHandler::cmdEvalPerformance(const std::string& rest) {
    if (role == "Менеджер подразделения") {
        if (!utils.CheckFieldsCount(rest, 6))return "Недостаточно полей для оценки производительности.";
        auto sp = utils.Split(rest, ';');
        // ID;ID_сотр;ID_должн;Оценка;Дата;Коммент
        if (!utils.ValidateID(sp[0]) || !utils.ValidateID(sp[1]) || !utils.ValidateID(sp[2]))return "Неверный формат ID.";
        auto v = da.LoadData("performance.txt");
        bool found = false;
        for (auto& x : v) {
            auto p = utils.Split(x, ';');
            if (p.size() > 0 && p[0] == sp[0]) { x = rest; found = true; break; }
        }
        if (!found)v.push_back(rest);
        da.SaveData("performance.txt", v);
        return "OK";
    }
    return "У вас нет прав для этой операции.";
}

std::string CommandHandler::cmdGetReports() {
    if (role == "Директор банка" || role == "Аналитик") {
        auto emp = da.LoadData("employees.txt");
        auto pos = da.LoadData("positions.txt");
        auto sal = da.LoadData("salaries.txt");
        auto perf = da.LoadData("performance.txt");
        auto cmp = da.LoadData("compensations.txt");
        auto loans = da.LoadData("loans.txt");
        auto trans = da.LoadData("transactions.txt");
        std::string r = "Сводный отчет:\n";
        for (auto& e : emp) {
            auto ep = utils.Split(e, ';');
            if (ep.size() < 8)continue;
            std::string eid = ep[0], pid = ep[1], fio = ep[2];
            std::string pname = "?";
            for (auto& po : pos) {
                auto pop = utils.Split(po, ';');
                if (pop.size() > 0 && pop[0] == pid) { pname = pop.size() > 1 ? pop[1] : "?"; break; }
            }
            std::string salary = "?";
            for (auto& s : sal) {
                auto sp = utils.Split(s, ';');
                if (sp.size() > 2 && sp[1] == eid) { salary = sp.size() > 6 ? sp[6] : "?"; break; }
            }
            std::string performance = "?";
            for (auto& pf : perf) {
                auto pfp = utils.Split(pf, ';');
                if (pfp.size() > 1 && pfp[1] == eid) { performance = pfp.size() > 3 ? pfp[3] : "?"; break; }
            }
            int compcount = 0;
            for (auto& cc : cmp) {
                auto ccp = utils.Split(cc, ';');
                if (ccp.size() > 2 && ccp[1] == eid)compcount++;
            }
            int loan_count = 0;
            for (auto& ll : loans) {
                auto lp = utils.Split(ll, ';');
                if (lp.size() > 1 && lp[1] == eid)loan_count++;
            }
            int trans_count = 0;
            for (auto& tt : trans) {
                auto tp = utils.Split(tt, ';');
                if (tp.size() > 1 && tp[1] == eid)trans_count++;
            }
            r += "Сотр:" + fio + " Должн:" + pname + " ЗП:" + salary + " Оценка:" + performance +
                " Компенсаций:" + std::to_string(compcount) + " Кредитов:" + std::to_string(loan_count) +
                " Транзакций:" + std::to_string(trans_count) + "\n";
        }
        return r;
    }
    return "У вас нет прав для этой операции.";
}

std::string CommandHandler::cmdSubmitLoan(const std::string& rest) {
    if (role == "Клиент" || role == "Операционист" || role == "Кассир") {
        if (!utils.CheckFieldsCount(rest, 7))return "Недостаточно полей для заявки на кредит.";
        auto sp = utils.Split(rest, ';');
        if (!utils.ValidateID(sp[0]) || !utils.ValidateID(sp[1]))return "Неверный формат ID.";
        return da.AddLine("loans.txt", rest);
    }
    return "У вас нет прав для этой операции.";
}

std::string CommandHandler::cmdApproveLoan(const std::string& rest) {
    if (role == "Кредитный менеджер") {
        auto sp = utils.Split(rest, ';');
        if (sp.size() < 2)return "Формат: ID;Статус(Одобрено/Отклонено)";
        std::string id = sp[0], st = sp[1];
        if (!utils.ValidateID(id))return "Неверный ID.";
        if (st != "Одобрено" && st != "Отклонено")return "Статус должен быть 'Одобрено' или 'Отклонено'.";
        std::vector<int> fupd; fupd.push_back(4);
        std::vector<std::string> vals; vals.push_back(st);
        return da.UpdateLineByID("loans.txt", id, 0, fupd, vals);
    }
    return "У вас нет прав для этой операции.";
}

std::string CommandHandler::cmdGetLogs() {
    if (role == "Аудитор") {
        auto v = da.LoadData("logs.txt");
        return v.empty() ? "Логи пусты." : da.JoinLines(v);
    }
    return "У вас нет прав для этой операции.";
}

std::string CommandHandler::cmdSubmitTransaction(const std::string& rest) {
    if (role == "Клиент" || role == "Операционист" || role == "Кассир") {
        if (!utils.CheckFieldsCount(rest, 6))return "Недостаточно полей для транзакции.";
        auto sp = utils.Split(rest, ';');
        if (!utils.ValidateID(sp[0]) || !utils.ValidateID(sp[1]))return "Неверный ID.";
        return da.AddLine("transactions.txt", rest);
    }
    return "У вас нет прав для этой операции.";
}

std::string CommandHandler::cmdApproveTransaction(const std::string& rest) {
    if (role == "Финансовый специалист" || role == "Аналитик") {
        auto sp = utils.Split(rest, ';');
        if (sp.size() < 2)return "Формат: ID;Статус(Одобрено/Отклонено)";
        std::string id = sp[0], st = sp[1];
        if (!utils.ValidateID(id))return "Неверный ID.";
        if (st != "Одобрено" && st != "Отклонено")return "Статус должен быть 'Одобрено' или 'Отклонено'.";
        std::vector<int> fupd; fupd.push_back(5);
        std::vector<std::string> vals; vals.push_back(st);
        return da.UpdateLineByID("transactions.txt", id, 0, fupd, vals);
    }
    return "У вас нет прав для этой операции.";
}
