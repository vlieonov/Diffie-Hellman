#include <iostream>
#include <cmath>
#include <map>
#define MOD 23
#define G 2
#define ALICE_SECRET 10
#define BOB_SECRET 5

int fast_pow_func(long long base, long long exp, long long mod)
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
    long long m = static_cast<long long>(std::round(sqrt(M)));
    std::map<long long, long long> babystep;

    //baby step ==> publicKey * g^j
    for (int j = 0; j < m; j++)
    {
        long long value = (publicKey * fast_pow_func(g, j, M)) % M;
		babystep[value] = j;
    }
    //giant step ==> g^(m)^i
	int g_m_i = 1;
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

int main()
{
    int alice = fast_pow_func(G, ALICE_SECRET, MOD);
    std::cout << G << "^" << ALICE_SECRET << " mod " << MOD << " = " << alice << std::endl;
    std::cout << "Stolen key: " << bsgs(alice, G, MOD) << std::endl;
}

