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
        // Обработка ошибки, если нужно
        // например, выдать сообщение о неверном формате адреса
    }
    if (connect(sock, (sockaddr*)&si, sizeof(si)) < 0) {
        cout << "Не удалось подключиться к серверу\n";
        return 1;
    }

    for (;;) {
        cout << "Меню:\n"
            "1. Войти\n"
            "2. Список сотрудников (GET_EMPLOYEES)\n"
            "3. Поиск сотрудников (SEARCH_EMPLOYEES)\n"
            "4. Добавить сотрудника (ADD_EMPLOYEE)\n"
            "5. Удалить сотрудника (DEL_EMPLOYEE)\n"
            "6. Добавить роль (ADD_ROLE)\n"
            "7. Изменить роль юзера (CHANGE_USER_ROLE)\n"
            "8. Подать компенсацию (SUBMIT_COMPENSATION)\n"
            "9. Одобрить компенсацию (APPROVE_COMPENSATION)\n"
            "10. Оценить производительность (EVAL_PERFORMANCE)\n"
            "11. Сводный отчет (GET_REPORTS)\n"
            "12. Подать кредит (SUBMIT_LOAN)\n"
            "13. Одобрить кредит (APPROVE_LOAN)\n"
            "14. Посмотреть логи (GET_LOGS)\n"
            "15. Подать транзакцию (SUBMIT_TRANSACTION)\n"
            "16. Одобрить транзакцию (APPROVE_TRANSACTION)\n"
            "0. Выход\n";
        string c; getline(cin, c);
        if (c == "1") {
            cout << "Логин:"; string u; getline(cin, u);
            cout << "Пароль:"; string p; getline(cin, p);
            cout << send_cmd(sock, "LOGIN " + u + " " + p) << "\n";
        }
        else if (c == "2") {
            cout << send_cmd(sock, "GET_EMPLOYEES") << "\n";
        }
        else if (c == "3") {
            cout << "Подстрока:"; string s; getline(cin, s);
            cout << send_cmd(sock, "SEARCH_EMPLOYEES " + s) << "\n";
        }
        else if (c == "4") {
            cout << "Данные сотрудника (ID;ID_должн;ФИО;ДатаРожд;Адрес;Контакт;ДатаНач;ДатаОконч):\n";
            string ln; getline(cin, ln);
            cout << send_cmd(sock, "ADD_EMPLOYEE " + ln) << "\n";
        }
        else if (c == "5") {
            cout << "ID сотрудника:\n"; string i; getline(cin, i);
            cout << send_cmd(sock, "DEL_EMPLOYEE " + i) << "\n";
        }
        else if (c == "6") {
            cout << "Новая роль:\n"; string r; getline(cin, r);
            cout << send_cmd(sock, "ADD_ROLE " + r) << "\n";
        }
        else if (c == "7") {
            cout << "Логин;НоваяРоль:\n"; string ln; getline(cin, ln);
            cout << send_cmd(sock, "CHANGE_USER_ROLE " + ln) << "\n";
        }
        else if (c == "8") {
            cout << "Компенсация (ID;ID_сотр;ID_должн;Тип;Сумма;Дата;Статус(Ожидает)):\n";
            string ln; getline(cin, ln);
            cout << send_cmd(sock, "SUBMIT_COMPENSATION " + ln) << "\n";
        }
        else if (c == "9") {
            cout << "ID;Статус(Одобрено/Отклонено):\n"; string ln; getline(cin, ln);
            cout << send_cmd(sock, "APPROVE_COMPENSATION " + ln) << "\n";
        }
        else if (c == "10") {
            cout << "(ID;ID_сотр;ID_должн;Оценка;Дата;Комментарий):\n"; string ln; getline(cin, ln);
            cout << send_cmd(sock, "EVAL_PERFORMANCE " + ln) << "\n";
        }
        else if (c == "11") {
            cout << send_cmd(sock, "GET_REPORTS") << "\n";
        }
        else if (c == "12") {
            cout << "(ID;ID_клиента;Сумма;Тип;Статус;ДатаПодачи;ДатаОдобр)\n"; string ln; getline(cin, ln);
            cout << send_cmd(sock, "SUBMIT_LOAN " + ln) << "\n";
        }
        else if (c == "13") {
            cout << "ID;Статус(Одобрено/Отклонено):\n"; string ln; getline(cin, ln);
            cout << send_cmd(sock, "APPROVE_LOAN " + ln) << "\n";
        }
        else if (c == "14") {
            cout << send_cmd(sock, "GET_LOGS") << "\n";
        }
        else if (c == "15") {
            cout << "(ID;ID_клиента;Сумма;ТипОпер;Дата;Статус)\n"; string ln; getline(cin, ln);
            cout << send_cmd(sock, "SUBMIT_TRANSACTION " + ln) << "\n";
        }
        else if (c == "16") {
            cout << "ID;Статус(Одобрено/Отклонено):\n"; string ln; getline(cin, ln);
            cout << send_cmd(sock, "APPROVE_TRANSACTION " + ln) << "\n";
        }
        else if (c == "0") {
            send_cmd(sock, "EXIT");
            break;
        }
        else {
            cout << "Неверный выбор\n";
        }
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
