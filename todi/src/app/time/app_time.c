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

	utc_time = getCurrentTime();
	g_u32_utcTime= mktime(&utc_time);
}
