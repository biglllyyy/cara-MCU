#ifndef _MID_TIME_H_
#define _MID_TIME_H_
#include "comm_typedef.h"

extern void	init_sys_time();
extern void	time_count(U16 message);
extern void	time_set(U8 hour,U8 min);
extern U16  time_get(void);

typedef  struct
{
    U8 valid;		/* ��ʱ���Ƿ���Ч */
    U8 timeout;	/* ��ʱʱ�䵽����ֹ��Ƭ����� */
    U32  curtime;	/* ��ȡ��ǰadd_timer ��ʱ��*/
} TIMER_TYPE;

/* ��ʱ��ģ�� */
void add_timer(TIMER_TYPE *timer);					/* ��Ӷ�ʱ�� */
void del_timer(TIMER_TYPE *timer);					/* ɾ����ʱ�� */
U8 is_timer_out(TIMER_TYPE  *timer,U16 period_ms);	/* ��ʱ���Ƿ�ʱ */
U8  is_timer_added(TIMER_TYPE  *timer);				/* ��ʱ���Ƿ��Ѿ���� */
void msleep(U32 ms);	/* ϵͳֹͣ����ms���� */

#endif
