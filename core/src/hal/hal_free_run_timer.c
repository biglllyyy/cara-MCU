#include "hal_free_run_timer.h"
#include "hal_gpio.h"
#include "mb91520.h"
#include "mid_buzzer.h"
#include "mid_dbg.h"

U8 		free_run_timer_stared = 0;	/* record free run timer wether start */
static unsigned char *data_addr,*data_addr_bak;
static unsigned long int data_length;

void (*func_app_beep_ctrl)(void);

void get_WAV_data(unsigned long int n, unsigned char * (* return_wavdata_addr)())
{
	data_addr = (*return_wavdata_addr)(&n);
	data_addr_bak = data_addr;
	data_length = n;
}

void hal_freeruntimer_Init(U8 channel, U32 us)
{
    U32 temp_value = 0;
	switch(channel)
    {
    case 0:
        FT0_TCCS0_STOP  = 1;    //stop counter
        //TCGSE_FRT0 = 1;			// free run timer0  Allow simultaneous activation/clear
        FT0_TCCS0_ECKE = 0;		//1:External clock,0:Peripheral clock.
        FT0_TCCS0_IRQZE = 0;	//Interrupt request disabled
        FT0_TCCS0_MSI  = 0;     //An interrupt will be generated when there is a match for the first time
        FT0_TCCS0_ICLR = 0;		// Compare clear interrupt flag
        FT0_TCCS0_ICRE = 0;		//0: Interrupt request disabled/ 1: enable

        FT0_TCCS0_SCLR = 0;		//SCLR: Timer clear = Clear the content of the timer
        FT0_TCCS0_BFE  = 0;		//Invalidate the compare clear buffer
        FT0_TCCS0_MODE = 0;		//0:Up count mode;1:up/down  count mode
        FT0_TCCS0_CLK  =  FRT_FRE_DIV;		//clk/64(clk = 40M, t= 1.6us);
        FT0_TCCS0_MODE2 = 0;	//Value set for MSI5 to MSI3 will be invalid
        FT0_TCCS0_MSI_  = 0;	//MS5~MS3:An interrupt occurs when there is a match for the first time

        FT0_TCDT0 = 0x0000;

		temp_value =	(us*(FRT_SUPPLYCLK_PCLK/1000))/(1000*(1<<FRT_FRE_DIV));//Compare clear value buffer,250us
		FT0_CPCLR0 = (U16)temp_value;
        FRS0_OS1 = 0;		//Setting disabled (operation is not guaranteed),Output compare0,1 disable
        FRS1_IS1 = 0;		//Input capture free-run timer selector,0:select free run timer 0
        break;
    case 1:
        FT1_TCCS1_STOP  = 1;    //stop counter
        //TCGSE_FRT0 = 1;	// free run timer0  Allow simultaneous activation/clear
        FT1_TCCS1_ECKE = 0;		//1:External clock,0:Peripheral clock.
        FT1_TCCS1_IRQZE = 0;	//Interrupt request disabled
        FT1_TCCS1_MSI  = 0;     //An interrupt will be generated when there is a match for the first time
        FT1_TCCS1_ICLR = 0;		// Compare clear interrupt flag
        FT1_TCCS1_ICRE = 0;		//0: Interrupt request disabled/ 1: enable
        FT1_TCCS1_SCLR = 0;		//SCLR: Timer clear = Clear the content of the timer
        FT1_TCCS1_BFE  = 0;		//Invalidate the compare clear buffer
        FT1_TCCS1_MODE = 0;		//0:Up count mode;1:up/down  count mode
        FT1_TCCS1_CLK  =  FRT_FRE_DIV;		//clk/64(clk = 40M, t= 1.6us);
        FT1_TCCS1_MODE2 = 0;	//Value set for MSI5 to MSI3 will be invalid
        FT1_TCCS1_MSI_  = 0;	//MS5~MS3:An interrupt occurs when there is a match for the first time
        FT1_TCDT1 = 0x0000;
		temp_value =	(us*(FRT_SUPPLYCLK_PCLK/1000))/(1000*(1<<FRT_FRE_DIV));//Compare clear value buffer,250us
		FT1_CPCLR1 = (U16)temp_value;
        FRS0_OS1 = 0;			//Setting disabled (operation is not guaranteed),Output compare0,1 disable
        break;
    case 2:
        FT2_TCCS2_STOP  = 1;    //stop counter
        //TCGSE_FRT0 = 1;	// free run timer0  Allow simultaneous activation/clear
        FT2_TCCS2_ECKE = 0;		//1:External clock,0:Peripheral clock(PCLK).
        FT2_TCCS2_IRQZE = 0;	//Interrupt request disabled
        FT2_TCCS2_MSI  = 0;     //An interrupt will be generated when there is a match for the first time
        FT2_TCCS2_ICLR = 0;		// Compare clear interrupt flag
        FT2_TCCS2_ICRE = 1;		//0: Interrupt request disabled/ 1: enable
        FT2_TCCS2_SCLR = 0;		//SCLR: Timer clear = Clear the content of the timer
        FT2_TCCS2_BFE  = 0;		//Invalidate the compare clear buffer
        FT2_TCCS2_MODE = 0;		//0:Up count mode;1:up/down  count mode
        FT2_TCCS2_CLK  =  FRT_FRE_DIV;	//clk/(1<<FRT_FRE_DIV)(clk = pclk, t= 1.6us);
        FT2_TCCS2_MODE2 = 0;	//Value set for MSI5 to MSI3 will be invalid
        FT2_TCCS2_MSI_  = 0;	//MS5~MS3:An interrupt occurs when there is a match for the first time
        FT2_TCDT2 = 0x0000;
        temp_value =	(us*(FRT_SUPPLYCLK_PCLK/1000))/(1000*(1<<FRT_FRE_DIV));//Compare clear value buffer,250us
		FT2_CPCLR2 = (U16)temp_value;
        FRS0_OS1 = 0;		//Setting disabled (operation is not guaranteed),Output compare0,1 disable
        break;
    case 3:
        //Operate as reload timer 32bit (count up)
        //count from internal clock divided by 2^8=256, PCLK1/256=156.25kHz (6.4us)
        FT3_TCCS3 = (0<<15)		//ECKE: Clock Selection = internal clock
                    |(0<<9)		//ICLR: Compare clear interrupt flag = Clear Interrupt Flag, (FT0_TCCS0_ICLR=0 to clear interrupt flag)
                    |(1<<8)		//ICRE: Compare clear interrupt request enabled = interrupt enabled
                    |(1<<6)		//STOP: Timer enabled = Count stop
                    |(0<<4)		//SCLR: Timer clear = Clear the content of the timer
                    |(1<<1);		//CLK: Clock freq selection = PCLK1/256

        FT3_TCDT3 = 0x0000;
        //Assign Interrupt priority
        //ICR33 = FRT_INTERRUPT_PRIORITY_0;

        //Assign Reload Value
        //FT4_CPCLR4 = FRT_RLDVALUE_0;
        FT3_CPCLR3 = FRT_RLDVALUE_1;

        break;

    case 4:
        //Operate as reload timer 32bit (count up)
        //count from internal clock divided by 2^8=256, PCLK1/256=156.25kHz (6.4us)
        FT4_TCCS4 = (0<<15)		//ECKE: Clock Selection = internal clock
                    |(0<<9)		//ICLR: Compare clear interrupt flag = Clear Interrupt Flag, (FT0_TCCS0_ICLR=0 to clear interrupt flag)
                    |(1<<8)		//ICRE: Compare clear interrupt request enabled = interrupt enabled
                    |(1<<6)		//STOP: Timer enabled = Count stop
                    |(0<<4)		//SCLR: Timer clear = Clear the content of the timer
                    |(1<<1);		//CLK: Clock freq selection = PCLK1/256

        FT4_TCDT4 = 0x0000;
        //Assign Interrupt priority
        //ICR34 = FRT_INTERRUPT_PRIORITY_0;

        //Assign Reload Value
        //FT4_CPCLR4 = FRT_RLDVALUE_0;
        FT4_CPCLR4 = FRT_RLDVALUE_5;
        //Start FRT
        FT4_TCCS4_STOP = 0;

        break;
    }

}


void hal_freeruntimer_start(U8 channel)
{
    switch(channel)
    {
    case 0:
        FT0_TCDT0 = 0x0000;
        FT0_TCCS0_STOP = 0;
        FT0_TCCS0_ICRE = 1;
        break;
    case 1:
        FT1_TCDT1 = 0x0000;
        FT1_TCCS1_STOP = 0;
        FT1_TCCS1_ICRE = 1;
        //free_run_timer_stared = 1;
        break;
    case 2:
        FT2_TCDT2 = 0x0000;
        FT2_TCCS2_STOP = 0;
        FT2_TCCS2_ICRE = 1;
        break;
    case 3:
        FT3_TCDT3 = 0x0000;
        FT3_TCCS3_STOP = 0;
        break;
    case 4:
        FT4_TCDT4 = 0x0000;
        FT4_TCCS4_STOP = 0;
        break;
    }
}


void hal_freeruntimer_stop(U8 channel)
{
    switch(channel)
    {
    case 0:
        FT0_TCCS0_STOP = 1;
        break;
    case 1:
        FT1_TCCS1_STOP = 1;
       // free_run_timer_stared = 0;
        break;
    case 2:
        FT2_TCCS2_STOP = 1;
        break;
    case 3:
        FT3_TCCS3_STOP = 1;
        break;
    case 4:
        FT4_TCCS4_STOP = 1;
        break;
    }
}



#if 0
__interrupt void ISR_FRT_3(void)
{
    if( FT3_TCCS3_ICLR )
    {
        PDR02_P1 ^= 1;

        FT3_TCCS3_ICLR = 0;	//clear match interrupt flag
    }
}
#endif

__interrupt void ISR_FreeRunTimer0(void)
{
    if(FT0_TCCS0_ICLR)   //clear match interrupt flag
    {
        FT0_TCCS0_ICLR  = 0;
        
        func_app_beep_ctrl();
    }    
}

__interrupt void ISR_FreeRunTimer1(void)
{
	static unsigned long int i = 0; 

    if(FT1_TCCS1_ICLR)   /* 中断标志 */
    {
        FT1_TCCS1_ICLR  = 0;
        
		if(i < data_length) /* 开始播放WAV音频数据 */
		{
			buzzer_start_test(28000,*data_addr);
			data_addr++;
			i++;
		}
		else /* 播放完成WAV音频数据 */
		{
			i = 0;
			data_addr = data_addr_bak;
			hal_freeruntimer_stop(1);/* 停止播放WAV音频的定时器 */
		}
    }    
}


__interrupt void ISR_FreeRunTimer2(void)
{
    if(FT2_TCCS2_ICLR)   //clear match interrupt flag
    {
        FT2_TCCS2_ICLR  = 0;
    }
    if( FT3_TCCS3_ICLR )
    {
        FT3_TCCS3_ICLR = 0;	//clear match interrupt flag
    }
    if( FT4_TCCS4_ICLR )
    {
        FT4_TCCS4_ICLR = 0;	//clear match interrupt flag
    }
}


