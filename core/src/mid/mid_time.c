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

/*��ʼ��ϵͳʱ��*/
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

/*ϵͳ�ۼ�ʱ��,need run by system every 1S*/

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

/*ʱ���ȡ����*/
U16  time_get(void)
{
    U16 systime;
    systime = (U16)sys_time.hour*100 + sys_time.minute; /*����λ��ʾСʱ������λ��ʾ����*/
    return systime;
}
/**************************************/
/* ��ʱ��ģ�飬�ṩ���½ӿ�
����ģ�飬GetTick()
*/
/*
1����Ӷ�ʱ��					add_timer(TIMER_TYPE,period)
2���Ƴ���ʱ��					del_timer(TIMER_TYPE)
3����ѯ��ʱ���Ƿ�ʱ		BOOL	is_timer_out(TIMER_TYPE_type)
4����ѯ��ʱ���Ƿ��Ѿ���ӡ�BOOL	is_added(TIMER_TYPE_type)
*/


/* ��Ӷ�ʱ�� */
void add_timer(TIMER_TYPE * timer)
{
    timer->valid = TRUE;
    timer->timeout = FALSE;
    timer->curtime = hal_timer_get_tick();
}

/* ɾ����ʱ�� */
void del_timer(TIMER_TYPE * timer)
{
    timer->valid = FALSE;
    timer->timeout = FALSE;
    timer->curtime = 0;
}

/* ��ʱ����ʱ */
U8 is_timer_out(TIMER_TYPE * timer, U16 period_ms)
{
    if(timer->valid)
    {
        period_ms /= 10;

        if(hal_timer_passed(timer->curtime) >= period_ms)
        {
            timer->timeout = TRUE; /* ��鵽ʱ�䵽�󣬳�ʱ��λ����ʹ��ʱ��һȦ����ʱ��־��Ȼ���� */
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

/* ��ʱ���Ƿ��Ѿ���� */
U8 is_timer_added(TIMER_TYPE  *timer)
{
    return (timer->valid);
}

/* ϵͳֹͣ����ms */
void msleep(U32 ms)
{
	U32 tk;
	tk = hal_timer_get_tick();
	
	while (hal_timer_passed(tk) < ms/10)
		wdg_feed();
}
