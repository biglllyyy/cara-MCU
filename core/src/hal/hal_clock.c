#include "hal_clock.h"
#include "comm_typedef.h"
#include "_fr.h"


void SelectPLLClock(U8 u8PLLMultiplicationFactor,
                    e_MCLK_2_PLL_DIV	 ePLLInputClockDivider,
                    e_PLL_OUT_DIV		 ePLLOutputDivider,
                    e_SOURCE_2_BASE_DIV  eSource_Base_Div,
                    e_BASE_2_PCLK1_DIV   eBase_Pclk1_Div,
                    e_BASE_2_TCLK_DIV    eBase_Tclk_Div)
{
#ifndef WIN32
    /* local variables */
    e_PLL_OUT_DIV  ePLLOutDiv;

    ePLLOutDiv =  ePLLOutputDivider; /* range 0-7 */

    /* re-configure PLL gear up/down */
    // check that gear down is not in progress
    while ( CCTL_CCCGRCR0_GRSTS == 1 && CCTL_CCCGRCR0_GRSTS == 3)
    {
        // clear watchdog
        WDTCPR1 = 0xa5;
    }

    // enable gear up/down
    CCTL_CCCGRCR0_GREN = 1;

    // configure gear up/down settings (currently very thing is set to max)
    CCTL_CCCGRCR1_GRSTN = 0x3F; // max start step
    CCTL_CCCGRCR1_GRSTP = 3;    // step increasing = 4 (max)
    CCTL_CCCGRCR2_GRLP = 0xFF;  // loop number for each step (max)
    /* check and ensure that cpu is running on main clock */
    // if main clock is disabled
    if ( CLKG_CSELR_MCEN != 1 )
    {
        // setup main oscillation stabilization wait time
        CLKG_CSTBR_MOSW = 0x00; // 8ms @ 4MHz
        // clear main timer interrupt
        CLKG_MTMCR_MTIF = 0;
        // enable main clock
        CLKG_CSELR_MCEN = 1;
        // wait for oscillation stabolization wait time
        while( CLKG_CMONR_MCRDY != 1 )
        {
            // clear watchdog
            WDTCPR1 = 0xa5;
        }
    } // main oscillator should be running here

    /* check for the clock source selected */
    if ( CLKG_CMONR_CKM == 3 ) // current clock source is subclock
    {
        CLKG_CSELR_CKS = 1; // select main clock

        // wait for sytem running on main clock
        while( CLKG_CMONR_CKM != 1 )
        {
            // clear hardware watchdog
            WDTCPR1 = 0xa5;
        }
    }
    else if ( CLKG_CMONR_CKM == 2 ) // running on PLL clock
    {
        // check that gear down is not in progress
        while ( CCTL_CCCGRCR0_GRSTS == 1 && CCTL_CCCGRCR0_GRSTS == 3)
        {
            // clear watchdog
            WDTCPR1 = 0xa5;
        }

        // gear down from PLL clock to main clock
        CCTL_CCCGRCR0_GRSTR = 1;

        // wait for gear down done
        while ( CCTL_CCCGRCR0_GRSTS != 0 )
        {
            // clear watchdog
            WDTCPR1 = 0xa5;
        }

        // change to main clock
        CLKG_CSELR_CKS = 0;

        // wait until that main clock is selected as clock source
        while( CLKG_CMONR_CKM != 0 )
        {
            // clear watchdog
            WDTCPR1 = 0xa5;
        }
    } // cpu should run on main clock now comming from PLL or sub clock

    /* >>>>> System running on MAIN Clock here <<<<< */

    // disable PLL
    CLKG_CSELR_PCEN = 0;

    /* re-configure PLL settings */
    // select PLL as output
    CCTL_CCPSSELR_PCSEL = 0;

    // set PLL stabilization wait time to ensure 256us PLL stabilization wait time
    if ( _4M == SYSTEM_MAINCLOCK_FREQ )
    {
        CLKG_PLLCR_POSW = 0x09;
    }
    else if ( _8M == SYSTEM_MAINCLOCK_FREQ)
    {
        CLKG_PLLCR_POSW = 0x0A;
    }

    // select PLL input clock divider
    CLKG_PLLCR_PDS = ePLLInputClockDivider;   /* input div 0-15*/

    // set the PLL output divider
    CCTL_CCPSDIVR_PODS = ePLLOutputDivider;

    // set the PLL feedback divider accordingly
    if((u8PLLMultiplicationFactor>= PLL_MUL_MIN) && (u8PLLMultiplicationFactor<= PLL_MUL_MAX))
    {
        CCTL_CCPLLFBR_IDIV = u8PLLMultiplicationFactor-1;
    }
    else
    {
        //default set
        CCTL_CCPLLFBR_IDIV = 20;
    }

    /* re-configure the CLKB, CLKP and CLKT divider */
    DIVR0_DIVB = eSource_Base_Div; // source_Base_div
    DIVR1_DIVT = eBase_Tclk_Div; //  base_TCLK1_div
    DIVR2_DIVP = eBase_Pclk1_Div;  // base_PCLK1_div

    /* re-configure the flash wait state settings */
    if ( ( ( ( SYSTEM_MAINCLOCK_FREQ / ( ePLLInputClockDivider + 1 ) ) * ( u8PLLMultiplicationFactor + 1 ) ) / ePLLOutDiv ) <= _8M )
    {
        // zero wait cycles for flash
        FCTLR_FAW = 0;
    }
    else
    {
        // one wait cycle for flash
        FCTLR_FAW = 1;
    }
    // clear PLL timer interrupt
    CLKG_PTMCR_PTIF = 0;

    // enable PLL
    CLKG_CSELR_PCEN = 1;

    // wait for PLL oscillation stabilization wait time
    while( CLKG_CMONR_PCRDY != 1 )
    {
        // clear watchdog
        WDTCPR1 = 0xa5;
    }
    // clear PLL timer interrupt
    CLKG_PTMCR_PTIF = 0;
    // select PLL as clock source
    CLKG_CSELR_CKS = 2;
    // start gear up process
    CCTL_CCCGRCR0_GRSTR = 1;
    // wait for gear down done
    while ( CCTL_CCCGRCR0_GRSTS != 2 )
    {
        // clear watchdog
        WDTCPR1 = 0xa5;
    }
    // wait for PLL clock being selected as clock source
    while( CLKG_CMONR_CKM != 2 )
    {
        // clear watchdog
        WDTCPR1 = 0xa5;
    }
    /* >>>>> System running on PLL Clock here <<<<< */
#endif
}




void SelectMainClock(e_SOURCE_2_BASE_DIV  eSource_Base_Div,
                     e_BASE_2_PCLK1_DIV   eBase_Pclk1_Div,
                     e_BASE_2_TCLK_DIV    eBase_Tclk_Div)
{
#ifndef WIN32
    if ( CLKG_CMONR_CKM == 0 )
    {
        // nothing to do here
    } // system is already running on Main Clock
    else if ( CLKG_CMONR_CKM == 2 ) // running on PLL clock (then Main Osc is also running)
    {
        /* re-configure PLL gear up/down */
        // check that gear down is not in progress
        while ( CCTL_CCCGRCR0_GRSTS == 1 && CCTL_CCCGRCR0_GRSTS == 3)
        {
            // clear watchdog
            WDTCPR1 = 0xa5;
        }

        // enable gear up/down
        CCTL_CCCGRCR0_GREN = 1;

        // configure gear up/down settings (currently very thing is set to max)
        CCTL_CCCGRCR1_GRSTN = 0x3F; // max start step
        CCTL_CCCGRCR1_GRSTP = 3;    // step increasing = 4 (max)
        CCTL_CCCGRCR2_GRLP = 0xFF;  // loop number for each step (max)

        // gear down from PLL clock to main clock
        CCTL_CCCGRCR0_GRSTR = 1;

        // wait for gear down done
        while ( CCTL_CCCGRCR0_GRSTS != 0 )
        {
            // clear watchdog
            WDTCPR1 = 0xa5;
        }

        // change to main clock
        CLKG_CSELR_CKS = 0;

        // wait until that main clock is selected as clock source
        while( CLKG_CMONR_CKM != 0 )
        {
            // clear watchdog
            WDTCPR1 = 0xa5;
        }

        // disable PLL
        CLKG_CSELR_PCEN = 0;
    } // cpu should run on main clock now comming from PLL
    else if ( CLKG_CMONR_CKM == 3 ) // current clock source is subclock
    {
        // if main clock is disabled
        if ( CLKG_CSELR_MCEN != 1 )
        {
            // setup main oscillation stabilization wait time
            CLKG_CSTBR_MOSW = 0x00; // 8ms @ 4MHz

            // clear main timer interrupt
            CLKG_MTMCR_MTIF = 0;

            // enable main clock
            CLKG_CSELR_MCEN = 1;

            // wait for oscillation stabolization wait time
            while( CLKG_CMONR_MCRDY != 1 )
            {
                // clear watchdog
                WDTCPR1 = 0xa5;
            }
        } // main oscillator should be running here

        CLKG_CSELR_CKS = 1; // select main clock

        // wait for sytem running on main clock
        while( CLKG_CMONR_CKM != 1 )
        {
            // clear hardware watchdog
            WDTCPR1 = 0xa5;
        }
    } // main clock should be clock source here coming from sub clock

    // zero wait cycles for flash
    FCTLR_FAW = 0;

    /* re-configure the CLKB, CLKP and CLKT divider */
    DIVR0_DIVB = eSource_Base_Div; // source_Base_div
    DIVR1_DIVT = eBase_Tclk_Div; //  base_TCLK1_div
    DIVR2_DIVP = eBase_Pclk1_Div;  // base_PCLK1_div

    /* >>>>> System running on MAIN Clock here <<<<< */
#endif
}




#ifndef __CPU_MB91F577S__

void SelectSubClock(e_SOURCE_2_BASE_DIV  eSource_Base_Div,
                    e_BASE_2_PCLK1_DIV   eBase_Pclk1_Div,
                    e_BASE_2_TCLK_DIV    eBase_Tclk_Div)
{
    if ( CLKG_CMONR_CKM == 3 ) // current clock source is subclock
    {
        // nothing to do
    }
    else // MCU is running on PLL or Main coming
    {
        if ( CLKG_CMONR_CKM == 2 ) // running on PLL clock
        {
            /* re-configure PLL gear up/down */
            // check that gear down is not in progress
            while ( CCTL_CCCGRCR0_GRSTS == 1 && CCTL_CCCGRCR0_GRSTS == 3)
            {
                // clear watchdog
                WDTCPR1 = 0xa5;
            }

            // enable gear up/down
            CCTL_CCCGRCR0_GREN = 1;

            // configure gear up/down settings (currently very thing is set to max)
            CCTL_CCCGRCR1_GRSTN = 0x3F; // max start step
            CCTL_CCCGRCR1_GRSTP = 3;    // step increasing = 4 (max)
            CCTL_CCCGRCR2_GRLP = 0xFF;  // loop number for each step (max)

            // gear down from PLL clock to main clock
            CCTL_CCCGRCR0_GRSTR = 1;

            // wait for gear down done
            while ( CCTL_CCCGRCR0_GRSTS != 0 )
            {
                // clear watchdog
                WDTCPR1 = 0xa5;
            }

            // change to main clock
            CLKG_CSELR_CKS = 0;

            // wait until that main clock is selected as clock source
            while( CLKG_CMONR_CKM != 0 )
            {
                // clear watchdog
                WDTCPR1 = 0xa5;
            }

            // disable PLL
            CLKG_CSELR_PCEN = 0;

        } // cpu should run on main clock now comming from PLL or sub clock
        else if ( CLKG_CMONR_CKM == 0 )
        {
            // nothing to do here
        } // system is already running on Main Clock

        /* >>>>> System is running on Main clock now <<<<< */

        /* check is sub clock is enabled */
        if ( CLKG_CSELR_SCEN == 0 ) // sub clock is not running
        {
            // setup sub oscillation stabilization wait time
            CLKG_CSTBR_SOSW = 0x07; // 1s @ 32kHz

            // clear main timer interrupt
            CLKG_STMCR_STIF = 0;

            CLKG_CSELR_SCEN = 1; // enable sub clock

            while ( CLKG_CMONR_SCRDY != 1 ) // wait for sub clock to start and stabilize
            {
                // clear watchdog
                WDTCPR1 = 0xa5;
            }
        } // sub clock should be up and running

        /* switch to sub clock as clock source */

        CLKG_CSELR_CKS = 3; // select sub clock

        // wait for sytem running on sub clock
        while( CLKG_CMONR_CKM != 3 )
        {
            // clear hardware watchdog
            WDTCPR1 = 0xa5;
        }
    }

    /* >>>>> System is running on sub clock now <<<<< */

    // zero wait cycles for flash
    FCTLR_FAW = 0;

    /* re-configure the CLKB, CLKP and CLKT divider */
    /* re-configure the CLKB, CLKP and CLKT divider */
    DIVR0_DIVB = eSource_Base_Div; // source_Base_div
    DIVR1_DIVT = eBase_Tclk_Div; //  base_TCLK1_div
    DIVR2_DIVP = eBase_Pclk1_Div;  // base_PCLK1_div
}

#endif // __CPU_MB91F577S__

/* end of module system.c */


#if 0
void hal_clock_init(void)
{
    /* main clock */
    CLKG_CSTBR_MOSW   =0;		/* main clock stb time?2^15 main clock cycle = 8ms */
    CLKG_PLLCR_POSW  = 0x0f;	/* 2^16 ? main clock cycle (Initial value),*/
    CLKG_CSELR_MCEN   = 1;		/* main clock enable */
    CLKG_CSELR_PCEN   = 1;		/* PLL enable */
    while(!CLKG_CMONR_MCRDY)	/* wait until clock stb,osc=4M,16384.0[ms] */
    {
        ;
    }
    while(!CLKG_CMONR_PCRDY)	/* wait PLL clock stb */
    {
        ;
    }
#if MAIN_OSC == _4M
    CLKG_PLLCR_PDS	= 0;		/*PLL/SSCG input clock = Main clock / 1 */
#elif MAIN_OSC == _8M
    CLKG_PLLCR_PDS	= 1;		/*PLL/SSCG input clock = Main clock / 2 */
#endif

    CCTL_CCPLLFBR_IDIV = 39;	/*pll feedback Input DIVider ratio settings, mul 40, = 4M * 40 = 160M */
    /* set pll clk = 80M */
    CCTL_CCPSDIVR_PODS = 0;		/* PLL clock = PLL macro oscillation clock /2 = 160M/2 */

    /* source clock */
    CLKG_CSELR_CKS = 2;			/*0: Division of the main clock (MCLK) by 2 */
    /*1: Division of the main clock (MCLK) by 2 */
    /*2: PLL/SSCG clock (PLLSSCLK) */
    /*3: Sub clock (SBCLK) */
    while(CLKG_CMONR_CKM != 2)	/* if ckm !=0, set cks not success */
    {
        CLKG_CSELR_CKS = 2;
    }
    /* HCLK/CCLK = 80M */
    //HCLK,CCLK = 80M
    DIVR0_DIVB = 0;				/* no divide pllclk = source clock */
    DIVR2_DIVP = 1;				/* PCLK1 = source clock/(1+1)  = 40M */
    DIVR1_DIVT = 1;				/* TCLK = source clock/(1+1)  = 40M */

    PICD_PDIV = 1;				/* PCLK2 = PLLCLK/2 = 40M */
    SACR_M  =  0;				/* PCLK2 select, Synchronous (PLL/SSCG clock for CPU/peripheral) */

    /* CAN module clock */
    CLKG_CSELR_PCEN =	1;
    CANPRE_CPCKS =  0;			/* can clock select PLL clock/ HCLK (on-chip bus clock) = 80M */
    /* CAN prescaler clock = 80M */
    /* RTC clcok =  */
    CCTL_CCRTSELR_CSC = 0;		/* Main oscillation clock select as RTC and PMUCLK,4M or 8M  */
#if(MAIN_OSC == _4M)
    CCTL_CCPMUCR0_FDIV = 0;		/* Divided by 128 (Initial value) */
#elif(MAIN_OSC == _8M)
    CCTL_CCPMUCR0_FDIV = 1;		/* Divided by 256 (Initial value) */
#endif
    /* PMCLK = MCLK/ FDIV*128 = 4M	*/
    CCTL_CCPMUCR1_GDIV = 0;		/* no dvide */
}
#endif

void cpu_goto_main_run(void)
{
    /* cclk select main clk */
    CLKG_CSELR_CKS = 0;
    while(CLKG_CMONR_CKM != 0)	/* if ckm !=0, set cks not success */
    {
        CLKG_CSELR_CKS = 0;
    }
}
