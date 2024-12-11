#pragma once
#include <winsock2.h>
SOCKET createServerSocket(int port);
void clientHandler(SOCKET clientSocket);
