#ifndef __MID_SWITCH_H_
#define __MID_SWITCH_H_
#include  "comm_typedef.h"
#include "mid_pin.h"
#include "mid_filter.h"

extern void mid_switch_init(U8 pin_in_num);
extern void mid_switch_task10(io_type_t *pin_io_in, pin_filter_t*pin_filter_in);
extern void mid_set_all_pin_highz(void);
extern U8   mid_get_io_filtered(pin_filter_t *pin_filter_in, U8 name);

#endif      /* __MID_SWITCH_H_ */
