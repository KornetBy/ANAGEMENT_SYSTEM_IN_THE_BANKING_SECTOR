#include "client.h"
#include "i18n.h"
#include <iostream>
#include <string>
#include <sstream>

static void sendReq(SOCKET s, const std::string& r) { send(s, r.c_str(), (int)r.size(), 0); }
static std::string recvResp(SOCKET s) {
    char b[4096]; int n = recv(s, b, 4096, 0);
    if (n <= 0)return "ERROR;";
    b[n] = '\0';
    return std::string(b);
}

void hrMenu(SOCKET sock) {
    while (true) {
        std::cout << "\n1.ADD_EMPLOYEE 2.REMOVE_EMPLOYEE 3.UPDATE_EMPLOYEE_FIELD 4.UPDATE_EMPLOYEE_SALARY 5.VIEW_EMPLOYEES 6.PAY_SALARY 7.VIEW_EMPLOYEES_REPORT 8.Exit\n";
        int ch; std::cin >> ch; std::string req;
        if (ch == 8)break;
        if (ch == 1) {
            std::cin.ignore();
            std::string fn, pos, dep, hd, note; double sal;
            std::cout << "Full name:"; getline(std::cin, fn);
            std::cout << "Position:"; getline(std::cin, pos);
            std::cout << "Dep:"; getline(std::cin, dep);
            std::cout << "Hire date:"; getline(std::cin, hd);
            std::cout << "Salary:"; std::cin >> sal; std::cin.ignore();
            std::cout << "Note:"; getline(std::cin, note);
            req = "ADD_EMPLOYEE;" + fn + ";" + pos + ";" + dep + ";" + hd + ";" + std::to_string(sal) + ";" + note;
        }
        else if (ch == 2) {
            int id; std::cout << "empId:"; std::cin >> id; req = "REMOVE_EMPLOYEE;" + std::to_string(id);
        }
        else if (ch == 3) {
            int id; std::string f, v;
            std::cout << "empId:"; std::cin >> id;
            std::cout << "field:"; std::cin >> f;
            std::cin.ignore();
            std::cout << "value:"; getline(std::cin, v);
            req = "UPDATE_EMPLOYEE_FIELD;" + std::to_string(id) + ";" + f + ";" + v;
        }
        else if (ch == 4) {
            int id; double ns;
            std::cout << "empId:"; std::cin >> id;
            std::cout << "new salary:"; std::cin >> ns;
            req = "UPDATE_EMPLOYEE_SALARY;" + std::to_string(id) + ";" + std::to_string(ns);
        }
        else if (ch == 5) {
            req = "VIEW_EMPLOYEES";
        }
        else if (ch == 6) {
            int id; double amt;
            std::cout << "empId:"; std::cin >> id;
            std::cout << "amount:"; std::cin >> amt;
            req = "PAY_SALARY;" + std::to_string(id) + ";" + std::to_string(amt);
        }
        else if (ch == 7) {
            std::string sd, ed;
            std::cout << "startDate:"; std::cin >> sd;
            std::cout << "endDate:"; std::cin >> ed;
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
        std::cout << "1.DEPOSIT 2.WITHDRAW 3.TRANSFER 4.VIEW_BALANCE 5.VIEW_HISTORY 6.EXIT\n";
        int ch; std::cin >> ch; std::string req;
        if (ch == 6)break;
        if (ch == 1) { double amt; std::cout << "Amount:"; std::cin >> amt; req = "DEPOSIT;" + std::to_string(amt); }
        else if (ch == 2) { double amt; std::cout << "Amount:"; std::cin >> amt; req = "WITHDRAW;" + std::to_string(amt); }
        else if (ch == 3) { int uid; double amt; std::cout << "To user:"; std::cin >> uid; std::cout << "Amt:"; std::cin >> amt; req = "TRANSFER;" + std::to_string(uid) + ";" + std::to_string(amt); }
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
        std::cout << "1.CREATE_ACCOUNT 2.DELETE_ACCOUNT 3.BLOCK_ACCOUNT 4.UNBLOCK_ACCOUNT 5.APPLY_INTEREST 6.VIEW_HISTORY(client) 7.SEARCH_BALANCE 8.GENERATE_REPORT 9.REQUEST_CREDIT 10.APPROVE_CREDIT 11.SET_CREDIT_TYPE 12.SET_CREDIT_TERM 13.EXIT\n";
        int ch; std::cin >> ch; std::string req;
        if (ch == 13)break;
        if (ch == 1) { std::string u, p, s; double b; std::cout << "User:"; std::cin >> u; std::cout << "Pass:"; std::cin >> p; std::cout << "Bal:"; std::cin >> b; std::cout << "status:"; std::cin >> s; req = "CREATE_ACCOUNT;" + u + ";" + p + ";" + std::to_string(b) + ";" + s; }
        else if (ch == 2) { int id; std::cout << "id:"; std::cin >> id; req = "DELETE_ACCOUNT;" + std::to_string(id); }
        else if (ch == 3) { int id; std::cout << "id:"; std::cin >> id; req = "BLOCK_ACCOUNT;" + std::to_string(id); }
        else if (ch == 4) { int id; std::cout << "id:"; std::cin >> id; req = "UNBLOCK_ACCOUNT;" + std::to_string(id); }
        else if (ch == 5) { double r; std::cout << "rate:"; std::cin >> r; req = "APPLY_INTEREST;" + std::to_string(r); }
        else if (ch == 6) { int id; std::cout << "clientId:"; std::cin >> id; req = "VIEW_HISTORY;" + std::to_string(id); }
        else if (ch == 7) { double amt; std::cout << "amt:"; std::cin >> amt; req = "SEARCH_BALANCE;" + std::to_string(amt); }
        else if (ch == 8) { std::string sd, ed; double minA, maxA; std::cout << "startDate:"; std::cin >> sd; std::cout << "endDate:"; std::cin >> ed; std::cout << "minAmt:"; std::cin >> minA; std::cout << "maxAmt:"; std::cin >> maxA; req = "GENERATE_REPORT;" + sd + ";" + ed + ";" + std::to_string(minA) + ";" + std::to_string(maxA); }
        else if (ch == 9) { double amt; int dur; std::cout << "amount:"; std::cin >> amt; std::cout << "duration:"; std::cin >> dur; req = "REQUEST_CREDIT;" + std::to_string(amt) + ";" + std::to_string(dur); }
        else if (ch == 10) { int cid; std::cout << "creditId:"; std::cin >> cid; req = "APPROVE_CREDIT;" + std::to_string(cid); }
        else if (ch == 11) { int cid; std::string t; std::cout << "creditId:"; std::cin >> cid; std::cout << "type:"; std::cin >> t; req = "SET_CREDIT_TYPE;" + std::to_string(cid) + ";" + t; }
        else if (ch == 12) { int cid, term; std::cout << "creditId:"; std::cin >> cid; std::cout << "term:"; std::cin >> term; req = "SET_CREDIT_TERM;" + std::to_string(cid) + ";" + std::to_string(term); }
        else continue;
        sendReq(sock, req);
        std::string resp = recvResp(sock);
        std::cout << resp << "\n";
    }
}

void peopleManagerMenu(SOCKET sock) {
    while (true) {
        std::cout << "1.VIEW_EMPLOYEES 2.EXIT\n";
        int ch; std::cin >> ch; std::string req;
        if (ch == 2)break;
        if (ch == 1) { req = "VIEW_EMPLOYEES"; }
        else continue;
        sendReq(sock, req);
        std::string resp = recvResp(sock);
        std::cout << resp << "\n";
    }
}
