
#ifndef __FREERUNTIMER_H__
#define __FREERUNTIMER_H__

#include "comm_typedef.h"
#include "hal_clock.h"

#define FRT_SUPPLYCLK_PCLK	PCLK			//Supply Clock for this module - 40Mhz
#define FRT_FRE_DIV			3				//range 0-8
#define FRT_OPERCLK_0		156250			//Operation Clock for this module - 156.25khz
#define FRT_RLDVALUE_0		16777215		//2^24 - 1
#define FRT_RLDVALUE_1		65535           //2^16 -1
#define FRT_RLDVALUE_2		32767           //2^16 -1
#define FRT_RLDVALUE_3		(9999)          //1ms
#define FRT_RLDVALUE_4		(99999)         //10ms
#define FRT_RLDVALUE_5		(999999)        //100ms
#define FRT_INTERRUPT_PRIORITY_0	(16+5)	//Interrupt Priority for FRT Channel 0



extern void hal_freeruntimer_Init(U8 channel, U32 us);
extern void hal_freeruntimer_start(U8 channel);
extern void hal_freeruntimer_stop(U8 channel);

extern U8  free_run_timer_stared;
extern void (*func_app_beep_ctrl)(void);
extern void get_WAV_data(unsigned long int n, unsigned char * (* return_wavdata_addr)());

#endif
