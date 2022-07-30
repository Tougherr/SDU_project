
#include <string.h>
#include "byteorder.h"
#include "immintrin.h"
#include "op_sm3.hpp"

void op_sm3_init(opsm3_ctx_t* ctx)
{
	ctx->digest[0] = 0x7380166F;
	ctx->digest[1] = 0x4914B2B9;
	ctx->digest[2] = 0x172442D7;
	ctx->digest[3] = 0xDA8A0600;
	ctx->digest[4] = 0xA96F30BC;
	ctx->digest[5] = 0x163138AA;
	ctx->digest[6] = 0xE38DEE4D;
	ctx->digest[7] = 0xB0FB0E4E;

	ctx->nblocks = 0;
	ctx->num = 0;
}

void op_sm3_update(opsm3_ctx_t* ctx, const unsigned char* data, size_t data_len)
{
	if (ctx->num) {
		unsigned int left = SM3_BLOCK_SIZE - ctx->num;
		if (data_len < left) {
			memcpy(ctx->block + ctx->num, data, data_len);
			ctx->num += data_len;
			return;
		}
		else {
			memcpy(ctx->block + ctx->num, data, left);
			op_sm3_compress(ctx->digest, ctx->block);
			ctx->nblocks++;
			data += left;
			data_len -= left;
		}
	}
	while (data_len >= SM3_BLOCK_SIZE) {
		op_sm3_compress(ctx->digest, data);
		ctx->nblocks++;
		data += SM3_BLOCK_SIZE;
		data_len -= SM3_BLOCK_SIZE;
	}
	ctx->num = data_len;
	if (data_len) {
		memcpy(ctx->block, data, data_len);
	}
}

void op_sm3_final(opsm3_ctx_t* ctx, unsigned char* digest)
{
	int i;
	uint32_t* pdigest = (uint32_t*)digest;
	uint32_t* count = (uint32_t*)(ctx->block + SM3_BLOCK_SIZE - 8);

	ctx->block[ctx->num] = 0x80;

	if (ctx->num + 9 <= SM3_BLOCK_SIZE) {
		memset(ctx->block + ctx->num + 1, 0, SM3_BLOCK_SIZE - ctx->num - 9);
	}
	else {
		memset(ctx->block + ctx->num + 1, 0, SM3_BLOCK_SIZE - ctx->num - 1);
		op_sm3_compress(ctx->digest, ctx->block);
		memset(ctx->block, 0, SM3_BLOCK_SIZE - 8);
	}

	count[0] = cpu_to_be32((ctx->nblocks) >> 23);
	count[1] = cpu_to_be32((ctx->nblocks << 9) + (ctx->num << 3));

	op_sm3_compress(ctx->digest, ctx->block);
	for (i = 0; i < sizeof(ctx->digest) / sizeof(ctx->digest[0]); i++) {
		pdigest[i] = cpu_to_be32(ctx->digest[i]);
	}
}

#define ROTATELEFT(X,n)  (((X)<<(n)) | ((X)>>(32-(n))))

#define P0(x) ((x) ^  ROTATELEFT((x),9)  ^ ROTATELEFT((x),17))
#define P1(x) ((x) ^  ROTATELEFT((x),15) ^ ROTATELEFT((x),23))

#define FF0(x,y,z) ( (x) ^ (y) ^ (z))
#define FF1(x,y,z) (((x) & (y)) | ( (x) & (z)) | ( (y) & (z)))

#define GG0(x,y,z) ( (x) ^ (y) ^ (z))
#define GG1(x,y,z) (((x) & (y)) | ( (~(x)) & (z)) )


void op_sm3_compress(uint32_t digest[8], const unsigned char block[64])
{
	int j;
	uint32_t W[68], W1[64];
	const uint32_t* pblock = (const uint32_t*)block;

	uint32_t A = digest[0];
	uint32_t B = digest[1];
	uint32_t C = digest[2];
	uint32_t D = digest[3];
	uint32_t E = digest[4];
	uint32_t F = digest[5];
	uint32_t G = digest[6];
	uint32_t H = digest[7];
	uint32_t SS1, SS2, TT1, TT2, T[64];

	
	for (j = 0; j < 2; j++) {
		W[0 + j * 8] = cpu_to_be32(pblock[0 + j * 8]);
		W[1 + j * 8] = cpu_to_be32(pblock[1 + j * 8]);
		W[2 + j * 8] = cpu_to_be32(pblock[2 + j * 8]);
		W[3 + j * 8] = cpu_to_be32(pblock[3 + j * 8]);
		W[4 + j * 8] = cpu_to_be32(pblock[4 + j * 8]);
		W[5 + j * 8] = cpu_to_be32(pblock[5 + j * 8]);
		W[6 + j * 8] = cpu_to_be32(pblock[6 + j * 8]);
		W[7 + j * 8] = cpu_to_be32(pblock[7 + j * 8]);
	}
	for (j = 0; j < 13; j++) {
		W[16 + 4 * j] = P1(W[16 + 4 * j - 16] ^ W[16 + 4 * j - 9] ^ ROTATELEFT(W[16 + 4 * j - 3], 15)) ^ ROTATELEFT(W[16 + 4 * j - 13], 7) ^ W[16 + 4 * j - 6];
		W[17 + 4 * j] = P1(W[17 + 4 * j - 16] ^ W[17 + 4 * j - 9] ^ ROTATELEFT(W[17 + 4 * j - 3], 15)) ^ ROTATELEFT(W[17 + 4 * j - 13], 7) ^ W[17 + 4 * j - 6];
		W[18 + 4 * j] = P1(W[18 + 4 * j - 16] ^ W[18 + 4 * j - 9] ^ ROTATELEFT(W[18 + 4 * j - 3], 15)) ^ ROTATELEFT(W[18 + 4 * j - 13], 7) ^ W[18 + 4 * j - 6];
		W[19 + 4 * j] = P1(W[19 + 4 * j - 16] ^ W[19 + 4 * j - 9] ^ ROTATELEFT(W[19 + 4 * j - 3], 15)) ^ ROTATELEFT(W[19 + 4 * j - 13], 7) ^ W[19 + 4 * j - 6];
	}
	for (j = 0; j < 8; j++) {
		__m128i a = _mm_loadu_epi32(&W[0 + 8 * j]);
		__m128i b = _mm_loadu_epi32(&W[4 + 8 * j]);
		__m128i c = _mm_loadu_epi32(&W[8 + 8 * j]);
		__m128i d = _mm_xor_si128(a, b);
		__m128i e = _mm_xor_si128(b, c);
		_mm_storeu_epi32(&W1[0 + 8 * j], d);
		_mm_storeu_epi32(&W1[4 + 8 * j], e);
	}

	uint32_t tmp1 = 0x79CC4519;
	uint32_t tmp2 = 0x7A879D8A;

	for (j = 0; j < 16; j++) {
		SS1 = ROTATELEFT((ROTATELEFT(A, 12) + E + ROTATELEFT(tmp1, j)), 7);
		SS2 = SS1 ^ ROTATELEFT(A, 12);
		TT1 = FF0(A, B, C) + D + SS2 + W1[j];
		TT2 = GG0(E, F, G) + H + SS1 + W[j];
		D = C;
		C = ROTATELEFT(B, 9);
		B = A;
		A = TT1;
		H = G;
		G = ROTATELEFT(F, 19);
		F = E;
		E = P0(TT2);
	}

	for (j = 16; j < 64; j++) {
		SS1 = ROTATELEFT((ROTATELEFT(A, 12) + E + ROTATELEFT(tmp2, j)), 7);
		SS2 = SS1 ^ ROTATELEFT(A, 12);
		TT1 = FF1(A, B, C) + D + SS2 + W1[j];
		TT2 = GG1(E, F, G) + H + SS1 + W[j];
		D = C;
		C = ROTATELEFT(B, 9);
		B = A;
		A = TT1;
		H = G;
		G = ROTATELEFT(F, 19);
		F = E;
		E = P0(TT2);
	}

	digest[0] ^= A;
	digest[1] ^= B;
	digest[2] ^= C;
	digest[3] ^= D;
	digest[4] ^= E;
	digest[5] ^= F;
	digest[6] ^= G;
	digest[7] ^= H;
}

void op_sm3(const unsigned char* msg, size_t msglen,
	unsigned char dgst[SM3_DIGEST_LENGTH])
{
	opsm3_ctx_t ctx;

	op_sm3_init(&ctx);
	op_sm3_update(&ctx, msg, msglen);
	op_sm3_final(&ctx, dgst);

	memset(&ctx, 0, sizeof(opsm3_ctx_t));
}