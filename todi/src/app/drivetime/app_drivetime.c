#include <string.h>
#include "app_drivetime.h"
#include "app_uart_cmd.h"
#include "app_rpm.h"
#include "app_data_store.h"

static void get_ee_drive_time(void);


static DrivingTime_T DrivingTime;

void app_drivetime_init(void)
{
	/* memset(&DrivingTime,0,sizeof(DrivingTime)); */
	get_ee_drive_time();

	memset(&uart_special_data.specialInfo.DrivingTime,0,sizeof(uart_special_data.specialInfo.DrivingTime));
}

void app_drivetime_task_1000ms(void)
{
	if(ON == g_u8IgnSts)
	{
		if(app_get_rpm())
		{
			DrivingTime.Second++;
			if(DrivingTime.Second>=60)
			{
				DrivingTime.Second-=60;
				DrivingTime.Minute++;
				if(DrivingTime.Minute>=60)
				{
					DrivingTime.Minute-=60;
					DrivingTime.Hour++;
					if(DrivingTime.Hour>99)
					{
						DrivingTime.Hour=0;
						DrivingTime.Second=0;
						DrivingTime.Minute=0;
					}
				}
			}
		}
		else
		{
			
		}
	}
	else
	{
		/* memset(&DrivingTime,0,sizeof(DrivingTime)); */
	}
	
	uart_special_data.specialInfo.DrivingTime.Hour=DrivingTime.Hour;
	uart_special_data.specialInfo.DrivingTime.Minute=DrivingTime.Minute;
}

void save_ee_drive_time(void)
{
	write_drive_time(DrivingTime.Hour, DrivingTime.Minute);
//	dbg_string("[write]hour:%d,min:%d\n", DrivingTime.Hour, DrivingTime.Minute);
}

static void get_ee_drive_time(void)
{
	DrivingTime_T drive_time_temp;
	
	if(read_drive_time(&drive_time_temp.Hour, &drive_time_temp.Minute) == TRUE)
	{
		memcpy(&DrivingTime, &drive_time_temp, sizeof(DrivingTime_T));
	}
	else
	{
		memset(&DrivingTime, 0, sizeof(DrivingTime_T));
		write_drive_time(DrivingTime.Hour, DrivingTime.Minute);
	}
	
}

void drive_time_clear(void)
{
	memset(&DrivingTime, 0, sizeof(DrivingTime_T));
	write_drive_time(DrivingTime.Hour, DrivingTime.Minute);
}

