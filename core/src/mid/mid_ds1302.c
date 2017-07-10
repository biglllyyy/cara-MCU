#include <time.h>
#include <stdlib.h>
#include "mid_ds1302.h"
#include "hal_ds1302.h"
#include "mid_dbg.h"


/*初始时间默认值*/
#define DEFAULT_YEAR	(2016)
#define DEFAULT_MON		(12)
#define DEFAULT_DATE	(1)
#define DEFAULT_HOUR	(0)
#define DEFAULT_MIN		(0)
#define DEFAULT_SEC     (0)
#define DEFAULT_WDAY    (4)
/*MCU上电时，如果小于这个时间，则设置为初始化时间*/
#define CMP_YEAR	(2016)
#define CMP_MON		(10)
#define CMP_DATE	(26)
#define CMP_HOUR	(0)
#define CMP_MIN		(0)
#define CMP_SEC     (0)

#define RETRY           (4)
#define DS1302_CONVERT_DEC2HEX(value) ((value)%10+(((value)/10)<<4)) 
#define DS1302_CONVERT_HEX2DEC(value) (((value)&0x0F)+((value)>>4)*10) 
#if (REF_YEAR!=2000)
#error reference year parameter is error
#endif

/* 应用层函数指针 */
void (*ptr_ds1302_io_config)(void);

static struct tm current_time;
static void delay_ms(U16 a_ms)
{
    U16 a_u16xValue = 0, a_u16iValue = 0, a_u16kValue = 0;

    for(a_u16xValue=0; a_u16xValue<a_ms; a_u16xValue++)     /*ms*/
    {
        for(a_u16iValue = 0; a_u16iValue < 1000; a_u16iValue++)    /*us*/
        {
            for(a_u16kValue=0; a_u16kValue<8; a_u16kValue++)
            {
                __wait_nop();   /*32 instructions for one us */
            }
        }
    }
}

static uint8_t readTimePara(uint8_t addr)
{
	uint8_t ret;
	ret=readDS1302(addr|DS1302_READ_MASK);
	return ret;
}
static ERROR_CODE writeTimePara(uint8_t addr,uint16_t data)
{
	uint8_t cnt=0;
	uint8_t read_value=0;
	do
	{
		DS1302WriteProtect(DISABLE);
		writeDS1302(addr,data);
		DS1302WriteProtect(ENABLE);
		read_value=readTimePara(addr);
		cnt++;
		delay_ms(1);
	}while(cnt<RETRY&&read_value!=data);
	if(read_value==data)
		return SUCCESS;
	else
		return FAIL;
}
static void mid_DS1302_BurstWrite(struct tm time)
{
	uint8_t buf[8]={0};
	buf[0] = DS1302_CONVERT_DEC2HEX(time.tm_sec);
    buf[1] = DS1302_CONVERT_DEC2HEX(time.tm_min);
    buf[2] = DS1302_CONVERT_DEC2HEX(time.tm_hour);
    buf[3] = DS1302_CONVERT_DEC2HEX(time.tm_mday);
    buf[4] = DS1302_CONVERT_DEC2HEX(time.tm_mon+1);
    buf[5] = DS1302_CONVERT_DEC2HEX(time.tm_wday+1);
    buf[6] = DS1302_CONVERT_DEC2HEX((time.tm_year+1900-REF_YEAR));
	writeDS1302_burst(buf);
}
static void mid_DS1302_BurstRead(void)
{
	uint8_t buf[8]={0};
	readDS1302_burst(buf);
	current_time.tm_sec=DS1302_CONVERT_HEX2DEC(buf[0]);
	current_time.tm_min=DS1302_CONVERT_HEX2DEC(buf[1]);
	current_time.tm_hour=DS1302_CONVERT_HEX2DEC(buf[2]);
	current_time.tm_mday=DS1302_CONVERT_HEX2DEC(buf[3]);
	current_time.tm_mon=DS1302_CONVERT_HEX2DEC(buf[4])-1;
	current_time.tm_wday=DS1302_CONVERT_HEX2DEC(buf[5])-1;
	current_time.tm_year=DS1302_CONVERT_HEX2DEC(buf[6])+REF_YEAR-1900;
}
struct tm getCurrentTime_DS1302(void)
{
	mid_DS1302_BurstRead();
	return current_time;
}
ERROR_CODE setCurrentTime_DS1302(struct tm time)
{
	uint8_t ret=0;
	uint8_t diffcnt=0;
	uint8_t para_check=0;
	diffcnt=(time.tm_sec!=current_time.tm_sec)+(time.tm_min!=current_time.tm_min)+
		(time.tm_hour!=current_time.tm_hour)+(time.tm_mday!=current_time.tm_mday)+
		(time.tm_mon!=current_time.tm_mon)+(time.tm_year!=current_time.tm_year)+
		(time.tm_wday!=current_time.tm_wday);
	para_check=!(time.tm_sec<=59&&time.tm_sec>=0)+!(time.tm_min<=59&&time.tm_min>=0)+
		!(time.tm_hour<=23&&time.tm_hour>=0)+!(time.tm_mday<=31&&time.tm_mday>=1)+
		!(time.tm_mon<=11&&time.tm_mon>=0)+!(time.tm_year>=(REF_YEAR-1900))+
		!(time.tm_wday<=6&&time.tm_wday>=0);
	if(para_check)
		return FAIL;
	if(diffcnt==0)
		return SUCCESS;
	if(diffcnt>=3)
	{
		DS1302WriteProtect(DISABLE);
		mid_DS1302_BurstWrite(time);
		DS1302WriteProtect(ENABLE);
		mid_DS1302_BurstRead();
		diffcnt=(time.tm_sec!=current_time.tm_sec)+(time.tm_min!=current_time.tm_min)+
		(time.tm_hour!=current_time.tm_hour)+(time.tm_mday!=current_time.tm_mday)+
		(time.tm_mon!=current_time.tm_mon)+(time.tm_year!=current_time.tm_year)+
		(time.tm_wday!=current_time.tm_wday);
		if(diffcnt)
			ret=FAIL;
		else
			ret=SUCCESS;
	}
	else
	{
		if(time.tm_sec!=current_time.tm_sec)
		{
			current_time.tm_sec=time.tm_sec;
			ret=writeTimePara(DS1302_REGADDR_SEC,DS1302_CONVERT_DEC2HEX((uint8_t)time.tm_sec));
		}
		if(time.tm_min!=current_time.tm_min)
		{
			current_time.tm_min=time.tm_min;
			ret=writeTimePara(DS1302_REGADDR_MIN,DS1302_CONVERT_DEC2HEX((uint8_t)time.tm_min));
		}
		if(time.tm_hour!=current_time.tm_hour)
		{
			current_time.tm_hour=time.tm_hour;
			ret=writeTimePara(DS1302_REGADDR_HR,DS1302_CONVERT_DEC2HEX((uint8_t)time.tm_hour));
		}
		if(time.tm_mday!=current_time.tm_mday)
		{
			current_time.tm_mday=time.tm_mday;
			ret=writeTimePara(DS1302_REGADDR_DATE,DS1302_CONVERT_DEC2HEX((uint8_t)time.tm_mday));
		}
		if(time.tm_mon!=current_time.tm_mon)
		{
			current_time.tm_mon=time.tm_mon;
			ret=writeTimePara(DS1302_REGADDR_MON,DS1302_CONVERT_DEC2HEX((uint8_t)(time.tm_mon+1)));
		}
		if(time.tm_year!=current_time.tm_year)
		{
			current_time.tm_year=time.tm_year;
			ret=writeTimePara(DS1302_REGADDR_YEAR,DS1302_CONVERT_DEC2HEX((uint8_t)(time.tm_year+1900-REF_YEAR)));
		}
		if(time.tm_wday!=current_time.tm_wday)
		{
			current_time.tm_wday=time.tm_wday;
			ret=writeTimePara(DS1302_REGADDR_WDAY,DS1302_CONVERT_DEC2HEX((uint8_t)(time.tm_wday+1)));
		}
	
	}
	return ret;
}
void setDS1302Default(void)
{
	uint32_t current_cal_time,default_cal_time;
	struct tm time;
	mid_DS1302_BurstRead();
	current_cal_time=mktime(&current_time);
	time.tm_sec=DEFAULT_SEC;
	time.tm_min=DEFAULT_MIN;
	time.tm_hour=DEFAULT_HOUR;
	time.tm_mday=DEFAULT_DATE;
	time.tm_mon=DEFAULT_MON-1;
	time.tm_wday=DEFAULT_WDAY;
	time.tm_year=DEFAULT_YEAR-1900;;
	default_cal_time=mktime(&time);
	if(current_cal_time<default_cal_time)
	{
		current_time=time;
		DS1302WriteProtect(DISABLE);
		mid_DS1302_BurstWrite(current_time);
		DS1302WriteProtect(ENABLE);
	}
	
}
void mid_DS1302_close(void)
{
	hal_stop_DS1302();
}
void mid_DS1302_open(void (*ptr)(void))
{
	/* 回调APP层的初始化函数 */
	if (ptr)
		ptr_ds1302_io_config = ptr;
	if (ptr_ds1302_io_config)
		ptr_ds1302_io_config();
	hal_start_DS1302();
	setDS1302Default();
}

