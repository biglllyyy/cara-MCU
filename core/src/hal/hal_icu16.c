#include <stdlib.h>
#include "hal_icu16.h"
#include "mb91520.h"
#include "hal_gpio.h"
#include "hal_free_run_timer.h"
#include "KeyPortReg.h"
#include "mid_filter.h"
#include "string.h"

avg_filter_t  dim_high_level= {0};
avg_filter_t  dim_low_level= {0};

U16   hal_icu_dim_lost_cnt = 0;			/* 背光脉冲信号丢失处理 */

void icu16_init(U8 chn)
{
    switch(chn)
    {
    case 0:

        ICS01_ICE0  =1;				// Interrupt request enabled/disable
        ICS01_EG0  = 3;				//edge select,2 bits,3:both edge
        LSYNS_LSYN0 = 0;			//0:External pin input (ICU3 to ICU0),1:Input Lin Synch Field detection
        break;

    case 1:
        hal_icu_dim_lost_cnt = 0;
        set_single_io_dir(15,0,IO_INPUT);
        EPFR[0] |= BIT2|BIT3;		//ICU1E[1:0]=0x03,input from icu 1_3 pin.
        WriteKeyProtectedRegister((unsigned int)&EPFR00,EPFR[0],BYTE_ACCESS);
        ICS01_ICE1  = 1;			// Interrupt request enabled/disable
        ICS01_EG1  = 3; 			//edge select,2 bits
        LSYNS_LSYN1 = 0;			//0:External pin input (ICU3 to ICU0),1:Input Lin Synch Field detection
        break;

    case 2:
        ICS23_ICE0  =1;				// Interrupt request enabled/disable
        ICS23_EG0  = 3;				//edge select,2 bits,3:both edge
        LSYNS_LSYN2 = 0;			//0:External pin input (ICU3 to ICU0),1:Input Lin Synch Field detection
        break;
    case 3:
        ICS23_ICE1  = 1;			// Interrupt request enabled/disable
        ICS23_EG1  = 3; 			//edge select,2 bits
        LSYNS_LSYN3 = 0;			//0:External pin input (ICU3 to ICU0),1:Input Lin Synch Field detection
        break;
    default:
        break;

    }
}

/* ICU1_3  input capture 1  interrupt */
__interrupt void icu16_1_int(void)
{
    static U16 count_bak = 0;
    U16    time_pass = 0;
    U16    temp = 0;
    __DI();
    if(1 == ICS01_ICP1)	 //An effective edge is detected.
    {
        hal_icu_dim_lost_cnt = 0;
        temp = IPCP1;
        if(temp > count_bak)
        {
            time_pass = temp - count_bak;
        }
        else
        {
            time_pass =0xFFFF- count_bak + temp + 1;
        }
        count_bak = temp;

        if(ICS01_IEI1)	// 1:rising_edge ,0: falling edge
        {
            avg_data_filter(&dim_low_level,time_pass);	 //low level time
        }
        else
        {
            avg_data_filter(&dim_high_level,time_pass);  //high level time
        }
    }
    else
    {

    }
    ICS01_ICP1 = 0;	 //clear interrupt flg
    __EI();
}

void icu_cfg_init(void)
{
    memset(&dim_high_level,0,sizeof(avg_filter_t));
    memset(&dim_low_level,0,sizeof(avg_filter_t));
    hal_freeruntimer_Init(FREE_TIMER_CHN,0); //free run timer 0 as icu timer
    icu16_init(ICU_INPUT_CHN);				//ICU 1 init
    hal_freeruntimer_start(FREE_TIMER_CHN);
}
