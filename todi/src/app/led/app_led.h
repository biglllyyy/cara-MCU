#ifndef _APP_LED_H__
#define _APP_LED_H__

#include "led_cfg.h"
#include "comm_typedef.h"
#include "mid_led.h"

typedef struct{
	U8			 is_less_3s_sts;
	U8			 is_less_6s_sts;
}led_check_t;

void app_init_led(void);
void app_pro_led(void);
U8 app_get_led_onoff_sts(LED_NAME_e led_name);
void app_leds_all_on_test(void);
void app_leds_all_off_test(void);

extern led_check_t  led_check;

extern U16 lin_turn_lost_timecnt;
extern U16 lin_rearfoglamp_lost_timecnt;
extern U16 lin_frontfoglamp_lost_timecnt;
extern U16 lin_highbeam_lost_timecnt; 
extern U16 lin_parking_lost_timecnt;
extern U16 led_check_time;
extern U16 lin_light_main_switch_lost_timecnt;
extern U8 led_selfcheck_enable;

#endif
