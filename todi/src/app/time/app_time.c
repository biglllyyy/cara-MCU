#include <time.h>
#include "app_time.h"
#include "mid_ds1302.h"
#include "app_uart_cmd.h"
#include "g_variable.h"
#include "app_info.h"

struct tm utc_time;

struct tm getCurrentTime()
{
	return (getCurrentTime_DS1302());
}
void setCurrentTime(struct tm time)
{
	setCurrentTime_DS1302(time);
}
void app_update_time_task()
{
	time_t temp=0;	
	//sizeoftime = sizeof(time_t);
	if(g_tUart1Rec.time_set_enable)
	{
		temp = g_tUart1Rec.u32UTCTime;
		setCurrentTime(*gmtime(&temp));		
	}
	utc_time = getCurrentTime();
	g_u32_utcTime= mktime(&utc_time);
}
