#include <string.h>
#include "mid_led.h"
#include "mid_scheduler.h"
#include "mid_pin.h"

#ifdef WIN32
#include "stdio.h"
#endif
/******************************************************************************/
typedef struct  
{
	LED_STS_e		sts;		//record led on/off/blink sts
	led_handle		pBlink;		//led blink handle
	led_blink_frq_e frq;		//blink frq
}led_ctl_t;

/******************************************************************************/
/*led variabble */
static led_ctl_t	led_ctl[LED_ALL] ={LED_OFF,0,F_1HZ};
io_type_t		    leds_out[LED_ALL] = {0};
void mid_init_led(void)
{
	U8 index = 0;
	//清除数组的内容
	memset(led_ctl,0,sizeof(led_ctl));
	for(index =0;index < LED_ALL;index++)
	{
		led_ctl[index].pBlink = NULL;
		led_ctl[index].frq = F_1HZ;
	}
}
void mid_led_ctl(LED_NAME_e name,LED_STS_e sts)
{
	if(name < LED_ALL)
	{
#ifdef	WIN32
		if(led_ctl[name].sts !=  sts)
		{
			//DBG("LED:%s",(U8)name);
			//DBG("sts:%d\n",sts);
		}
#endif
		switch(sts)
		{
		case LED_ON:
		case LED_OFF:
			if(LED_BLINK == led_ctl[name].sts)
			{
				MidSchDeleteTask(led_ctl[name].pBlink);
			}
			mid_set_io_sts(&leds_out[name],sts);
			led_ctl[name].sts = sts;
			break;
		case LED_BLINK:
			if(LED_BLINK != led_ctl[name].sts)
			{
				led_ctl[name].sts = sts;
				switch(led_ctl[name].frq)
				{
				case F_1HZ:
					MidSchAddTask(led_ctl[name].pBlink,500);
					break;
				case F_2HZ:
					MidSchAddTask(led_ctl[name].pBlink,250);
					break;
				case F_5HZ:
					MidSchAddTask(led_ctl[name].pBlink,100);
					break;
				default:break;
				}
#ifdef WIN32
				printf("LED[%d]= %d\n",name,led_ctl[name].sts);
#endif
			}
			break;
		default:
			break;
		}
	}
}
U8 mid_led_blink_register(led_handle hdl,LED_NAME_e led_name,led_blink_frq_e blink_frq)
{
	U8 ret = FAIL;
	if((hdl != NULL)&&(led_name < LED_ALL))
	{
		led_ctl[led_name].pBlink = hdl;
		led_ctl[led_name].frq = blink_frq;
		ret = TRUE;
	}
	return ret;
}


U8  mid_get_led_onoff_sts(LED_NAME_e led_name)
{
	return(leds_out[led_name].value);
}
