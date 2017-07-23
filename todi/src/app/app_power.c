#include <stdlib.h>
#include "app_power.h"
#include "app_system.h"
#include "app_backlight.h"
#include "app_led.h"
#include "app_buzzer.h"
#include "app_can.h"
#include "app_io.h"
#include "app_uart_cmd.h"
#include "mid_time.h"
#include "mid_scheduler.h"
#include "hal_ext_int.h"
#include "hal_low_power.h"
#include "hal_wdg.h"
#include "hal_reload_timer.h"
#include "hal_free_run_timer.h"
#include "mb91520.h"
#include "g_variable.h"
#include "comm_typedef.h"
#include "mid_ds1302.h"
#include "mid_switch.h"
#include "io_cfg.h"
#include "app_lin.h"
#include "app_trip.h"
#include "app_fuel.h"
#include "app_door.h"
#include "app_IPconfig.h"
#include "app_drivetime.h"
#include "app_data_store.h"
#include "g_variable.h"



#define IGN_OFF_2_SLEEP_TIME    200
#define GLOBLE_CAN_LOST_TIME    500
#define GLOBLE_LIN_LOST_TIME    500

TIMER_TYPE  lcd_on_time = {0};
TIMER_TYPE  core_startup_time = {0};
U8 IP_sts=0;


static U8 door_open_wake_up=0;
U8   g_u8_pre_sleep_flag = 0;			/*pre sleep state*/

//U8 gSimIgnOff = 0;	/* 模拟IGN OFF用，仅调试用 */
void keep_state(void);

static void state_pwr_nml(STATE_ACTION action);
static void state_pwr_check(STATE_ACTION action);
static void state_pwr_pre_sleep(STATE_ACTION action);
static void state_pwr_sleep(STATE_ACTION action);
static void state_pwr_off(STATE_ACTION action);
/***  打印当前电源状态 ***/
static void print_pwr_state(SYS_POWER_STATE  state);

struct
{
    SYS_POWER_STATE current;
    SYS_POWER_STATE next;
    state_handle list[PWR_ALL];
} pwr_state =
{
    PWR_PRE_SLEEP,
    PWR_PRE_SLEEP,
    {
        /* the order must same as the  SLEEP_MODE order*/
        state_pwr_nml,
        state_pwr_check,
        state_pwr_pre_sleep,
		state_pwr_off,
        state_pwr_sleep,
    },
};


/* 关闭ARM电源 */
void app_A20_power_off(void)
{
   // MidSchDeleteTask(app_backlight_ctl);
	MidSchDeleteTask(app_frame_sent_task);
	//MidSchDeleteTask(app_frame_get_task);  for 206
	MidSchDeleteTask(app_frame_get_task20);
    if(is_timer_added(&lcd_on_time))
    {
        del_timer(&lcd_on_time);
    }
    app_backlight_off();
    g_u8_A20_power_sts= OFF;
	g_U8_core_start_work = 0;
    A20_POWER_OFF();
	app_uart_frame_init();	//clear uart data
    mcu_rec_general.settingsInfo.first.bits.closeReq = 0;
    mcu_rec_general.settingsInfo.first.bits.commandReq = 0;
    g_u8_uart_req_bak = 0;
}

/* 给ARM上电 */
void app_A20_power_on(void)
{
	U8 st = mid_get_io_sts(&VCC_4V2_EN);
	
    if (!st) A20_POWER_ON();
	mid_set_io_sts(&BL_GMA_SD,ON);/* open LCD data EN */
	dbg_string("Power ON ARM\n");

	//app_backlight_init();
    g_u8_A20_power_sts= ON;
    g_u8_power_down_req = 0;
	//MidSchAddTask(app_frame_get_task,20);//开始接受ARM发送的数据  for 206
     //MidSchAddTask(app_frame_get_task20,20);
//	g_u16_led_check_times = 0;
//	led_check_time=0;
}

/*
	系统电源状态任务，在各个状态间切换
*/
void app_power_manager_task10(void)
{
    static U8 ig_sts_bak = 0;

    g_u8IgnSts = mid_get_io_filtered(pin_filter_in, PIN_IN_WAKEUP3);	/*获取滤波后IGN输入*/

	/* 模拟IGN OFF，仅调试用 */
	/* 
	if (gSimIgnOff == 1)
	{
		g_u8IgnSts = OFF;
		mcu_rec_general.settingsInfo.first.bits.closeReq = 1;
	}
	*/
	
    if(OFF== ig_sts_bak)//IGN OFF
    {
        g_u16_ig_off_time++;
        if(g_u16_ig_off_time >= IGN_OFF_2_SLEEP_TIME)	/* 2s */
        {
            g_u16_ig_off_time = IGN_OFF_2_SLEEP_TIME;
        }
    }
    else//IGN ON
    {
        g_u16_ig_off_time = 0;
    }

    if(ig_sts_bak != g_u8IgnSts)//IGN OFF和ON状态切换时
    {
		ig_sts_bak = g_u8IgnSts;
        if(g_u8IgnSts)
        {
            app_igon_action();			/* ig from off-> on action */
        }
        else
        {
            app_igoff_action();			/* ig from on -> off action */
        }
    }
    keep_state();						/* state  switch */
}


void first_state(SYS_POWER_STATE state)
{
    pwr_state.current = state;
    pwr_state.next = pwr_state.current;
    pwr_state.list[pwr_state.current](ENTER);
}


void next_state(SYS_POWER_STATE  state)
{
    pwr_state.next = state;
    print_pwr_state(state);
}

void keep_state(void)
{
    if(pwr_state.current == pwr_state.next)
    {
        pwr_state.list[pwr_state.current](KEEP);
    }
    else
    {
        pwr_state.list[pwr_state.current](LEAVE);
        pwr_state.current = pwr_state.next;
        pwr_state.list[pwr_state.current](ENTER);
    }
}


static void state_pwr_nml(STATE_ACTION action)
{
    switch(action)
    {
    case ENTER:	//Clean_play_WAV_flag(); /* 使能播放WAV */
        break;
    case LEAVE: 
        break;
    case KEEP:		
        if((OFF == g_u8IgnSts)&&(g_u16_ig_off_time >= IGN_OFF_2_SLEEP_TIME))	/* 熄火时间超过两秒钟 */
        {
            next_state(PWR_PRE_SLEEP);
        }
        break;
        /* add your code here */
    default:
        break;
    }
}

static void state_pwr_check(STATE_ACTION action)
{	
	switch(action)
    {
    case ENTER:
        app_A20_power_on();  
		mid_set_io_sts(&BUZ_CTL_IO,ON); /* 打开蜂鸣器 */
		add_timer(&core_startup_time);
		add_timer(&lcd_on_time);
        break;
    case LEAVE:
        del_timer(&lcd_on_time);
        del_timer(&core_startup_time);
		
        break;
    case KEEP:
		if(is_timer_added(&lcd_on_time)==TRUE)
		{
        	if(is_timer_out(&lcd_on_time,1000))//等待1000ms之后，打开背光
        	{
				del_timer(&lcd_on_time);
				app_backlight_init();        	    
        	}
        }

		if(is_backlight_ON()==1)
		{
			if(IPconfig.Sts==1)//仪表已激活
			{
				reboot_ARM();
	        }
			else /*仪表未激活*/
			{
				next_state(PWR_NML);
			}
		}		
        
       	/*if((OFF == g_u8IgnSts)&&(0 == g_U8_core_start_work))
		{
				next_state(PWR_OFF);
		}*/
        break;
        default:
        break;
    }
}


static void state_pwr_pre_sleep(STATE_ACTION action)
{
	static U32 enter_pre_sleep_tick;
	
	switch(action)
	{
		case ENTER:
			g_u8_pre_sleep_flag = ON;/*浅睡眠升级标志位*/
			enter_pre_sleep_tick = hal_timer_get_tick();
				
			/* 必须在这里保存，因为Work flash的保存是异步的，不能在断电前一瞬间再保存 */
			/* 里程小计 */
			write_sub_trip1(info.Trip1);
			write_sub_trip2(info.Trip2);

			/* 存储行驶时间信息 */
			save_ee_drive_time();
			
			/* 保存平均油耗相关信息 */
			save_ee_AFE_fuel_used();
			save_ee_AFE_distance();			
						
			mcu_rec_general.settingsInfo.first.bits.closeReq = 0;
				
			uart_special_data.specialInfo.LedWarning.third.bit.ClearTrip1=0;
			uart_special_data.specialInfo.LedWarning.third.bit.ClearTrip2=0;

	        g_u8_power_down_req = 1;
			g_u16_wait_A20_down_time=0;
		break;
		case LEAVE:
			g_u8_pre_sleep_flag = OFF;/*浅睡眠升级标志位*/
			can_lost.all_lost_keep_time = 0;
			g_u16_wait_A20_down_time = 0;
			mcu_rec_general.settingsInfo.first.bits.closeReq = 0;
			
		break;
		case KEEP:
			if(g_u8IgnSts == ON)
			{
				next_state(PWR_NML);
			}
			else 
			{				 	
				/*收到关机请求、IGNOFF后40s未收到关屏请求，关闭CORE板*/
				if(get_uart_close_lcd_req() == 1 || \
					hal_timer_passed(enter_pre_sleep_tick) >= 40*HZ)
				{
					//dbg_string("time_passed:%d\n",hal_timer_passed(enter_pre_sleep_tick));
					mcu_rec_general.settingsInfo.first.bits.closeReq=0;
        			next_state(PWR_OFF);
				}	
				/*如果检测到门开信号，重新计时*/
				if(app_get_door_open_sts() == ON)
					enter_pre_sleep_tick = hal_timer_get_tick();
			}
		break;
		default:
		break;
	}
}

static void state_pwr_sleep(STATE_ACTION action)
{

    switch(action)
    {
    case ENTER:
        can_lost.all_lost_keep_time = 0;
        g_u16_wait_A20_down_time = 0;
        
        
        //mid_set_output_pin_highz();
		can_chip_mode_switch(&io_can_stb, &io_can_en, GO_TO_SLEEP_MODE);//关闭CAN收发器
		app_lin_stop();//关闭LIN收发器
        hal_freeruntimer_stop(BEEP_FRT_CHN);//关闭蜂鸣器
        mid_set_io_sts(&BUZ_CTL_IO,OFF);/* 关闭蜂鸣器 */
        hal_extern_interrupt_init(IGN_WAKE_INT_CHN,HIGH_LEVEL);					/* set ig wake up interrupt */
        hal_extern_interrupt_init(LIN_WAKE_INT_CHN,LOW_LEVEL);					/* set lin wake up interrupr */
        
        break;
    case LEAVE:
        /* add your code herer */
        g_u16_ig_off_time = 0;
        //mid_switch_init();
        app_init_can();
        app_lin_init();
        break;
    case KEEP:

        hal_reloadtimer_stop(3);
        
        hal_ext_int_start(IGN_WAKE_INT_CHN);				/* start ign wake up interrupt */
        hal_ext_int_start(LIN_WAKE_INT_CHN);
        
        wdg_feed();
		mid_set_io_sts(&MCU_HOLD_ON,OFF);
				
        while((FALSE == g_u8IgnWakeUpFlg) && (FALSE == g_u8LinWakeUpFlg))
        {
			wdg_feed();
//           	DS1302_de_init();
           	app_leds_all_off_test();//2015/10/16 14:13:41
            hal_entry_low_power_mode();	/* MCU Stop */
            wdg_feed();
        }
        hal_exit_low_power_mode();
        
        if((g_u8IgnWakeUpFlg)||(g_u8LinWakeUpFlg))
        {
            g_u8IgnWakeUpFlg = 0;
            hal_ext_int_stop(IGN_WAKE_INT_CHN);	  /* stop IGN wake up interrupt */
            g_u8LinWakeUpFlg = 0;
            hal_ext_int_stop(LIN_WAKE_INT_CHN);	  /* stop lin wake up interrupt */
        }
        
        variable_init();
		next_state(PWR_OFF);

    default:
        break;
    }
}

/************************************************/
static void state_pwr_off(STATE_ACTION action)
{
	switch(action)
 	{
 		case ENTER:
			app_A20_power_off();
		break;
		case LEAVE:
			can_lost.all_lost_keep_time = 0;
			door_open_wake_up=0;
		break;
		case KEEP:
			door_open_wake_up=app_get_door_open_sts();	/* 判断POWER OFF KEEP期间有没有门开 */

			if((ON == g_u8IgnSts)||(door_open_wake_up)) /* 在屏灭的情况下，点火 或者 门开动作 都会打开屏幕 */
			{
				door_open_wake_up=0;
				next_state(PWR_CHECK);
			}
			else
			{
				if(lin_lost_timecnt>GLOBLE_LIN_LOST_TIME)
				{
					next_state(PWR_SLEEP);
				}
			}
		default:
		break;
	}
}

/************************************************/

SYS_POWER_STATE app_get_power_sts(void)
{
    return  (pwr_state.current);
}
/*************************************************/

/*** 调试模式下，打印当前电源状态 ***/
static void print_pwr_state(SYS_POWER_STATE  state){
#ifdef __DEBUG__
	switch (state)
	{
		case PWR_NML:
			dbg_string("now state PWR_NML\n");
			break;
			
		case PWR_CHECK:
			dbg_string("now state PWR_CHECK\n");
			break;
			
		case PWR_PRE_SLEEP:
			dbg_string("now state PWR_PRE_SLEEP\n");
			break;

		case PWR_OFF:
			dbg_string("now state PWR_OFF\n");
			break;

		case PWR_SLEEP:
			dbg_string("now state PWR_SLEEP\n");
			break;
			
		default:
			dbg_string("now state state_pwr_error\n");
			break;
	}
#endif
}

void reboot_ARM(void)
{
	if (is_timer_added(&core_startup_time) == TRUE)
	{
		if(is_timer_out(&core_startup_time,30000))	/* CORE板启动超时，则重新启动CORE板 */
		{
			del_timer(&core_startup_time);
			/*** 30s后 重新开启定时器 ***/
			add_timer(&core_startup_time);
					
			/* 模拟IGN OFF，仅调试用 */
			/*
				gSimIgnOff = 1;
				next_state(PWR_PRE_SLEEP);
			*/
					
			/*** 调试状态下，30s不启动核心板也不给它掉电，方便调试内核程序 ***/
			#ifndef __DEBUG__
				if(mid_get_io_sts(&pin_io_in[PIN_IN_ARM_LOAD]) == ON) /* 如果插上跳冒拉低，则不会重启 */
				{
					next_state(PWR_OFF);
				}
			#endif		
			dbg_string("TEST 30s NO Communication\n");
		}
		else
		{
		
			if(1 == g_U8_core_start_work)//CORE板启动成功
			{
				del_timer(&core_startup_time);
				next_state(PWR_NML);
			}
		} 
	}
}

