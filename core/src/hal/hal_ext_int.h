#ifndef __HAL_EXT_INT_H__
#define __HAL_EXT_INT_H__

#include "comm_typedef.h"


#define  RADR_INPUT_CHN		(1u)

#define  IGN_WAKE_INT_CHN   13
#define  CAN_WAKE_INT_CHN		0
#define  LIN_WAKE_INT_CHN   5   

typedef enum
{
    LOW_LEVEL = 0,
    HIGH_LEVEL = 1,
    RISING_EDGE = 2,
    FALLING_EDGE = 3,
} INT_TYPE_e;

extern	U16  radr_data;

extern void hal_extern_interrupt_init(U8 chn,INT_TYPE_e type);
extern void hal_ext_int_start(U8 chn);
extern void hal_ext_int_stop(U8 chn);
extern void radr_data_init(void);
#endif
