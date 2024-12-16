#include "client.h"
#include "i18n.h"
#include <iostream>
#include <string>
#include <sstream>

static void sendReq(SOCKET s, const std::string& r) { send(s, r.c_str(), (int)r.size(), 0); }
static std::string recvResp(SOCKET s) {
    char b[4096]; int n = recv(s, b, 4096, 0);
    if (n <= 0)return "ОШИБКА;";
    b[n] = '\0';
    return std::string(b);
}

void hrMenu(SOCKET sock) {
    while (true) {
        std::cout << "\n1.Добавить сотрудника 2.Удалить сотрудника 3.Обновить поле сотрудника 4.Обновить зарплату сотрудника 5.Просмотреть сотрудников 6.Выплатить зарплату 7.Просмотреть отчет о сотрудниках 8.Выйти\n";
        int ch; std::cin >> ch; std::string req;
        if (ch == 8)break;
        if (ch == 1) {
            std::cin.ignore();
            std::string fn, pos, dep, hd, note; double sal;
            std::cout << "Полное имя:"; getline(std::cin, fn);
            std::cout << "Должность:"; getline(std::cin, pos);
            std::cout << "Отдел:"; getline(std::cin, dep);
            std::cout << "Дата приема на работу:"; getline(std::cin, hd);
            std::cout << "Зарплата:"; std::cin >> sal; std::cin.ignore();
            std::cout << "Примечание:"; getline(std::cin, note);
            req = "ADD_EMPLOYEE;" + fn + ";" + pos + ";" + dep + ";" + hd + ";" + std::to_string(sal) + ";" + note;
        }
        else if (ch == 2) {
            int id; std::cout << "id сотрудника:"; std::cin >> id; req = "REMOVE_EMPLOYEE;" + std::to_string(id);
        }
        else if (ch == 3) {
            int id; std::string f, v;
            std::cout << "id сотрудника:"; std::cin >> id;
            std::cout << "Поле:"; std::cin >> f;
            std::cin.ignore();
            std::cout << "Значение:"; getline(std::cin, v);
            req = "UPDATE_EMPLOYEE_FIELD;" + std::to_string(id) + ";" + f + ";" + v;
        }
        else if (ch == 4) {
            int id; double ns;
            std::cout << "id сотрудника:"; std::cin >> id;
            std::cout << "Новая зарплата:"; std::cin >> ns;
            req = "UPDATE_EMPLOYEE_SALARY;" + std::to_string(id) + ";" + std::to_string(ns);
        }
        else if (ch == 5) {
            req = "VIEW_EMPLOYEES";
        }
        else if (ch == 6) {
            int id; double amt;
            std::cout << "id сотрудника:"; std::cin >> id;
            std::cout << "Сумма:"; std::cin >> amt;
            req = "PAY_SALARY;" + std::to_string(id) + ";" + std::to_string(amt);
        }
        else if (ch == 7) {
            std::string sd, ed;
            std::cout << "Дата начала:"; std::cin >> sd;
            std::cout << "Дата окончания:"; std::cin >> ed;
            req = "VIEW_EMPLOYEES_REPORT;" + sd + ";" + ed;
        }
        else continue;
        sendReq(sock, req);
        std::string resp = recvResp(sock);
        std::cout << resp << "\n";
    }
}

void userMenu(SOCKET sock) {
    while (true) {
        std::cout << "1.Пополнить баланс 2.Снять средства 3.Перевести средства 4.Просмотреть баланс 5.Просмотреть историю операций 6.Выйти\n";
        int ch; std::cin >> ch; std::string req;
        if (ch == 6)break;
        if (ch == 1) { double amt; std::cout << "Сумма:"; std::cin >> amt; req = "DEPOSIT;" + std::to_string(amt); }
        else if (ch == 2) { double amt; std::cout << "Сумма:"; std::cin >> amt; req = "WITHDRAW;" + std::to_string(amt); }
        else if (ch == 3) { int uid; double amt; std::cout << "Кому (id пользователя):"; std::cin >> uid; std::cout << "Сумма:"; std::cin >> amt; req = "TRANSFER;" + std::to_string(uid) + ";" + std::to_string(amt); }
        else if (ch == 4) { req = "VIEW_BALANCE"; }
        else if (ch == 5) { req = "VIEW_HISTORY"; }
        else continue;
        sendReq(sock, req);
        std::string resp = recvResp(sock);
        std::cout << resp << "\n";
    }
}

void workerMenu(SOCKET sock) {
    while (true) {
        std::cout << "1.Создать аккаунт 2.Удалить аккаунт 3.Заблокировать аккаунт 4.Разблокировать аккаунт 5.Применить процентную ставку 6.Просмотреть историю операций (клиента) 7.Найти баланс 8.Сгенерировать отчет 9.Запросить кредит 10.Одобрить кредит 11.Установить тип кредита 12.Установить срок кредита 13.Выйти\n";
        int ch; std::cin >> ch; std::string req;
        if (ch == 13)break;
        if (ch == 1) { std::string u, p, s; double b; std::cout << "Пользователь:"; std::cin >> u; std::cout << "Пароль:"; std::cin >> p; std::cout << "Баланс:"; std::cin >> b; std::cout << "Статус:"; std::cin >> s; req = "CREATE_ACCOUNT;" + u + ";" + p + ";" + std::to_string(b) + ";" + s; }
        else if (ch == 2) { int id; std::cout << "id:"; std::cin >> id; req = "DELETE_ACCOUNT;" + std::to_string(id); }
        else if (ch == 3) { int id; std::cout << "id:"; std::cin >> id; req = "BLOCK_ACCOUNT;" + std::to_string(id); }
        else if (ch == 4) { int id; std::cout << "id:"; std::cin >> id; req = "UNBLOCK_ACCOUNT;" + std::to_string(id); }
        else if (ch == 5) { double r; std::cout << "Ставка:"; std::cin >> r; req = "APPLY_INTEREST;" + std::to_string(r); }
        else if (ch == 6) { int id; std::cout << "id клиента:"; std::cin >> id; req = "VIEW_HISTORY;" + std::to_string(id); }
        else if (ch == 7) { double amt; std::cout << "Сумма:"; std::cin >> amt; req = "SEARCH_BALANCE;" + std::to_string(amt); }
        else if (ch == 8) { std::string sd, ed; double minA, maxA; std::cout << "Дата начала:"; std::cin >> sd; std::cout << "Дата окончания:"; std::cin >> ed; std::cout << "Минимальная сумма:"; std::cin >> minA; std::cout << "Максимальная сумма:"; std::cin >> maxA; req = "GENERATE_REPORT;" + sd + ";" + ed + ";" + std::to_string(minA) + ";" + std::to_string(maxA); }
        else if (ch == 9) { double amt; int dur; std::cout << "Сумма:"; std::cin >> amt; std::cout << "Длительность:"; std::cin >> dur; req = "REQUEST_CREDIT;" + std::to_string(amt) + ";" + std::to_string(dur); }
        else if (ch == 10) { int cid; std::cout << "id кредита:"; std::cin >> cid; req = "APPROVE_CREDIT;" + std::to_string(cid); }
        else if (ch == 11) { int cid; std::string t; std::cout << "id кредита:"; std::cin >> cid; std::cout << "Тип:"; std::cin >> t; req = "SET_CREDIT_TYPE;" + std::to_string(cid) + ";" + t; }
        else if (ch == 12) { int cid, term; std::cout << "id кредита:"; std::cin >> cid; std::cout << "Срок:"; std::cin >> term; req = "SET_CREDIT_TERM;" + std::to_string(cid) + ";" + std::to_string(term); }
        else continue;
        sendReq(sock, req);
        std::string resp = recvResp(sock);
        std::cout << resp << "\n";
    }
}

void peopleManagerMenu(SOCKET sock) {
    while (true) {
        std::cout << "1.Просмотреть сотрудников 2.Выйти\n";
        int ch; std::cin >> ch; std::string req;
        if (ch == 2)break;
        if (ch == 1) { req = "VIEW_EMPLOYEES"; }
        else continue;
        sendReq(sock, req);
        std::string resp = recvResp(sock);
        std::cout << resp << "\n";
    }
}
