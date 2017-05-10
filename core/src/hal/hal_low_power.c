#include "mb91520.h"
#include "hal_low_power.h"
#include "hal_clock.h"

static void mode_select(t_low_power_mode type)
{
    U8 temp = 0;

    switch(type)
    {
    case NORMAL:
    {
        PMUCTLR_SHDE = 0;		// Not power down
        STBCR      = 0x00;
        temp = STBCR;
        while(temp)
        {
            temp = STBCR;
            WDTCPR1 = 0xa5;
        }
        break;
    }
    case SLEEP:
    {
        DIVR1_TSTP = 1;		// External bus clock stop
        STBCR_SLEEP = 1;
        temp       = STBCR;	// Enter sleep mode
        STBCR_SLVL = 2; 	// CPU and BUS all sleep
        temp       = STBCR;	// Active by reading
        __wait_nop();
        break;
    }
    case STANDBY_WATCH:
    {
        STBCR_SLVL = 0x00;	// External pins hold previous state
        PMUCTLR_SHDE=0;		// Not power down

        temp       = STBCR;	// Enter standby mode
        temp      &= 0x1F;
        temp      |= 0x40;
        STBCR	   = temp;

        temp       = STBCR;	// Active by reading
        break;
    }
    case STANDBY_WATCH_POWERDOWN:
    {
        STBCR_SLVL = 0x03;	// External pins hold previous state
        PMUCTLR_SHDE=1;		// Power down
        temp       = STBCR;	// Enter standby mode
        temp      &= 0x1F;
        temp      |= 0x40;
        STBCR	   = temp;

        temp       = STBCR;	// Active by reading
        break;
    }
    case STANDBY_STOP:
    {
        MAIN_CLK_MCLK_4M_CCLK_4M_PCLK_TCLK_4M();		/* switch to main clk */
        STBCR_SLVL = 0x00;	// 00:External pins hold hold previous state  02:External pins hold hiz state
        PMUCTLR_SHDE=0;		// Not power down
        STBCR_STOP = 1;		// stop
        STBCR_TIMER = 0;		// stop
        STBCR_SLEEP = 0;		// stop
        temp      = STBCR;	// Enter standby mode
        while(!(temp & 0x80))	// not trans to stop mode
        {
            WDTCPR1 = 0xa5;
            temp    = STBCR;	// Enter standby mode
        }
        break;
    }
    case STANDBY_STOP_POWERDOWN:
    {
        MAIN_CLK_MCLK_4M_CCLK_4M_PCLK_TCLK_4M();		/* switch to main clk */
        STBCR_SLVL = 0x03;	// External pins hold hiz state
        PMUCTLR_SHDE=1;		// power down
        STBCR_STOP = 1;		// stop
        temp       = STBCR;	// Enter standby mode
        while(!(temp & 0x80))	// not trans to stop mode
        {
            WDTCPR1 = 0xa5;
            temp    = STBCR;	// Enter standby mode
        }
    }
    default:
        break;
    }
}


void hal_entry_low_power_mode(void)
{
   mode_select(STANDBY_STOP);
}


void hal_exit_low_power_mode(void)
{
    CCTL_CCRTSELR = 0;		/* as hal recory not init */
    //hal_clock_init();		/*  init clock */
    PLL_CLK_MCLK_4M_PLL_80M_CCLK_80M_PCLK_TCLK_40M();
    mode_select(NORMAL);
}
