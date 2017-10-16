#ifndef AES_H
#define AES_H

#include "comm_typedef.h"

#define KEY_TABLE	\
	0x1a, 0x8f, 0x9c, 0xab, 0x1d, 0x9e, 0x6f, 0x0e, \
	0xa6, 0x60, 0xab, 0xc5, 0xb5, 0xf8, 0x5b, 0x50

#define AES_BLOCKSIZE	16


void aes_encrypt_init(void);
void aes_decrypt_init(void);
void aes_encrypt(U8 *in, U8 *out);	//º”√‹
void aes_decrypt(U8 *data);		//Ω‚√‹

#endif // AES_H

