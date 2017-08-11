#ifndef APP_CAN_H__
	#define APP_CAN_H__
#include "comm_typedef.h"
#include "hal_can.h"
#include "boot.h"
#include "protocol.h"

#define BOOT_ADDR		(CAN_TX_ID)

#define    ID_RECV_NUM_ALL	(1)		//!<定义接受节点数量
#define    ID_SENT_NUM_ALL  (5)			//!<定义发送节点数量
#define    ID_PROCESS_ALL	(ID_RECV_NUM_ALL + ID_SENT_NUM_ALL)
#define    PERIOD_MS(time)  (time/10)

#define    CHECK_ID			(BOOT_ADDR + OnlineCheckID)
#define    Erase_ID			(BOOT_ADDR + EraseFlashID)
#define    Info_ID          (BOOT_ADDR + BlockWriteInfoID)
#define    Write_ID         (BOOT_ADDR + WriteBlockFlashID)

extern can_msg_t can_msg[ID_PROCESS_ALL];
void app_init_can(void);
U32 IsCanEmpty(void);
U32 ReadCanData(PROTOCOL_MSG *msg);
U32 SendCanData(PROTOCOL_MSG *msg);



#endif 
