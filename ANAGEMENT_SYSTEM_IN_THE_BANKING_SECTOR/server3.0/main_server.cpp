#include "server.h"
#include "windows.h"
int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    Server srv;
    srv.Run();
    return 0;
}
