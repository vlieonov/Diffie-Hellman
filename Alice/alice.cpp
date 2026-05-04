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
	if (clientSock < 0)
	{
		std::cerr << "Failed to create socket" << std::endl;
		WSACleanup();
		ExitProcess(EXIT_FAILURE);
	}
	else std::cout << "Socket is ok" << std::endl;

	sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	clientService.sin_port = htons(DEFAULT_PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &clientService.sin_addr) <= 0) {
        std::cerr << "Invalid address" << std::endl;
        return 1;
    }

    if (connect(clientSock, (sockaddr*)&clientService, sizeof(clientService))) {
		std::cerr << "Connection failed: " << WSAGetLastError() << std::endl;
		WSACleanup();
        return 1;
    }
	else std::cout << "Connected to Bob" << std::endl;
    
    int msg = 123;
    std::string s = std::to_string(msg);
    send(clientSock, s.c_str(), (int)s.length(), 0);

    char buffer[1024];
    int bytesReceived = recv(clientSock, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        std::cout << "Public key received:" << buffer <<std::endl;
        long long key = std::stoll(buffer);
    }
    else if (bytesReceived == 0) std::cout << "Connection is closed" << std::endl;
    else std::cerr << "recv failed" << std::endl;

    system("pause");
    closesocket(clientSock);
	std::cout << "Connection closed" << std::endl;
    WSACleanup();
    return 0;
}