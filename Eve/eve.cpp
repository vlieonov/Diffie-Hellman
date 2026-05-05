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
    
	std::cout << "Eve:" << std::endl;

    SOCKET eveSock = INVALID_SOCKET;
    if ((eveSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        std::cerr << "failed to create Socket" << std::endl;
        WSACleanup();
        ExitProcess(EXIT_FAILURE);
        return 1;
    }
    else std::cout << "Socket is ok" << std::endl;

    sockaddr_in eveServAddr;
    eveServAddr.sin_family = AF_INET;
    eveServAddr.sin_addr.s_addr = INADDR_ANY;
    eveServAddr.sin_port = htons(ATTACK_PORT);

    if (bind(eveSock, (SOCKADDR*)&eveServAddr, sizeof(eveServAddr)) != 0) {
        std::cerr << "Bind failed" << std::endl;
        WSACleanup();
        ExitProcess(EXIT_FAILURE);
        return 1;
    }
    else std::cout << "Bind is ok" << std::endl;

    if (listen(eveSock, 1) != 0) {
        std::cerr << "listen failed" << std::endl;
        WSACleanup();
        ExitProcess(EXIT_FAILURE);
        return 1;
    }
    else std::cout << "Eve is listening..." << std::endl;

    SOCKET acceptSock = INVALID_SOCKET;
    acceptSock = accept(eveSock, NULL, NULL);
    
    if (acceptSock == INVALID_SOCKET) {
        std::cerr << "accept failed" << std::endl;
        WSACleanup();
        ExitProcess(EXIT_FAILURE);
        return 1;
    }
    else std::cout << "accept is ok" << std::endl;
    long long aliceKey = 0;
    char buffer[1024];
    int recvResult = recv(acceptSock, buffer, sizeof(buffer) - 1, 0);
    if (recvResult > 0) {
        buffer[recvResult] = '\0';
        std::cout << "Received from Alice:" << buffer << std::endl;
        aliceKey = std::stoll(buffer);
    }
    else if (recvResult == 0) std::cout << "Connection closed by Alice" << std::endl;
	else {
		std::cerr << "recv failed" << std::endl;
		WSACleanup();
		ExitProcess(EXIT_FAILURE);
		return 1;
	}

    int msg = fast_pow_func(G, EVE_SECRET, MOD);
    std::string s = std::to_string(msg);
    send(acceptSock, s.c_str(), (int)s.length(), 0);

    //Eve gets cyphered msg from Alice
	std::string aliceMsg;
    char buffer_secret[1024];
    int temp = recv(acceptSock, buffer_secret, sizeof(buffer) - 1, 0);
    if (temp > 0) {
        buffer_secret[temp] = '\0';
        std::cout << "Received from Alice:" << buffer_secret << std::endl;
        aliceMsg = buffer_secret;
    }
    else if (temp == 0) std::cout << "Connection closed by Alice" << std::endl;
    else {
        std::cerr << "recv failed" << std::endl;
        WSACleanup();
        ExitProcess(EXIT_FAILURE);
        return 1;
    }

	//Eve decrypts the msg from Alice
    std::string encryptedMsg = encryptMsg(aliceMsg, fast_pow_func(aliceKey, EVE_SECRET, MOD));
	std::cout << "Decrypted message from Alice: " << encryptedMsg << std::endl;

    //Eve Client
	SOCKET eveToBobSock = INVALID_SOCKET;
	if ((eveToBobSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		std::cerr << "failed to create Socket" << std::endl;
		WSACleanup();
		ExitProcess(EXIT_FAILURE);
		return 1;
	}
	else std::cout << "Socket is ok" << std::endl;

	sockaddr_in bobAddr;
	bobAddr.sin_family = AF_INET;
	bobAddr.sin_port = htons(DEFAULT_PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &bobAddr.sin_addr) <= 0) {
        std::cerr << "Invalid address" << std::endl;
        return 1;
    }

    if (connect(eveToBobSock, (SOCKADDR*)&bobAddr, sizeof(bobAddr))) {
        std::cerr << "connect failed" << std::endl;
        WSACleanup();
        ExitProcess(EXIT_FAILURE);
        return 1;
    }
    else std::cout << "Eve is connected to Alice" << std::endl;

    char bufferFromBob[1024];
    int recvFromBob = recv(eveToBobSock, buffer, sizeof(buffer) - 1, 0);
    if (recvFromBob > 0) {
		buffer[recvFromBob] = '\0';
		std::cout << "Received from Bob:" << buffer << std::endl;
		long long bobKey = std::stoll(buffer);
	}
	else if (recvFromBob == 0) std::cout << "Connection closed by Bob" << std::endl;
	else {
		std::cerr << "recv failed" << std::endl;
		WSACleanup();
		ExitProcess(EXIT_FAILURE);
		return 1;
	}

    int msgToBob = 321;
    std::string str = std::to_string(msgToBob);
    send(eveToBobSock, str.c_str(), (int)str.length(), 0);

    system("pause");
    closesocket(acceptSock);
    closesocket(eveSock);
    WSACleanup();
    return 0;
}