#pragma comment(lib, "ws2_32.lib")
#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <string>
#include "diffie.h"
int main()
{
    WSADATA wsaData;

    if ((WSAStartup(MAKEWORD(2, 2), &wsaData) != 0))
    {
        ExitProcess(EXIT_FAILURE);
    }

    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
    {
        std::cerr << "Could not find a usable version of Winsock.dll" << std::endl;
        WSACleanup();
        ExitProcess(EXIT_FAILURE);
    }

    SOCKET eveSock = INVALID_SOCKET;
    if (eveSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP) < 0) {
        std::cerr << "failed to create Socket" << std::endl;
        WSACleanup();
        ExitProcess(EXIT_FAILURE);
        return 1;
    }
    else std::cout << "Socket is ok" << std::endl;

    return 0;
}