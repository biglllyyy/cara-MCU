#include "hal_ppg.h"


/*----------------------------------------------------------------
背光策略：
输入  PWM              位置等              亮度
-------------------------------------------------------------------
	  VALID              X                 随着PWM调节
-------------------------------------------------------------------
	  INVALID	         ON                20%~30%
	                     OFF               100%
-------------------------------------------------------------------
*/


PPGUtil   ppg_lcd_backlight = {0};
PPGUtil   ppg_lcd2_backlight = {0};


void mid_set_lcd_frq(U8 pwm_channel,U16 pwm_frq,U8 pwm_dutyCycle)
{
	HalPwmStart(&ppg_lcd_backlight, pwm_channel, pwm_frq,pwm_dutyCycle); 
}


