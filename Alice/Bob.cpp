#pragma comment(lib, "ws2_32.lib")
#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <string>
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

	SOCKET serverSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSock < 0) 
    {
		std::cerr << "Failed to create socket" << std::endl;
		WSACleanup();
		ExitProcess(EXIT_FAILURE);
    }
	else std::cout << "Socket is ok" << std::endl;
	
	int port = 8081;
    sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(port);

    return 0;
}