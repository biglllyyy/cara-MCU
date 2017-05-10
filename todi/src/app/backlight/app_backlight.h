#ifndef __APP_BACKLIGHT_H__
#define __APP_BACKLIGHT_H__

#include "comm_typedef.h"
/* 1-10K */
#define    LCD_BACKLIGHT_FRQ      1500
#define    LCD2_BACKLIGHT_FRQ      1500

extern void flag_backlight_ON();
extern void flag_backlight_OFF();
extern U8   is_backlight_ON();
extern void app_backlight_init(void);
extern void app_backlight_get_duty_freq(void);
extern void app_backlight_ctl(void);
extern void app_backlight_off(void);

#endif
