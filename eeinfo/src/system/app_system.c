
#include <stdlib.h>
#include "app_system.h"
#include "mid_scheduler.h"
#include "mid_switch.h"
#include "mid_can.h"
#include "mid_adc.h"
#include "mid_ds1302.h"
#include "hal_icu16.h"
#include "hal_clock.h"
#include "hal_adc.h"
#include "hal_reload_timer.h"
#include "hal_uart.h"
#include "hal_gpio.h"
#include "g_variable.h"
#include "hal_free_run_timer.h"
#include "io_cfg.h"
#include "app_io.h"
#include "mid_dbg.h"


static void app_tasks_init(void)
{
	//上电之后，这些任务就开始一直执行
}	

static void app_hardware_init(void)
{
	// PLL_CLK_MCLK_8M_PLL_80M_CCLK_40M_PCLK_TCLK_40M();
	//PLL_CLK_MCLK_4M_PLL_80M_CCLK_80M_PCLK_TCLK_40M();
	PLL_CLK_MCLK_4M_PLL_80M_CCLK_40M_PCLK_TCLK_40M();
    hal_gpio_Init();							/* initiate GPIO ports */
    mid_switch_init(PIN_IN_ALL);							/* init io in and out */
	app_cfg_io();
	uart_open(UART_A20_CHN,115200, 8, 1, 0);		/* 不校验 */
	dbg_init();	/* 初始化debug 串口 */
//	AT24CXX_Init();
    MidSchInit();
    MidSchStart();
    mid_adc_init();
    
    /* reload timer 0, 1000us一次中断*/
    hal_reloadtimer_init(0, 1000);
    hal_reloadtimer_start(0);
	hal_freeruntimer_Init(4,5000000);
    //hal_freeruntimer_start(0);
}

static void app_software_init(void)
{
	variable_init();
}

/** 打印GPIO的数值 **/
void print_gpio(void)
{

	dbg_string("   PDR , DDR , PFR \n");
	dbg_string("01:0x%02x, 0x%02x, 0x%02x\n", PDR01, DDR01, PFR01);
	dbg_string("02:0x%02x, 0x%02x, 0x%02x\n", PDR02, DDR02, PFR02);
	dbg_string("03:0x%02x, 0x%02x, 0x%02x\n", PDR03, DDR03, PFR03);
	dbg_string("04:0x%02x, 0x%02x, 0x%02x\n", PDR04, DDR04, PFR04);
	dbg_string("05:0x%02x, 0x%02x, 0x%02x\n", PDR05, DDR05, PFR05);
	dbg_string("06:0x%02x, 0x%02x, 0x%02x\n", PDR06, DDR06, PFR06);
	dbg_string("07:0x%02x, 0x%02x, 0x%02x\n", PDR07, DDR07, PFR07);

}

void app_sys_init(void)
{
    __DI();
    app_hardware_init();
    app_software_init();

    app_tasks_init();
    __EI();
}




