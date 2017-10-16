#ifndef __MID_DATA_STORE_H__
#define __MID_DATA_STORE_H__

#include "mid_flash.h"
#include "aes.h"

char *mid_aes_decrypt(char *src);
U8 mid_save_data(U8 *buf, short len);
U8 mid_load_data(U8 *buf, short len);

#endif
