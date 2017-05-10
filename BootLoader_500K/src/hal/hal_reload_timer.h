/*
 * hal_ReloadTimer.h
 *
 *  Created on: 2014-9-23
 *      Author: lipengfei
 */

#ifndef HAL_RELOADTIMER_H_
#define HAL_RELOADTIMER_H_

#include "comm_typedef.h"

#define RLDTR_TimeTick	(10000)	//10000us -->10ms

#define PIRORITY_RLDTR_TimeTick	(6 + 16)	//16 (highest) ~30 (lowest), disable: 31

#define TICK_CHN	   (0)
#define TICK_US		   (10000)					/* 10ms */

#define TickInit()   hal_reloadtimer_init((U8)TICK_CHN,(U16)TICK_US)
#define TickStart()  hal_reloadtimer_start((U8)TICK_CHN)

#define GetTick()    hal_timer_get_tick()

#define T_BASE	(TICK_US/1000)
#define T_10MS	(10/T_BASE)
#define T_100MS	(100/T_BASE)
#define T_1S 	(1000/T_BASE)

extern  U32 hal_timer_passed(U32 since);
extern void hal_reloadtimer_init(U8 u8channel, U16 u16microsecond);
extern void hal_reloadtimer_start(U8 u8channel);
extern void hal_reloadtimer_stop(U8 u8channel);
extern void hal_set_Waning(U8 u8WarnType);
extern U32 	hal_timer_get_tick(void);

void hal_set_Waning(U8 u8WarnType);

#endif /* HAL_RELOADTIMER_H_ */
