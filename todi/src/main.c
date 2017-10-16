/*****************************************************************************/
/*  FR-81S Family Template Project V01L01                                    */
/*  ALL RIGHTS RESERVED, COPYRIGHT (C) FUJITSU SEMICONDUCTOR LIMITED 2012    */
/*  LICENSED MATERIAL - PROGRAM PROPERTY OF FUJITSU SEMICONDUCTOR LIMITED    */
/*****************************************************************************/
/*****************************************************************************
  MAIN.C
  - description
  - See README.TXT for project description and disclaimer.
******************************************************************************/
#include "_fr.h"
#include "mid_scheduler.h"
#include "app_system.h"
#include "hal_wdg.h"
#ifdef WIN32
#include "hal_uart.h"
#endif
 

extern void icr_manage(void);

/*****************************************************************************/
/*  Main application to control the program flow                             */
/*****************************************************************************/

void main(void)
{   
   	U32 temp = RSTRR;
    wdg_feed();
    // Allow all interrupt levels
    __set_il(0x1F); /* ILM = 31 */
    // Enable interrupts
    __EI();
    wdg_feed();
    app_sys_init();
    icr_manage();
	//dbg_printf("RSTRR = %x",temp);
    while(1)
    {
        wdg_feed();
#ifdef WIN32
        uart0_rx_interrupt();
		uart1_tx_interrupt();
#endif
		dbg_string("-->main\n;");
        MidSchDispatchTasks();
		
    }

}


/* dummy Int routine */
__interrupt void dummy(void)
{
	__wait_nop();
}
