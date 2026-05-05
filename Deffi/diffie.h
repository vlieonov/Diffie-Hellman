#pragma once
#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <string>
#pragma comment(lib, "ws2_32.lib")

#define MOD 17
#define G 3
#define ALICE_SECRET 15
#define BOB_SECRET 5
#define EVE_SECRET 13
#define DEFAULT_PORT 8080
#define ATTACK_PORT 8081
long long fast_pow_func(long long base, long long exp, long long mod);
int bsgs(long long publicKey, long long g, long long M);
std::string encryptMsg(std::string msg, long long key);
std::string recv_func(SOCKET sock);