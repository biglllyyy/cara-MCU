/*****************************************************************************/
/*  FR-81S Family Template Project V01L01                                    */
/*  ALL RIGHTS RESERVED, COPYRIGHT (C) FUJITSU SEMICONDUCTOR LIMITED 2012    */
/*  LICENSED MATERIAL - PROGRAM PROPERTY OF FUJITSU SEMICONDUCTOR LIMITED    */
/*****************************************************************************/
/*----------------------------------------------------------------------------
  VECTORS.C
  - Interrupt vector definition
-----------------------------------------------------------------------------*/
#include "_fr.h"
#include "mb91520.h"
#include "comm_typedef.h"
#include "mid_dbg.h"

#define ICR_PRI_MAX  31
#define ICR_PRI_MIN  16
#define ICR_CHN		 47
#define ICR_BASE_ADDRESS  0x0440

static void icr_set(U8 n,U8 pri)
{
    do
    {
        if((pri >ICR_PRI_MAX) || (pri < ICR_PRI_MIN))
            break;
        if(n>ICR_CHN)
            break;
        IO_ICR[n].byte = pri;
    }
    while(0);
}

/*---------------------------------------------------------------------------
   Prototypes
   Add your own prototypes here. Each vector definition needs is proto-
   type. Either do it here or include a header file containing them.
-----------------------------------------------------------------------------*/

__interrupt void Vectors_Isr_DefaultHandler (void);
 __interrupt void uart0_rx_interrupt(void);
__interrupt void uart1_rx_interrupt(void);
__interrupt void uart1_tx_interrupt(void);
__interrupt void reload_timer_0(void);
__interrupt void CAN_0_int(void);

//__interrupt void ISR_LIN0_Reception(void);
//__interrupt void ISR_LIN0_Transmission(void);
//__interrupt void ISR_LIN1_Reception(void);
//__interrupt void ISR_LIN1_Transmission(void);
//__interrupt void ISR_ReloadTimer3(void);
//__interrupt void ISR_LIN4_Transmission(void);
//__interrupt void ISR_LIN4_Reception(void);
// Interrupt vector definition
//
// Use the following statements to define the interrupt vector table
// i.e. add your interrupt handlers here (ensure to define the ISR prototype,
// e.g. by adding the appropriate header file above).
// All resource related vectors are predefined. Remaining software interrupts
// can be added here as well.
#ifndef  WIN32

#pragma	intvect	Vectors_Isr_DefaultHandler 15	///< Non-Maskable Interrupt
#pragma	intvect	Vectors_Isr_DefaultHandler 16	///< External interrupt 0-7
#pragma	intvect	Vectors_Isr_DefaultHandler 17	///< External interrupt 8-15  External low-voltage detection interrupt
#pragma	intvect	reload_timer_0             18	///< Reload timer 0/1/4/5
#pragma	intvect	Vectors_Isr_DefaultHandler 19	///< Reload timer 3/6/7
#pragma	intvect	Vectors_Isr_DefaultHandler 20	///< Multi-function serial interface ch.0 (reception completed) Multi-function serial interface ch.0 (status)
#pragma	intvect	uart0_rx_interrupt		   21	///< Multi-function serial interface ch.0 (transmission completed)
#pragma	intvect	uart1_rx_interrupt		   22	///< ICR06
#pragma	intvect	uart1_tx_interrupt 		   23	///< ICR07
#pragma	intvect	Vectors_Isr_DefaultHandler 24	///< ICR08
#pragma	intvect	Vectors_Isr_DefaultHandler 25	///< ICR09
#pragma	intvect	Vectors_Isr_DefaultHandler 26	///< Multi-function serial interface ch.3 (reception completed) Multi-function serial interface ch.3 (status)
#pragma	intvect	Vectors_Isr_DefaultHandler 27	///< Multi-function serial interface ch.3 (transmission completed)
#pragma	intvect	Vectors_Isr_DefaultHandler 28	///< Multi-function serial interface ch.4 (reception completed) Multi-function serial interface ch.4 (status)
#pragma	intvect	Vectors_Isr_DefaultHandler 29	///< Multi-function serial interface ch.4 (transmission completed)
#pragma	intvect	Vectors_Isr_DefaultHandler 30	///< Multi-function serial interface ch.5 (reception completed) Multi-function serial interface ch.5 (status)
#pragma	intvect	Vectors_Isr_DefaultHandler 31	///< Multi-function serial interface ch.5 (transmission completed)
#pragma	intvect	Vectors_Isr_DefaultHandler 32	///< Multi-function serial interface ch.6 (reception completed) Multi-function serial interface ch.6 (status)
#pragma	intvect	Vectors_Isr_DefaultHandler 33	///< Multi-function serial interface ch.6 (transmission completed)
#pragma	intvect	CAN_0_int 34	///< CAN0
#pragma	intvect	Vectors_Isr_DefaultHandler 35	///< CAN1
#pragma	intvect	Vectors_Isr_DefaultHandler 36	///< Up/down counter 0 Up/down counter 1
#pragma	intvect	Vectors_Isr_DefaultHandler 37	///< Real time clock
#pragma	intvect	Vectors_Isr_DefaultHandler 38	///< ICR22
#pragma	intvect	Vectors_Isr_DefaultHandler 39	///< 16-bit Free-run timer 0 (0 detection) / (compare clear)
#pragma	intvect	Vectors_Isr_DefaultHandler 40	///< PPG 1/10/11/20/30/31 16-bit Free-run timer 1 (0 detection) / (compare clear)
#pragma	intvect	Vectors_Isr_DefaultHandler 41	///< PPG 2/3/12/13/23/43  16-bit Free-run timer 2 (0 detection) / (compare clear)
#pragma	intvect	Vectors_Isr_DefaultHandler 42	///< PPG 4/24/35
#pragma	intvect	Vectors_Isr_DefaultHandler 43	///< PPG 7/16/17/27/37
#pragma	intvect	Vectors_Isr_DefaultHandler 44	///< PPG 19
#pragma	intvect	Vectors_Isr_DefaultHandler 45	///< 16-bit ICU 0 (fetching) / 16-bit ICU 1 (fetching)
#pragma	intvect	Vectors_Isr_DefaultHandler 46	///< Main timer Sub timer PLL timer 16-bit ICU 2 (fetching) /16-bit ICU 3 (fetching)
#pragma	intvect	Vectors_Isr_DefaultHandler 47	///< Clock calibration unit (sub oscillation) Multi-function serial interface ch.9 (reception completed) Multi-function serial interface ch.9 (status)
#pragma	intvect	Vectors_Isr_DefaultHandler 48	///< A/D converter 0/1/7/10/11/14/15/16/17/22/27/28/31
#pragma	intvect	Vectors_Isr_DefaultHandler 49	///< Clock calibration unit (CR oscillation) Multi-function serial interface ch.9 (transmission completed) 16-bit OCU 0 (match) / 16-bit OCU 1 (match)
#pragma	intvect	Vectors_Isr_DefaultHandler 50	///< 32-bit Free-run timer 4 16-bit OCU 2 (match) / 16-bit OCU 3 (match)
#pragma	intvect	Vectors_Isr_DefaultHandler 51	///< 16-bit OCU 4 (match) / 16-bit OCU 5 (match)
#pragma	intvect	Vectors_Isr_DefaultHandler 52	///< 32-bit ICU6 (fetching/measurement) Multi-function serial interface ch.10 (reception completed) Multi-function serial interface ch.10 (status)
#pragma	intvect	Vectors_Isr_DefaultHandler 53	///< Multi-function serial interface ch.10 (transmission completed)
#pragma	intvect	Vectors_Isr_DefaultHandler 54	///< 32-bit ICU8 (fetching/measurement) Multi-function serial interface ch.11 (reception completed) Multi-function serial interface ch.11 (status)
#pragma	intvect	Vectors_Isr_DefaultHandler 55	///< 32-bit ICU9 (fetching/measurement) WG dead timer underflow 0 / 1/ 2 WG dead timer reload 0 / 1/ 2 WG DTTI 0
#pragma	intvect	Vectors_Isr_DefaultHandler 56
#pragma	intvect	Vectors_Isr_DefaultHandler 57	///< 32-bit ICU5 (fetching/measurement) A/D converter 32/34/35/37/38/40/41/42/43/44/45/46/47
#pragma	intvect	Vectors_Isr_DefaultHandler 58	///< 32-bit OCU7/11 (match)
#pragma	intvect	Vectors_Isr_DefaultHandler 59	///< 32-bit OCU8/9 (match)
#pragma	intvect	Vectors_Isr_DefaultHandler 60	///< ICR44
#pragma	intvect	Vectors_Isr_DefaultHandler 61	///< ICR45
#pragma	intvect	Vectors_Isr_DefaultHandler 62	///< DMAC0/1/2/3/4/5/6/7/8/9/10/11/12/13/14/15
#pragma	intvect	Vectors_Isr_DefaultHandler 63	///< Delay interrupt

#endif

/**
 ******************************************************************************
 ** This function is a placeholder for all vector definitions. Either use
 ** your own placeholder or add necessary code here.
 *****************************************************************************/
__interrupt void Vectors_Isr_DefaultHandler (void)
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
		Console("vector isr default interrupt \n");
    }
}

//中断优先级管理
void icr_manage(void)
{
    int i = 0;
    for(i =0; i<ICR_CHN; i++)
    {
        icr_set(i,ICR_PRI_MAX);		//close all interrupt
    }
    //icr_set(0,18);      //ig on wakeup,0-7 int,can wake up
    //icr_set(1,21);		//8-15 intterrupt,ign wake up
   icr_set(2,16);      //Reload timer 0/1/4/5,vector = 18,
    //icr_set(3,19);		//system timer;
   icr_set(4,22);      //Multi-function serial interface ch.0 (reception completed),vecor = 20
   icr_set(5,22);		//uart 0,debug use
   icr_set(6,22); 	    //Multi-function serial interfacech.1 (reception completed),vector 22,UART1_rx_int
   icr_set(7,22);      //uart1 sent intterrupt
    //icr_set(12,22);      //Multi-function serial interface ch.4 (reception completed)
    //icr_set(13,22);      //Multi-function serial interface ch.4 (transmission completed)
    icr_set(18,23);     //CAN0 interrupt
    //icr_set(23,26);
    //icr_set(25,26);		// turn beep
    //icr_set(29,24);		//16-bit ICU 0 (fetching) / 16-bit ICU 1 (fetching),vector 45
    //icr_set(32,25);		 //A/D converter0/1/7/10/11/14/15/16/17/22/27/28/31,vector 48
    //icr_set(41,25);     //A/D converter 57 39 ICR41 318H 000FFF18H 41 32/34/35/37/38/40/41/42/43/44/45/46/47,vector 57
}
