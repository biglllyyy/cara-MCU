#include "mid_trigger.h"
/*
������˼·��
1�������жϡ�
2��״̬�Ƚϣ����ϴ������жϽ���Ƚϣ���
3������ִ�С�
*/
typedef  struct
{
    U16 state;
    U16 (*handle)(U16 action);
} trigger;

/* ���ش�������state!=0Ϊ�棬state==0Ϊ�� */
/* state�ɼٵ����򴥷�ACT_TRUE�����浽���򴥷�ACT_FALSE�� */
void touch_switch_trigger(trigger_handle handle, U16 *state, U16 new_state)
{
    new_state = new_state ? ACT_TRUE : ACT_FALSE;		/*notice this ,not 0 ,treat as the same value*/
    if(*state != new_state)
    {
        *state = new_state;
        handle(*state);
    }
}

/* ״̬��������state!=0Ϊ�棬state==0Ϊ�� */
/* state�ı��Ϊ���򴥷�ACT_TRUE���ı��Ϊ���򴥷�ACT_FALSE�� */
void touch_state_trigger(trigger_handle handle, U16 *state, U16 new_state)
{
    if(*state != new_state)
    {
        *state = new_state;
        handle(*state ? ACT_TRUE : ACT_FALSE);
    }
}

/* ���󴥷�����state!=0Ϊ�棬state==0Ϊ�� */
/* state�����򴥷�ACT_TRUE����С�����򴥷�ACT_FALSE�� */
void touch_increase_trigger(trigger_handle handle, U16 *state, U16 new_state)
{
    if(*state < new_state)
    {
        handle(ACT_TRUE);
    }
    else if((*state > 0) && (new_state == 0))
    {
        handle(ACT_FALSE);
    }
    else {}

    *state = new_state;
}

/*״̬������������ֵΪ������Ҫ����������ֵΪ�٣�����Ҫ����*/
U8 is_trigger16(U16 *old_state, U16 new_state)
{
    U8 ret = 0;
    if(*old_state != new_state)
    {
        *old_state = new_state;
        ret = 1;
    }
    return ret;
}
/*״̬������������ֵΪ������Ҫ����������ֵΪ�٣�����Ҫ����*/
trigger_t is_trigger8(U8 *old_state, U8 new_state)
{
	trigger_t ret = NO_TRIGGER;
	if(*old_state != new_state)
	{
		if(new_state)
		{
			ret = UP_TRIGGER;
		}
		else
		{
			ret = DOWN_TRIGGER;
		}
		*old_state = new_state;
	}
	else
	{
		ret = NO_TRIGGER;
	}
	return ret;
}

/*״̬������������ֵΪ������Ҫ����������ֵΪ�٣�����Ҫ����*/
U8 is_up_trigger(U16 *old_state, U16 new_state)
{
    U8 ret = 0;
    if(new_state > *old_state)
    {
        *old_state = new_state;
        ret = 1;
    }
    else
    {
        *old_state = new_state;
    }
    return ret;
}
/*״̬������������ֵΪ������Ҫ����������ֵΪ�٣�����Ҫ����*/
U8 is_down_trigger(U16 *old_state, U16 new_state)
{
    U8 ret = 0;
    if(*old_state > new_state)
    {
        *old_state = new_state;
        ret = 1;
    }
    else
    {
        *old_state = new_state;
    }
    return ret;
}

/*��һ�����е��κ�һλ��0���䵽1ʱ����������������ֵΪTRUE,�������¶���������ֵΪFALSE*/
U8  bit_ture_tiegger(U32 *old_state, U32 new_state)
{
    U8	i=0;
    U32 move=0x01;
    U8  ret = 0;
    if(*old_state != new_state)
    {
        for(i=0; i<32; i++)
        {           
			if((*old_state & move) < (new_state & move))
            {
                *old_state = new_state;
                ret = 1;
                break;
            }
            else if((*old_state & move) > (new_state & move))
           	{
           		*old_state &= (~move); 
				ret = 2;
				break;
           	}
			else
			{
				move <<= 1;
			}
        }
    }
    else
    {
    	*old_state = new_state;
    }
    return ret;
}
