#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include <openssl/sm3.h>
#include <openssl/evp.h>

using namespace std;


struct item {
	unsigned char dgst[SM3_DIGEST_LENGTH]{};
};

void sm3(const unsigned char* msg, size_t msglen, unsigned char dgst[SM3_DIGEST_LENGTH]) {
	EVP_MD_CTX* ctx = EVP_MD_CTX_new();
	unsigned int dgst_len = 0;
	EVP_DigestInit_ex(ctx, EVP_sm3(), NULL);
	EVP_DigestUpdate(ctx, msg, msglen);
	EVP_DigestFinal(ctx, dgst, &dgst_len);

	EVP_MD_CTX_free(ctx);
	ctx = NULL;
}


int main(int argc, char** argv)
{
	unsigned char msg[] = "Rho method";
	size_t rholen = 10;
	size_t cmp_len = 2;

	item* Rho = new item[rholen];
	sm3(msg, strlen((const char *)msg), Rho[0].dgst);
	int i = 0;
	while (memcmp(Rho[i].dgst, Rho[(i + 1) % rholen].dgst, cmp_len) != 0) {
		sm3(Rho[i].dgst, SM3_DIGEST_LENGTH, Rho[(i + 1) % rholen].dgst);
		i = (i + 1) % rholen;
	}
	std::cout << "succeed!Get one Rho" << std::endl;
	i = (i + 1) % rholen;
	for (int j = 0; j < rholen; j++) {
		for (int k = 0; k < SM3_DIGEST_LENGTH; k++)
			printf("%x", Rho[i].dgst[k]);
		printf("\n");
		i = (i + 1) % rholen;
	}
	return 0;
}