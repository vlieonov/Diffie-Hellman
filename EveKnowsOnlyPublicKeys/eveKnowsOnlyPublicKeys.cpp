#include "diffie.h"

//test of BSGS algorithm in case if Eve knows only Public keys, for example Alice's Public key.
int main()
{
	long long alicePublicKey = fast_pow_func(G, ALICE_SECRET, MOD);
	long long bobPublicKey = fast_pow_func(G, BOB_SECRET, MOD);

	std::cout << "Alice's Public Key: " << alicePublicKey << std::endl;
	std::cout << "Bob's Public Key: " << bobPublicKey << std::endl;

	long long mixKeyFromAlice = fast_pow_func(alicePublicKey, ALICE_SECRET, MOD);
	long long mixKeyFromBob = fast_pow_func(bobPublicKey, ALICE_SECRET, MOD);
	std::cout << "\nMix Key from Alice: " << mixKeyFromAlice << std::endl;
	std::cout << "Mix Key from Bob: " << mixKeyFromBob << std::endl;

	std::cout << "\nExpected result: " << ALICE_SECRET << std::endl;
	long long result = bsgs(alicePublicKey, G, MOD);
	std::cout << "Eve founded Alice's secret key: " << result << std::endl;

	std::cout << "\nExpected result: " << BOB_SECRET << std::endl;
	result = bsgs(bobPublicKey, G, MOD);
	std::cout << "Eve founded Bob's secret key: " << result << std::endl;
}
