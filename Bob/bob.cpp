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

    std::cout << "Bob:" << std::endl;

    SOCKET serverSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSock < 0)
    {
        std::cerr << "Failed to create socket" << std::endl;
        WSACleanup();
        ExitProcess(EXIT_FAILURE);
        return 1;
    }
    else std::cout << "Socket is ok" << std::endl;

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(DEFAULT_PORT);

    if (bind(serverSock, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "bind failed:" << WSAGetLastError() << std::endl;
        closesocket(serverSock);
        WSACleanup();
        return 1;
    }
    else {
        std::cout << "bind is ok" << std::endl;
    }

    if (listen(serverSock, 1) == SOCKET_ERROR) {
        std::cerr << "listen : error listening on socket" << WSAGetLastError() << std::endl;
        closesocket(serverSock);
        WSACleanup();
        return 1;
    }
    else std::cout << "Bob is listening" << std::endl;

    SOCKET acceptSocket;
    acceptSocket = accept(serverSock, NULL, NULL);

    if (acceptSocket == INVALID_SOCKET) {
        std::cerr << "accept failed:" << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }
    else std::cout << "accept is ok" << std::endl;

    int bobPublicKey = fast_pow_func(G, BOB_SECRET, MOD);
    std::string s = std::to_string(bobPublicKey);
    send(acceptSocket, s.c_str(), (int)s.length(), 0);

    long long receivedPublicKey = std::stoll(recv_func(acceptSocket));

    closesocket(acceptSocket);
    closesocket(serverSock);
    WSACleanup();
    return 0;
}