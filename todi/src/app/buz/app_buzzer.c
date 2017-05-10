#include <stdlib.h>
#include "app_buzzer.h"
#include "mid_buzzer.h"
#include "buzzer_cfg.h"
#include "mid_scheduler.h"
#include "mid_trigger.h"
#include "hal_reload_timer.h"
#include "string.h"
#include "hal_free_run_timer.h"
#include "app_can.h"
#include "app_power.h"
#include "app_led.h"
#include "app_spd.h"
#include "g_variable.h"
#include "mid_time.h"
#include "mid_led.h"
#include "app_CarKey.h"
#include "app_IPconfig.h"
#include "app_uart_cmd.h"
#include "app_wavdata.h"
#include "app_door.h"
#include "app_fuel.h"
#include "mid_dbg.h"
#include "app_rpm.h"
#include "app_fuel_method2.h"

/* one type sound can store 32  sound source */
U16		buz_sts[BEEPCOUNT] = {0};
static beep_turn_warning_e beep_turn_warning;
static U8 beep_120kmph_warning;
//static U8 play_WAV_flag = 1; /* 启动时不播放 */


static buz_handle buz_hdl[BEEPCOUNT] = {NULL}; 
/*
static const U8 tone[]=
{
 	64, 0, CHORD_A4|CHORD_EN, 128, 0, CHORD_A4|CHORD_EN, 192, 0, CHORD_C4|CHORD_EN,
	0, 3, CHORD_EoS|CHORD_EN
};
*/

static const U8 tone_ding[]=
{
 	0, 1, CHORD_G4|CHORD_EN,
	0, 2, CHORD_G4|CHORD_EN,
	0, 3, CHORD_EoS|CHORD_EN
};

static const U8 tone_dong[]=
{
 	0, 1, CHORD_C4|CHORD_EN, 
	0, 2, CHORD_C4|CHORD_EN, 
	0, 3, CHORD_EoS|CHORD_EN
};

static const U8 tone_startup[]=
{
 	50, 0, CHORD_A4|CHORD_EN, 100, 0, CHORD_C4|CHORD_EN, 150, 0, CHORD_E4|CHORD_EN,0, 4, CHORD_A5|CHORD_EN,
	0, 4, CHORD_EoS|CHORD_EN
};

/********************* function declare ***********************************/
static void app_buz_init_sound_kind(void);
static void app_buz_fuction_register(void);
static void app_buz_turn(U16 action);					/* 转向 */
static void app_buz_key_not_out(U16 action);	
//static void app_buz_safe_selt_off_first(U16 action);
static void app_buz_safe_selt_off_second(U16 action);
static void app_buz_light_on(U16 action);
static void app_buz_radar_level_4(U16 action);
static void app_buz_radar_level_3(U16 action);
static void app_buz_radar_level_2(U16 action);
static void app_buz_radar_level_1(U16 action);
static void app_buz_radar_failure(U16 action);
static void app_buz_radar_active(U16 action);
static void app_buz_spd_120kmph(U16 action);
static void app_buz_fast_leakagestates(U16 action);
static void app_buz_esp_closed(U16 action);
static void app_buz_brake(U16 action);
static void app_buz_door(U16 action);
static void app_buz_fuel_empty(U16 action);
static void app_buz_startup(U16 action);


/*********************************************************************/

/*周期的处理各种蜂鸣器*/
/*该函数10ms执行一次*/
void app_buz_ctl(void)
{
    //buz_logic_process();
	U8 index = 0;
	//conditon check 
	for(index = 0;index<BEEPCOUNT;index++)
	{
		if(buz_hdl[index] != NULL)
		{
			buz_hdl[index](ACT_TOUCH);//根据条件控制报警音ON或者OFF
		}
	}
	
	process_buzzer();	
}

//上电的时候初始化一次
void app_buz_init(void)
{
	U8 index = 0;
	app_buz_init_sound_kind();		//init your sound kind
	app_buz_fuction_register();		//register your  process function 
	memset(&buz_sts,0,sizeof(buz_sts)/sizeof(buz_sts[0]));
	for(index = 0;index<BEEPCOUNT;index++)
	{
		if(buz_hdl[index] != NULL)
		{
			buz_hdl[index](ACT_RESET);				//init the static variable
		}
	}
}

/* 播放WAV音频初始化 */
//void app_buz_play_WAV_Init(void)
//{
//	unsigned long int n = 0;
//	hal_freeruntimer_Init(1,125); /* 设置播放WAV的定时器，是free_run_timer1,1s中断125次 */
//	get_WAV_data(n,return_wavdata_addr);
//}

/* 播放WAV音频任务 */
//void app_buz_play_WAV_task(void)
//{
//	if(play_WAV_flag != 0) 
//		return;
//	hal_freeruntimer_start(1); /* 打开播放WAV音频格式的定时器 */
//	play_WAV_flag = 1;		
//}

//void Set_play_WAV_flag(void)
//{
//	play_WAV_flag = 1;
//}

//void Clean_play_WAV_flag(void)
//{
//	play_WAV_flag = 0;
//}


static  void app_buz_init_sound_kind(void)
{
	static buzzer  buzzer_turn =
	{
		BEEP_TURN,
		Hz(62500),
		0, 1,  /* cycle index, total*/
		0, 2, /*  onoff index, length*/
		on(10), off(0) /* onoff.data[]  */
	};
	static buzzer  buzzer_key_not_out=
	{
		BEEP_KEY_NOT_OUT,
		Hz(2000),
		0, 4,  /* cycle index, total*/
		0, 2, /*  onoff index, length*/
		on(100), off(50) /* onoff.data[] */
	};
//	static buzzer  buzzer_safe_selt_off_first =
//	{
//		BEEP_SAFE_SELT_OFF_FIRST,
//		Hz(2000),
//		0, 6,  /* cycle index, total*/
//		0, 2, /*  onoff index, length*/
//		on(500), off(500) /* onoff.data[]  */
//	};
	static buzzer  buzzer_safe_selt_off_second=
	{
		BEEP_SAFE_SELT_OFF_SECOND,
		Hz(BUZZER_CHORD_FREQUENCY),
		0, 2,  /* cycle index, total*/
		0, 2, /*  onoff index, length*/
		on(2000), off(1000), /* onoff.data[]  */
		tone_ding
	};
	static buzzer  buzzer_light_on =//TBD	
	{
		BEEP_LIGHT_ON,
		Hz(2000),
		0, 60,  /* cycle index, total*/
		0, 2, /*  onoff index, length*/
		on(500), off(500), /* onoff.data[]  */
		NULL
	};
	static buzzer  buzzer_radar_level_4 =
	{
		BEEP_RADAR_LEVEL_4,
		Hz(800),
		0, 0,  /* cycle index, total*/
		0, 2, /*  onoff index, length*/
		on(80), off(120), /* onoff.data[]  */
		NULL
	};
	static buzzer  buzzer_radar_level_3 =
	{
		BEEP_RADAR_LEVEL_3,
		Hz(800),
		0, 0,  /* cycle index, total*/
		0, 2, /*  onoff index, length*/
		on(100), off(50), /* onoff.data[]  */
		NULL
	};
	static buzzer  buzzer_radar_level_2 =
	{
		BEEP_RADAR_LEVEL_2,
		Hz(800),
		0, 0,  /* cycle index, total*/
		0, 2, /*  onoff index, length*/
		on(80), off(20),/* onoff.data[]  */
		NULL
	};
	static buzzer  buzzer_radar_level_1 =
	{
		BEEP_RADAR_LEVEL_1,
		Hz(800),
		0, 0,  /* cycle index, total*/
		0, 2, /*  onoff index, length*/
		on(100), off(0) ,/* onoff.data[]  */
		NULL
	};
	static buzzer  buzzer_radar_failure =
	{
		BEEP_RADAR_FAILURE,
		Hz(2000),
		0, 0,  /* cycle index, total*/
		0, 2, /*  onoff index, length*/
		on(1500), off(500), /* onoff.data[]  */
		NULL
	};
	static buzzer  buzzer_radar_active =
	{
		BEEP_RADAR_ACTIVE,
		Hz(800),
		0, 1,  /* cycle index, total*/
		0, 2, /*  onoff index, length*/
		on(100), off(100), /* onoff.data[]  */
		NULL
	};
	static buzzer buzzer_spd_120kmph=
	{
		BEEP_SPD_120KMPH,
		Hz(1666),
		0,10,/* cycle index, total*/
		0,2,/*  onoff index, length*/
		on(570),off(570),/* onoff.data[]  */
		NULL
	};
	static buzzer buzzer_fast_leakagestates=//TBD
	{
		BEEP_FAST_LEAKAGESTATES,
		Hz(800),
		0,1,/* cycle index, total*/
		0,2,/*  onoff index, length*/
		on(10000),off(0),/* onoff.data[]  */
		NULL
	};
	static buzzer buzzer_esp_closed=
	{
		BEEP_ESP_CLOSED,
		Hz(2000),
		0, 1,  /* cycle index, total*/
		0, 2, /*  onoff index, length*/
		on(100), off(100), /* onoff.data[]  */
		NULL
	};
	static buzzer buzzer_load_success=
	{
		BEEP_LOAD_OK,
		Hz(800),
		0, 4,  /* cycle index, total*/
		0, 2, /*  onoff index, length*/
		on(500), off(500), /* onoff.data[]  */
		NULL
	};
	static buzzer buzzer_brak= /* 行驶中手刹未松 */
	{
		BEEP_BRAKE,
		Hz(500),
		0, 0,  /* cycle index, total*/
		0, 2, /*  onoff index, length*/
		on(50), off(1000), /* onoff.data[]  */
		NULL
	};
	static buzzer buzzer_door= /* 行驶中车门未关 */
	{
		BEEP_DOOR,
		Hz(500),
		0, 0,  /* cycle index, total*/
		0, 2, /*  onoff index, length*/
		on(50), off(1000), /* onoff.data[]  */
		NULL
	};

	static buzzer buzzer_fuel_empty= /* 燃油低报警 */
	{
		BEEP_FUEL_EMPTY,
		Hz(BUZZER_CHORD_FREQUENCY),
		0, 5,  /* cycle index, total*/
		0, 2, /*  onoff index, length*/
		on(2000), off(1000), /* onoff.data[]  */
		tone_dong
	};

	static buzzer buzzer_startup= /* 开机和弦音 */
	{
		BEEP_STARTUP,
		Hz(BUZZER_CHORD_FREQUENCY),
		0, 1,  /* cycle index, total*/
		0, 1, /*  onoff index, length*/
		on(5000), off(10), /* onoff.data[]  */
		tone_startup
	};

	init_buzzer();
	
	buz_init_beep_val(BEEP_TURN,&buzzer_turn);
	buz_init_beep_val(BEEP_KEY_NOT_OUT,&buzzer_key_not_out);
//	buz_init_beep_val(BEEP_SAFE_SELT_OFF_FIRST,&buzzer_safe_selt_off_first);		//buz beep sound variable init
	buz_init_beep_val(BEEP_SAFE_SELT_OFF_SECOND,&buzzer_safe_selt_off_second);
	buz_init_beep_val(BEEP_LIGHT_ON,&buzzer_light_on);
	buz_init_beep_val(BEEP_RADAR_LEVEL_4,&buzzer_radar_level_4);
	buz_init_beep_val(BEEP_RADAR_LEVEL_3,&buzzer_radar_level_3);
	buz_init_beep_val(BEEP_RADAR_LEVEL_2,&buzzer_radar_level_2);
	buz_init_beep_val(BEEP_RADAR_LEVEL_1,&buzzer_radar_level_1);
	buz_init_beep_val(BEEP_RADAR_FAILURE,&buzzer_radar_failure);
	buz_init_beep_val(BEEP_RADAR_ACTIVE,&buzzer_radar_active);
	buz_init_beep_val(BEEP_SPD_120KMPH,&buzzer_spd_120kmph);
	buz_init_beep_val(BEEP_FAST_LEAKAGESTATES,&buzzer_fast_leakagestates);
	buz_init_beep_val(BEEP_ESP_CLOSED,&buzzer_esp_closed);
	buz_init_beep_val(BEEP_LOAD_OK,&buzzer_load_success);
	buz_init_beep_val(BEEP_BRAKE,&buzzer_brak);
	buz_init_beep_val(BEEP_DOOR,&buzzer_door);
	buz_init_beep_val(BEEP_FUEL_EMPTY,&buzzer_fuel_empty);
	buz_init_beep_val(BEEP_STARTUP,&buzzer_startup);
}

static  void app_buz_fuction_register(void)
{
	//register your buz function process here 
	buz_hdl[BEEP_TURN] = app_buz_turn;
	buz_hdl[BEEP_KEY_NOT_OUT] = app_buz_key_not_out;
//	buz_hdl[BEEP_SAFE_SELT_OFF_FIRST] = app_buz_play_WAV_task;
	buz_hdl[BEEP_SAFE_SELT_OFF_SECOND] =app_buz_safe_selt_off_second;
	buz_hdl[BEEP_LIGHT_ON] = app_buz_light_on;
	buz_hdl[BEEP_RADAR_FAILURE] = app_buz_radar_failure;
	buz_hdl[BEEP_RADAR_LEVEL_4] = app_buz_radar_level_4;
	buz_hdl[BEEP_RADAR_LEVEL_3] = app_buz_radar_level_3;
	buz_hdl[BEEP_RADAR_LEVEL_2] = app_buz_radar_level_2;
	buz_hdl[BEEP_RADAR_LEVEL_1] = app_buz_radar_level_1;
	buz_hdl[BEEP_RADAR_ACTIVE] = app_buz_radar_active;
	buz_hdl[BEEP_SPD_120KMPH] = app_buz_spd_120kmph;
	buz_hdl[BEEP_FAST_LEAKAGESTATES] = app_buz_fast_leakagestates;
	buz_hdl[BEEP_ESP_CLOSED]	= app_buz_esp_closed;
	buz_hdl[BEEP_BRAKE]	= app_buz_brake;
	buz_hdl[BEEP_DOOR]	= app_buz_door;
	buz_hdl[BEEP_FUEL_EMPTY] = app_buz_fuel_empty;
	buz_hdl[BEEP_STARTUP] = app_buz_startup;
}

/* 行驶中手刹未松 */
static void app_buz_brake(U16 action)
{
	static U16 state = 0;
    U16 new_state = 0;
	
    switch(action)
    {
    	case ACT_RESET:
    	    state = 0;
    	    break;
    	case ACT_TOUCH:
    	    if((g_u8IgnSts == ON) && (app_get_fact_spd()>0) && (uart_special_data.specialInfo.LedWarning.third.bit.BrakingFault==YES))
    	    {
    	    	new_state=1;
    	    }
    	    else
    	    {
    	    	new_state=0;
    	    }
			touch_state_trigger(app_buz_brake, &state,new_state);
    	    break;
    	case ACT_TRUE:
    			start_buzzer(BEEP_BRAKE);			
    			break;
    	case ACT_FALSE:
    			stop_buzzer(BEEP_BRAKE);			
    	  		break;
    	case ACT_TIMER:
    	    /* when you add_timer(sample_trigger, 1000) in ACT_TURE or ACT_FALSE,
    	    this will handle the MSG_TIMER every 1000ms interval */
    	    break;
    	default:
    	    break;
    }
}


/* 行驶中车门未关 */
static void app_buz_door(U16 action)
{
	static U16 state = 0;
    U16 new_state = 0;
	
	U16 door_state = app_get_door_open_sts();
	U16 fact_spd   = app_get_fact_spd();

    switch(action)
    {
    	case ACT_RESET:
    	    state = 0;
    	    break;
    	case ACT_TOUCH:
			/*车速大于15Km时还有门开信号*/
			if((ON == g_u8IgnSts) && (YES == door_state) && (fact_spd >15))     	    
    	    {
    	    	new_state=1;
    	    }
    	    else
    	    {
    	    	new_state=0;
    	    }
    	  
			touch_state_trigger(app_buz_door, &state,new_state);
    	    break;
    	case ACT_TRUE:
    		start_buzzer(BEEP_DOOR);			
    		break;
    	case ACT_FALSE:
    		stop_buzzer(BEEP_DOOR);
    	  	break;
    	case ACT_TIMER:
    	    /* when you add_timer(sample_trigger, 1000) in ACT_TURE or ACT_FALSE,
    	    this will handle the MSG_TIMER every 1000ms interval */
    	    break;
    	default:
    	    break;
    }
}



/* 燃油过低报警 */
static void app_buz_fuel_empty(U16 action)
{
	static U16 state = 0;
	static U16  delay_time_cnt = 0;
    U16 new_state = 0;	

	U8 fuel = get_fuel_percent3();

    switch(action)
    {
    	case ACT_RESET:
    	    state = 0;
    	    break;
    	case ACT_TOUCH:
			/* 燃油过低,小于10%声音报警*/
			
			if((delay_time_cnt >= _3S)&&(fuel<=10)&&(app_get_rpm()>0)) 	    
    	    {
    	    	new_state=1;
    	    }
    	    else
    	    {
    	    	new_state=0;

				/*熄火->点火，延时3s，重新判断声音*/
				if(OFF == g_u8IgnSts){
					delay_time_cnt = 0;
				}
				else
				{
					if(delay_time_cnt < _3S){
						delay_time_cnt ++;
					}
				}				
    	    }
			touch_state_trigger(app_buz_fuel_empty, &state,new_state);
    	    break;
    	case ACT_TRUE:
    			start_buzzer(BEEP_FUEL_EMPTY);
			func_app_beep_ctrl=buzzer_chord_player_play_one_step;
    			break;
    	case ACT_FALSE:
    			stop_buzzer(BEEP_FUEL_EMPTY);
    	case ACT_TIMER:
    	    /* when you add_timer(sample_trigger, 1000) in ACT_TURE or ACT_FALSE,
    	    this will handle the MSG_TIMER every 1000ms interval */
    	    break;
    	default:
    	    break;
    }
}




/*
	左右转向灯
*/
static void app_buz_turn(U16 action)
{
    static U16 state = 0;
    U16 new_state = 0;
		
		if((IsAnyBeepWarnigOn())||(beep_120kmph_warning==1))/*特殊处理的声音优先级比其他声音要低,120km报警优先级比转向灯高*/
		{
			
		}
		else
		{
	    	switch(action)
	    	{
	    		case ACT_RESET:
	    		    state = 0;
	    		    break;
	    		case ACT_TOUCH:
	    		    if(app_get_led_onoff_sts(LED_LEFTTURNLIGHT)||
	    		    	app_get_led_onoff_sts(LED_RIGHTTURNLIGHT))
	    		    {
	    		    	new_state=1;
	    		    }
	    		    else
	    		    {
	    		    	new_state=0;
	    		    }    		    
	    		  
					touch_state_trigger(app_buz_turn, &state,new_state);
	    		    break;
	    		case ACT_TRUE:
	    				beep_turn_warning=TURN_ON_WARNING;
	    				func_app_beep_ctrl=app_beep_turn_ctrl;
	    				hal_freeruntimer_Init(BEEP_FRT_CHN,5);
	    	   		hal_freeruntimer_start(BEEP_FRT_CHN);
	    				break;
	    		case ACT_FALSE:
	    				beep_turn_warning=TURN_OFF_WARNING;
	    				func_app_beep_ctrl=app_beep_turn_ctrl;
	    				hal_freeruntimer_Init(BEEP_FRT_CHN,5);
	    	   		hal_freeruntimer_start(BEEP_FRT_CHN);
	    		  	break;
	    		case ACT_TIMER:
	    		    /* when you add_timer(sample_trigger, 1000) in ACT_TURE or ACT_FALSE,
	    		    this will handle the MSG_TIMER every 1000ms interval */
	    		    break;
	    		default:
	    		    break;
	    	}  
    }
}


/*
熄火，左前门车门打开之后，钥匙未拔
*/
static void app_buz_key_not_out(U16 action)
{
	static beep_on_time_T key_not_out_beep_on;
	static U16 state = 0;
    U16 new_state = 0;

    switch(action)
    {
    	case ACT_RESET:
    	    state = 0;
    	    break;
    	case ACT_TOUCH:
    	    if((g_u8IgnSts == OFF)&&
    	    	(uart_general_data.generalInfo.leftFrontDoor==OPEN)&&
    	    	(app_get_car_key_pos()!=KEY_OUT)&&
    	    	(key_not_out_beep_on.cycle_timecnt<=_1S)&&
    	    	(key_not_out_beep_on.total_timecnt<=_20MIN))
    	    {
    	    	new_state=1;
    	    	key_not_out_beep_on.cycle_timecnt++;
    	    	key_not_out_beep_on.total_timecnt++;
    	    }
    	    else
    	    {
    	    	if(ON == g_u8IgnSts)//熄火-》点火之后，才会重新报警
    	    	{
    	    		key_not_out_beep_on.total_timecnt=0;
    	    	}
    	    	key_not_out_beep_on.cycle_timecnt=0;
    	    	new_state=0;
    	    }
    	  
					touch_state_trigger(app_buz_key_not_out, &state,new_state);
    	    break;
    	case ACT_TRUE:
    			start_buzzer(BEEP_KEY_NOT_OUT);
    			break;
    	case ACT_FALSE:
    			stop_buzzer(BEEP_KEY_NOT_OUT);
    	  	break;
    	case ACT_TIMER:
    	    /* when you add_timer(sample_trigger, 1000) in ACT_TURE or ACT_FALSE,
    	    this will handle the MSG_TIMER every 1000ms interval */
    	    break;
    	default:
    	    break;
    }
}

/*
点火后，第一次判断车子还未动时(没有车速)，判断到安全带未系
*/
static void app_buz_safe_selt_off_first(U16 action)
{
		static beep_on_time_T safe_selt_off_first_beep_on;
		static U16 state = 0;
    U16 new_state = 0;

    switch(action)
    {
    	case ACT_RESET:
    	    state = 0;
    	    break;
    	case ACT_TOUCH:
    	    if((g_u8IgnSts == ON)&&
    	    	(app_get_fact_spd()==NO_SPEED)&&
    	    	((uart_special_data.specialInfo.LedWarning.first.bit.SeatbeltsNotFastened==YES)||((app_get_led_onoff_sts(LED_SAFETYBELTSIDE)==LED_ON)))&&
    	    	(safe_selt_off_first_beep_on.total_timecnt<=_8S)&&
    	    	(safe_selt_off_first_beep_on.delay_timecnt>=_2S))/*&&
    	    	(mcu_rec_general.settingsInfo .warningSoundAndInterfaceSync==SAFE_SELT))*/
    	    {
    	    	safe_selt_off_first_beep_on.total_timecnt++;
    	    	new_state=1;
    	    }
    	    else
    	    {
    	    	//熄火-》点火，延时2S，才重新判断一次安全带是否未系
    	    	if(g_u8IgnSts == OFF)
    	    	{
    	    		safe_selt_off_first_beep_on.total_timecnt=0;
    	    		safe_selt_off_first_beep_on.delay_timecnt=0;
    	    	}
    	    	else
    	    	{
    	    		if(safe_selt_off_first_beep_on.delay_timecnt<_2S)
    	    		{
    	    			safe_selt_off_first_beep_on.delay_timecnt++;
    	    		}
    	    	}
    	    	new_state=0;
    	    }
    	  
					touch_state_trigger(app_buz_safe_selt_off_first, &state,new_state);
    	    break;
    	case ACT_TRUE:
				//dbg_string("buz_safe_selt_first start\n");
    			start_buzzer(BEEP_SAFE_SELT_OFF_FIRST);
    			break;
    	case ACT_FALSE:
				//dbg_string("buz_safe_selt_first stop\n");
    			stop_buzzer(BEEP_SAFE_SELT_OFF_FIRST);
    	  	break;
    	case ACT_TIMER:
    	    /* when you add_timer(sample_trigger, 1000) in ACT_TURE or ACT_FALSE,
    	    this will handle the MSG_TIMER every 1000ms interval */
    	    break;
    	default:
    	    break;
    }
}

/*
点火后，车速大于20km/h,判断到安全带未系
*/
static void app_buz_safe_selt_off_second(U16 action)
{
	static beep_on_time_T safe_selt_off_second_beep_on;
	static U16 state = 0;
    U16 new_state = 0;

    switch(action)
    {
    	case ACT_RESET:
    	    state = 0;
    	    break;
    	case ACT_TOUCH:
    	    if((g_u8IgnSts == ON)&&
    	    	(app_get_dis_spd()>=SPEED_20KMPH)&&
    	    	((uart_special_data.specialInfo.LedWarning.first.bit.SeatbeltsNotFastened==YES)||(app_get_led_onoff_sts(LED_SAFETYBELTSIDE)==LED_ON))&&
    	    	(safe_selt_off_second_beep_on.cycle_timecnt<=_23S))/*&&
    	    	(mcu_rec_general.settingsInfo .warningSoundAndInterfaceSync==SAFE_SELT))*/
    	    {
    	    	new_state=1;
    	    	safe_selt_off_second_beep_on.cycle_timecnt++;
    	    }
    	    else
    	    {
    	    	safe_selt_off_second_beep_on.cycle_timecnt=0;
    	    	new_state=0;
    	    }
    	  
					touch_state_trigger(app_buz_safe_selt_off_second, &state,new_state);
    	    break;
    	case ACT_TRUE:
    			start_buzzer(BEEP_SAFE_SELT_OFF_SECOND);
			func_app_beep_ctrl=buzzer_chord_player_play_one_step;
    			break;
    	case ACT_FALSE:
			stop_buzzer(BEEP_SAFE_SELT_OFF_SECOND);
    	  	break;
    	case ACT_TIMER:
    	    /* when you add_timer(sample_trigger, 1000) in ACT_TURE or ACT_FALSE,
    	    this will handle the MSG_TIMER every 1000ms interval */
    	    break;
    	default:
    	    break;
    }
}

/*
拔出钥匙后，左前门开着的情况下，打开灯光,会提示灯光未关
*/
static void app_buz_light_on(U16 action)
{
	static beep_on_time_T light_on_beep_on;
	static U16 state = 0;
    U16 new_state = 0;

    switch(action)
    {
    	case ACT_RESET:
    	    state = 0;
    	    break;
    	case ACT_TOUCH:
    	    if((app_get_car_key_pos()==KEY_OUT)&&(app_get_rpm() == 0)&&
    	    	(uart_general_data.generalInfo.leftFrontDoor==OPEN)&&
    	    	(app_get_led_onoff_sts(LED_LIGHTMAINCTL)||app_get_led_onoff_sts(LED_REARFOGLAMP)||app_get_led_onoff_sts(LED_FRONTFOGLAMP)||app_get_led_onoff_sts(LED_HIGHBEAM))&&
    	    	(light_on_beep_on.total_timecnt<=_1MIN)
    	    	)
    	    {
    	    	light_on_beep_on.total_timecnt++;
    	    	new_state=1;
    	    }
    	    else
    	    {
    	    	if(g_u8IgnSts == ON)//熄火-》点火之后，才重新开始灯光未关的报警
    	    	{
    	    		light_on_beep_on.total_timecnt=0;
    	    	}
    	    	new_state=0;
    	    }
			touch_state_trigger(app_buz_light_on, &state,new_state);
    	    break;
    	case ACT_TRUE:
    			start_buzzer(BEEP_LIGHT_ON);
    			break;
    	case ACT_FALSE:
    			stop_buzzer(BEEP_LIGHT_ON);
    	  	break;
    	case ACT_TIMER:
    	    /* when you add_timer(sample_trigger, 1000) in ACT_TURE or ACT_FALSE,
    	    this will handle the MSG_TIMER every 1000ms interval */
    	    break;
    	default:
    	    break;
    }
}

//获取雷达报警灯的等级
static U8 get_radar_sound_level(void)
{
	U8 radar_level_temp;
	switch(IPconfig.protocol)                               
		{                                                     
			case HAVAL_2013_2P4L_AT_2WD:                            
				if(can0.id_3df.RearRadarsoundlevel==0)
				{
					radar_level_temp=can0.id_3df.FrontRadarsoundlevel;
				}
				else
				{
					if((can0.id_3df.RearRadarsoundlevel<=can0.id_3df.FrontRadarsoundlevel))
					{
						radar_level_temp=can0.id_3df.RearRadarsoundlevel;
					}
					else
					{
						if(can0.id_3df.FrontRadarsoundlevel==0)
						{
							radar_level_temp=can0.id_3df.RearRadarsoundlevel;
						}
						else
						{
							radar_level_temp=can0.id_3df.FrontRadarsoundlevel;
						}
					}
				}                                            
				break;                                            
			case HAVAL_2013_1P5T_MT_2WD:                            
				if(can1.id_3df.RearRadarsoundlevel==0)
				{
					radar_level_temp=can1.id_3df.FrontRadarsoundlevel;
				}
				else
				{
					if((can1.id_3df.RearRadarsoundlevel<=can1.id_3df.FrontRadarsoundlevel))
					{
						radar_level_temp=can1.id_3df.RearRadarsoundlevel;
					}
					else
					{
						if(can1.id_3df.FrontRadarsoundlevel==0)
						{
							radar_level_temp=can1.id_3df.RearRadarsoundlevel;
						}
						else
						{
							radar_level_temp=can1.id_3df.FrontRadarsoundlevel;
						}
					}
				}                                             
				break;                                            
			case HAVAL_2012_1P5T_MT_2WD:                            
				                                             
				break;
				
			/*2015_1P5T_MT_2WD和2015_1P5T_6AT_2WD一样*/
			case HAVAL_2015_1P5T_MT_2WD: 
			case HAVAL_2015_1P5T_6AT_2WD:                           
				if(can3.id_3df.RearRadarsoundlevel==0)
				{
					radar_level_temp=can3.id_3df.FrontRadarsoundlevel;
				}
				else
				{
					if((can3.id_3df.RearRadarsoundlevel<=can3.id_3df.FrontRadarsoundlevel))
					{
						radar_level_temp=can3.id_3df.RearRadarsoundlevel;
					}
					else
					{
						if(can3.id_3df.FrontRadarsoundlevel==0)
						{
							radar_level_temp=can3.id_3df.RearRadarsoundlevel;
						}
						else
						{
							radar_level_temp=can3.id_3df.FrontRadarsoundlevel;
						}
					}
				}                                        
				break;    
			case HAVAL_2011_2P0T_MT_2WD:                            
				                                             
				break;       
			case HAVAL_2013_1P5T_MT_4WD:
				if(can5.id_3df.RearRadarsoundlevel==0)
				{
					radar_level_temp=can5.id_3df.FrontRadarsoundlevel;
				}
				else
				{
					if((can5.id_3df.RearRadarsoundlevel<=can5.id_3df.FrontRadarsoundlevel))
					{
						radar_level_temp=can5.id_3df.RearRadarsoundlevel;
					}
					else
					{
						if(can5.id_3df.FrontRadarsoundlevel==0)
						{
							radar_level_temp=can5.id_3df.RearRadarsoundlevel;
						}
						else
						{
							radar_level_temp=can5.id_3df.FrontRadarsoundlevel;
						}
					}
				}
				
				break;                                 
		}
		return radar_level_temp;
}
/*
界面有雷达信号后，根据报警音信号，发出雷达4级报警音
*/
static void app_buz_radar_level_4(U16 action)
{
		static U16 state = 0;
    U16 new_state = 0;
    U8 radar_level_temp;
    
    radar_level_temp=get_radar_sound_level();
    
    switch(action)
    {
    	case ACT_RESET:
    	    state = 0;
    	    break;
    	case ACT_TOUCH:
    	    if(((uart_special_data.specialInfo.Radar.first.val)||
    	    	(uart_special_data.specialInfo.Radar.second.val)||
    	    	(uart_special_data.specialInfo.Radar.third.val)||
    	    	(uart_special_data.specialInfo.Radar.fourth.val))&&
    	    	(radar_level_temp==4)&&(LIDAR==app_uart_get_sound_sync()))
    	    {
    	    	new_state=1;
    	    }
    	    else
    	    {
    	    	new_state=0;
    	    }
    	  
			touch_state_trigger(app_buz_radar_level_4, &state,new_state);
    	    break;
    	case ACT_TRUE:
    			start_buzzer(BEEP_RADAR_LEVEL_4);
    			break;
    	case ACT_FALSE:
    			stop_buzzer(BEEP_RADAR_LEVEL_4);
    	  	break;
    	case ACT_TIMER:
    	    /* when you add_timer(sample_trigger, 1000) in ACT_TURE or ACT_FALSE,
    	    this will handle the MSG_TIMER every 1000ms interval */
    	    break;
    	default:
    	    break;
    }
}
/*
界面有雷达信号后，根据报警音信号，发出雷达3级报警音
*/
static void app_buz_radar_level_3(U16 action)
{
	static U16 state = 0;
    U16 new_state = 0;
    U8 radar_level_temp;
    
    radar_level_temp=get_radar_sound_level();
		
    switch(action)
    {
    	case ACT_RESET:
    	    state = 0;
    	    break;
    	case ACT_TOUCH:
    	    if(((uart_special_data.specialInfo.Radar.first.val)||
    	    	(uart_special_data.specialInfo.Radar.second.val)||
    	    	(uart_special_data.specialInfo.Radar.third.val)||
    	    	(uart_special_data.specialInfo.Radar.fourth.val))&&
    	    	(radar_level_temp==3)&&(LIDAR==app_uart_get_sound_sync()))
    	    {
    	    	new_state=1;
    	    }
    	    else
    	    {
    	    	new_state=0;
    	    }
    	  
					touch_state_trigger(app_buz_radar_level_3, &state,new_state);
    	    break;
    	case ACT_TRUE:
    			start_buzzer(BEEP_RADAR_LEVEL_3);
    			break;
    	case ACT_FALSE:
    			stop_buzzer(BEEP_RADAR_LEVEL_3);
    	  	break;
    	case ACT_TIMER:
    	    /* when you add_timer(sample_trigger, 1000) in ACT_TURE or ACT_FALSE,
    	    this will handle the MSG_TIMER every 1000ms interval */
    	    break;
    	default:
    	    break;
    }
}

/*
界面有雷达信号后，根据报警音信号，发出雷达2级报警音
*/
static void app_buz_radar_level_2(U16 action)
{
	static U16 state = 0;
    U16 new_state = 0;
    U8 radar_level_temp;
    
   radar_level_temp=get_radar_sound_level();
   
    switch(action)
    {
    	case ACT_RESET:
    	    state = 0;
    	    break;
    	case ACT_TOUCH:
    	    if(((uart_special_data.specialInfo.Radar.first.val)||
    	    	(uart_special_data.specialInfo.Radar.second.val)||
    	    	(uart_special_data.specialInfo.Radar.third.val)||
    	    	(uart_special_data.specialInfo.Radar.fourth.val))&&
    	    	(radar_level_temp==2)&&(LIDAR==app_uart_get_sound_sync()))
    	    {
    	    	new_state=1;
    	    }
    	    else
    	    {
    	    	new_state=0;
    	    }
    	  
					touch_state_trigger(app_buz_radar_level_2, &state,new_state);
    	    break;
    	case ACT_TRUE:
    			start_buzzer(BEEP_RADAR_LEVEL_2);
    			break;
    	case ACT_FALSE:
    			stop_buzzer(BEEP_RADAR_LEVEL_2);
    	  	break;
    	case ACT_TIMER:
    	    /* when you add_timer(sample_trigger, 1000) in ACT_TURE or ACT_FALSE,
    	    this will handle the MSG_TIMER every 1000ms interval */
    	    break;
    	default:
    	    break;
    }
}
/*
界面有雷达信号后，根据报警音信号，发出雷达1级报警音
*/
static void app_buz_radar_level_1(U16 action)
{
	static U16 state = 0;
    U16 new_state = 0;
    U8 radar_level_temp;
    
   radar_level_temp=get_radar_sound_level();
		
    switch(action)
    {
    	case ACT_RESET:
    	    state = 0;
    	    break;
    	case ACT_TOUCH:
    	    if(((uart_special_data.specialInfo.Radar.first.val) ||
    	    	(uart_special_data.specialInfo.Radar.second.val)||
    	    	(uart_special_data.specialInfo.Radar.third.val) ||
    	    	(uart_special_data.specialInfo.Radar.fourth.val))&&
    	    	(radar_level_temp==1)&&(app_uart_get_sound_sync()==LIDAR))
    	    {
    	    	new_state=1;
    	    }
    	    else
    	    {
    	    	new_state=0;
    	    }
    	  
			touch_state_trigger(app_buz_radar_level_1, &state,new_state);
    	    break;
    	case ACT_TRUE:
    			start_buzzer(BEEP_RADAR_LEVEL_1);
    			break;
    	case ACT_FALSE:
    			stop_buzzer(BEEP_RADAR_LEVEL_1);
    	  	break;
    	case ACT_TIMER:
    	    /* when you add_timer(sample_trigger, 1000) in ACT_TURE or ACT_FALSE,
    	    this will handle the MSG_TIMER every 1000ms interval */
    	    break;
    	default:
    	    break;
    }
}
/*
雷达失效报警
*/
static void app_buz_radar_failure(U16 action)
{
	static U16 state = 0;
    U16 new_state = 0;
    
    switch(action)
    {
    	case ACT_RESET:
    	    state = 0;
    	    break;
    	case ACT_TOUCH:
    	    if(uart_special_data.specialInfo.Radar.fifth.bit.RadarFailure==1)//雷达失效
    	    {
    	    	new_state=1;
    	    }
    	    else
    	    {
    	    	new_state=0;
    	    }
    	  
					touch_state_trigger(app_buz_radar_failure, &state,new_state);
    	    break;
    	case ACT_TRUE:
    			start_buzzer(BEEP_RADAR_FAILURE);
    			break;
    	case ACT_FALSE:
    			stop_buzzer(BEEP_RADAR_FAILURE);
    	  	break;
    	case ACT_TIMER:
    	    /* when you add_timer(sample_trigger, 1000) in ACT_TURE or ACT_FALSE,
    	    this will handle the MSG_TIMER every 1000ms interval */
    	    break;
    	default:
    	    break;
    }
}
/*
挂倒档后，激活倒车雷达，会有一声嘟的声音
*/
static void app_buz_radar_active(U16 action)
{
	static beep_on_time_T radar_active_beep_on;
	static U16 state = 0;
    U16 new_state = 0;
		
    switch(action)
    {
    	case ACT_RESET:
    	    state = 0;
    	    break;
    	case ACT_TOUCH:
    	    if((g_u8IgnSts == ON)&&
    	    	(uart_general_data.generalInfo.gear==REVERSE_GEAR)&&
    	    	(radar_active_beep_on.delay_timecnt>=_1S))
    	    {
    	    	new_state=1;
    	    }
    	    else
    	    {
    	    	//熄火-》点火，延时1s，重新判断倒档声音
    	    	if(g_u8IgnSts==OFF)
    	    	{
    	    		radar_active_beep_on.delay_timecnt=0;
    	    	}
    	    	else
    	    	{
    	    		if(radar_active_beep_on.delay_timecnt<_1S)
    	    		{
    	    			radar_active_beep_on.delay_timecnt++;
    	    		}
    	    	}
    	    	
    	    	new_state=0;
    	    }
    	  
				touch_state_trigger(app_buz_radar_active, &state,new_state);
    	    break;
    	case ACT_TRUE:
    			start_buzzer(BEEP_RADAR_ACTIVE);
    			break;
    	case ACT_FALSE:
    			stop_buzzer(BEEP_RADAR_ACTIVE);
    	  	break;
    	case ACT_TIMER:
    	    /* when you add_timer(sample_trigger, 1000) in ACT_TURE or ACT_FALSE,
    	    this will handle the MSG_TIMER every 1000ms interval */
    	    break;
    	default:
    	    break;
    }
}
/*
120KM/H超速报警
*/
static void app_buz_spd_120kmph(U16 action)
{
	static U16 state = 0;
    	U16 new_state = 0;
		
	if((IsAnyBeepWarnigOn()))//特殊处理的声音优先级比其他声音要低
	{
		
	}
	else
	{
    	switch(action)
    	{
    		case ACT_RESET:
			    		    state = 0;
			    		    break;
    		case ACT_TOUCH:
			    		    if(get_uart_Overspeed() == 1) /* 超速报警 */
			    		    {
			    		    	new_state=1;
			    		    }
			    		    else
			    		    {
			    		    	new_state=0;
			    		    }
							touch_state_trigger(app_buz_spd_120kmph, &state,new_state);
    		    			break;
    		case ACT_TRUE:
			    		   	start_buzzer(BEEP_SPD_120KMPH);
			    			break;
    		case ACT_FALSE:
		    				stop_buzzer(BEEP_SPD_120KMPH);
		    		 	 	break;
    		case ACT_TIMER:
			    		    break;
    		default:		break;
    	}
	 }
}

/*
轮胎快速漏气报警
*/
static void app_buz_fast_leakagestates(U16 action)
{
	static U16 state = 0;
    U16 new_state = 0;
		
    switch(action)
    {
    	case ACT_RESET:
    	    state = 0;
    	    break;
    	case ACT_TOUCH:
    		switch(IPconfig.protocol)
				{
					case HAVAL_2013_2P4L_AT_2WD:
						if(can0.id_311.BCM_TPFastLeakageInd==1)
    	    	{
    	    		new_state=1;
    	    	}
    	    	else
    	    	{
    	    		new_state=0;
    	    	}
						break;
					case HAVAL_2013_1P5T_MT_2WD:
						if(can1.id_311.BCM_TPFastLeakageInd==1)
    	    	{
    	    		new_state=1;
    	    	}
    	    	else
    	    	{
    	    		new_state=0;
    	    	}
						break;
					case HAVAL_2012_1P5T_MT_2WD:
						if(can2.id_311.BCM_TPFastLeakageInd==1)
    	    	{
    	    		new_state=1;
    	    	}
    	    	else
    	    	{
    	    		new_state=0;
    	    	}
						break;
					case HAVAL_2015_1P5T_MT_2WD: 
					case HAVAL_2015_1P5T_6AT_2WD:
						if(can3.id_311.BCM_TPFastLeakageInd==1)
    	    	{
    	    		new_state=1;
    	    	}
    	    	else
    	    	{
    	    		new_state=0;
    	    	}
						break;
					case HAVAL_2011_2P0T_MT_2WD:
						if(can4.id_311.BCM_TPFastLeakageInd==1)
    	    	{
    	    		new_state=1;
    	    	}
    	    	else
    	    	{
    	    		new_state=0;
    	    	}
						break;
					case HAVAL_2013_1P5T_MT_4WD:
						if(can5.id_311.BCM_TPFastLeakageInd==1)
    	    	{
    	    		new_state=1;
    	    	}
    	    	else
    	    	{
    	    		new_state=0;
    	    	}
						
						break;
				}
					touch_state_trigger(app_buz_fast_leakagestates, &state,new_state);
    	    break;
    	case ACT_TRUE:
    			start_buzzer(BEEP_FAST_LEAKAGESTATES);
    			break;
    	case ACT_FALSE:
    			stop_buzzer(BEEP_FAST_LEAKAGESTATES);
    	  	break;
    	case ACT_TIMER:
    	    /* when you add_timer(sample_trigger, 1000) in ACT_TURE or ACT_FALSE,
    	    this will handle the MSG_TIMER every 1000ms interval */
    	    break;
    	default:
    	    break;
    }
}

/*ESP关闭报警*/
static void app_buz_esp_closed(U16 action)
{
	static U16 state = 0;
	U16 new_state = 0;
	
  switch(action)
  {
  	case ACT_RESET:
  	    state = 0;
  	    break;
  	case ACT_TOUCH:
  		switch(IPconfig.protocol)
			{
				case HAVAL_2013_2P4L_AT_2WD:
					new_state=0;
					break;
				case HAVAL_2013_1P5T_MT_2WD:
					new_state=0;
					break;
				case HAVAL_2012_1P5T_MT_2WD:
					new_state=0;
					break;
				case HAVAL_2015_1P5T_MT_2WD: 
				case HAVAL_2015_1P5T_6AT_2WD:
					if(uart_special_data.specialInfo.LedWarning.second.bit.ESP == 3)
  	    			{
  	    				new_state=1;
  	    			}
  	    			else
  	    			{
  	    				new_state=0;
  	    			}
					break;
				case HAVAL_2011_2P0T_MT_2WD:
					new_state=0;
					break;
				case HAVAL_2013_1P5T_MT_4WD:
					new_state=0;
					break;
			}
				touch_state_trigger(app_buz_esp_closed, &state,new_state);
  	    break;
  	case ACT_TRUE:
  			start_buzzer(BEEP_ESP_CLOSED);
  			break;
  	case ACT_FALSE:
  			stop_buzzer(BEEP_ESP_CLOSED);
  	  	break;
  	case ACT_TIMER:
  	    /* when you add_timer(sample_trigger, 1000) in ACT_TURE or ACT_FALSE,
  	    this will handle the MSG_TIMER every 1000ms interval */
  	    break;
  	default:
  	    break;
  }
}

void app_set_buz_sts(buz_name_t buzzer_name,U8 value)
{
	if(buzzer_name < BEEPCOUNT)
	{
		buz_sts[buzzer_name] = value;
	}
}

//特殊的声音做特殊的处理
//转向灯
void app_beep_turn_ctrl(void)
{
	static U8 beep_turn_ctrl_step=0;
	
	 beep_turn_ctrl_step++;
   switch(beep_turn_warning)
   {
   	case TURN_ON_WARNING:
   		switch(beep_turn_ctrl_step)
   		{
   			case 1:
   				buzzer_start_test(CONST_TICKTOCK_FREQ,CONST_TICKTOCK_DUTY);
   				hal_freeruntimer_Init(BEEP_FRT_CHN,CONST_TICK_TIME);
   				hal_freeruntimer_start(BEEP_FRT_CHN);
   				break;
   			case 2:
   				buzzer_stop_test();
   				hal_freeruntimer_Init(BEEP_FRT_CHN,CONST_TICK_TIME);
   				hal_freeruntimer_start(BEEP_FRT_CHN);
   				break;
   			case 3:
   				buzzer_start_test(CONST_TICKTOCK_FREQ,CONST_TICKTOCK_DUTY);
   				hal_freeruntimer_Init(BEEP_FRT_CHN,CONST_TICK_TIME);
   				hal_freeruntimer_start(BEEP_FRT_CHN);
   				break;
   			case 4:
   				buzzer_stop_test();
   				hal_freeruntimer_Init(BEEP_FRT_CHN,CONST_TICK_TIME);
   				hal_freeruntimer_start(BEEP_FRT_CHN);
   				break;
   			default:
   				buzzer_stop_test();
   				hal_freeruntimer_stop(BEEP_FRT_CHN);
   				beep_turn_ctrl_step=0;
   				break;
   		}
   		break;
   	case TURN_OFF_WARNING:
   		switch(beep_turn_ctrl_step)
   		{
   			case 1:
   				buzzer_start_test(CONST_TICKTOCK_FREQ,CONST_TICKTOCK_DUTY);
   				hal_freeruntimer_Init(BEEP_FRT_CHN,CONST_TOCK_TIME);
   				hal_freeruntimer_start(BEEP_FRT_CHN);
   				break;
   			case 2:
   				buzzer_stop_test();
   				hal_freeruntimer_Init(BEEP_FRT_CHN,CONST_TOCK_TIME);
   				hal_freeruntimer_start(BEEP_FRT_CHN);
   				break;
   			case 3:
   				buzzer_start_test(CONST_TICKTOCK_FREQ,CONST_TICKTOCK_DUTY);
   				hal_freeruntimer_Init(BEEP_FRT_CHN,CONST_TOCK_TIME);
   				hal_freeruntimer_start(BEEP_FRT_CHN);
   				break;
   			case 4:
   				buzzer_stop_test();
   				hal_freeruntimer_Init(BEEP_FRT_CHN,CONST_TOCK_TIME);
   				hal_freeruntimer_start(BEEP_FRT_CHN);
   				break;
   			default:
   				buzzer_stop_test();
   				hal_freeruntimer_stop(BEEP_FRT_CHN);
   				beep_turn_ctrl_step=0;
   				break;
   		}
   		break;
   	default:
   		buzzer_stop_test();
   		hal_freeruntimer_stop(BEEP_FRT_CHN);
   		beep_turn_ctrl_step=0;
   		break;
   }     	
}

/* 开机和弦音 */
static void app_buz_startup(U16 action)
{
	static U16 state = 0;
    U16 new_state = 0;	

    switch(action)
    {
    	case ACT_RESET:
    	    state = 0;
    	    break;
    	case ACT_TOUCH:
			if(app_get_power_sts()==PWR_NML)
			{
				new_state=1;
			}
			touch_state_trigger(app_buz_startup, &state,new_state);
    	    break;
    	case ACT_TRUE:
    			start_buzzer(BEEP_STARTUP);
			func_app_beep_ctrl=buzzer_chord_player_play_one_step;
    			break;
    	case ACT_FALSE:
    			stop_buzzer(BEEP_STARTUP);
    	case ACT_TIMER:
    	    /* when you add_timer(sample_trigger, 1000) in ACT_TURE or ACT_FALSE,
    	    this will handle the MSG_TIMER every 1000ms interval */
    	    break;
    	default:
    	    break;
    }
}

/*
static const U8 duttyArray[100] =
{
	100,97,94,91,88,85,82,79,76,73,
	70,67,64,62,60,58,56,54,52,50,
	48,46,44,43,42,41,40,39,38,37,
	36,35,34,34,33,33,31,31,31,29,
	29,28,27,27,26,26,25,24,24,24,
	23,23,23,23,22,22,21,21,21,21,
	21,19,19,19,19,18,18,18,17,17,
	16,15,15,15,15,14,14,13,13,12,
	12,11,11,10, 9, 9, 8, 8, 7, 7,
	 6, 6, 5, 5, 4, 4, 3, 3, 2, 1
};

//120km/h超速报警
void app_beep_120kmph_ctrl(void)
{
	static U8 beep_120kmph_ctrl_step=0;

	if(beep_120kmph_ctrl_step>=(sizeof(duttyArray)/sizeof(U8)))
	{
		buzzer_stop_test();
		beep_120kmph_ctrl_step=0;
		if(beep_120kmph_warning)
		{
  		hal_freeruntimer_Init(BEEP_FRT_CHN,5);//重新开始报警
  		hal_freeruntimer_start(BEEP_FRT_CHN);
  		beep_120kmph_times_cnt++;
		}
		else
		{
			hal_freeruntimer_stop(BEEP_FRT_CHN);
		}
	}
	else
	{
		buzzer_start_test(CONST_120KMPH_FREQ,duttyArray[beep_120kmph_ctrl_step]);
  	hal_freeruntimer_Init(BEEP_FRT_CHN,CONST_CHANGE_DUTY_PERIOD);
  	hal_freeruntimer_start(BEEP_FRT_CHN);
  	
  	if(beep_120kmph_ctrl_step<0xff)//防止溢出
  	{
			beep_120kmph_ctrl_step++;
		}
	}
}
*/
void app_buz_load_success( void )
{
	start_buzzer(BEEP_LOAD_OK);
}
