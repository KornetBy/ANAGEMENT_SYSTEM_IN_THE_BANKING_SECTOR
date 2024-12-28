// main.cpp
#include "Server.h"
#include <iostream>

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    Server server("0.0.0.0", 54000,

        "C:\\proj\\server\\server_logs.txt",
        "C:\\proj\\server\\users.txt",
        "C:\\proj\\server\\employees.txt",
        "C:\\proj\\server\\positions.txt",
        "C:\\proj\\server\\courses.txt",
        "C:\\proj\\server\\compensation_requests.txt",
        "C:\\proj\\server\\salaries.txt",
        "C:\\proj\\server\\schedules.txt",
        "C:\\proj\\server\\job_responsibilities.txt",
        "C:\\proj\\server\\departments.txt",
        "C:\\proj\\server\\performance_evaluations.txt",
        "C:\\proj\\server\\history.txt");
    if (!server.start()) {
        std::cout << "Не удалось запустить сервер.\n";
        return 1;
    }
    server.run();
    server.stop();
    return 0;
}
