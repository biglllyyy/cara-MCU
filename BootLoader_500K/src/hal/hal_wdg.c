#include "mb91520.h"
#include "hal_wdg.h"

void wdg_feed(void)
{
    WDTCPR1 = 0xa5;
}

void wdg_init(void)
{
	//CR oscillator ¡Á 2^15 cycles,100Khz
	//t = 0.1ms*2^15 = 3276.8ms = 3.2S 
	
}

U8 hal_get_reset_factor(void)
{
   return RSTRR;    	  
}

void hal_soft_reset(void)
{
	U8 read = 0;
	RSTCR_SRST = 1;
    read = RSTCR_SRST;		//mcu_reset request   
    if(read)
    {
		__wait_nop();
    }
}
