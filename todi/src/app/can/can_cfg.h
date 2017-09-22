#ifndef		__CAN_CFG_H__
#define		__CAN_CFG_H__

#include "comm_typedef.h"
#include "hal_can.h"

#define    CAN_CHN			0
#define    ID_RECV_NUM_ALL	(20+2)
#define    ID_SENT_NUM_ALL  5
#define    ID_PROCESS_ALL	(ID_RECV_NUM_ALL + ID_SENT_NUM_ALL)

/*ip receive*/
#define    PC_T0_MCU_ID		  	 0x7E7
#define ID_REC_01_0C03A1A7       0x0C03A1A7
#define ID_REC_02_0C04A1A7       0x0C04A1A7
#define ID_REC_03_0C05A1A7       0x0C05A1A7
#define ID_REC_04_0C06A1A7       0x0C06A1A7
#define ID_REC_05_0C07A1A7       0x0C07A1A7
#define ID_REC_06_0C08A1A7       0x0C08A1A7
#define ID_REC_07_1818D0F3       0x1818D0F3
#define ID_REC_08_1819D0F3       0x1819D0F3
#define ID_REC_09_181AD0F3       0x181AD0F3
#define ID_REC_10_180028F3       0x180028F3
#define ID_REC_11_180128F3       0x180128F3
#define ID_REC_12_180228F3       0x180228F3
#define ID_REC_13_180328F3       0x180328F3
#define ID_REC_14_180428F3       0x180428F3
#define ID_REC_15_180028F4       0x180028F4
#define ID_REC_16_180128F4       0x180128F4
#define ID_REC_17_180228F4       0x180228F4
#define ID_REC_18_0C09A79B       0x0C09A79B
#define ID_REC_19_18FFC09E       0x18FFC09E
#define ID_REC_20_18FECA00       0x18FECA00

#define    UPDATA_ID			 0x120

#define ID_REC_01_0C03A1A7       0x0C03A1A7

/* ip sent */

#define    MCU_TO_PC_ID			 0x7EF                  
#define    ID_SEND_1_0C19A7A1    0x0C19A7A1   
#define    ID_SEND_2_0C1AA7A1    0x0C1AA7A1	  
#define    ID_SEND_3_18FED917    0x18FED917	  
#define    ACK_UPDATA_ID         (UPDATA_ID|0x01)	  
  
#define ID_REC_01_68X       0x680
#define ID_REC_02_67X       0x670
#define ID_REC_03_62X       0x620
#define ID_REC_04_63X       0x630
#define ID_REC_05_64X       0x640
#define ID_REC_06_65X       0x650
#define ID_REC_07_56X       0x560
#define ID_REC_08_45X       0x450


#define BCAN_ID_SEND_6A4         (0x6A4)
#define BCAN_ID_SEND_454         (0x454)


#endif
