#include "ReportViewer.h"
#include "Client.h"
#include <iostream>

void ReportViewer::viewSalaryReport() {
    Client::sendRequest("GET_REPORT SALARY");
    std::cout << Client::receiveResponse();
}

void ReportViewer::viewCompensationReport() {
    Client::sendRequest("GET_REPORT COMPENSATION");
    std::cout << Client::receiveResponse();
}

void ReportViewer::viewDivisionReport(const std::string& divisionId) {
    Client::sendRequest("GET_DIVISION_REPORT " + divisionId);
    std::cout << Client::receiveResponse();
}
