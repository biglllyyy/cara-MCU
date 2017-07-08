#ifndef		__CAN_CFG_H__
#define		__CAN_CFG_H__

#include "comm_typedef.h"
#include "hal_can.h"

#define    CAN_CHN			0
#define    ID_RECV_NUM_ALL	19
#define    ID_SENT_NUM_ALL  5
#define    ID_PROCESS_ALL	(ID_RECV_NUM_ALL + ID_SENT_NUM_ALL)

/*ip receive*/
#define    PC_T0_MCU_ID		  	0x7E7
#define    ID_REC_1_18AA28F3    0x18AA28F3
#define    ID_REC_2_1818D0F3    0x1818D0F3
#define    ID_REC_3_1819D0F3    0x1819D0F3
#define    ID_REC_4_181AD0F3    0x181AD0F3
#define    ID_REC_5_181BD0F3    0x181BD0F3
#define    ID_REC_6_181CD0F3    0x181CD0F3
#define    ID_REC_7_181DD0F3    0x181DD0F3
#define    ID_REC_8_181ED0F3    0x181ED0F3
#define    ID_REC_9_181FD0F3    0x181FD0F3
#define    ID_REC_10_180028F3    0x180028F3
#define    ID_REC_11_180028F4    0x180028F4
#define    ID_REC_12_1002FF1E    0x1002FF1E
#define    ID_REC_13_1003FF1E    0x1003FF1E
#define    ID_REC_14_1004FF1E    0x1004FF1E
#define    ID_REC_15_1429289B    0x1429289B
#define    ID_REC_16_142A289B    0x142A289B
#define    ID_REC_17_1828272B    0x1828272B
#define    ID_REC_18_142A2895    0x142A2895




/* ip sent */
#define    MCU_TO_PC_ID			 0x7EF
#define    ID_SEND_1_1800F328    0x1800F328
#define    ID_SEND_2_1882BBAB    0x1882BBAB
#define    ID_SEND_3_18AB9B28    0x18AB9B28
#define    ID_SEND_4_18FFA017    0x18FFA017


#endif
