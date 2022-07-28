#define SM3_DIGEST_LENGTH	32
#define SM3_BLOCK_SIZE		64
#define SM3_HMAC_SIZE		(SM3_DIGEST_LENGTH)

#include <sys/types.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif


	typedef struct {
		uint32_t digest[8];
		int nblocks;
		unsigned char block[64];
		int num;
	} opsm3_ctx_t;

	void op_sm3_init(opsm3_ctx_t* ctx);
	void op_sm3_update(opsm3_ctx_t* ctx, const unsigned char* data, size_t data_len);
	void op_sm3_final(opsm3_ctx_t* ctx, unsigned char digest[SM3_DIGEST_LENGTH]);
	void op_sm3_compress(uint32_t digest[8], const unsigned char block[SM3_BLOCK_SIZE]);
	void op_sm3(const unsigned char* data, size_t datalen,
		unsigned char digest[SM3_DIGEST_LENGTH]);

#ifdef __cplusplus
}
#endif


