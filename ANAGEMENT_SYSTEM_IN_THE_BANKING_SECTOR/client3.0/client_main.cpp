#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <string>
#include <ws2tcpip.h> 
#pragma comment(lib,"ws2_32.lib")
using namespace std;

static string send_cmd(SOCKET s, const string& c) {
    send(s, c.c_str(), (int)c.size(), 0);
    char buf[8192]; int r = recv(s, buf, 8192, 0);
    if (r <= 0)return "";
    return string(buf, buf + r);
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    INET_ADDRSTRLEN;
    WSADATA w; WSAStartup(MAKEWORD(2, 2), &w);
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in si; 
    si.sin_family = AF_INET; 
    si.sin_port = htons(5000); 
    if (InetPton(AF_INET, TEXT("127.0.0.1"), &si.sin_addr) != 1) {
        // ��������� ������, ���� �����
        // ��������, ������ ��������� � �������� ������� ������
    }
    if (connect(sock, (sockaddr*)&si, sizeof(si)) < 0) {
        cout << "�� ������� ������������ � �������\n";
        return 1;
    }

    for (;;) {
        cout << "����:\n"
            "1. �����\n"
            "2. ������ ����������� (GET_EMPLOYEES)\n"
            "3. ����� ����������� (SEARCH_EMPLOYEES)\n"
            "4. �������� ���������� (ADD_EMPLOYEE)\n"
            "5. ������� ���������� (DEL_EMPLOYEE)\n"
            "6. �������� ���� (ADD_ROLE)\n"
            "7. �������� ���� ����� (CHANGE_USER_ROLE)\n"
            "8. ������ ����������� (SUBMIT_COMPENSATION)\n"
            "9. �������� ����������� (APPROVE_COMPENSATION)\n"
            "10. ������� ������������������ (EVAL_PERFORMANCE)\n"
            "11. ������� ����� (GET_REPORTS)\n"
            "12. ������ ������ (SUBMIT_LOAN)\n"
            "13. �������� ������ (APPROVE_LOAN)\n"
            "14. ���������� ���� (GET_LOGS)\n"
            "15. ������ ���������� (SUBMIT_TRANSACTION)\n"
            "16. �������� ���������� (APPROVE_TRANSACTION)\n"
            "0. �����\n";
        string c; getline(cin, c);
        if (c == "1") {
            cout << "�����:"; string u; getline(cin, u);
            cout << "������:"; string p; getline(cin, p);
            cout << send_cmd(sock, "LOGIN " + u + " " + p) << "\n";
        }
        else if (c == "2") {
            cout << send_cmd(sock, "GET_EMPLOYEES") << "\n";
        }
        else if (c == "3") {
            cout << "���������:"; string s; getline(cin, s);
            cout << send_cmd(sock, "SEARCH_EMPLOYEES " + s) << "\n";
        }
        else if (c == "4") {
            cout << "������ ���������� (ID;ID_�����;���;��������;�����;�������;�������;���������):\n";
            string ln; getline(cin, ln);
            cout << send_cmd(sock, "ADD_EMPLOYEE " + ln) << "\n";
        }
        else if (c == "5") {
            cout << "ID ����������:\n"; string i; getline(cin, i);
            cout << send_cmd(sock, "DEL_EMPLOYEE " + i) << "\n";
        }
        else if (c == "6") {
            cout << "����� ����:\n"; string r; getline(cin, r);
            cout << send_cmd(sock, "ADD_ROLE " + r) << "\n";
        }
        else if (c == "7") {
            cout << "�����;���������:\n"; string ln; getline(cin, ln);
            cout << send_cmd(sock, "CHANGE_USER_ROLE " + ln) << "\n";
        }
        else if (c == "8") {
            cout << "����������� (ID;ID_����;ID_�����;���;�����;����;������(�������)):\n";
            string ln; getline(cin, ln);
            cout << send_cmd(sock, "SUBMIT_COMPENSATION " + ln) << "\n";
        }
        else if (c == "9") {
            cout << "ID;������(��������/���������):\n"; string ln; getline(cin, ln);
            cout << send_cmd(sock, "APPROVE_COMPENSATION " + ln) << "\n";
        }
        else if (c == "10") {
            cout << "(ID;ID_����;ID_�����;������;����;�����������):\n"; string ln; getline(cin, ln);
            cout << send_cmd(sock, "EVAL_PERFORMANCE " + ln) << "\n";
        }
        else if (c == "11") {
            cout << send_cmd(sock, "GET_REPORTS") << "\n";
        }
        else if (c == "12") {
            cout << "(ID;ID_�������;�����;���;������;����������;���������)\n"; string ln; getline(cin, ln);
            cout << send_cmd(sock, "SUBMIT_LOAN " + ln) << "\n";
        }
        else if (c == "13") {
            cout << "ID;������(��������/���������):\n"; string ln; getline(cin, ln);
            cout << send_cmd(sock, "APPROVE_LOAN " + ln) << "\n";
        }
        else if (c == "14") {
            cout << send_cmd(sock, "GET_LOGS") << "\n";
        }
        else if (c == "15") {
            cout << "(ID;ID_�������;�����;�������;����;������)\n"; string ln; getline(cin, ln);
            cout << send_cmd(sock, "SUBMIT_TRANSACTION " + ln) << "\n";
        }
        else if (c == "16") {
            cout << "ID;������(��������/���������):\n"; string ln; getline(cin, ln);
            cout << send_cmd(sock, "APPROVE_TRANSACTION " + ln) << "\n";
        }
        else if (c == "0") {
            send_cmd(sock, "EXIT");
            break;
        }
        else {
            cout << "�������� �����\n";
        }
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
