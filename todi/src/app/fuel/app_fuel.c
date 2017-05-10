#include <stdlib.h>
#include "app_fuel.h"
#include "app_spd.h"
#include "mid_filter.h"
#include "fuel_cfg.h"
#include "string.h"
#include "arithmetic.h"
#include "mid_switch.h"
#include "mid_adc.h"
#include "io_cfg.h"
#include "g_variable.h"
#include "mid_trigger.h"
#include "app_rpm.h"
#include "app_trip.h"
#include "app_data_store.h"
#include "app_printf.h" 

shake_filter_t  fuel_filter;
U16 g_u16_fuel_res = 0;
U16 g_u16_fuel_volum = U16_INVALID_VALUE;

U16 g_u16_instance_consump = 0;//无效值
U32 s_u32_instance_accumulation = 0;
U16 s_u16_accumulation_count = 0;
U16 s_u16_spd_accumulate = 0;

U32	AFE_fuel_used;
U32	AFE_distance;
U32	AFE_Fuel_Accum;
U32	AFE_Distance_Accum;
U16 AFE;
U16 FuelSigTn_bak;
U16 FuelSigTn;
U32 AFE_fuel_used_bak;
U16 rpm_300_time_cnt;
U16 refresh_time_cnt;

/* 计算燃油的关键数据 */
typedef struct{
	U16 *buf;	/* 缓冲指针 */
	U8 buf_cnt;	/* 缓冲中的元素个数 */
	U8 pos;		/* 当前位置 */ 
	U8 dirty;	/* 缓冲是否脏 */
	U16 avg;	/* 缓冲中内容的平均值 */
}T_AD_BUF;

static U16 ad_buf_1[FUEL_AD_BUF_1_NUM];	/* 用于采样的一级缓存 */
static U16 ad_buf_2[FUEL_AD_BUF_2_NUM];	/* 用于采样的二级缓存 */

static T_AD_BUF m_buf_1 = {ad_buf_1, FUEL_AD_BUF_1_NUM, 0, 0, 0};
static T_AD_BUF m_buf_2 = {ad_buf_2, FUEL_AD_BUF_2_NUM, 0, 0, 0};

static U32 tk_has_speed;	/* 上一次有车速的tick */

/******************************************************************************/


static const T_LINEAR_INTERPOLATION fuel_res_2_vol[] = 
{
	{ RES_FUEL_FULL, ((FUEL_FULL_VOLUM_ML)*100UL/100) },
	{ 26<<RES_SHIFT, ((FUEL_FULL_VOLUM_ML)*90UL/100) },
	{ 31<<RES_SHIFT, ((FUEL_FULL_VOLUM_ML)*80UL/100) },
	{ 38<<RES_SHIFT, ((FUEL_FULL_VOLUM_ML)*70UL/100) },
	{ 46<<RES_SHIFT, ((FUEL_FULL_VOLUM_ML)*60UL/100) },
	{ 56<<RES_SHIFT, ((FUEL_FULL_VOLUM_ML)*50UL/100) },
	{ 96<<RES_SHIFT, ((FUEL_FULL_VOLUM_ML)*13UL/100) },
	{ 101<<RES_SHIFT, ((FUEL_FULL_VOLUM_ML)*10UL/100) },
	{ RES_FUEL_EMPTY, ((FUEL_FULL_VOLUM_ML)*0UL/100) },
};

/*---------------------------------------------------------------------------*/
/* Local Fucntion declaration                                                */
/*---------------------------------------------------------------------------*/
static U16  app_fuel_ad_2_res(U16 ad1,U16 ad2);
static U16  app_convert_res_2_volume(U16 res);
static U16  app_convert_ad_2_volume(U16 ad);

static U8 g_u8IgnSts_bak=OFF;
static U8 ign_on_action_flag=0;
	
/* 初始化缓冲, 把旧的燃油数据清除 */
static void init_fuel_buffer(void)
{
	memset(&ad_buf_1,0,sizeof(ad_buf_1));
	m_buf_1.pos = 0;
	m_buf_1.dirty = 0;
	m_buf_1.avg = 0;
	
	memset(&ad_buf_2,0,sizeof(ad_buf_2));
	m_buf_2.pos = 0;
	m_buf_2.dirty = 0;
	m_buf_2.avg = 0;
		
	fuel_filter.last_value = RES_FUEL_EMPTY;
	g_u16_fuel_res = 0;
	g_u16_fuel_volum = 0xffff;
		
	ign_on_action_flag=0; /* 丢掉旧数据重新计算燃油 */
	g_u8IgnSts_bak=OFF;

}

void app_init_fuel(void)
{
	init_fuel_buffer();
	mid_adc_start(FUEL_CHN1);
	
	AFE_fuel_used = 0;
	AFE_distance = 0;

	/* g_u16_instance_consump = INVALID_FUEL_CONSUMPTION; */
	s_u32_instance_accumulation = 0;
	s_u16_accumulation_count = 0;
	s_u16_spd_accumulate = 0;
	
}
U8 get_g_u8IgnSts_bak(void)
{
	return g_u8IgnSts_bak;
}
void set_g_u8IgnSts_bak(U8 value)
{
	g_u8IgnSts_bak = value;
}
/* IGN ON，立即计算燃油量 */
U8 app_read_fuel_ign_on(void)
{
	static U8 read_flag = 0;
	
	if(g_u8IgnSts == OFF)
	{
		read_flag = 0;
		return 0;
	}
	
	/* 点火，重新计算油量 */
	if (read_flag == 0) 
		init_fuel_buffer();
	if (read_flag > 3) return 0;	/* 只运行3次 */
	
	app_pro_fuel_task();
	
	read_flag += 1;

	return 0;
}

/* 根据剩余油量计算燃油百分比 */
U8 app_get_fuel_percent(void)
{
	U8 ret = 0;

	ret = (g_u16_fuel_volum*100UL)/FUEL_FULL_VOLUM_ML ;	
	
	return ret;
}

/* 返回剩余油量，单位ML */
U16 app_get_fuel_volum_ml(void)
{	
	return g_u16_fuel_volum;
}


/* 将AD值放入指定缓冲，连续放cnt次 */
static void push_ad_to_buf(T_AD_BUF *ptr,U16 ad, U16 cnt)
{
	U16 i;
	
	for (i = 0; i < cnt; i++)
	{
		if (i >= ptr->buf_cnt) break;	/* 防止越界 */

		if (ptr->buf[ptr->pos] != ad)
		{
			ptr->dirty = 1;
			ptr->buf[ptr->pos] = ad;
		}
		ptr->pos ++;
		ptr->pos %= ptr->buf_cnt;
	}
}
/* 计算指定缓冲的平均值 */
static U16 calc_buf_average(T_AD_BUF *ptr)
{
	U32 sum;
	U16 i;
	//if (m_buf_1.dirty) dbg_printf("Buf1 dirty\n");
	if (ptr->dirty == 0) return ptr->avg;
	
	sum = 0;
	for (i = 0; i < ptr->buf_cnt; i++)
		sum += ptr->buf[i];
	
	ptr->avg = (U16)(sum/ptr->buf_cnt);
	ptr->dirty = 0;

	return ptr->avg;
}

/* 返回二级缓冲AD的平均值 */
U16 app_get_average_ad(void)
{
	U16 ad;

	ad = calc_buf_average(&m_buf_2);
	return ad;
}
/* 一级缓冲的平均值进入二级缓冲, sec秒一次 */
static void buf_1_to_buf_2(U16 sec)
{
	static U32 tk;
	U16 avg;
	
	if (hal_timer_passed(tk) < sec*HZ) return;

	avg = calc_buf_average(&m_buf_1);
	if (ad_buf_2[m_buf_2.pos] != avg)
	{
		ad_buf_2[m_buf_2.pos] = avg;
		m_buf_2.dirty = 1;
	}
	/* 可以从这个打印信息的频率来判断业务逻辑是否如设计时所期望 */
	dbg_printf("push to buf 2, AD=%d, sec=%d, VOL:%d\n", m_buf_2.buf[m_buf_2.pos], sec, g_u16_fuel_volum);

	m_buf_2.pos ++;
	m_buf_2.pos %= FUEL_AD_BUF_2_NUM;
	tk = hal_timer_get_tick();
}

/* 任务:读取燃油表电阻值 */
void app_pro_fuel_task(void)
{
	U16 ad_value = 0;
	U16 fuel_res = 0;
	U16 temp, fuel_v1, fuel_v2;
	U16 ad_saved;
	static U8 refuel_flag = 0; /* 加油标志 */
	
	/* IGN OFF，丢掉所有旧数据 */
	if (g_u8IgnSts == OFF)
	{
		/* ON-->OFF, 保存油箱电阻值 */
		if (g_u8IgnSts_bak == ON)
		{
//			save_fuel_ad();		
			init_fuel_buffer();
		}
		g_u8IgnSts_bak = g_u8IgnSts;
		return;
	}

	if ((g_u8IgnSts_bak == OFF) && (g_u8IgnSts == ON))
		ign_on_action_flag=1;
		
	ad_value = mid_adc_get(FUEL_CHN1);/* 先获取AD值, 0xFFF为无效 */
	//dbg_printf("AD:%d\n", ad_value);

	/* AD值不能超过上下限 */
	ad_value = (ad_value > AD_FUEL_EMPTY)? AD_FUEL_EMPTY:ad_value;
	ad_value = (ad_value < AD_FUEL_FULL)? AD_FUEL_FULL:ad_value;

	fuel_res = app_fuel_ad_2_res(ad_value, U16_INVALID_VALUE);/* AD值转换为电阻值 */
	//dbg_printf("Fuel resistor:%d\n", fuel_res);
	if(fuel_res == U16_INVALID_VALUE) return;	/* 如果不是有效的电阻值 */
			
	temp = limit_range_filter(fuel_res,1);/* 限幅滤波，单位时间内电阻值变化大于限定值，就认为是无效值 */
			
	if(temp == U16_INVALID_VALUE) return; /* 如果经过滤波后得到的不是有效值 */
	//dbg_printf("Valid resistor:%d\n", fuel_res);
		
	temp = ad_value;
	/* 捕获到熄火-->点火的一次动作，直接取第一个“有效”阻值填满整个缓冲*/
	if (ign_on_action_flag)
	{
		tk_has_speed = hal_timer_get_tick();
		/* 保存的AD值与新AD值所标示的油量差异大于±12L，认为加过油 */
		read_fuel_tank_ad(&ad_saved);
		if ((ad_saved >= AD_FUEL_FULL) && (ad_saved <= AD_FUEL_EMPTY))
		{
			fuel_v1 = app_convert_ad_2_volume(ad_value);
			fuel_v2 = app_convert_ad_2_volume(ad_saved);
			/* 当前油量与存储油量差异小，或者有车速时，直接使用上次存储的油量  */
			if ((Absolute(fuel_v1 - fuel_v2) < REFUEL_THRESHOLD) || (app_get_fact_spd() > 0))
			{
				temp = ad_saved;
				dbg_printf("Use saved RES:%d(%d,%d)\n", temp, fuel_v1, fuel_v2);
			}
			else
				dbg_printf("Refueled:%d(new),%d(old)\n", fuel_v1, fuel_v2);
		}
		dbg_printf("get AD:%d(new), %d(old)\n", ad_value,ad_saved);
		push_ad_to_buf(&m_buf_1, temp, FUEL_AD_BUF_1_NUM);
		push_ad_to_buf(&m_buf_2, temp, FUEL_AD_BUF_2_NUM);
	}
	else
		push_ad_to_buf(&m_buf_1, temp, 1);
	
	if (app_get_fact_spd() > 0)
	{
		tk_has_speed = hal_timer_get_tick();
		refuel_flag = 0; /* 有车速，不可能加油 */
	}

	/* 一二级缓冲燃油量差异判断是否在加油 */
	ad_value = calc_buf_average(&m_buf_1);
	fuel_v1 = app_convert_ad_2_volume(ad_value);
	
	/* 有效阻值放入二级缓冲。30秒无车速时判断加油，以决定更新油量的频率 */
	if (hal_timer_passed(tk_has_speed) >= 30*HZ)
	{		
		if (Absolute(fuel_v1 - g_u16_fuel_volum) >= REFUEL_THRESHOLD) 
		{
			//dbg_printf("Fuel: %d, %d, refueling\n", fuel_v1, g_u16_fuel_volum);
			refuel_flag = 1;
		}
		
		/* 加油中，更新速度加快 */
		if (refuel_flag) buf_1_to_buf_2(1); 
		else
			if (app_get_rpm() >0) buf_1_to_buf_2(180); /* 无转速、无车速、不加油，就不用刷新剩余油量了 */
	}
	else /* 行车中，油量涨得慢，掉得快  */
	{
		if (fuel_v1 > g_u16_fuel_volum) buf_1_to_buf_2(180); 
		else 
			buf_1_to_buf_2(60);
	}
				
	/* 累加二级缓冲中的AD值，然后求平均 */
	ad_value = calc_buf_average(&m_buf_2);
	
	g_u16_fuel_volum = app_convert_ad_2_volume(ad_value);
				
	ign_on_action_flag=0;
		
	g_u8IgnSts_bak = g_u8IgnSts;	
}

/* 阻值-->燃油量, 单位ML */
static U16 app_convert_res_2_volume(U16 res)
{
	res = (res > RES_FUEL_EMPTY) ? RES_FUEL_EMPTY:res;
	res = (res < RES_FUEL_FULL) ? RES_FUEL_FULL:res;

	/* 根据电阻信号换算为燃油量 */
	return linear_interpolation(fuel_res_2_vol,ARRAY_SIZE(fuel_res_2_vol),SOURCE_UP_ORDER,res);
}

/* AD值-->燃油量，单位ML */
static U16 app_convert_ad_2_volume(U16 ad)
{
	U16 vol, res;
		
	res = app_fuel_ad_2_res(ad, U16_INVALID_VALUE);
	vol = app_convert_res_2_volume(res);
	
	return vol;
}

/* AD值转换为电阻值 */
static U16 app_fuel_ad_2_res(U16 ad1,U16 ad2)
{
	S16      ret = 0;
	
	//计算电阻值，采用不同的方法，两路输入时
	if(ad2 != U16_INVALID_VALUE)
	{
		if(ad1 > ad2)
		{
			ret = SERIOUS_RES * ad2/(ad1-ad2);
		}
		else if(ad1 < ad2)
		{
			ret = SERIOUS_RES * ad1/(ad2-ad1);
		}
		else
		{
			ret = RES_FUEL_EMPTY;
		}
	}
	else  //一路输入
	{
		if (ad1 >=8) ad1 -= 8;
		ad1 >>=1;		//give up the last bit,in fact 11 bit
		if(ad1 < 0x7FF)
		{
			ret =  (S16)((((U32)UP_RES*ad1)<<RES_SHIFT) /(AD_11_MAX - ad1));
			if(ret < 0)
			{
				ret = U16_INVALID_VALUE;
			}
		}
		else
		{
			ret = U16_INVALID_VALUE;
		}
		
	}
	return (ret);
}

U16 app_get_avg_fuel_consump(void)
{
//	U16 ret;
//	ret = AFE;	
	return AFE;
}

U16  app_get_ins_fuel_consump(void)
{
	return g_u16_instance_consump;
}

/*
1) 瞬时油耗每500ms进行更新一次；
2) 显示范围： （0.1～19.9）L/100km(刻度为0.1L/100km)；
3) 在行驶过程中以 L/100km 为单位显示瞬时油耗。
*/
void fuel_ins_consumption_cal(void)
{
	U8 spd = 0;
	/* static U8 spd_bak = 0; */

	spd = app_get_fact_spd();
	
	if(spd > 0)//行驶过程中
	{
		s_u16_accumulation_count++;
		s_u16_spd_accumulate += spd;
		
		if(s_u16_accumulation_count >= 5)
		{
			if(s_u16_spd_accumulate != 0)
				g_u16_instance_consump = (s_u32_instance_accumulation * CONSUMPTION_SCALE * 36) / ((U32)s_u16_spd_accumulate * DECIMAL_BITS);
			else
				g_u16_instance_consump = 0;
			
			//设置瞬时油耗显示范围
			if(g_u16_instance_consump >= MAX_FUEL_CONSUMPTION)
			{
				g_u16_instance_consump = MAX_FUEL_CONSUMPTION;
			}
			
			s_u16_accumulation_count = 0;
			s_u16_spd_accumulate = 0;
			s_u32_instance_accumulation = 0;
		}
	}
	else
	{
		/* spd_bak = spd; */
		s_u16_accumulation_count = 0;
		s_u32_instance_accumulation = 0;
		s_u16_spd_accumulate = 0;
		g_u16_instance_consump = 0;  	/* 无效值 */
	}
}

void app_cal_avg_fuel_consump(void)
{
	U16 rpm = app_get_rpm();
	U8	spd = app_get_fact_spd();
	static U8 read_info_flag = 0;

	if(read_info_flag == 0)
	{
		read_info_flag = 1;
		get_ee_AFE_info();
	}	
	if(ON == g_u8IgnSts)
	{
		//油耗的累计（在app_can.c里面）
		//点火后，转速大于300转，并且持续3S之后，车速大于3KM/H的情况下，才进行油耗的累计
		if(rpm>=300)
		{
			rpm_300_time_cnt++;
			rpm_300_time_cnt = (rpm_300_time_cnt>3) ? 3 : rpm_300_time_cnt;
		}
		else
		{
			rpm_300_time_cnt=0;
		}  
			
		if(rpm_300_time_cnt>=3)//有转速超过3s才开始累积，怠速时也累积
		{
			AFE_fuel_used_bak=AFE_fuel_used;//累计油耗
		}
		else
		{
			AFE_fuel_used=AFE_fuel_used_bak;//油耗保持之前的值，不进行累计
		}
			
		AFE_Fuel_Accum=AFE_fuel_used/1000000;
		//里程的累计（在app_trip.c里面）
		AFE_Distance_Accum=AFE_distance/10;
			
		//平均油耗的计算
		if(AFE_distance<1)//小于100m显示0
		{
			AFE = 0;//
		} 
		else 
		{
			if(refresh_time_cnt>=5)//5S刷新一次平均油耗
			{
				refresh_time_cnt=0;
				/* 累计油耗或者累计里程大于阀值后，将这两个数值除以2，再用来计算平均油耗 */
				if((AFE_Fuel_Accum>=200)||(AFE_Distance_Accum>=2000))
				{
					AFE_fuel_used>>=1;
					AFE_distance>>=1;
				}
				
				/* 平均油耗=累计使用燃油/累计行驶里程*100，单位0.1L/100KM */
				AFE = AFE_fuel_used / (AFE_distance * 100);
				//dbg_printf("FUEL used:%d, Distance:%d, AFE:%d, fuel:%d\n",AFE_fuel_used, AFE_distance, AFE, g_u16_fuel_volum/1000);
				//设置AFE的显示范围 :1--199
				if(AFE>199)
				{
					 AFE=199; 
					//app_clear_avg_fuel_consump();
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
		FuelSigTn_bak=0;
		FuelSigTn=0;
		rpm_300_time_cnt=0;
		refresh_time_cnt=0;
	}
}


///
void save_ee_AFE_fuel_used(void)
{
	write_afe_fuel(AFE_fuel_used);
}

void save_ee_AFE_distance(void)
{
	write_afe_distance(AFE_distance);
}



void get_ee_AFE_info(void)
{	
	read_afe_distance(&AFE_distance);
	if (read_afe_fuel(&AFE_fuel_used) == TRUE)
	{
		AFE_fuel_used_bak=AFE_fuel_used;
		//AFE=0;
		AFE=AFE_fuel_used/(AFE_distance*100);//不清0
		FuelSigTn_bak=0;
		FuelSigTn=0;
		rpm_300_time_cnt=0;
		refresh_time_cnt=0;
		return;
	}
			
	
	/* 判断到这里，认为是第一次上电 */
	//app_clear_avg_fuel_consump();
}

void app_clear_avg_fuel_consump(void)
{
	AFE_fuel_used=750000;//单位：uL
	AFE_distance=100;//单位：0.1KM
	save_ee_AFE_fuel_used();
	save_ee_AFE_distance();
	AFE_Fuel_Accum=0;//单位：L
	AFE_Distance_Accum=0;//单位：KM
	AFE=0;//单位：0.1L/100KM
	FuelSigTn_bak=0;
	FuelSigTn=0;
	AFE_fuel_used_bak=AFE_fuel_used;//单位：uL
	rpm_300_time_cnt=0;
	refresh_time_cnt=0;
}
/*
void save_fuel_ad(void)
{
	U16 res;
	
	res = app_get_average_ad();
	write_fuel_tank_ad(res);
	
	dbg_printf("Save RES:%d\n", res);
}
*/



