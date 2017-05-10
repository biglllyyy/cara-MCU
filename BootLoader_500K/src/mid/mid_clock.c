#include "Mb91520.h"
#include "Comm_typedef.h"
#include "hal_clock.h"

void mid_Clock_Init(void)
{
	hal_pll_init();
    PLL_CLK_MCLK_4M_PLL_80M_CCLK_40M_PCLK_TCLK_40M();
}




