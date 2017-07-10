#include "app_backlight.h"
#include "backlight_cfg.h"
#include "app_led.h"
#include "hal_ppg.h"
#include "app_io.h"
#include "mid_dbg.h"
#include "mid_scheduler.h"

static U8 flag_backlight_on;
U8 app_get_position_sts(void);		//get positon sts 

PPGUtil   ppg_lcd2_backlight = {0};
//前一时刻的背光PWM频率
static U8 prev_backlight_PWM_fre = 0;

/*背光频率每次+/-VALUE值，逼近fre*/
U8 soft_start_backlight_ctl(U8 fre, const U8 VALUE)
{
	/*如果设置频率跟上次相同，直接返回*/
	if(fre == prev_backlight_PWM_fre) return 0; 

	if(fre > prev_backlight_PWM_fre){
		
		prev_backlight_PWM_fre += VALUE; 
	}
	else if(fre < prev_backlight_PWM_fre)
	{
		prev_backlight_PWM_fre -= VALUE; 
	}
	HalPwmStart(&ppg_lcd2_backlight,LCD2_BL_PWM_CHN,LCD2_BACKLIGHT_FRQ,(prev_backlight_PWM_fre*0xff/100));
	//dbg_string("prev_backlight_PWM_fre:%d\n",prev_backlight_PWM_fre);
	return 1;
}


void app_backlight_ctl(void)
{
    U8  pos_light_switch = 0;
    do
    {
        pos_light_switch =app_get_position_sts();
        if(pos_light_switch)
        {
			soft_start_backlight_ctl(20, 1);
        }
        else
        {
  			soft_start_backlight_ctl(80, 1);
        }
    }while(0);
}
/*
关掉背光
close backlight pwm output
disable supply power
*/
void app_backlight_off(void)
{
//  HalPwmInit(&ppg_lcd_backlight,LCD_BL_PWM_CHN);
//	HalPwmStop(&ppg_lcd_backlight,PPG_PIN_LOW); 	/* stop pwm ouput */
//  HalPwmInit(&ppg_lcd2_backlight,LCD2_BL_PWM_CHN);

	/* 删除调节背光的任务*/
	prev_backlight_PWM_fre = 0;
	MidSchDeleteTask(app_backlight_ctl);	
	HalPwmStop(&ppg_lcd2_backlight,PPG_PIN_LOW);		/* stop pwm ouput */
	mid_set_io_sts(&BL_POWER_CTRL,OFF);	/* close backlight en */
	mid_set_io_sts(&BL_GMA_SD,OFF);	/* close Gmamma_SD en */
	flag_backlight_OFF();
}

/*
backlight init pwm out
backlight supply power enable
*/
void app_backlight_init(void)
{
//	HalPwmInit(&ppg_lcd_backlight,LCD_BL_PWM_CHN);
//	HalPwmStop(&ppg_lcd_backlight,PPG_PIN_LOW);			/* stop pwm ouput */

	prev_backlight_PWM_fre = 0;
	HalPwmInit(&ppg_lcd2_backlight,LCD2_BL_PWM_CHN);
	HalPwmStop(&ppg_lcd2_backlight,PPG_PIN_LOW);			/* stop pwm ouput */
	mid_set_io_sts(&BL_POWER_CTRL,ON);				/* open back light EN */
	MidSchAddTask(app_backlight_ctl,50);  
	flag_backlight_ON();
}

/*
get position  sts,
changed your code in this fuction
*/
U8 app_get_position_sts(void)
{
	U8 ret = 0;

	if(app_get_led_onoff_sts(LED_POSITION)==LED_OFF)
	{
			ret=0;
	}
	else
	{
			ret=1;
	}
	return ret;
}


void flag_backlight_ON()
{
	flag_backlight_on = 1;
}
void flag_backlight_OFF()
{
	flag_backlight_on = 0;
}
U8 is_backlight_ON()
{
	return flag_backlight_on;
}
