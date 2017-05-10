#ifndef		__CAN_CFG_H__
#define		__CAN_CFG_H__

#include "comm_typedef.h"
#include "hal_can.h"

#define    CAN_CHN			0
#define    ID_RECV_NUM_ALL	22
#define    ID_SENT_NUM_ALL  4
#define    ID_PROCESS_ALL	(ID_RECV_NUM_ALL + ID_SENT_NUM_ALL)

/*ip receive*/
#define    PC_T0_MCU_ID		  	0x7E7
#define    ID_REC_1_265			0x265
#define    ID_REC_2_111			0x111
#define    ID_REC_3_271			0x271
#define    ID_REC_4_131			0x131
#define    ID_REC_5_095			0x095
#define    ID_REC_6_311			0x311
#define    ID_REC_7_371			0x371
#define    ID_REC_8_351			0x351
#define    ID_REC_9_3DF         0x3df
#define    ID_REC_10_345		0x345
#define    ID_REC_11_221		0x221
#define    ID_REC_12_7A6		0x7A6
#define    ID_REC_13_251 		0x251
#define	   ID_REC_14_116		0x116
#define    ID_REC_15_211		0x211
#define    ID_REC_16_242		0x242
#define    ID_REC_17_0A5		0x0A5
#define    ID_REC_18_766		0x766
#define    ID_REC_19_365		0x365
#define    ID_REC_20_08B		0x08B
#define    ID_REC_21_485		0x485



/* ip sent */
#define      MCU_TO_PC_ID		0x7EF 
#define 	 ID_SEND_1_345		0x345
#define		 ID_SEND_2_766		0x766
#define		 ID_SEND_3_7A6		0x7A6


#endif
