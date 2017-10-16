//#inlcude "mb91520.h"
#include "mid_interrupt.h"
#include "vectors.h"



void mid_Interrupt_Init(void)
{
	icr_manage();
}


void mid_Interrupt_Enable(void)
{
	__set_il(0x1F); 								// Allow all interrupt levels,ILM = 31	 
	__EI(); 										// Enable interrupts	 
}

void mid_Interrupt_Disable(void)
{
	//__set_il(0x1F); 								// Allow all interrupt levels,ILM = 31	 
	__DI(); 										// Enable interrupts	 
}


