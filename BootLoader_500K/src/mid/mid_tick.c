#include "mid_tick.h"


void mid_System_Tick_Init(void)
{
	TickInit();
	TickStart();	
}


unsigned int mid_System_Tick_Now(void)
{
	return GetTick();
}

unsigned int mid_System_Tick_Passed(unsigned int tick_now)
{
	return hal_timer_passed(tick_now);
}








