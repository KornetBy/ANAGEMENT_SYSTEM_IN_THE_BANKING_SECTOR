#include "command_handler.h"
#include "utils.h"

CommandHandler::CommandHandler(std::string& r, Utils& u, DataAccess& d, AuthService& a)
    : role(r), utils(u), da(d), auth(a)
{
    // �����������, ������ ���������������� � ������ �������������
}

std::string CommandHandler::Handle(const std::string& cmdline) {
    auto p = cmdline.find(' ');
    std::string cmd = (p == std::string::npos ? cmdline : cmdline.substr(0, p));
    std::string rest = (p == std::string::npos ? "" : cmdline.substr(p + 1));

    // ���� ������������ �� �����������, ��������� ������ LOGIN, EXIT, HELP
    if (!isAuthorized()) {
        if (cmd == "LOGIN") return cmdLogin(rest);
        if (cmd == "EXIT") return cmdExit();
        if (cmd == "HELP") {
            return "��� ������ ������ ���������� ��������������.\n�������:\nLOGIN <�����> <������>\nEXIT - �����\nHELP - ������";
        }
        return "�� �� ������������! ����������, ����������� ������� LOGIN ��� HELP.";
    }

    // ������������ �����������
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
    return "����������� �������. ������� HELP ��� ������ ��������� ������.";
}

std::string CommandHandler::cmdLogin(const std::string& rest) {
    if (isAuthorized()) return "�� ��� ������������ ��� " + role + ".";
    auto q = rest.find(' ');
    if (q == std::string::npos)return "�������� ������. �����������: LOGIN <�����> <������>";
    std::string u = rest.substr(0, q), pw = rest.substr(q + 1), rr;
    if (auth.Authenticate(u, pw, rr)) {
        role = rr; user = u;
        return "�� ������� ����� � ������� ��� " + rr + ". ������� HELP ��� ������ ��������� ������.";
    }
    return "�������� ����� ��� ������.";
}

std::string CommandHandler::cmdHelp() {
    std::string base = "��������� �������:\n"
        "EXIT - �����\n"
        "HELP - ������\n";
    if (role == "������������� �������") {
        base += "ADD_EMPLOYEE <������> - �������� ����������\n"
            "DEL_EMPLOYEE <ID> - ������� ����������\n"
            "ADD_ROLE <����> - �������� ����� ����\n"
            "CHANGE_USER_ROLE <�����;����> - �������� ���� ������������\n";
    }
    else if (role == "HR-����������") {
        base += "ADD_EMPLOYEE <������> - �������� ����������\n"
            "DEL_EMPLOYEE <ID> - ������� ����������\n"
            "SUBMIT_COMPENSATION <������> - ������ �����������\n"
            "APPROVE_COMPENSATION <ID;������> - ��������/��������� �����������\n";
    }
    else if (role == "�������� �����" || role == "��������") {
        base += "GET_REPORTS - �������� ������� �����\n";
        if (role == "��������")
            base += "APPROVE_TRANSACTION <ID;������> - ��������/��������� ����������\n";
    }
    else if (role == "���������� ����������") {
        base += "APPROVE_COMPENSATION <ID;������> - ��������/��������� �����������\n"
            "APPROVE_TRANSACTION <ID;������> - ��������/��������� ����������\n";
    }
    else if (role == "�������� �������������") {
        base += "EVAL_PERFORMANCE <ID;ID_����;ID_�����;������;����;�������> - ������� ������������������\n";
    }
    else if (role == "������" || role == "������������" || role == "������") {
        base += "SUBMIT_COMPENSATION <������> - ������ �����������\n"
            "SUBMIT_LOAN <������> - ������ ������ �� ������\n"
            "SUBMIT_TRANSACTION <������> - ������ ����������\n";
    }
    else if (role == "��������� ��������") {
        base += "APPROVE_LOAN <ID;������> - ��������/��������� ������\n";
    }
    else if (role == "�������") {
        base += "GET_LOGS - �������� �����\n";
    }
    return base;
}

std::string CommandHandler::cmdExit() {
    // ������ ������ ����� �������� �������� ����������
    return "";
}

std::string CommandHandler::cmdAddEmployee(const std::string& rest) {
    if (role == "������������� �������" || role == "HR-����������") {
        if (!utils.CheckFieldsCount(rest, 8))
            return "������������ ����� ��� ���������� ����������. ����� 8 ����� ����������� ';'.";
        return da.AddLine("employees.txt", rest);
    }
    return "� ��� ��� ���� ��� ���� ��������.";
}

std::string CommandHandler::cmdDelEmployee(const std::string& rest) {
    if (role == "������������� �������" || role == "HR-����������") {
        if (!utils.ValidateID(rest))return "�������� ������ ID.";
        return da.DelLineByID("employees.txt", rest);
    }
    return "� ��� ��� ���� ��� ���� ��������.";
}

std::string CommandHandler::cmdAddRole(const std::string& rest) {
    if (role == "������������� �������") {
        if (rest.empty())return "���� �� ����� ���� ������.";
        return da.AddLine("roles.txt", rest);
    }
    return "� ��� ��� ���� ��� ���� ��������.";
}

std::string CommandHandler::cmdChangeUserRole(const std::string& rest) {
    if (role == "������������� �������") {
        auto sp = utils.Split(rest, ';');
        if (sp.size() < 2)return "����� ������� �����;����";
        if (!da.RoleExists(sp[1]))return "��������� ���� �� ����������.";
        bool ok = da.UpdateUserRole(sp[0], sp[1]);
        return ok ? "OK" : "������������ �� ������.";
    }
    return "� ��� ��� ���� ��� ���� ��������.";
}

std::string CommandHandler::cmdSubmitCompensation(const std::string& rest) {
    // ��� ������ ����������� ����: ������, ������������, �������� �������������, HR-����������
    if (role == "������" || role == "������������" || role == "�������� �������������" || role == "HR-����������") {
        if (!utils.CheckFieldsCount(rest, 7))return "������������ �����. ����� 7 ����� ����� ';'.";
        return da.AddLine("compensations.txt", rest);
    }
    return "� ��� ��� ���� ��� ���� ��������.";
}

std::string CommandHandler::cmdApproveCompensation(const std::string& rest) {
    if (role == "HR-����������" || role == "���������� ����������") {
        auto sp = utils.Split(rest, ';');
        if (sp.size() < 2)return "������: ID;������(��������/���������)";
        std::string id = sp[0], st = sp[1];
        if (!utils.ValidateID(id))return "�������� ID.";
        if (st != "��������" && st != "���������")return "������ ������ ���� '��������' ��� '���������'.";
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
        return "�� ������� ����������� � ����� ID.";
    }
    return "� ��� ��� ���� ��� ���� ��������.";
}

std::string CommandHandler::cmdEvalPerformance(const std::string& rest) {
    if (role == "�������� �������������") {
        if (!utils.CheckFieldsCount(rest, 6))return "������������ ����� ��� ������ ������������������.";
        auto sp = utils.Split(rest, ';');
        // ID;ID_����;ID_�����;������;����;�������
        if (!utils.ValidateID(sp[0]) || !utils.ValidateID(sp[1]) || !utils.ValidateID(sp[2]))return "�������� ������ ID.";
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
    return "� ��� ��� ���� ��� ���� ��������.";
}

std::string CommandHandler::cmdGetReports() {
    if (role == "�������� �����" || role == "��������") {
        auto emp = da.LoadData("employees.txt");
        auto pos = da.LoadData("positions.txt");
        auto sal = da.LoadData("salaries.txt");
        auto perf = da.LoadData("performance.txt");
        auto cmp = da.LoadData("compensations.txt");
        auto loans = da.LoadData("loans.txt");
        auto trans = da.LoadData("transactions.txt");
        std::string r = "������� �����:\n";
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
            r += "����:" + fio + " �����:" + pname + " ��:" + salary + " ������:" + performance +
                " �����������:" + std::to_string(compcount) + " ��������:" + std::to_string(loan_count) +
                " ����������:" + std::to_string(trans_count) + "\n";
        }
        return r;
    }
    return "� ��� ��� ���� ��� ���� ��������.";
}

std::string CommandHandler::cmdSubmitLoan(const std::string& rest) {
    if (role == "������" || role == "������������" || role == "������") {
        if (!utils.CheckFieldsCount(rest, 7))return "������������ ����� ��� ������ �� ������.";
        auto sp = utils.Split(rest, ';');
        if (!utils.ValidateID(sp[0]) || !utils.ValidateID(sp[1]))return "�������� ������ ID.";
        return da.AddLine("loans.txt", rest);
    }
    return "� ��� ��� ���� ��� ���� ��������.";
}

std::string CommandHandler::cmdApproveLoan(const std::string& rest) {
    if (role == "��������� ��������") {
        auto sp = utils.Split(rest, ';');
        if (sp.size() < 2)return "������: ID;������(��������/���������)";
        std::string id = sp[0], st = sp[1];
        if (!utils.ValidateID(id))return "�������� ID.";
        if (st != "��������" && st != "���������")return "������ ������ ���� '��������' ��� '���������'.";
        std::vector<int> fupd; fupd.push_back(4);
        std::vector<std::string> vals; vals.push_back(st);
        return da.UpdateLineByID("loans.txt", id, 0, fupd, vals);
    }
    return "� ��� ��� ���� ��� ���� ��������.";
}

std::string CommandHandler::cmdGetLogs() {
    if (role == "�������") {
        auto v = da.LoadData("logs.txt");
        return v.empty() ? "���� �����." : da.JoinLines(v);
    }
    return "� ��� ��� ���� ��� ���� ��������.";
}

std::string CommandHandler::cmdSubmitTransaction(const std::string& rest) {
    if (role == "������" || role == "������������" || role == "������") {
        if (!utils.CheckFieldsCount(rest, 6))return "������������ ����� ��� ����������.";
        auto sp = utils.Split(rest, ';');
        if (!utils.ValidateID(sp[0]) || !utils.ValidateID(sp[1]))return "�������� ID.";
        return da.AddLine("transactions.txt", rest);
    }
    return "� ��� ��� ���� ��� ���� ��������.";
}

std::string CommandHandler::cmdApproveTransaction(const std::string& rest) {
    if (role == "���������� ����������" || role == "��������") {
        auto sp = utils.Split(rest, ';');
        if (sp.size() < 2)return "������: ID;������(��������/���������)";
        std::string id = sp[0], st = sp[1];
        if (!utils.ValidateID(id))return "�������� ID.";
        if (st != "��������" && st != "���������")return "������ ������ ���� '��������' ��� '���������'.";
        std::vector<int> fupd; fupd.push_back(5);
        std::vector<std::string> vals; vals.push_back(st);
        return da.UpdateLineByID("transactions.txt", id, 0, fupd, vals);
    }
    return "� ��� ��� ���� ��� ���� ��������.";
}
