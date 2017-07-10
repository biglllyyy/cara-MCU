#ifndef _RTC_H_
#define _RTC_H_

#include "comm_typedef.h"

//Clock
#define RTC_OPERATIONCLK		4000000	//4/*Mhz, Master CLK (Crystal)*/
#define RTC_OPERATIONCLKSUB 	32000	/*32Khz, Sub CLK (Crystal)*/

//RTC Clock Source
#define MAINCLOCK 0
#define SUBCLOCK  1

typedef struct
{
	U16 year;
	U8 month;
	U8 day;
}Date;

typedef struct
{
	U8 hour;
	U8 minute;
	U8 second;
}Time;

typedef struct
{
	Date date;
	Time time;
	U16 day_raw;
}DateTime;

extern void RTC_init(U16 year, U8 month,U8 day,U8 hour, U8 minute, U8 second, U8 Clock_Source);
extern DateTime RTC_get_date_time(void);

#endif
