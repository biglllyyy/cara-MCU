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

//U8 gSimIgnOff = 0;	/* ģ��IGN OFF�ã��������� */
void keep_state(void);

static void state_pwr_nml(STATE_ACTION action);
static void state_pwr_check(STATE_ACTION action);
static void state_pwr_pre_sleep(STATE_ACTION action);
static void state_pwr_sleep(STATE_ACTION action);
static void state_pwr_off(STATE_ACTION action);
/***  ��ӡ��ǰ��Դ״̬ ***/
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


/* �ر�ARM��Դ */
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

/* ��ARM�ϵ� */
void app_A20_power_on(void)
{
	U8 st = mid_get_io_sts(&VCC_4V2_EN);
	
    if (!st) A20_POWER_ON();
	mid_set_io_sts(&BL_GMA_SD,ON);/* open LCD data EN */
	dbg_string("Power ON ARM\n");

	//app_backlight_init();
    g_u8_A20_power_sts= ON;
    g_u8_power_down_req = 0;
	//MidSchAddTask(app_frame_get_task,20);//��ʼ����ARM���͵�����  for 206
     //MidSchAddTask(app_frame_get_task20,20);
//	g_u16_led_check_times = 0;
//	led_check_time=0;
}

/*
	ϵͳ��Դ״̬�����ڸ���״̬���л�
*/
void app_power_manager_task10(void)
{
    static U8 ig_sts_bak = 0;

    g_u8IgnSts = mid_get_io_filtered(pin_filter_in, PIN_IN_WAKEUP3);	/*��ȡ�˲���IGN����*/

	/* ģ��IGN OFF���������� */
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

    if(ig_sts_bak != g_u8IgnSts)//IGN OFF��ON״̬�л�ʱ
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
    case ENTER:	//Clean_play_WAV_flag(); /* ʹ�ܲ���WAV */
        break;
    case LEAVE: 
        break;
    case KEEP:		
        if((OFF == g_u8IgnSts)&&(g_u16_ig_off_time >= IGN_OFF_2_SLEEP_TIME))	/* Ϩ��ʱ�䳬�������� */
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
		mid_set_io_sts(&BUZ_CTL_IO,ON); /* �򿪷����� */
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
        	if(is_timer_out(&lcd_on_time,1000))//�ȴ�1000ms֮�󣬴򿪱���
        	{
				del_timer(&lcd_on_time);
				app_backlight_init();        	    
        	}
        }

		if(is_backlight_ON()==1)
		{
			if(IPconfig.Sts==1)//�Ǳ��Ѽ���
			{
				reboot_ARM();
	        }
			else /*�Ǳ�δ����*/
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
			g_u8_pre_sleep_flag = ON;/*ǳ˯��������־λ*/
			enter_pre_sleep_tick = hal_timer_get_tick();
				
			/* ���������ﱣ�棬��ΪWork flash�ı������첽�ģ������ڶϵ�ǰһ˲���ٱ��� */
			/* ���С�� */
			write_sub_trip1(info.Trip1);
			write_sub_trip2(info.Trip2);

			/* �洢��ʻʱ����Ϣ */
			save_ee_drive_time();
			
			/* ����ƽ���ͺ������Ϣ */
			save_ee_AFE_fuel_used();
			save_ee_AFE_distance();			
						
			mcu_rec_general.settingsInfo.first.bits.closeReq = 0;
				
			uart_special_data.specialInfo.LedWarning.third.bit.ClearTrip1=0;
			uart_special_data.specialInfo.LedWarning.third.bit.ClearTrip2=0;

	        g_u8_power_down_req = 1;
			g_u16_wait_A20_down_time=0;
		break;
		case LEAVE:
			g_u8_pre_sleep_flag = OFF;/*ǳ˯��������־λ*/
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
				/*�յ��ػ�����IGNOFF��40sδ�յ��������󣬹ر�CORE��*/
				if(get_uart_close_lcd_req() == 1 || \
					hal_timer_passed(enter_pre_sleep_tick) >= 40*HZ)
				{
					//dbg_string("time_passed:%d\n",hal_timer_passed(enter_pre_sleep_tick));
					mcu_rec_general.settingsInfo.first.bits.closeReq=0;
        			next_state(PWR_OFF);
				}	
				/*�����⵽�ſ��źţ����¼�ʱ*/
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
		can_chip_mode_switch(&io_can_stb, &io_can_en, GO_TO_SLEEP_MODE);//�ر�CAN�շ���
		app_lin_stop();//�ر�LIN�շ���
        hal_freeruntimer_stop(BEEP_FRT_CHN);//�رշ�����
        mid_set_io_sts(&BUZ_CTL_IO,OFF);/* �رշ����� */
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
			door_open_wake_up=app_get_door_open_sts();	/* �ж�POWER OFF KEEP�ڼ���û���ſ� */

			if((ON == g_u8IgnSts)||(door_open_wake_up)) /* �����������£���� ���� �ſ����� �������Ļ */
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

/*** ����ģʽ�£���ӡ��ǰ��Դ״̬ ***/
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
		if(is_timer_out(&core_startup_time,30000))	/* CORE��������ʱ������������CORE�� */
		{
			del_timer(&core_startup_time);
			/*** 30s�� ���¿�����ʱ�� ***/
			add_timer(&core_startup_time);
					
			/* ģ��IGN OFF���������� */
			/*
				gSimIgnOff = 1;
				next_state(PWR_PRE_SLEEP);
			*/
					
			/*** ����״̬�£�30s���������İ�Ҳ���������磬��������ں˳��� ***/
			#ifndef __DEBUG__
				if(mid_get_io_sts(&pin_io_in[PIN_IN_ARM_LOAD]) == ON) /* ���������ð���ͣ��򲻻����� */
				{
					next_state(PWR_OFF);
				}
			#endif		
			dbg_string("TEST 30s NO Communication\n");
		}
		else
		{
		
			if(1 == g_U8_core_start_work)//CORE�������ɹ�
			{
				del_timer(&core_startup_time);
				next_state(PWR_NML);
			}
		} 
	}
}

