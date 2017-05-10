//=============================================================================
// Copyright Atmel Corporation 2003. All Rights Reserved.
//
// File:			des.h
// Compiler:		IAR Atmel AVR C/EC++ Compiler
// Output Size:
// Based on work by:ÿE, VU
// Created:			4-Feb-2003	JP (Atmel Finland)
// Modified:	
//
// Support Mail:	avr@atmel.com
//
// Description:		Please refer to Application Note Documentation for more
//					information.
//
//					For details on DES, please refer to the official FIPS 46-3
//					document:
//
//				http://csrc.nist.gov/publications/fips/fips46-3/fips46-3.pdf
//
//=============================================================================

#ifndef AES_H
#define AES_H

#include "comm_typedef.h"

#define KEY_TABLE	\
	0x1a, 0x8e, 0x9c, 0xab, 0x1d, 0x9e, 0x6f, 0x0e, \
	0xa6, 0x60, 0xab, 0xc5, 0xb5, 0xf8, 0x5b, 0x50

#define AES_BLOCKSIZE	16


void aes_encrypt_init(void);
void aes_decrypt_init(void);
void aes_encrypt(U8 *in, U8 *out);	//º”√‹
void aes_decrypt(U8 *data);		//Ω‚√‹

#endif // AES_H

