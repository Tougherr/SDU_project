#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <openssl/sm3.h>
#include <openssl/evp.h>
#include <math.h>

using namespace std;


void print_dgst(const uint8_t str[SM3_DIGEST_LENGTH]) {
	for (int i = 0; i < SM3_DIGEST_LENGTH; i++) {
		printf("%x", str[i]);
	}
	printf("\n");
}

int main(int argc, char** argv) {

	char plain[] = "birthday attack";
	int plain_len = strlen(plain);

	uint8_t dgst[SM3_DIGEST_LENGTH];
	unsigned int dgst_len = 0;

	EVP_MD_CTX* ctx = EVP_MD_CTX_new();
	EVP_DigestInit_ex(ctx, EVP_sm3(), NULL);
	EVP_DigestUpdate(ctx, plain, plain_len);
	EVP_DigestFinal(ctx, dgst, &dgst_len);

	size_t cmp_bytes = 3;

	printf("message   digest: ");
	print_dgst(dgst);
	EVP_MD_CTX_free(ctx);

	uint8_t attack[32] = { 0 };
	uint8_t dgst1[SM3_DIGEST_LENGTH];
	unsigned int dgst_len1 = 0;

	for (int i = 0; ; i++) {
		ctx = EVP_MD_CTX_new();
		EVP_DigestInit_ex(ctx, EVP_sm3(), NULL);
		EVP_DigestUpdate(ctx, attack, 32);
		EVP_DigestFinal(ctx, dgst1, &dgst_len1);
		EVP_MD_CTX_free(ctx);
		int flag = memcmp(dgst, dgst1, cmp_bytes);
		if (flag == 0) {
			printf("collision digest: ");
			print_dgst(dgst1);
			break;
		}
		*((uint64_t*)attack) += 1;
	}

	return 0;
}