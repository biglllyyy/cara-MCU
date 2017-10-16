#ifndef _MID_CAN_H__
	#define _MID_CAN_H__
#include "comm_typedef.h"
#include "hal_can.h"


#define    CAN_CHN			0
//!<canÄ£Ê½Ñ¡Ôñ
typedef enum{
	NORMAL_MODE = 0,
	STANDBY_MODE = 1,
	GO_TO_SLEEP_MODE = 2,
}can_mode_t;


void mid_can_init(U8 chn,pCanAnalyse can_rx_handle);


#endif









