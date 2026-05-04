#pragma comment(lib, "ws2_32.lib")
#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
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

	SOCKET clientSock = INVALID_SOCKET;
    clientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
    WSACleanup();
    return 0;
}