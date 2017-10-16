#include "hal_ext_int.h"
#include "hal_gpio.h"
#include "mb91520.h"
#include "KeyPortReg.h"
#include "hal_gpio_cfg.h"
#include "hal_free_run_timer.h"
#include "g_variable.h"

extern U16  sice_last_int_time;
#if  0
static  U16  temp_radr_data = 0;		/* temp radr data */
#endif
S8   	radr_sts = -1;					/* radr sts */
U16  	radr_data = 0xFFFF;				/* at lsat radr data */
U8 		radr_frame_head = 0;			/* wether checked frame head */

void radr_data_init(void)
{
    //temp_radr_data = 0;
    radr_sts = -1;
    radr_data = 0xFFFF;
    radr_frame_head = 0;
}



void hal_extern_interrupt_init(U8 chn,INT_TYPE_e type)
{
    switch(chn)
    {
    case 0:
        set_single_io_dir(9,6,IO_INPUT);
        ENIR0_EN0 = 0;
        EIRR0_ER0  = 0;
        ELVR0_LBLA0 = (U8)type;
        break;

    case 1:
        set_single_io_dir(9,6,IO_INPUT);
        ENIR0_EN1 = 0;				// forbid interrupt
        EIRR0_ER1 = 0;				// Clear Int flag
        ELVR0_LBLA1 = (U8)type;		// int type
        break;
    case 5:
    		set_single_io_dir(7,2,IO_INPUT);
        ENIR0_EN5 = 0;				// forbid interrupt
        EIRR0_ER5 = 0;				// Clear Int flag
        ELVR0_LBLA5 = (U8)type;		// int type
        break;

    case 13:
        set_single_io_dir(6,0,IO_INPUT);
        ENIR1_EN5 = 0;				    // forbid interrupt
        EIRR1_ER5 = 0;					// Clear Int flag
        ELVR1_LBLA5 = (U8)type;		// int type
        break;
    default:
        break;

    }
    if(chn <=7)
    {
        //ICR0 = 20;		/* extern int 0-7 */
    }
    else if(chn <= 15)
    {
        //ICR1 = 21; 	 	/* extern int 8-15 */
    }
}


void hal_ext_int_start(U8 chn)
{
    switch(chn)
    {
    case 0:
        EIRR0_ER0 = 0;				// Clear Int flag
        ENIR0_EN0 = 1;				// enable interrupt
        break;
    case 1:
        EIRR0_ER1 = 0;				// Clear Int flag
        ENIR0_EN1 = 1;				// enable interrupt
        break;
    case 5:
        EIRR0_ER5 = 0;				// Clear Int flag
        ENIR0_EN5 = 1;				// enable interrupt
        break;
    case 13:
        EIRR1_ER5 = 0;					// Clear Int flag
        ENIR1_EN5 = 1;				    // enable interrupt
        break;
    }
}


void hal_ext_int_stop(U8 chn)
{
    switch(chn)
    {
    case 0:
        EIRR0_ER0 = 0;				// Clear Int flag
        ENIR0_EN0 = 0;				// forbid interrupt
        break;
    case 1:
        EIRR0_ER1 = 0;				// Clear Int flag
        ENIR0_EN1 = 0;				// forbid interrupt
        free_run_timer_stared = 0;
    case 5:
    		EIRR0_ER5 = 0;				// Clear Int flag
        ENIR0_EN5 = 0;				// forbid interrupt
        break;
    case 13:
        EIRR1_ER5 = 0;					// Clear Int flag
        ENIR1_EN5 = 0;				   // forbid interrupt
        break;
    default:
        break;
    }
}


__interrupt void Extern_INT1 (void)
{
#if 0
    static  U16 time_bak = 0;
    U16     time    = 0;	  // get current time
    U16     time_passed = 0; // last interrupt time
    volatile U8    temp_data = 0;

    if(EIRR0_ER1)
    {
        if(0 == free_run_timer_stared)
        {
            hal_freeruntimer_start(RADR_FRT_CHN);
        }
        sice_last_int_time = 0;
        EIRR0_ER1 = 0;	// Clear Int flag

        time = FT1_TCDT1;

        if(time > time_bak)
        {
            time_passed = time - time_bak;
        }
        else
        {
            time_passed =0xFFFF- time_bak + time + 1;
        }
        time_bak = time;


        if((time_passed >= 1250) && (time_passed < 1875))			//2ms~3ms֮�䣬every free run time value as 1.6 us
        {
            radr_frame_head = 1;
            radr_sts = 0;
        }

        if(radr_frame_head)   //from second int read data
        {
            temp_data = PIN_RADR_DATA_IN;
            temp_radr_data |=   temp_data << (15-radr_sts);
            radr_sts++;
            if(radr_sts == 16)
            {
                radr_data = temp_radr_data;
                radr_frame_head = 0;
                radr_sts = 0;
                temp_radr_data = 0;
            }

        }
        else
        {
            radr_sts = 0;
            radr_frame_head = 0;
            temp_radr_data = 0;
        }
    }
#endif
}

__interrupt void ext_interrupt_8_15_isr (void)
{
    /* IGN wake up */
    if(EIRR1_ER5)				//8+5 =13
    {
        g_u8IgnWakeUpFlg = ON;
        EIRR1_ER5 = 0;			// Clear Int flag
        ENIR1_EN5 = 0;		 	// forbid interrupt
        __wait_nop();
        //PDR04 = 0x00;
    }
}

__interrupt void ext_interrupt_0_7_isr(void)
{
    /* lin wake up */
    if(EIRR0_ER5)				//0+5 =5
    {
        g_u8LinWakeUpFlg = ON;
        EIRR0_ER5 = 0;			// Clear Int flag
        ENIR0_EN5 = 0;		 	// forbid interrupt
        __wait_nop();
        //PDR04 = 0x00;
    }
}
