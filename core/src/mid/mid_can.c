#include <string.h>
#include "mid_can.h"
#include "hal_wdg.h"
#include "g_variable.h"
#include "hal_gpio.h"
#ifdef  WIN32
#include "stdio.h"
#endif


pCanProcess  can_rx_pro =NULL;
/*
byte  =  0    1    2     3     4     5     6     7
start = 7~0,15~8,23~16,31~24,39~32,47~40,55~48,63~56
end   = 7~0,15~8,23~16,31~24,39~32,47~40,55~48,63~56
(start >= end) && ((start / 8) == (end / 8))
*/
/*
	the  following define macro used cont the bit in the can msg. for simply the operation to shift
*/

#define BITS_COUNT(start_bit, end_bit) ((start_bit) - (end_bit) + 1)

#define CAN_BYTE8(byte) ((U8)(msg->data[(byte)]))

/*实现一个byte内的任意数据长度数据获取*/
#define CAN_BITS8(start_bit, end_bit) \
	((U8)(((msg->data[(start_bit) / 8]) >> ((end_bit) % 8)) & BitMask8[(start_bit) - (end_bit)]))


#define CAN_BITS16(hi_start, hi_end, lo_start, lo_end) \
	(((U16)CAN_BITS8(hi_start, hi_end) << BITS_COUNT(lo_start, lo_end)) | \
	((U16)CAN_BITS8(lo_start, lo_end)))

#define CAN_BYTE16(hibyte, lobyte) \
	(((U16)CAN_BYTE8(hibyte) << 8) | CAN_BYTE8(lobyte))



#define    INTEL_FORMAT     0
#define    MOTO_FORMAT      1
#define  CAN_FRAME_FORMAT   INTEL_FORMAT

#if  CAN_FRAME_FORMAT == MOTO_FORMAT
static const U8 moto_format[64]=
{
    56,57,58,59,60,61,62,63,
    48,49,50,51,52,53,54,55,
    40,41,42,43,44,45,46,47,
    32,33,34,35,36,37,38,39,
    24,25,26,27,28,29,30,31,
    16,17,18,19,20,21,22,23,
    8,9,10,11,12,13,14,15,
    0,1,2,3,4,5,6,7
};
#endif

static U8 cntRecv, cntSend;
static can_msg_t *can_msg;
static U8 mPhyChip = 1;
/****************************************************/
/* motor format
  BIT7---------------------------------BIT0
  |<-MSB-------------------------------<-|
  BIT15--------------------------------BIT8
  |<---------------LSB|start
*/
/* interl format
BIT7---------------------------------BIT0
|->------------------------------>LSB--|start
BIT15--------------------------------BIT8
				    |->MSB----------->-|
*/
/*****************************************************/



U32 mid_can_get(can_msg_t *msg,U8 start_bit,U8 lenth)
{
    /*start bit convert */
    U32 ret = 0;				/* return value */
    U8  new_start = 0;			/* cal new order start bit */
    U8  data_bytes = 0;			/* cal signal bytes */
    U8  new_end = 0;			/* cal end bit  */
    U8  start_byte = 0;			/* cal start date bytes number */

#if  CAN_FRAME_FORMAT == MOTO_FORMAT
    new_start = moto_format[start_bit];  /* changed format, prevent cal end > start */
    new_end   = new_start + lenth -1;
    start_byte  = new_start>>3;
    data_bytes = (new_end>>3) - (new_start>>3) +1;

    if(0 == data_bytes)
    {

    }
    else if(1 == data_bytes)
    {
        ret = (U8)msg->data[7-start_byte];
        ret >>= (new_start&0x07);
        ret &=  BitMask32[new_end-new_start];
    }
    else if(2 == data_bytes)
    {
        ret |= (U32)msg->data[7-start_byte];
        ret |= (U32)msg->data[6-start_byte]<< 8;
        ret >>= (new_start&0x07);
        ret &= BitMask32[new_end-new_start];
    }
    else if(3 == data_bytes)
    {
        ret |=  msg->data[7-start_byte];
        ret |= (U32)msg->data[6-start_byte]<<8;
        ret |= (U32)msg->data[5-start_byte]<<16;
        ret >>= (new_start&0x07);
        ret &= BitMask32[new_end-new_start];
    }
    else if(4 == data_bytes)
    {
        ret |= (U32)msg->data[7-start_byte];
        ret |= (U32)msg->data[4-start_byte]<<24;
        ret |= (U32)msg->data[5-start_byte]<<16;
        ret |= (U32)msg->data[6-start_byte]<<8;
        ret >>= (new_start&0x07);
        ret &= BitMask32[new_end-new_start];
    }
    else
    {

    }
#else
    new_start = start_bit;
    new_end = start_bit + lenth -1;
    start_byte  = start_bit>>3;
    data_bytes = (new_end>>3) - start_byte +1;
    if(0 == data_bytes)
    {
        DBG("input para err\n",0);
    }
    else if(1 == data_bytes)
    {
        ret = (U8)msg->data[start_byte];
        ret >>= (new_start&0x07);
        ret &=  BitMask32[lenth-1];
    }
    else if(2 == data_bytes)
    {
        ret |= (U32)msg->data[start_byte];
        ret |= (U32)msg->data[start_byte+1]<< 8;
        ret >>=(new_start&0x07);
        ret &= BitMask32[lenth-1];
    }
    else if(3 == data_bytes)
    {
        ret |=  msg->data[start_byte];
        ret |= (U32)msg->data[start_byte+1]<<8;
        ret |= (U32)msg->data[start_byte+2]<<16;
        ret >>= (new_start&0x07);
        ret &= BitMask32[lenth-1];
    }
    else if(4 == data_bytes)
    {
        ret |= (U32)msg->data[start_byte];
        ret |= (U32)msg->data[start_byte+3]<<24;
        ret |= (U32)msg->data[start_byte+2]<<16;
        ret |= (U32)msg->data[start_byte+1]<<8;
        ret >>= (new_start&0x07);
        ret &= BitMask32[lenth-1];
    }
    else
    {
        DBG("can data lenth too long \n",0);
    }
#endif

    return ret;
}

void mid_can_prepare(U8 nRecv, U8 nSend,can_msg_t*msg)
{

	cntRecv = nRecv;
	cntSend = nSend;
	can_msg=msg;
	hal_can_prepare(nRecv,nSend,msg);
	wdg_feed();
}
void mid_can_init(U8 chn, U8 chip)
{
	mPhyChip = chip;
	hal_can_init(chn);
    wdg_feed();
}



/* signal lost task */
void mid_can_get_task10(void)
{
    U8		index = 0;
    U32		nMob = 0;
	static  U16  count = 0;
	static  U16     max_count = 0;
	
	can_msg_t can_msg_temp ={0};

    do{
    	if(can_rx_pro == NULL)//如果函数指针为空
    	break;
    	for(index =0; index < cntRecv; index++)
    	{
        	can_msg[index].count++;
					if((can_msg[index].count*10) >= can_msg[index].period_ms)
					{	
						can_rx_pro(&can_msg[index],CAN_LOST);
						can_msg[index].lost = 1;
						can_msg[index].count = 0;
						can_msg[index].new_frame = 0;
						
					}
					if(can_msg[index].lost)
					{
						can_lost.id_lost_cnt++;
					}
    	}
    	if(can_lost.id_lost_cnt >= cntRecv)
    	{
        	can_lost.all_lost_flg = 1;
    	}
    	else
    	{
        	can_lost.all_lost_flg = 0;
    	}
   	  can_lost.id_lost_cnt = 0;
    
    
    }while(0);
    do
	{
		if(hal_canerror_statecheck() == CAN_BOFF)   
		{
			break;
		}
		nMob = HalChkMob();
		if(0 == nMob)
		{
			count++;
			if(max_count < count)
			{
				max_count = count;
			}
			break;
		}
		count = 0;
		for (index = 0; index < cntRecv; index ++)				    /* receive buffer frome buffer3 */
		{
			if (TRUE == ((nMob >> index) & 0x01))							/* 检测哪一个报文缓存器中存在信息*/
			{
				can_msg_temp.buffer_num = index+1;
				if(TRUE == hal_can_get(&can_msg_temp))					/* 将接收数据放在tCanMsg结构里面,buffer start form 1*/
				{
					can_msg[index].new_frame = 1;
					can_msg[index].lost = 0;
					can_msg[index].count = 0;
					//can_msg_temp.id = can_msg[index].id;
					can_msg_temp.dlc = can_msg[index].dlc;
					if(can_rx_pro != NULL)
					{
						can_rx_pro(&can_msg_temp,CAN_PARSE);
					}
				}
			}
		}
	}while(0);	
}


/* id lost or no check */
U8  mid_can_lost_sts_get(U8 id)
{
    U8 ret = 0;
    if(id < cntRecv)
    {
        ret = can_msg[id].lost;
    }
    return(ret);

}


#ifdef WIN32
void mid_can_get_test(void)
{
    U32 start_bit;
    U32 len;
    U32 ret = 0;
    U8 i = 0;
    while(1)
    {
        printf("cna_msg:");
        for(i =0; i<8; i++)
        {
            printf("%2x ",can_msg[0].data[i]);
        }
        printf("\n");
        printf("start_bit :");
        scanf_s("%d",&start_bit);
        printf("len:");
        scanf_s("%d",&len);
        ret = mid_can_get(&can_msg[0],start_bit,len);
        printf("%x\n",ret);
    }
}
#endif

void can_chip_mode_switch(io_type_t *io, io_type_t *en, can_mode_t mode)
{	
	switch(mode)
	{
		case NORMAL_MODE:
			if (mPhyChip == TJA1042T)
				mid_set_io_sts(io,OFF);
			else if (mPhyChip == TJA1043)
			{
				mid_set_io_sts(io,ON);
				mid_set_io_sts(en,ON);
			}
			break;
		case STANDBY_MODE:
			if (mPhyChip == TJA1042T)
				mid_set_io_sts(io,ON);
			else if (mPhyChip == TJA1043)
			{
				mid_set_io_sts(io,OFF);
				mid_set_io_sts(en,OFF);
			}
			break;
		case GO_TO_SLEEP_MODE:
			if (mPhyChip == TJA1042T)
				mid_set_io_sts(io,ON);
			else if (mPhyChip == TJA1043)
			{
				mid_set_io_sts(en,OFF);
				mid_set_io_sts(io,ON);
			}
			break;
		default:break;
	}
}
