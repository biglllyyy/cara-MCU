#ifndef __MID_LED_H__
#define __MID_LED_H__

#include "comm_typedef.h"
#include "mid_pin.h"

typedef  void (* led_handle)(void);

#define  BASE_T				  (BASE_50MS)
#define  LED_CHECK_TIME_3S	  TimeMs(3000,BASE_T)	
#define  LED_CHECK_TIME_6S	  TimeMs(6300,BASE_T)
#define  LED_KEEP_STS_5S	  TimeMs(3000,BASE_T)

typedef enum
{
	LED_LEFTTURNLIGHT = 0,
	LED_REARFOGLAMP,
	LED_FRONTFOGLAMP,
	LED_HIGHBEAM,
	LED_PARK,
	LED_POSITION,
	LED_SAFETYBELTSIDE,
	LED_LIGHTMAINCTL,
	LED_RIGHTTURNLIGHT,
	LED_RESERVELED1,
	LED_RESERVELED2,
	LED_ALL
}LED_NAME_e;



typedef enum
{
	LED_OFF = 0,
	LED_ON  = 1,
	LED_BLINK = 2
}LED_STS_e;

typedef enum
{
	F_1HZ = 1,
	F_2HZ = 2,
	F_5HZ = 3
}led_blink_frq_e;


void mid_init_led(void);
extern void mid_led_ctl(LED_NAME_e name,LED_STS_e sts);
U8 mid_led_blink_register(led_handle hdl,LED_NAME_e led_name,led_blink_frq_e blink_frq);

U8  mid_get_led_onoff_sts(LED_NAME_e led_name);


extern io_type_t leds_out[LED_ALL];
#endif
