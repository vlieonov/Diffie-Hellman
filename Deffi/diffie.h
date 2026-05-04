#pragma once
#define MOD 23
#define G 2
#define ALICE_SECRET 10
#define BOB_SECRET 5
#define DEFAULT_PORT 8080
#define ATTACK_PORT 8081
int fast_pow_func(long long base, long long exp, long long mod);
int bsgs(long long publicKey, long long g, long long M);