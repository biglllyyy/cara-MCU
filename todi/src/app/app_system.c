#include <stdio.h>
#include "app_system.h"
#include "app_power.h"
#include "app_led.h"
#include "app_buzzer.h"
#include "app_uart_cmd.h"
#include "app_can.h"
#include "app_io.h"
#include "app_trip.h"
#include "app_spd.h"
#include "app_rpm.h"
#include "app_temp.h"
#include "app_fuel.h"
#include "app_tpms.h"
#include "app_uart_cmd.h"
#include "app_service.h"
#include "app_lin.h"
#include "app_IPconfig.h"
#include "app_gear.h"

#include "mid_scheduler.h"
#include "mid_switch.h"
#include "mid_can.h"
#include "mid_adc.h"
#include "mid_ds1302.h"
#include "mid_flash.h" 

#include "hal_icu16.h"
#include "app_backlight.h"
#include "hal_clock.h"
#include "hal_adc.h"
#include "hal_reload_timer.h"
#include "hal_uart.h"
#include "hal_gpio.h"
#include "g_variable.h"
#include "app_drivetime.h"
#include "app_key.h"
#include "app_UserSetting.h"
#include "app_radr.h"
#include "app_door.h"
#include "app_hw_version.h"
#include "CarTemp.h"
#include "app_fuel_method2.h"
#include "app_time.h"
#include "app_display.h"
#include "app_uds.h"
#include "app_ad_capture.h"

extern void app_radar_ctl(void);
static void app_task_10ms(void);
extern void app_task_20ms(void);
static void app_task_100ms(void);
static void app_task_50ms(void);
static void app_task_500ms(void);
static void app_task_1000ms(void);
static void app_task_2000ms(void);
static void app_task_5000ms(void);





/* work flash写测试 */
static void task_wf_test(void)
{
    static unsigned char buf[8]={0x00,0xaa,0x56,0x78,0x9a,0xbc,0xde,0xfa};
    static unsigned int addr=0x0000;
    static int cnt=0;
  
  	if (cnt % 32 == 0) 
  		mid_erase_flash_async(addr);
  	else
  	{
    	mid_write_flash_async(addr, buf, 8);
    	buf[0] += 1;
    	addr += 0x10;
    }
    cnt++;
    if (addr >= 0xc000) addr = 0x4000;
}

static void task_mid_wf_test(void)
{
	U8 buf[16] = {0x01,0x02,0x03,0x04,0xaa,0xbb,0xcc,0xdd,0xee,0xff,0x11,0x22,0x33,0x44,0x55,0x66};
	mid_save_data(buf, sizeof(buf));
}

static void app_tasks_init(void)
{
	/* 上电之后，这些任务就开始一直执行 */
	MidSchAddTask(app_task_10ms,10);
	MidSchAddTask(app_task_20ms,20);
	MidSchAddTask(app_task_50ms,50);
	MidSchAddTask(app_task_100ms,100);
	MidSchAddTask(app_task_500ms,500);
	MidSchAddTask(app_task_1000ms,1000);
	MidSchAddTask(app_task_2000ms,2000);
	MidSchAddTask(app_task_5000ms,5000);
	//MidSchAddTask(task_mid_wf_test, 250);
}	

static void app_hardware_init(void)
{
	// PLL_CLK_MCLK_8M_PLL_80M_CCLK_40M_PCLK_TCLK_40M();
	// PLL_CLK_MCLK_4M_PLL_80M_CCLK_80M_PCLK_TCLK_40M();
	PLL_CLK_MCLK_4M_PLL_80M_CCLK_40M_PCLK_TCLK_40M();
    hal_gpio_Init();							/* initiate GPIO ports */
    app_cfg_io_in();	/* init io in and out  在app_cfg_io函数中有调用 */
    mid_switch_init(PIN_IN_ALL);							
	app_cfg_io();
	app_uart_frame_init();
	uart_open(UART_A20_CHN,115200, 8, 1, 0);		/* 不校验 */
	dbg_init();	// 初始化debug 串口
	/* AT24CXX_Init(); */
    MidSchInit();
    MidSchStart();
    mid_adc_init();
    app_init_can();
    mid_DS1302_open(DS1302_io_config);
	app_trip_init();
	//app_lin_init();

}

static void app_software_init(void)
{
	variable_init();
	//app_init_led();
	app_buz_init();
	//app_init_fuel();
	//HW_Version_Read_Init();
	//app_buz_play_WAV_Init();
	//app_door_init();
	app_gear_init();
	app_user_setting_init();
	app_tpms_init();
	
	app_radar_init();
	app_key_init();
	//app_IPconfig_init();
	app_service_init();
	app_drivetime_init();
	app_init_temp();
	app_init_rpm();
	app_init_spd();
	app_CarTemp_init();	
	first_state(PWR_OFF);   
	//for 206
	ad_capture_info_init();

}

/** 打印GPIO的数值 **/
static void print_gpio(void)
{
#ifdef __DEBUG__
	dbg_string("   PDR , DDR , PFR \n");
	//dbg_string("01:0x%02x, 0x%02x, 0x%02x\n", PDR01, DDR01, PFR01);
	dbg_string("02:0x%02x, 0x%02x, 0x%02x\n", PDR02, DDR02, PFR02);
	dbg_string("03:0x%02x, 0x%02x, 0x%02x\n", PDR03, DDR03, PFR03);
	dbg_string("04:0x%02x, 0x%02x, 0x%02x\n", PDR04, DDR04, PFR04);
	dbg_string("05:0x%02x, 0x%02x, 0x%02x\n", PDR05, DDR05, PFR05);
	dbg_string("06:0x%02x, 0x%02x, 0x%02x\n", PDR06, DDR06, PFR06);
	//dbg_string("07:0x%02x, 0x%02x, 0x%02x\n", PDR07, DDR07, PFR07);
	dbg_string("14:0x%02x, 0x%02x, 0x%02x\n", PDR14, DDR14, PFR14);
	dbg_string("15:0x%02x, 0x%02x, 0x%02x\n", PDR15, DDR14, PFR15);
#endif
}

void app_sys_init(void)
{
    __DI();
    app_hardware_init();
    app_software_init();
    print_gpio();
    app_tasks_init();
/*enable VCC_4V2_EN*/
#if 0
	set_single_io_dir(0,2,IO_GENERAL_OUTPUT);
	set_single_io_sts(0,2,1);

	set_single_io_dir(1,0,IO_GENERAL_OUTPUT);
	set_single_io_sts(1,0,1);

#endif
    __EI();
}
/* ig off -> on action */
void app_igon_action(void) /*IGN 从OFF 到ON之后 */
{
   	MidSchAddTask(app_can_sent_task,50);
    MidSchAddTask(app_io_task,10);
	//app_A20_power_on();
	//MidSchAddTask(app_backlight_ctl,100);
	//MidSchAddTask(app_buz_play_WAV_task,10000);
}
/* ig on -> off action */
void app_igoff_action(void) /* IGN 从ON 到OFF之后 */
{
	MidSchDeleteTask(app_can_sent_task);
	MidSchDeleteTask(app_io_task);
	app_leds_all_off_test();
	//app_A20_power_off();
}

void app_task_10ms(void)
{
	dbg_string(">>mid_switch_task10\n");
	mid_switch_task10(pin_io_in, pin_filter_in); /* 得到滤波后输入的值 */
	dbg_string(">>app_power_manager_task10\n");
	app_power_manager_task10(); /* 实现系统电源状态的切换 */
	dbg_string(">>mid_can_get_task10\n");
	mid_can_get_task10(); /* 实现CAN 报文的解析 */
	mid_can1_get_task10(); /* 实现CAN 报文的解析 */
	//app_lin_task10(); /* 实现LIN报文的解析 */
	dbg_string(">>app_buz_ctl\n");
	app_buz_ctl();	
	dbg_string(">>end\n");
	mid_adc_manager_task();	/* ADC管理，所有有关ADC的APP层操作必须放在这后面 */
}

void app_task_20ms(void)
{
	dbg_string(">>app_process_spd_task\n");
	app_process_spd_task();
	dbg_string(">>task_work_flash\n");
	task_work_flash();	/* work flash异步擦写 */	
	dbg_string(">>app_uds_task\n");
	app_uds_task();
	dbg_string(">>app_frame_get_task20\n");
	app_frame_get_task20();
	dbg_string(">>end\n");
	
}
void app_task_50ms(void)
{
	dbg_string(">>app_process_rpm_task\n");
	app_process_rpm_task();
	//dbg_string(">>app_radar_ctl\n");
	//app_radar_ctl();
	//app_pro_led();
	dbg_string(">>mid_adc_manager_task\n");
	
	dbg_string(">>end\n");
	
}

void app_task_100ms(void)
{
	dbg_string(">>app_info_task100\n");
	app_info_task100();
	wdg_feed();
//	mid_ds1302_time_update(&(mcu_rec_general.settingsInfo.Time.u32Time));
	dbg_string(">>app_update_time_task\n");
	app_update_time_task();
	wdg_feed();
	//app_pro_temp_task();
	//wdg_feed();
	//app_tpms_task();
	//wdg_feed();
	//app_gear_pro_100ms();
	//wdg_feed();
	//fuel_ins_consumption_cal();
	//wdg_feed();
	dbg_string(">>app_key_scan_task_100ms\n");
	app_key_scan_task_100ms();	/* 按键任务 */
	wdg_feed();
	//app_get_door_sts_100ms();	/* 车门状态 */
	//wdg_feed();
	//app_lin_lost_time_cnt_100ms();
	//wdg_feed();
	//app_can_lost_time_cnt_100ms();
	//wdg_feed();
	dbg_string(">>app_frame_sent_task\n");
	app_frame_sent_task(); 		/* 工程模式发送串口框架任务 */
	//wdg_feed();
//	app_demo_frame_sent_task(); /* 普通模式发送串口框架任务 */
//	app_read_fuel_ign_on();	/* 读取初始燃油量 */	
//  app_buz_play_WAV_task(); /* 播放WAV音频任务，只执行一次 */	
	//app_clac_fuel_method2_task();
	//wdg_feed();


	//add for 206
	//void ad_capture_info_init(void);
	dbg_string(">>ad_capture_info_get_data\n");
	ad_capture_info_get_data();
	dbg_string(">>end\n");
}

/* 检查UART是否还有ARM的信息，*/
static void app_check_arm_alive(void)
{
	SYS_POWER_STATE pwr;

	if(get_uart_first_frame() == FALSE) /* 未收到串口来的第一帧 */
	{
		uart_set_heartbeat_tick(hal_timer_get_tick());
		return;
	}
		/* 只检测已激活仪表 */
	if (IPconfig.Sts != 1) return;
	
	pwr = app_get_power_sts();
	if (pwr != PWR_OFF)	/* 深度休眠状态，ARM已经关机，不重启ARM */
	{
		app_uart_arm_alive();
	}
	else	/* 未深度休眠时更新UART心跳 */
	{
		uart_set_heartbeat_tick(hal_timer_get_tick());
	}
	


}

void app_task_1000ms(void)
{
	dbg_string(">>app_show_HW_version\n");
	app_show_HW_version(); /* 打印硬件版本信息，只执行一次 */
	dbg_string(">>app_rest_mile_cal_task\n");

	app_rest_mile_cal_task();
	dbg_string(">>app_drivetime_task_1000ms\n");
	app_drivetime_task_1000ms();
	dbg_string(">>app_get_rest_service_task_1000ms\n");
	app_get_rest_service_task_1000ms();
	dbg_string(">>app_cal_avg_fuel_consump\n");
	app_cal_avg_fuel_consump();
	//app_IPconfig_pro_1000ms();
	dbg_string(">>app_check_arm_alive\n");
	app_check_arm_alive();	/* 检查ARM是否活着，如果否就给ARM重新上电 */
	dbg_string(">>app_calc_this_trip_AFE_task\n");
	app_calc_this_trip_AFE_task();
	dbg_string(">>end\n");

}

static void app_task_2000ms(void)
{
	/*随机改变演示模式的数据*/
	//app_display_rand_data_task(); 
}

static void app_task_5000ms(void)
{
		dbg_string(">>app_get_CarTemp_5000ms\n");
		app_get_CarTemp_5000ms();
		dbg_string(">>end\n");
}



void app_task_500ms( void )
{
		
	//app_pro_fuel_task();	/* 剩余燃油量计算 */	
}

void app_soft_reset(void)
{
	save_fuel_ad();	/* 保存燃油量 */
	mid_flash_sync();	/* 同步Flash, 因为要断电了 */
	hal_soft_reset();
}
