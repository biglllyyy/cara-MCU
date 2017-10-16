#include "mid_time.h"
#include "mid_scheduler.h"
#include "hal_reload_timer.h"


typedef struct tm  utc_time;

typedef  struct
{
    U8  second;
    U8  minute;
    U8  hour;
} TIME_TYPE_S;

static TIME_TYPE_S  sys_time;

/*初始化系统时间*/
void init_sys_time()
{
    sys_time.minute = 0;
    sys_time.second	= 0;
    sys_time.hour = 12;
}
/*set the time*/
void time_set(U8 hour,U8 min)
{
    sys_time.hour = hour;
    sys_time.minute = min;
    sys_time.second = 0;
}

/*系统累计时间,need run by system every 1S*/

void time_count(U16 message)
{
    sys_time.second++;
    if(sys_time.second>=60)
    {
        sys_time.second = 0;
        sys_time.minute++;
        if(sys_time.minute>=60)
        {
            sys_time.minute = 0;
            sys_time.hour++;
            if(sys_time.hour>=24)
            {
                sys_time.hour = 0;
            }
        }
    }
}

/*时间获取函数*/
U16  time_get(void)
{
    U16 systime;
    systime = (U16)sys_time.hour*100 + sys_time.minute; /*高两位表示小时，低两位表示分钟*/
    return systime;
}
/**************************************/
/* 定时器模块，提供以下接口
依赖模块，GetTick()
*/
/*
1、添加定时器					add_timer(TIMER_TYPE,period)
2、移除定时器					del_timer(TIMER_TYPE)
3、查询定时器是否到时		BOOL	is_timer_out(TIMER_TYPE_type)
4、查询定时器是否已经添加。BOOL	is_added(TIMER_TYPE_type)
*/


/* 添加定时器 */
void add_timer(TIMER_TYPE * timer)
{
    timer->valid = TRUE;
    timer->timeout = FALSE;
    timer->curtime = hal_timer_get_tick();
}

/* 删除定时器 */
void del_timer(TIMER_TYPE * timer)
{
    timer->valid = FALSE;
    timer->timeout = FALSE;
    timer->curtime = 0;
}

/* 定时器到时 */
U8 is_timer_out(TIMER_TYPE * timer, U16 period_ms)
{
    if(timer->valid)
    {
        period_ms /= 10;

        if(hal_timer_passed(timer->curtime) >= period_ms)
        {
            timer->timeout = TRUE; /* 检查到时间到后，超时置位，即使定时跑一圈，超时标志依然存在 */
        }
        else
        {
            timer->timeout = FALSE;
        }
    }
    else
    {
        timer->timeout = FALSE;
    }
    return timer->timeout;
}

/* 定时器是否已经添加 */
U8 is_timer_added(TIMER_TYPE  *timer)
{
    return (timer->valid);
}

/* 系统停止运行ms */
void msleep(U32 ms)
{
	U32 tk;
	tk = hal_timer_get_tick();
	
	while (hal_timer_passed(tk) < ms/10)
		wdg_feed();
}
