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

    std::cout << "Alice:" << std::endl;

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
	clientService.sin_port = htons(ATTACK_PORT);

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
    
    int alicePublicKey = fast_pow_func(G, ALICE_SECRET, MOD);
    std::string s = std::to_string(alicePublicKey);
    send(clientSock, s.c_str(), (int)s.length(), 0);

    long long receivedPublicKey = std::stoll(recv_func(clientSock));

	//Alice send cyphered msg to Bob(Eve)
	std::string msgToBob = "Hello Bob!";
	std::string encryptedMsg = encryptMsg(msgToBob, fast_pow_func(receivedPublicKey, ALICE_SECRET, MOD));
	send(clientSock, encryptedMsg.c_str(), (int)encryptedMsg.length(), 0);

    system("pause");
    closesocket(clientSock);
	std::cout << "Connection closed" << std::endl;
    WSACleanup();
    return 0;
}