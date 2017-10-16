#ifndef _HASH_H_
#define _HASH_H_

#include "comm_typedef.h"

typedef enum{
	RS_HASH = 0,
	JS_HASH = 1,
	PJW_HASH = 2,
	ELF_HASH = 3,
	BKDR_HASH = 4,
	SDBM_HASH = 5,
	DJB_HASH = 6,
	DEK_HASH = 7,
	BP_HASH = 8,
	FNV_HASH = 9,
	AP_HASH = 10,
	HASH_ALL
}hash_type_e;

U32  hash_cal(hash_type_e hash,U8 *str,U32 len);
#endif