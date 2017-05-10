#ifndef __MID_LCD_H__
#define __MID_LCD_H__


/* 1-10K */
#define  LCD_BACKLIGHT_FRQ	1500
#define  LCD2_BACKLIGHT_FRQ 1500

#define  LCD_BL_PWM_CHN		2
#define  LCD2_BL_PWM_CHN	3


void mid_set_lcd_frq(U8 pwm_channel,U16 pwm_frq,U8 pwm_dutyCycle);




#endif

