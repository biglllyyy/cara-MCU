#include "mb91520.h"
#include "hal_reload_timer.h"
#include "hal_ppg.h"
#include "comm_typedef.h"
#ifdef WIN32
#include <windows.h>
#endif

typedef struct
{
	U32  tick_cnt;			//record the buzzer current pos in the arry
	U32  tick_all;			//the  buz keep the time 
	U8	*duty_list;		    //record the changed data table
	U8   duty_offset;		//record the current duty value in the array 
	U8   duty_cnt;			//current duty times
	U8   duty_cnt_max;		//record the same duty run times;
}buz_sound_ctl_t;

buz_sound_ctl_t buz_ctl;
static U32 	gU32TickCount = 0;
/* 每隔1ms切换一次占空比,PWM输出50K Hz,实现声音幅度渐变效果，100个数据，10ms播放完毕 */
static const U8 duttyArray[100] =
{
	100,97,94,91,88,85,82,79,76,73,
	70,67,64,62,60,58,56,54,52,50,
	48,46,44,43,42,41,40,39,38,37,
	36,35,34,34,33,33,31,31,31,29,
	29,28,27,27,26,26,25,24,24,24,
	23,23,23,23,22,22,21,21,21,21,
	21,19,19,19,19,18,18,18,17,17,
	16,15,15,15,15,14,14,13,13,12,
	12,11,11,10, 9, 9, 8, 8, 7, 7,
	 6, 6, 5, 5, 4, 4, 3, 3, 2, 1
};
/* 以下数据采用指数曲线模拟而成，方程y=((1000-0.00001(x-100)^5)+0.5)/1000 */
static const U8  duttyArray2[100]={
	100,96,91,87,83,78,74,71,67,63,
	60,57,54,51,48,45,43,40,38,36,
	34,32,30,28,26,25,23,22,20,19,
	18,17,16,15,14,13,12,11,10, 9,
	9, 8,  8, 7, 7, 6, 6, 5, 5, 4,
	4, 4,  4, 3, 3, 3, 3, 2, 2, 2,
	2, 2,  2, 2, 2, 2, 1, 1, 1, 1,
	1, 1,  1, 1, 1, 1, 1, 1, 1, 1,
	1, 1,  1, 1, 1, 1, 1, 1, 1, 1,
	1, 1,  1, 1, 1, 1, 1, 1, 1, 1
};

void buz_init(buz_sound_ctl_t *buz)
{
	buz->duty_list = (U8*)&duttyArray2[0];
	buz->tick_all = (sizeof(duttyArray2)/sizeof(duttyArray2[0]))<<1;
	buz->tick_cnt = 0;
	buz->duty_cnt_max =  10;
	buz->duty_offset = 0;
	buz->duty_cnt = 0;
}

/* 声音一 */
#if 0
#define BUZZER_ON		(900*2)		/* buzzer on:500ms */
#define BUZZER_OFF		(1100*2)    /* buzzer off: 1500ms */
#define PERIOS_CNTS	(BUZZER_ON+BUZZER_OFF)
#endif


#define BUZ_WARN1_PERIOD		(900)    /* ms */
#define BUZ_WARN2_PERIOD		(500)    /* ms */

void hal_set_Waning(U8 u8WarnType)
{
	if (u8WarnType == 1)
	{
		buz_init(&buz_ctl);
		buz_ctl.duty_cnt_max = 9;
		buz_ctl.tick_all = 2*buz_ctl.duty_cnt_max*(sizeof(duttyArray2)/sizeof(duttyArray2[0]));
	}
	else if(u8WarnType == 2)
	{
		buz_init(&buz_ctl);
		buz_ctl.duty_cnt_max = 5;
		buz_ctl.tick_all =2*buz_ctl.duty_cnt_max*(sizeof(duttyArray)/sizeof(duttyArray[0]));
	}
	else
	{
		buz_init(&buz_ctl);
		buz_ctl.duty_cnt_max = 10;
	}
}


/* 声音二 */
#if 0
#define BUZZER_ON		(500*2)   /* buzzer on:500ms */
#define BUZZER_OFF		(1500*2)   /* buzzer off: 1500ms */
#define PERIOS_CNTS	(BUZZER_ON+BUZZER_OFF)
#endif

/* 返回系统时间相对于since，过了多久 */
U32 hal_timer_passed(U32 since)
{
#ifndef WIN32  
	U32 now = gU32TickCount;
#else
	U32 now = hal_timer_get_tick();
#endif
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
#ifndef WIN32
    return (gU32TickCount);
#else
	return((U32)(GetTickCount() / 10));
#endif
}

/* reload 3 used as system  timer */
__interrupt void ISR_Reloadtimer3(void)
{

}


/*****************************************************************************/
/* reload timer 1 ,uesd as can0 */
/*****************************************************************************/
__interrupt void reload_timer_0(void)
{
	U8  duty = 0;
	wdg_feed();

	if (TMCSR0_UF == 1)
	{
		TMCSR0_UF = 0;   // reset underflow interrupt request flag
		gU32TickCount++;
	}
	else if(TMCSR4_UF == 1) //500us
	{
		TMCSR4_UF = 0;		 /* clear interrupt flag  */
		if(buz_ctl.tick_cnt < buz_ctl.tick_all)	//buz tick_cnt 
		{	
			buz_ctl.tick_cnt++;
			if(buz_ctl.tick_cnt&0x01)			//if tick 
			{
				if(buz_ctl.duty_cnt < buz_ctl.duty_cnt_max)	
				{
					buz_ctl.duty_cnt++;
					
				}
				else
				{
					buz_ctl.duty_offset++;
					buz_ctl.duty_cnt = 0;
				}
				if(buz_ctl.duty_offset < 100)
				{
					duty = buz_ctl.duty_list[buz_ctl.duty_offset];
					PPG5_PCSR = PCSR_VALUE;
					PPG5_PDUT = PDUT_1_PERCENT*duty;
					PPG5_PCN_PGMS = 0;				//start pwm output
				}
			}
			else
			{
				PPG5_PCN_PGMS = 1;				//talk: stop pwm output
			}	
		}
		else
		{
			buz_ctl.tick_cnt = 0;
			buz_ctl.duty_cnt = 0;
			buz_ctl.duty_offset = 0;
			hal_reloadtimer_stop(4);
		}
	}
}

