#include <iostream>
#include <cmath>
#define MOD 23
#define G 2
#define ALICE 12
#define BOB 7

int fast_pow_func(int base, int exp, int mod) 
{
    int res = 1;
    while (exp > 0) {
    if (exp & 1) {
        res = (res * base) % mod;
    }
    base = (base * base) % mod;
    exp >>= 1;
    }
    return res;
}

int bsgs(int publicKey, int g, int mod) 
{
    int m = static_cast<int>(std::round(sqrt(MOD)));

    return 1;
}

int main()
{
    std::cout << G << "^" << ALICE << " mod " << MOD << std::endl;
    std::cout << fast_pow_func(G, ALICE, MOD) << std::endl;
}

