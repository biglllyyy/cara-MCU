#ifndef __ICR16_H__
#define __ICR16_H__
#include "comm_typedef.h"
#include "mid_filter.h"

#define  FREE_TIMER_CHN  0
#define  ICU_INPUT_CHN   1

extern void icu_cfg_init(void);

extern  U16   hal_icu_dim_lost_cnt;			/* 背光脉冲信号丢失处理 */
extern  avg_filter_t  dim_high_level;
extern  avg_filter_t  dim_low_level;


#endif
