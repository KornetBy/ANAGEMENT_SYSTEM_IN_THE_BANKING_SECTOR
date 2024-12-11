#pragma once
#include <winsock2.h>
#include <string>

SOCKET connectToServer(const std::string& ip, int port);
