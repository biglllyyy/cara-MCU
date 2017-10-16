#include "mb91520.h"
#include "hal_reload_timer.h"
#include "hal_ppg.h"
#include "comm_typedef.h"


static U32 	gU32TickCount = 0;

/* 返回系统时间相对于since，过了多久 */
U32 hal_timer_passed(U32 since)
{
	U32 now = gU32TickCount;

    if(now >= since)
    {
        return (now - since);
    }
    else
    {
        /* 系统时间溢出时，进行判定 */
        return (now + (0xFFFFFFFFu - since + 1));
    }
}


void hal_reloadtimer_init(U8 u8channel, U16 u16microsecond)
{
    switch(u8channel)
    {
    case 0:
        TMCSR0 = (0<<14) 	//MOD: Operaqtion Mode = single mode
                 |(0<<12) 	//TRGM: Trigger Mode = No external trigger
                 |(5<<9) 		//CSL: Clock sources = PCLK1/2^6 = 625kHz (1.6us)
                 |(0<<8) 		//Gate, don't care in this use case
                 |(0<<7)		//EF: Extended Flag, don't care in this use case
                 |(0<<5)		//OUTL Low, output level for TOTn pin as Low
                 |(1<<4)		//RELD: reload mode, 1 enable; 0 disable
                 |(1<<3)		//INTE: Interrupt Enable, 1 enable; 0 disable
                 |(0<<2)		//UF: Writing 0 to clear underflow flag
                 |(0<<1)		//CNTE: Stop count operation
                 |(0<<0);		//TRG: No Software Trigger

        //ICR3= PIRORITY_RLDTR_TimeTick;//ReloadTmr0Int_Priority;
        //set reload value Todo
   #if (PCLK == _80M)   
        TMRLRA0 = (__HWORD)(u16microsecond/0.8);	//1.6us each count
   #elif(PCLK == _40M)
   		TMRLRA0 = (__HWORD)(u16microsecond/1.6);	//1.6us each count	
   #endif    
        break;
    case 1:
        /* reload timer 1 setting */
        IO_ICR[2].byte = 0x10;			/* reload timer 1  "level 2" */
        TMCSR1 = (0<<14) 	//MOD: Operaqtion Mode = single mode
                 |(0<<12) 	//TRGM: Trigger Mode = No external trigger
                 |(5<<9) 		//CSL: Clock sources = PCLK1/2^6 = 625kHz (1.6us)
                 |(0<<8) 		//Gate, don't care in this use case
                 |(0<<7)		//EF: Extended Flag, don't care in this use case
                 |(0<<5)		//OUTL Low, output level for TOTn pin as Low
                 |(1<<4)		//RELD: reload mode, 1 enable; 0 disable
                 |(1<<3)		//INTE: Interrupt Enable, 1 enable; 0 disable
                 |(0<<2)		//UF: Writing 0 to clear underflow flag
                 |(0<<1)		//CNTE: Stop count operation
                 |(0<<0);		//TRG: No Software Trigger

        #if (PCLK == _80M)   
        TMRLRA0 = (__HWORD)(u16microsecond/0.8);	//1.6us each count
   		#elif(PCLK == _40M)
   		TMRLRA0 = (__HWORD)(u16microsecond/1.6);	//1.6us each count	
   		#endif 
        //IO_TMCSR1.hword = 0x081b;		/* set TMCSR1  */


    case 3:    /* Reload timer 3 */
        TMCSR3 = (0<<14) 	//MOD: Operaqtion Mode = single mode
                 |(0<<12) 	//TRGM: Trigger Mode = No external trigger
                 |(5<<9) 	//CSL: Clock sources = PCLK1/2^6 = 625kHz (1.6us)
                 |(0<<8) 	//Gate, don't care in this use case
                 |(0<<7)		//EF: Extended Flag, don't care in this use case
                 |(0<<5)		//OUTL Low, output level for TOTn pin as Low
                 |(1<<4)		//RELD: reload mode, 1 enable; 0 disable
                 |(1<<3)		//INTE: Interrupt Enable, 1 enable; 0 disable
                 |(0<<2)		//UF: Writing 0 to clear underflow flag
                 |(0<<1)		//CNTE: Stop count operation
                 |(0<<0);		//TRG: No Software Trigger

        //ICR3= PIRORITY_RLDTR_TimeTick;//ReloadTmr0Int_Priority;
        //set reload value Todo
        #if (PCLK == _80M)   
        TMRLRA0 = (__HWORD)(u16microsecond/0.8);	//1.6us each count
   		#elif(PCLK == _40M)
   		TMRLRA0 = (__HWORD)(u16microsecond/1.6);	//1.6us each count	
   		#endif 

    case 4:    /* Reload timer 0 */
    		TMCSR4 = (0<<14) 	//MOD: Operaqtion Mode = single mode
    		 		|(0<<12) 	//TRGM: Trigger Mode = No external trigger
    		 		|(5<<9) 		//CSL: Clock sources = PCLK1/2^6 = 625kHz (1.6us)
    		 		|(0<<8) 		//Gate, don't care in this use case
    		 		|(0<<7)		//EF: Extended Flag, don't care in this use case
    		 		|(0<<5)		//OUTL Low, output level for TOTn pin as Low
    		 		|(1<<4)		//RELD: reload mode, 1 enable; 0 disable
    		 		|(0<<3)		//INTE: Interrupt Enable, 1 enable; 0 disable
    		 		|(0<<2)		//UF: Writing 0 to clear underflow flag
    		 		|(1<<1)		//CNTE: Stop count operation
    		 		|(0<<0);		//TRG: No Software Trigger

    		ICR2 =  PIRORITY_RLDTR_TimeTick - 1;//ReloadTmr0Int_Priority;
		   #if (PCLK == _80M) 
    		//set reload value Todo
    		TMRLRA4 = (__HWORD)(u16microsecond/0.8);	//1.6us each count
		   #elif(PCLK == _40M)
			TMRLRA4 = (__HWORD)(u16microsecond/1.6);	//1.6us each count
		   #endif


    default:
        break;
    }
}

void hal_reloadtimer_start(U8 u8channel)
{
    switch(u8channel)
    {
    case 0:
        TMCSR0_CNTE = 1;
        TMCSR0_TRG = 1;
        break;
    case 1:
        TMCSR1_CNTE = 1;
        TMCSR1_TRG = 1;
        break;
    case 3:    /* Reload timer 3 */
        TMCSR3_CNTE = 1;
        TMCSR3_TRG = 1;
    case 4:
    	TMCSR4_CNTE = 0;
		TMCSR4_INTE = 1;
		TMCSR4_CNTE = 1;
		TMCSR4_TRG = 1;
        break;
    default:
        break;
    }
}

void hal_reloadtimer_stop(U8 u8channel)
{
    switch(u8channel)
    {
    case 0:
        TMCSR0_CNTE = 0;
        TMCSR0_TRG = 0;
        break;
    case 1:
        TMCSR1_CNTE = 0;
        TMCSR1_TRG = 0;
        break;
    case 3:
        TMCSR3_CNTE = 0;
        TMCSR3_TRG = 0;
        break;
    case 4:
		TMCSR4_INTE = 0;
		TMCSR4_CNTE = 0;
        break;
    default:
        break;
    }
}



U32 hal_timer_get_tick(void)
{
    return (gU32TickCount);
}

/* reload 3 used as system  timer */
__interrupt void ISR_Reloadtimer3(void)
{

}


/*****************************************************************************/
/* reload timer 1  */
/*****************************************************************************/
__interrupt void reload_timer_0(void)
{

	if (TMCSR0_UF == 1)
	{
		TMCSR0_UF = 0;   // reset underflow interrupt request flag
		gU32TickCount++;
	}
	else if(TMCSR4_UF == 1) //500us
	{
		TMCSR4_UF = 0;		 /* clear interrupt flag  */
	}
}

