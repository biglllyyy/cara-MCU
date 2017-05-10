#ifndef _MID_TRIGGER_H_
#define _MID_TRIGGER_H_

#include "comm_typedef.h"
typedef void (* trigger_handle)(U16 action);

typedef enum  
{
	NO_TRIGGER = 0,
	UP_TRIGGER = 1,
	DOWN_TRIGGER = 2
}trigger_t;


#define touch_trigger(handle)	(handle)(ACT_TOUCH)
#define reset_trigger(handle)	(handle)(ACT_RESET)
#define enable_trigger(handle)	handle(ACT_ENABLE)
#define disable_trigger(handle) handle(ACT_DISABLE)

extern void touch_switch_trigger(trigger_handle handle, U16 *state, U16 new_state);
extern void touch_state_trigger(trigger_handle handle, U16 *state, U16 new_state);
extern void touch_increase_trigger(trigger_handle handle, U16 *state, U16 new_state);
extern U8	is_trigger(U16 *old_state, U16 new_state);
extern U8	is_up_trigger(U16 *old_state, U16 new_state);
extern U8	is_down_trigger(U16 *old_state, U16 new_state);
extern trigger_t is_trigger8(U8 *old_state, U8 new_state);
extern U8 bit_ture_tiegger(U32 *old_state, U32 new_state);

#endif
