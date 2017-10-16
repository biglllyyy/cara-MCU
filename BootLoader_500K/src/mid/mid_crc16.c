#include  "comm_typedef.h"
#include  "hal_wdg.h"

const U16 crc16_half_byte[16]= {	0x0000, 0x1021,0x2042,0x3063,
                                    0x4084, 0x50a5, 0x60c6,0x70e7,
                                    0x8108, 0x9129, 0xa14a,0xb16b,
                                    0xc18c, 0xd1ad, 0xe1ce,0xf1ef
                               };
U16  api_cal_crc16(U8 *ptr,U32 len)
{
    U8		da = 0;
    U16     crc = 0;
    while(len-- != 0)
    {
        da = ((U8)(crc/256))/16;
        crc <<= 4;
        crc ^= crc16_half_byte[da^(*ptr/16)];
        da = ((U8)(crc/256)/16);
        crc <<= 4;
        crc ^= crc16_half_byte[da^(*ptr&0x0f)];
        ptr++;
		wdg_feed();  //MCU bin 256K Bytes 
    }
    return(crc);
}
