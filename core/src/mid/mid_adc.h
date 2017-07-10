#ifndef _MID_ADC_H
#define _MID_ADC_H

#include "comm_typedef.h"

void mid_adc_init(void);
void mid_adc_start(U8  signal_chn);
void mid_adc_stop(U8  signal_chn);
void mid_adc_manager_task(void);
U16  mid_adc_get(U8 single_chn);

#endif
