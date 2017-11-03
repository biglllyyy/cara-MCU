#ifndef		__CAN_CFG_H__
#define		__CAN_CFG_H__

#include "comm_typedef.h"
#include "hal_can.h"

#define    CAN_CHN			0
#define    ID_RECV_NUM_ALL	(11+2)
#define    ID_SENT_NUM_ALL  5
#define    ID_PROCESS_ALL	(ID_RECV_NUM_ALL + ID_SENT_NUM_ALL)

#define    CAN_CH1			1
#define    ID1_RECV_NUM_ALL	(8)
#define    ID1_SENT_NUM_ALL  2
#define    ID1_PROCESS_ALL	(ID1_RECV_NUM_ALL + ID1_SENT_NUM_ALL)
//#define    ID1_PROCESS_ALL	(ID1_RECV_NUM_ALL)



/*ip receive*/
#define    PC_T0_MCU_ID		  	 0x7E7

#define ID_REC_01_180689F4       0x180689F4
#define ID_REC_02_180789F4		 0X180789F4
#define ID_REC_03_181B80f4		 0X181B80f4
#define ID_REC_04_0C008980		 0X0C008980
#define ID_REC_05_0C018980		 0X0C018980
#define ID_REC_06_0C028980		 0X0C028980
#define ID_REC_07_0C068980		 0X0C068980
#define ID_REC_08_0C058980		 0X0C058980
#define ID_REC_09_0C048980		 0X0C048980
#define ID_REC_10_18XX89F4		 0X180089F4
#define ID_REC_11_610			 0X610




#define    UPDATA_ID			 0x140

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
