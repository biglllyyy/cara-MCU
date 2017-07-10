#include "mid_trigger.h"
/*
触发器思路：
1、条件判断。
2、状态比较（与上次条件判断结果比较）。
3、动作执行。
*/
typedef  struct
{
    U16 state;
    U16 (*handle)(U16 action);
} trigger;

/* 开关触发器：state!=0为真，state==0为假 */
/* state由假到真则触发ACT_TRUE，由真到假则触发ACT_FALSE。 */
void touch_switch_trigger(trigger_handle handle, U16 *state, U16 new_state)
{
    new_state = new_state ? ACT_TRUE : ACT_FALSE;		/*notice this ,not 0 ,treat as the same value*/
    if(*state != new_state)
    {
        *state = new_state;
        handle(*state);
    }
}

/* 状态触发器：state!=0为真，state==0为假 */
/* state改变后为真则触发ACT_TRUE，改变后为假则触发ACT_FALSE。 */
void touch_state_trigger(trigger_handle handle, U16 *state, U16 new_state)
{
    if(*state != new_state)
    {
        *state = new_state;
        handle(*state ? ACT_TRUE : ACT_FALSE);
    }
}

/* 增大触发器：state!=0为真，state==0为假 */
/* state增大则触发ACT_TRUE，减小至零则触发ACT_FALSE。 */
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

/*状态触发器，返回值为真则需要触发，返回值为假，不需要触发*/
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
/*状态触发器，返回值为真则需要触发，返回值为假，不需要触发*/
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

/*状态触发器，返回值为真则需要触发，返回值为假，不需要触发*/
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
/*状态触发器，返回值为真则需要触发，返回值为假，不需要触发*/
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

/*当一个字中的任何一位从0跳变到1时，触发动作，返回值为TRUE,不触发新动作，返回值为FALSE*/
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
