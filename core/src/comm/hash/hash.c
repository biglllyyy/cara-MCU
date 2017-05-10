#include "hash.h" 
#include "comm_typedef.h"


U32 RSHash(U8* str, U32 len)  
{  
	U32 b    = 378551;  
	U32 a    = 63689;  
	U32 hash = 0;  
	U32 i    = 0;  

	for(i = 0; i < len; str++, i++)  
	{  
		hash = hash * a + (*str);  
		a    = a * b;  
	}  

	return hash;  
}  
/* End Of RS Hash Function */  


U32 JSHash(U8* str, U32 len)  
{  
	U32 hash = 1315423911;  
	U32 i    = 0;  

	for(i = 0; i < len; str++, i++)  
	{  
		hash ^= ((hash << 5) + (*str) + (hash >> 2));  
	}  

	return hash;  
}  
/* End Of JS Hash Function */  


U32 PJWHash(U8* str, U32 len)  
{  
	const U32 BitsInUnsignedInt = (U32)(sizeof(U32) * 8);  
	const U32 ThreeQuarters     = (U32)((BitsInUnsignedInt  * 3) / 4);  
	const U32 OneEighth         = (U32)(BitsInUnsignedInt / 8);  
	const U32 HighBits          = (U32)(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);  
	U32 hash              = 0;  
	U32 test              = 0;  
	U32 i                 = 0;  

	for(i = 0; i < len; str++, i++)  
	{  
		hash = (hash << OneEighth) + (*str);  

		if((test = hash & HighBits)  != 0)  
		{  
			hash = (( hash ^ (test >> ThreeQuarters)) & (~HighBits));  
		}  
	}  

	return hash;  
}  
/* End Of  P. J. Weinberger Hash Function */  


U32 ELFHash(U8* str, U32 len)  
{  
	U32 hash = 0;  
	U32 x    = 0;  
	U32 i    = 0;  

	for(i = 0; i < len; str++, i++)  
	{  
		hash = (hash << 4) + (*str);  
		if((x = hash & 0xF0000000L) != 0)  
		{  
			hash ^= (x >> 24);  
		}  
		hash &= ~x;  
	}  

	return hash;  
}  
/* End Of ELF Hash Function */  


U32 BKDRHash(U8* str, U32 len)  
{  
	U32 seed = 131; /* 31 131 1313 13131 131313 etc.. */  
	U32 hash = 0;  
	U32 i    = 0;  

	for(i = 0; i < len; str++, i++)  
	{  
		hash = (hash * seed) + (*str);  
	}  

	return hash;  
}  
/* End Of BKDR Hash Function */  


U32 SDBMHash(U8* str, U32 len)  
{  
	U32 hash = 0;  
	U32 i    = 0;  

	for(i = 0; i < len; str++, i++)  
	{  
		hash = (*str) + (hash << 6) + (hash << 16) - hash;  
	}  

	return hash;  
}  
/* End Of SDBM Hash Function */  


U32 DJBHash(U8* str, U32 len)  
{  
	U32 hash = 5381;  
	U32 i    = 0;  

	for(i = 0; i < len; str++, i++)  
	{  
		hash = ((hash << 5) + hash) + (*str);  
	}  

	return hash;  
}  
/* End Of DJB Hash Function */  


U32 DEKHash(U8* str, U32 len)  
{  
	U32 hash = len;  
	U32 i    = 0;  

	for(i = 0; i < len; str++, i++)  
	{  
		hash = ((hash << 5) ^ (hash >> 27)) ^ (*str);  
	}  
	return hash;  
}  
/* End Of DEK Hash Function */  


U32 BPHash(U8* str, U32 len)  
{  
	U32 hash = 0;  
	U32 i    = 0;  
	for(i = 0; i < len; str++, i++)  
	{  
		hash = hash << 7 ^ (*str);  
	}  

	return hash;  
}  
/* End Of BP Hash Function */  


U32 FNVHash(U8* str, U32 len)  
{  
	const U32 fnv_prime = 0x811C9DC5;  
	U32 hash      = 0;  
	U32 i         = 0;  

	for(i = 0; i < len; str++, i++)  
	{  
		hash *= fnv_prime;  
		hash ^= (*str);  
	}  

	return hash;  
}  
/* End Of FNV Hash Function */  

U32 APHash(U8* str, U32 len)  
{  
	U32 hash = 0xAAAAAAAA;  
	U32 i    = 0;  

	for(i = 0; i < len; str++, i++)  
	{  
		hash ^= ((i & 1) == 0) ? (  (hash <<  7) ^ (*str) * (hash >> 3)) :  
			(~((hash << 11) + ((*str) ^ (hash >> 5))));  
	}  

	return hash;  
}  

U32  hash_cal(hash_type_e hash,U8 *str,U32 len)
{
	U32 ret = 0;
	switch(hash)
	{
	case RS_HASH:
		ret = RSHash(str,len);
		break;
	case JS_HASH:
		ret = JSHash(str,len);
		break;
	case  PJW_HASH:
		ret = PJWHash(str,len);
		break;
	case ELF_HASH:
		ret = ELFHash(str,len);
		break;
	case  BKDR_HASH:
		ret = BKDRHash(str,len);
		break;
	case SDBM_HASH:
		ret = SDBMHash(str,len);
		break;
	case DJB_HASH:
		ret = DJBHash(str,len);
		break;
	case DEK_HASH:
		ret = DEKHash(str,len);
		break;
	case BP_HASH:
		ret = BPHash(str,len);
		break;
	case FNV_HASH:
		ret = FNVHash(str,len);
		break;
	case AP_HASH:
		ret = APHash(str,len);
		break;
	default:break;
	}	
	
	return ret;
}
/* End Of AP Hash Function */  