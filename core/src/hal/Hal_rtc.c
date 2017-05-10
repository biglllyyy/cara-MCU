#include "Hal_rtc.h"
#include "mb91520.h"

#define RTC_YEAR_MAX 180

static const U16 day_accumulate[RTC_YEAR_MAX]={
0,365,730,1096,1461,1826,2191,2557,2922,3287,3652,4018,4383,4748,5113,5479,5844,6209,6574,6940,7305,7670,8035,
8401,8766,9131,9496,9862,10227,10592,10957,11323,11688,12053,12418,12784,13149,13514,13879,14245,14610,14975,
15340,15706,16071,16436,16801,17167,17532,17897,18262,18628,18993,19358,19723,20089,20454,20819,21184,21550,
21915,22280,22645,23011,23376,23741,24106,24472,24837,25202,25567,25933,26298,26663,27028,27394,27759,28124,
28489,28855,29220,29585,29950,30316,30681,31046,31411,31777,32142,32507,32872,33238,33603,33968,34333,34699,
35064,35429,35794,36160,36525,36890,37255,37621,37986,38351,38716,39082,39447,39812,40177,40543,40908,41273,
41638,42004,42369,42734,43099,43465,43830,44195,44560,44926,45291,45656,46021,46387,46752,47117,47482,47847,
48212,48577,48942,49308,49673,50038,50403,50769,51134,51499,51864,52230,52595,52960,53325,53691,54056,54421,
54786,55152,55517,55882,56247,56613,56978,57343,57708,58074,58439,58804,59169,59535,59900,60265,60630,60996,
61361,61726,62091,62457,62822,63187,63552,63918,64283,64648,65013,65379};

static const U16 day_accumulate_within_normal_year[12]={31,59,90,120,151,181,212,243,273,304,334,365};

static const U16 day_accumulate_within_leap_year[12]={31,60,91,121,152,182,213,244,274,305,335,366};

static void RTC_Refresh_DateTime(void);
static Date RTC_Day_Convert_Date(U16 day);
static U16 RTC_Date_Convert_Day(Date date);
static Bool RTC_is_leap_year(U16 year);
static void RTC_init_hardware(U8 clock_source);

static  DateTime date_time;

DateTime RTC_get_date_time(void)
{
	RTC_Refresh_DateTime();
	return date_time;
}

static void RTC_Refresh_DateTime(void)
{
	if(date_time.day_raw!=WTDR)
	{
		date_time.day_raw=WTDR;
		date_time.date=RTC_Day_Convert_Date(date_time.day_raw);
	}

	date_time.time.hour=WTHR & 0x1F;
	date_time.time.minute=WTMR & 0x3F;
	date_time.time.second=WTSR & 0x3F;
}

static Date RTC_Day_Convert_Date(U16 day)
{
	Date date;
	U32 i;
	U16 year;
	U16 day_within_year;
	Bool is_leap_year;
	
	year=day/365;
	if(day<day_accumulate[year])
	{
		year=year-1;
	}

	day_within_year=day-day_accumulate[year];
	date.year=1970+year;

	is_leap_year=RTC_is_leap_year(date.year);

	for(i=0;i<12;i++)
	{
		if(is_leap_year)
		{
			if(day_within_year<day_accumulate_within_leap_year[i])
			{
				date.month=i+1;
				if(i==0)
				{
					date.day=day_within_year+1;
				}
				else
				{
					date.day=day_within_year-day_accumulate_within_leap_year[i-1]+1;
				}
				break;
			}
		}
		else
		{
			if(day_within_year<day_accumulate_within_normal_year[i])
			{
				date.month=i+1;
				if(i==0)
				{
					date.day=day_within_year+1;
				}
				else
				{
					date.day=day_within_year-day_accumulate_within_normal_year[i-1]+1;
				}
				break;
			}
		}
	}
	return date;
}

static U16 RTC_Date_Convert_Day(Date date)
{
	Bool is_leap_year;
	U16 day;
	
	if((date.year<1970)||(date.year>2149)||(date.month>12)||(date.day>31))
	{
		return 0;
	}

	is_leap_year=RTC_is_leap_year(date.year);
	day=day_accumulate[date.year-1970];

	if(date.month==1)
	{
		day+=date.day-1;
	}
	else
	{
		if(is_leap_year)
		{
			day+=day_accumulate_within_leap_year[date.month-2]+date.day-1;
		}
		else
		{
			day+=day_accumulate_within_normal_year[date.month-2]+date.day-1;
		}
	}

	return day;
}

static Bool RTC_is_leap_year(U16 year)
{
	if(year%100==0)
	{
		if(year%400==0)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}

	if(year%4==0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

static void RTC_init_hardware(U8 clock_source)							
{
	switch(clock_source)
	{
		case MAINCLOCK:
		{
			//Use Main Clock as RTC Clock
			CCTL_CCRTSELR_CSC = 0;

			//WTBRL = (RTC_OPERATIONCLK/4) - 1;			//	Set number of clock cycle for 0.5s
			*((U32 *)(0x0564)) = (U32)((RTC_OPERATIONCLK/4) - 1);			//	Set number of clock cycle for 0.5s
			break;
		}
		case SUBCLOCK:
		{
			//Use Sub Clock as RTC Clock
			CCTL_CCRTSELR_CSC = 1;

			//WTBRL = (RTC_OPERATIONCLK/4) - 1;			//	Set number of clock cycle for 0.5s
			*((U32 *)(0x0564)) = (U32)((RTC_OPERATIONCLKSUB/4) - 1);		
			break;
		}
		default:
			break;
			
	}

	WTCRL_ST = 0;								//	ST: start = 0, all the counters (Second, Minute, Hour, Day) are cleared
}

void RTC_init(U16 year, U8 month,U8 day,U8 hour, U8 minute, U8 second, U8 Clock_Source)
{
	if((hour>24)||(minute>60)||(second>60))
	{
		return;
	}

	date_time.date.year=year;
	date_time.date.month=month;
	date_time.date.day=day;
	date_time.day_raw=RTC_Date_Convert_Day(date_time.date);

	if(date_time.day_raw==0)
	{
		return;
	}
	
	date_time.time.hour=hour;
	date_time.time.minute=minute;
	date_time.time.second=second;
	
	RTC_init_hardware(Clock_Source);

	WTDR = date_time.day_raw;
	WTHR = date_time.time.hour;
	WTMR = date_time.time.minute;
	WTSR = date_time.time.second;
	
	WTCRL_ST = 1;
}

