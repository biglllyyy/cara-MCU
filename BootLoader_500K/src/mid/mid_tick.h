#ifndef __MID_TICK_H__
#define __MID_TICK_H__

#include "hal_reload_timer.h"


void mid_System_Tick_Init(void);
unsigned int mid_System_Tick_Now(void);
unsigned int mid_System_Tick_Passed(unsigned int tick_now);

#endif
