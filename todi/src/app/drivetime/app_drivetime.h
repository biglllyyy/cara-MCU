#ifndef _DRIVE_TIME_H
#define _DRIVE_TIME_H

#include "comm_typedef.h"

typedef struct
{
	U8 Hour;
	U8 Minute;
	U8 Second;
}DrivingTime_T;

void app_drivetime_init(void);
void app_drivetime_task_1000ms(void);
void save_ee_drive_time(void);
void drive_time_clear(void);


#endif
