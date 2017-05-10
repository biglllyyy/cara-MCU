#include <time.h>
#include "app_time.h"
#include "mid_ds1302.h"
#include "app_uart_cmd.h"
#include "g_variable.h"

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
	if((time_t)mcu_rec_general.settingsInfo.Time.u32Time!=mktime(&utc_time)
		&&mcu_rec_general.settingsInfo.Time.u32Time!=0)
	{
		temp = mcu_rec_general.settingsInfo.Time.u32Time;
		setCurrentTime(*gmtime(&temp));		
	}
	utc_time = getCurrentTime();
	g_u32_utcTime= mktime(&utc_time);
}
