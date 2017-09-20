#include <string.h>
#include "app_trip.h"
#include "app_spd.h"
#include "crc16.h"
#include "aes.h"
#include "hal_reload_timer.h"
#include "mid_string.h"
#include "g_variable.h"
#include "hal_wdg.h"
#include "app_fuel.h"
#include "app_rpm.h"
#include "app_can.h"
#include "app_data_store.h"
#include "mid_filter.h"
#include "app_fuel_method2.h"
#define LARGEST(a,b,c)  (a>b?(a>c?a:c):(b>c?b:c))

/**********************************************************************************/
U16  g_u16_rest_mile = 0;
INFO_TYPE      info;
/**********************************************************************************/

void get_ee_sub_trip(void);
void get_ee_total_trip(void);

void app_trip_init(void)
{
	memset(&info,0,sizeof(info));
	
	aes_encrypt_init(); /*根据key生成expand key */
	check_ee_data();
	get_ee_total_trip(); /* 获取总里程 */
	get_ee_sub_trip(); /* 获取里程小计 */ 
	info.old_time = hal_timer_get_tick(); /* 获取当前系统节拍 */
}

/* 根据当前车速计算行驶里程 */
static void app_info_trip_cal(U16 spd)    
{
	U32 current_mile = 0;
	U32 time_passed = 0;
	
	time_passed = hal_timer_passed(info.old_time);		//the time between last time 
	info.old_time = hal_timer_get_tick();
	
	if(ON == g_u8IgnSts)
	{
		if(spd > 0)/*有车速*/
		{	
			current_mile = ((U32)spd << MAX_SHIFT_ENLARGE)*time_passed/ 360;//结果单位是m*2^16
 	
			info.sub_trip_temp += current_mile;
			
			info.total_trip_temp +=  current_mile + ((info.trip_last_bit*100) << MAX_SHIFT_ENLARGE);
			info.trip_last_bit = 0;
 	
			current_mile = info.sub_trip_temp >> MAX_SHIFT_ENLARGE;
			
			if(current_mile  >= ONE_HUNDRED)
			{
				info.Trip1++; /*小计1 */	
				info.Trip2++;	/* 小计2 */
				AFE_distance++;	/* 平均油耗 */
				set_this_trip_distance(get_this_trip_distance()+1);//本次路程的里程
				
				info.sub_trip_temp  -= (U32)ONE_HUNDRED << MAX_SHIFT_ENLARGE;
				
				if(info.Trip1 >= MAX_SUB_TRIP)
				{
					app_sub_trip1_clear();
				}
				
				if(info.Trip2 >= MAX_SUB_TRIP)
				{
					app_sub_trip2_clear();
				}
				write_sub_trip1(info.Trip1);  //100m write once
				if(AFE_distance==0xffffffff)//防止溢出
				{
					AFE_distance=0xfffffffe;
				}
			}
			
			current_mile = info.total_trip_temp  >> MAX_SHIFT_ENLARGE;	/*  total trip */
			if(current_mile >= ONE_KILOMETER)
			{
				
				info.total_trip_temp -= (U32)ONE_KILOMETER << MAX_SHIFT_ENLARGE;
				if(info.Odo == U32_INVALID_VALUE)
				{
					//DBG("ODO ERR\n",0);
				}
				else
				{	
					info.Odo++;
					if(info.Odo > MAX_SUM_ODO)
					{
						info.Odo = MAX_SUM_ODO;
					}
					else
					{
						wdg_feed();
						write_total_trip(info.Odo);
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
		
	}
}




void get_ee_sub_trip(void)
{
	U32 sub_trip;
	
	/* 读取里程小计1 */
	if(read_sub_trip1(&sub_trip) == TRUE)
	{
		info.Trip1 = sub_trip;
		info.total_trip_temp = ((sub_trip%10)*100)<<MAX_SHIFT_ENLARGE;
	}
	else
	{
		info.Trip1=0;
		write_sub_trip1(0);
	}
	
	/* 读取里程小计2 */
	if(read_sub_trip2(&sub_trip)==TRUE)
	{
		info.Trip2 = sub_trip;
		info.total_trip_temp = ((sub_trip%10)*100)<<MAX_SHIFT_ENLARGE;
	}
	else
	{
		info.Trip2=0;
		write_sub_trip2(0);
	}
}
 
U8 app_sub_trip1_clear(void)
{
	U8 ret=SUCCESS;
	info.Trip1 = 0;
	info.sub_trip_temp = 0;	
	info.total_trip_temp = 0;
	if(write_sub_trip1(0)==FAIL)
	{
		ret=FAIL;
	}
	
	return ret;
}


U8 app_sub_trip2_clear(void)
{
	U8 ret=SUCCESS;
	info.Trip2 = 0;
	info.sub_trip_temp = 0;
	info.total_trip_temp = 0;
	if(write_sub_trip2(0)==FAIL)
	{
		ret=FAIL;
	}
	
	return ret;
}

U32 app_get_total_trip(void)
{
	U32 ret = 0;
	
	if(!info.Odo_fault)
	{
		ret = info.Odo;
	}
	else
	{
		ret = U32_INVALID_VALUE;
	}
	return (ret);
}

 
U16 app_get_sub_trip1(void)
{
	//当里程小计的整数部分比总里程要大的时候，清零里程小计
	if((info.Trip1/10)>info.Odo)
	{
		info.Trip1=0;
	}
	return (info.Trip1);
}

U16 app_get_sub_trip2(void)
{
	//当里程小计的整数部分比总里程要大的时候，清零里程小计
	if((info.Trip2/10)>info.Odo)
	{
		info.Trip2=0;
	}
	return (info.Trip2);
}

U8  app_get_odo_clear_cnt(void)
{
	return(info.allow_odo_clear_cnt);
}

/* 100ms调用一次，获取车速并计算里程 */
void app_info_task100(void)
{
	U16 spd = 0;
	spd = app_get_fact_spd();
#ifdef __DEBUG__
	//spd = 650*100; /* 测试 */
#endif
	app_info_trip_cal(spd); /* 根据车速计算里程 */
}

/* 计算续航里程 */
static U16 app_info_rest_mile_cal(U16 fuel_volum_ml,U16 avg_oil_consumption)
{
	U32 temp;

	/* 总里程小于固定值时，平均油耗假设为10，以此计算续航里程 */
	if(AFE_distance < 500) /*单位0.1km,50KM*/
	{
		temp = fuel_volum_ml/(10*10);
	}
	else /* 用真实平均油耗计算续航里程 */
	{
		temp = fuel_volum_ml/avg_oil_consumption;
	}
	//dbg_string("REST mile:%d, FUEL:%d\n", temp, fuel_volum_ml/1000);
	return (U16)temp;
}

U16  app_rest_mile_get(void)
{
	return g_u16_rest_mile; 
}

void app_rest_mile_cal_task(void)
{
	U16 rpm = 0;
	U16 rest_mile_temp = 0; 
	U16 fuel_volum = 0;
	U16 avg_fuel_consump = 0;
	S16 temp;
	static U16 refresh_time_cnt=0;

	if(g_u8IgnSts)
	{
		rpm = app_get_rpm();
		if(rpm > 0)
		{
			if(refresh_time_cnt>=5)
			{
				refresh_time_cnt=0;
				
				//start cal the rest mile
				fuel_volum	 = get_fuel_volum();//先获取剩余油量
				avg_fuel_consump = app_get_avg_fuel_consump();//再获取本次路程的平均油耗
				rest_mile_temp = app_info_rest_mile_cal(fuel_volum,avg_fuel_consump);//计算续行里程
				
				//续行里程范围50-999，小于50不显示，大于999显示999
				if(rest_mile_temp<50)
				{
					g_u16_rest_mile=0;	
				}
				else if(rest_mile_temp>999)
				{
					g_u16_rest_mile=999;
				}
				else
				{
					//rest_mile_temp=rest_mile_temp;
					temp=g_u16_rest_mile-rest_mile_temp;
					
					if(Absolute(temp)>=40)
					{
						g_u16_rest_mile=rest_mile_temp;
					}
					else if(Absolute(temp)>=20)
					{
						if(temp>=0)
						{
							g_u16_rest_mile-=5;
						}
						else
						{
							g_u16_rest_mile=rest_mile_temp;
						}
					}
					else if(Absolute(temp)>=1)
					{
						if(temp>=0)
						{
							g_u16_rest_mile-=1;
						}
						else
						{
							g_u16_rest_mile=rest_mile_temp;
						}
					}
				}  
			}
			else
			{
				refresh_time_cnt++;
			}
		}
	}
	else
	{
		refresh_time_cnt=0;
	}
}

U8  app_odo_clear_no_limit(void)
{
	info.Odo = 0;
	info.total_trip_temp = 0;
	
	return (write_total_trip(0));
}

/***********************************************
 
 从存储设备中读取总里程，如果无有效值就直接
 清零并存储
 
 **********************************************/
void get_ee_total_trip(void)
{
	U32  temp_odo = 0;
	
	wdg_feed();
			
	if(read_total_trip(&temp_odo) != TRUE)
	{
		dbg_string("Not valid total trip, trying to clean\n");
		app_odo_clear_no_limit();/*里程数据清零*/
		temp_odo = 0;
	}
			
	info.Odo = temp_odo;
}
/***********************************************
 
 判断内存中数据是否有效，无效则恢复默认值
 
 **********************************************/
extern T_DATA_STORE *get_data_store_ptr(void);
void check_ee_data(void)
{
	T_DATA_STORE *sData;
	sData = get_data_store_ptr();
	if (0 == sData)
	{
	    //!<数据未初始化
	    memset(&info,0,sizeof(info));
		write_total_trip(0);//!<调用函数对数据进行保存
	}
}

