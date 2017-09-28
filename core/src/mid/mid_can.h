#ifndef __MID_CAN_H__
#define __MID_CAN_H__
#include "comm_typedef.h"
#include "hal_can.h"
#include "mid_pin.h"

#define    TJA1042T		0
#define    TJA1043		1

typedef enum{
	CAN_PARSE = 0,
	CAN_LOST = 1
}can_pro_way_e;

typedef enum{
	NORMAL_MODE = 0,
	STANDBY_MODE = 1,
	GO_TO_SLEEP_MODE = 2,
}can_mode_t;

/* ------ Public data type declarations ----------------------------*/

typedef void (* pCanProcess)(can_msg_t *msg,can_pro_way_e way);

extern void mid_can_prepare(U8 nRecv, U8 nSend,can_msg_t*msg);
extern void mid_can_init(U8 chn, U8 chip);
extern  U8  mid_can_lost_sts_get(U8 id);
extern  U32 mid_can_get(can_msg_t *msg,U8 start_bit,U8 lenth);
extern void mid_can_get_task10(void);
extern void mid_can1_get_task10(void);

extern void mid_can_get_test(void);
extern void can_chip_mode_switch(io_type_t *io, io_type_t *en, can_mode_t mode);

extern pCanProcess can_rx_pro;
extern pCanProcess  can1_rx_pro;

#endif
