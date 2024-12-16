#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <map>

// Константы
constexpr int BUFFER_SIZE = 1024;

// Функции клиента
std::map<std::string, std::string> loadClientConfig(const std::string& configFile);
void connectToServer(SOCKET& clientSocket, const std::string& serverIp, int serverPort);
void sendRequest(SOCKET clientSocket, const std::string& request);
std::string receiveResponse(SOCKET clientSocket);

#endif // CLIENT_H
