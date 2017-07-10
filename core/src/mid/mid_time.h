#ifndef _MID_TIME_H_
#define _MID_TIME_H_
#include "comm_typedef.h"

extern void	init_sys_time();
extern void	time_count(U16 message);
extern void	time_set(U8 hour,U8 min);
extern U16  time_get(void);

typedef  struct
{
    U8 valid;		/* 定时器是否有效 */
    U8 timeout;	/* 计时时间到，防止单片机溢出 */
    U32  curtime;	/* 获取当前add_timer 的时间*/
} TIMER_TYPE;

/* 定时器模块 */
void add_timer(TIMER_TYPE *timer);					/* 添加定时器 */
void del_timer(TIMER_TYPE *timer);					/* 删除定时器 */
U8 is_timer_out(TIMER_TYPE  *timer,U16 period_ms);	/* 定时器是否到时 */
U8  is_timer_added(TIMER_TYPE  *timer);				/* 定时器是否已经添加 */
void msleep(U32 ms);	/* 系统停止运行ms毫秒 */

#endif
