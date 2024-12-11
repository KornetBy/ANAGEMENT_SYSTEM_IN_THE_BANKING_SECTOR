#include <iostream>
#include <winsock2.h>
#include <string>
#include <Ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024

void initWinsock() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "Ошибка инициализации Winsock!" << endl;
        exit(1);
    }
}

SOCKET createClientSocket() {
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        cerr << "Ошибка создания сокета!" << endl;
        WSACleanup();
        exit(1);
    }
    return clientSocket;
}

void connectToServer(SOCKET& clientSocket) {
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    //serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "Ошибка подключения к серверу!" << endl;
        closesocket(clientSocket);
        WSACleanup();
        exit(1);
    }
    cout << "Подключение к серверу успешно." << endl;
}

void sendRequest(SOCKET& clientSocket, const string& message) {
    send(clientSocket, message.c_str(), message.length(), 0);
}

string receiveResponse(SOCKET& clientSocket) {
    char buffer[BUFFER_SIZE];
    int bytesRead = recv(clientSocket, buffer, BUFFER_SIZE, 0);
    if (bytesRead == SOCKET_ERROR) {
        cerr << "Ошибка получения ответа от сервера!" << endl;
        return "";
    }
    buffer[bytesRead] = '\0';
    return string(buffer);
}

void clientMenu(SOCKET& clientSocket) {
    while (true) {
        string response = receiveResponse(clientSocket);
        cout << response << endl;

        string command;
        getline(cin, command);

        sendRequest(clientSocket, command);

        if (command == "4") {
            break; // Выход из меню
        }

        if (command == "1" || command == "2" || command == "3") {
            // Отправка дополнительных данных
            string accountNumber, amount;
            if (command == "1") {
                cout << "Введите номер аккаунта: ";
                getline(cin, accountNumber);
                sendRequest(clientSocket, accountNumber);

                cout << "Введите сумму для пополнения: ";
                getline(cin, amount);
                sendRequest(clientSocket, amount);
            }
            else if (command == "2") {
                cout << "Введите номер аккаунта: ";
                getline(cin, accountNumber);
                sendRequest(clientSocket, accountNumber);

                cout << "Введите сумму для снятия: ";
                getline(cin, amount);
                sendRequest(clientSocket, amount);
            }
            else if (command == "3") {
                cout << "Введите номер аккаунта: ";
                getline(cin, accountNumber);
                sendRequest(clientSocket, accountNumber);
            }
        }

        response = receiveResponse(clientSocket);
        cout << response << endl;
    }
}

void workerMenu(SOCKET& clientSocket) {
    while (true) {
        string response = receiveResponse(clientSocket);
        cout << response << endl;

        string command;
        getline(cin, command);

        sendRequest(clientSocket, command);

        if (command == "4") {
            break; // Выход из меню
        }

        if (command == "1") {
            string accountNumber, ownerName;
            cout << "Введите номер аккаунта: ";
            getline(cin, accountNumber);
            sendRequest(clientSocket, accountNumber);

            cout << "Введите имя владельца: ";
            getline(cin, ownerName);
            sendRequest(clientSocket, ownerName);
        }
        else if (command == "2" || command == "3") {
            string accountNumber;
            cout << "Введите номер аккаунта: ";
            getline(cin, accountNumber);
            sendRequest(clientSocket, accountNumber);
        }

        response = receiveResponse(clientSocket);
        cout << response << endl;
    }
}

int main() {
    initWinsock();

    SOCKET clientSocket = createClientSocket();
    connectToServer(clientSocket);

    // Выбор роли
    cout << "Введите вашу роль (worker/client): ";
    string role;
    getline(cin, role);
    sendRequest(clientSocket, role);

    string authResponse = receiveResponse(clientSocket);
    cout << authResponse << endl;

    if (authResponse == "Ошибка авторизации.") {
        closesocket(clientSocket);
        WSACleanup();
        return 0;
    }

    if (role == "worker") {
        workerMenu(clientSocket);
    }
    else if (role == "client") {
        clientMenu(clientSocket);
    }

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
