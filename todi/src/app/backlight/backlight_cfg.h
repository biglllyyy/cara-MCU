#ifndef __BACKLIGHT_H__
#define __BACKLIGHT_H__


#define  ACC_IN_CHN			    0
#define	 TOTAL_RES				1000


#define  BL_ADJUST_NONE		0
#define  BL_ADJUST_PWM		1
#define  BL_ADJUST_RES		2

#define  BACKLIGHT_ADJUST_SOURCE    BL_ADJUST_NONE

#define  RES_IN_MIN_PERCNET				13	
#define  RES_IN_MAX_PERCENT				87

#define  LCD_BL_PWM_CHN						3 //sch0.4: 1->3 PPG,no use 
#define  LCD2_BL_PWM_CHN					2


#define  BL_OUT1_FRQ            10000
#define  BL_OUT2_FRQ            10000

#endif 
