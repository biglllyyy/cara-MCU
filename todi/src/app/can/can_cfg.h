#ifndef		__CAN_CFG_H__
#define		__CAN_CFG_H__

#include "comm_typedef.h"
#include "hal_can.h"

#define    CAN_CHN			0
#define    ID_RECV_NUM_ALL	18
#define    ID_SENT_NUM_ALL  3
#define    ID_PROCESS_ALL	(ID_RECV_NUM_ALL + ID_SENT_NUM_ALL)

/*ip receive*/
#define    PC_T0_MCU_ID		  	0x7E7
#define    ID_REC_01_100017EF    0x100017EF
#define    ID_REC_02_1801FBEF    0x1801FBEF
#define    ID_REC_03_1811FBEF    0x1811FBEF
#define    ID_REC_04_18FF08F2    0x18FF08F2
#define    ID_REC_05_10F8159E    0x10F8159E
#define    ID_REC_06_10F8169E    0x10F8169E
#define    ID_REC_07_18F8179E    0x18F8179E
#define    ID_REC_08_18F8189E    0x18F8189E
#define    ID_REC_09_18F8199E    0x18F8199E
#define    ID_REC_10_18F81A9E    0x18F81A9E
#define    ID_REC_11_18FF12F7    0x18FF12F7
#define    ID_REC_12_18FF0AF8    0x18FF0AF8
#define    ID_REC_13_18FF0BF8    0x18FF0BF8
#define    ID_REC_14_18FF0CF9    0x18FF0CF9
#define    ID_REC_15_18FF0DF9    0x18FF0DF9
#define    ID_REC_16_1801EFA9    0x1801EFA9
#define    ID_REC_17_104C1000    0x104C1000




/* ip sent */
#define    MCU_TO_PC_ID			 0x7EF
#define    ID_SEND_1_1801EF17    0x1801EF17    //100ms
#define    ID_SEND_2_1802EF17    0x1802EF17	  //500ms


#endif
