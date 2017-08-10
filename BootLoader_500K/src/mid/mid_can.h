#ifndef _MID_CAN_H__
	#define _MID_CAN_H__
#include "comm_typedef.h"


#define    CAN_CHN			0
//!<canÄ£Ê½Ñ¡Ôñ
typedef enum{
	NORMAL_MODE = 0,
	STANDBY_MODE = 1,
	GO_TO_SLEEP_MODE = 2,
}can_mode_t;


extern void mid_can_init(U8 chn);

#endif









