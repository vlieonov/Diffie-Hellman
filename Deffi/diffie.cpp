#include "diffie.h"
#include <cmath>
#include <map>

long long fast_pow_func(long long base, long long exp, long long mod)
{
    long long res = 1;
    while (exp > 0) {
    if (exp & 1) {
        res = (res * base) % mod;
    }
    base = (base * base) % mod;
    exp >>= 1;
    }
    return res;
}

//g^x mod M = publicKey ==> x = i * m - j ==> g^(m)^i = publicKey * g^j. 
int bsgs(long long publicKey, long long g, long long M)
{
    long long m = static_cast<long long>(std::ceil(sqrt(M)));
    std::map<long long, long long> babystep;

    //baby step ==> publicKey * g^j
    for (int j = 0; j < m; j++)
    {
        long long value = (publicKey * fast_pow_func(g, j, M)) % M;
		babystep[value] = j;
    }
    //giant step ==> g^(m)^i
	long long g_m_i = 1;
    long long g_m = fast_pow_func(g, m, M);
    for (int i = 1; i < m; i++) 
    {
		g_m_i = (g_m_i * g_m) % M;
        if (babystep.count(g_m_i)) {
			return i * m - babystep[g_m_i];
        }
    }
    return -1;
}

std::string encryptMsg(std::string msg, long long key) 
{
	std::string encryptedMsg = msg;
    for (size_t i = 0; i < msg.length(); i++) {
        encryptedMsg[i] = msg[i] ^ (static_cast<char>(key % 256));
    }
    return encryptedMsg;
}

std::string recv_func(SOCKET sock) 
{
	char buffer[1024];
	int bytesReceived = recv(sock, buffer, sizeof(buffer) - 1, 0);
	if (bytesReceived > 0) {
		buffer[bytesReceived] = '\0';
		std::cout << "Received: " << buffer << std::endl;
		return std::string(buffer);
	}
	else if (bytesReceived == 0) {
		std::cout << "Connection closed by client" << std::endl;
		return "CLOSED";
	}
	else {
		std::cerr << "recv failed:" << WSAGetLastError() << std::endl;
		closesocket(sock);
		WSACleanup();
		return "ERROR";
	}
}