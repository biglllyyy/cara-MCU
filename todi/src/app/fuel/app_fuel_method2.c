#include "app_fuel_method2.h"
#include "fuel_cfg.h"
#include "app_data_store.h"
#include "arithmetic.h"
#include "g_variable.h"
#include "mid_adc.h"
#include "app_spd.h"
#include "app_rpm.h"
#include "mid_filter.h"

#define FUEL_CHN1		(12)
#define FUEL_FULL_VOLUM_ML (58000)
#define ADBUF1_CNT    (10)
#define ADBUF2_CNT   (9)
#define AFE_CNT		 (5)

static U16 adbuf1[ADBUF1_CNT];
static U16 adbuf2[ADBUF2_CNT];

typedef struct{
	U16* buf; /*缓冲指针*/
	U16 max_ad;
	U16 min_ad;
	U8 buf_cnt;
	U8 pos;
}T_BUF;

typedef struct{
	U16 sec; //更新时间
	U8  step;//步长
}T_FUEL_CHANGE_ARGV;

/*AD与油量对应表*/
static const T_LINEAR_INTERPOLATION fuel_ad_2_volm[] = 
{
	{ AD_FUEL_FULL, ((FUEL_FULL_VOLUM_ML)*100UL/100) },//21欧
//	{ 970, 			 ((FUEL_FULL_VOLUM_ML)*90UL/100) },//
//	{ 1084, 		 ((FUEL_FULL_VOLUM_ML)*80UL/100) },//
//	{ 1212, 		 ((FUEL_FULL_VOLUM_ML)*70UL/100) },//
//	{ 1350, 		 ((FUEL_FULL_VOLUM_ML)*60UL/100) },//
	{ 1460, 		 ((FUEL_FULL_VOLUM_ML)*50UL/100) },//55
	{ 2000, 		 ((FUEL_FULL_VOLUM_ML)*15UL/100) },//95
	{ 2100,          ((FUEL_FULL_VOLUM_ML)*10UL/100) },//105
	{ AD_FUEL_EMPTY, ((FUEL_FULL_VOLUM_ML)*0UL/100) }, //120
};

typedef enum{
	FUEL_STATE_NO_ADD = 0,
	FUEL_STATE_ADD,
	FUEL_STATE_LEAK,
	FUEL_STATE_ALL
}E_FUEL_STATE;



static U8 flag_ign_off2on = 0;
static U32 tk_ign_off2on = 0;

static U16 actual_fuel_ad;
static U16 disp_fuel_ad;
static U16 fuel_volum;
static U8 fuel_percent3;

static U16 this_trip_AFE;
static U16 this_afe[AFE_CNT];
static U32 this_trip_used_fuel = 0;
static U32 this_trip_used_fuel_bak = 0;
static U32 this_trip_distance  = 0;
static U32 prev_trip_used_fuel = 0;
static U32 prev_trip_distance  = 0;
static U32 this_trip_time_off2on = 0;

U32 total_used_fuel = 0;
U32 total_used_fuel_ign_off2on = 0;


static U16 test_data_ad_saved = 0;

static T_BUF m_adbuf1 = {adbuf1, 0, 0, ADBUF1_CNT, 0};
static T_BUF m_adbuf2 = {adbuf2, 0, 0, ADBUF2_CNT, 0};
static T_BUF m_afebuf = {this_afe, 0, 0, AFE_CNT, 0};
static T_FUEL_CHANGE_ARGV fuel_change_argv;


static E_FUEL_STATE has_added_fuel(U16 cmp_fuel_ad);
static U16 ad_to_volm(U16 ad);
static void first_ignon_process();
static void adbuf1_to_adbuf2(U16 sec);
static void disFuel_damping_argv_system(T_FUEL_CHANGE_ARGV* data);
void clac_disp_fuel_ad(T_FUEL_CHANGE_ARGV* data);

void init_adbuf()
{
	memset(&adbuf1, 0, sizeof(adbuf1));
	memset(&adbuf2, 0, sizeof(adbuf2));
	memset(&this_afe, 0, sizeof(this_afe));
	m_adbuf1.pos = 0;
	m_adbuf2.pos = 0;
	actual_fuel_ad = 0;
	fuel_percent3 = 0;
}

void push_ad_to_adbuf(T_BUF* ptr, U16 ad)
{
	ptr->buf[ptr->pos] = ad;
	ptr->pos ++;
	ptr->pos %= ptr->buf_cnt;
}

/*找到buf中最大值*/
void find_buf_maxVal(T_BUF* ptr)
{
	U16 max_data = ptr->buf[0];
	U8 i;
	U8 max_pos = 0;
	
	for(i=1; i< ptr->buf_cnt; i++)
	{
		if(max_data < ptr->buf[i])
		{
			max_data = ptr->buf[i];
			max_pos = i;
		}
	}
//	dbg_string("max_data:%d max_pos:%d\n",max_data,max_pos);
	ptr->max_ad = ptr->buf[max_pos]; 
}
/*找到buf中最小值*/
void find_buf_minVal(T_BUF* ptr)
{
	U16 min_data = ptr->buf[0];
	U8 i;
	U8 min_pos = 0;
	
	for(i=1; i< ptr->buf_cnt; i++)
	{
		if(min_data > ptr->buf[i])
		{
			min_data = ptr->buf[i];
			min_pos = i;
		}		
	}
//	dbg_string("min_data:%d min_pos:%d\n", min_data,min_pos);
	ptr->min_ad = ptr->buf[min_pos]; 
}
U16 calc_avg_without_maxmin(T_BUF* ptr)
{
	U32 sum = 0;
	U16 avg = 0;
	U8 i = 0;
	
	find_buf_maxVal(ptr);
	find_buf_minVal(ptr);
	
	for(i=0; i < ptr->buf_cnt; i++)
	{
		sum += ptr->buf[i];
	}
	sum -= (ptr->max_ad + ptr->min_ad);
	avg = (U16)(sum/(ptr->buf_cnt-2));
	
	return avg;
}


void app_clac_fuel_method2_task()
{
	U16 ad_value = 0;
	static U8 ignSts_bak = OFF;
		
	/*IGN ON -> OFF*/
	if(g_u8IgnSts == OFF)
	{
		if(ignSts_bak == ON)
		{
			dbg_string("ign off disp_fuel_ad:%d\n",disp_fuel_ad);
			if(disp_fuel_ad<AD_FUEL_FULL) disp_fuel_ad = AD_FUEL_FULL;
			if(disp_fuel_ad>AD_FUEL_EMPTY) disp_fuel_ad = AD_FUEL_EMPTY;
			
			write_fuel_tank_ad(disp_fuel_ad);
			ignSts_bak = g_u8IgnSts;			
		}
		return;
	}	
	/*IGN OFF -> ON 初次上电*/
	if((g_u8IgnSts==ON)&&(ignSts_bak==OFF))
	{
		flag_ign_off2on = 1;
		init_adbuf();	
		tk_ign_off2on = hal_timer_get_tick();
	}
	ad_value = (U16)(mid_adc_get(FUEL_CHN1));
/*	
	{ //实时打印实测AD值
		static U32 cnt;
		dbg_string("\n\n@@cnt:%d ad_value:%d,g_u8IgnSts:%d ignSts_bak:%d\n",cnt++, ad_value,g_u8IgnSts,ignSts_bak);
	}
*/	
	/*如果读到的数据异常*/
	if(ad_value<AD_FUEL_FULL) ad_value = AD_FUEL_FULL-AD_FUEL_RESERVE; //防止加满油显示少一点
	if(ad_value>AD_FUEL_EMPTY) ad_value = AD_FUEL_EMPTY;
	
	push_ad_to_adbuf(&m_adbuf1, ad_value);
	
	
	/*初次上电*/
	if(flag_ign_off2on==1)
	{
		first_ignon_process();
	}
	else
	{
		adbuf1_to_adbuf2(1);	
		disFuel_damping_argv_system(&fuel_change_argv);
		clac_disp_fuel_ad(&fuel_change_argv);
	}	
		
	fuel_volum = ad_to_volm(disp_fuel_ad);	
	/*显示油量百分比，不四舍五入*/
	fuel_percent3 = (U8)(fuel_volum/580);
//	dbg_string("fuel_percent:%d%\n",fuel_percent3);
	ignSts_bak = g_u8IgnSts;
}

void first_ignon_process()
{
	U16 ad_saved = 0;
	
	if(hal_timer_passed(tk_ign_off2on)>=150) //1.5S 初次上电要保证采样填充满buf1
	{
		read_fuel_tank_ad(&ad_saved);
		dbg_string("ad_saved:%d\n",ad_saved);

		test_data_ad_saved = ad_saved;
		
		/*如果存储数据异常 直接显示测量值*/
		if(ad_saved<AD_FUEL_FULL||ad_saved>AD_FUEL_EMPTY)
		{
			disp_fuel_ad = calc_avg_without_maxmin(&m_adbuf1);
		}
		else 
		{
			/* 没加过油或者有车速时，直接使用上次存储的油量  */
			if((has_added_fuel(ad_saved)==FUEL_STATE_NO_ADD)|| (app_get_fact_spd() > 0))
			{
				disp_fuel_ad = ad_saved;
			}
			else
			{
				disp_fuel_ad = calc_avg_without_maxmin(&m_adbuf1);
			}						
		}					
		flag_ign_off2on = 0; //只执行一次
	}
	else
	{
		//点火1s内,显示实时的测量值。防止点火后立马熄火时会保存错误值
		disp_fuel_ad = mid_adc_get(FUEL_CHN1);
	}
}

//sec秒 buf1数据放到buf2一次
static void adbuf1_to_adbuf2(U16 sec)
{
	static U32 tk = 0;
	U16 avg1;
	
	if(hal_timer_passed(tk) < sec*HZ) return;

	avg1 = calc_avg_without_maxmin(&m_adbuf1);
	push_ad_to_adbuf(&m_adbuf2, avg1);
	tk = hal_timer_get_tick();
/*	{//查看buf2中的数据
		U16 avg2;
		avg2 = calc_avg_without_maxmin(&m_adbuf2);
		dbg_string("push to buf2, sec:%d ad1_avg:%d ad2_avg:%d ad2_pos:%d\n", sec, avg1, avg2, m_adbuf2.pos);
	}
*/
}

/*显示油量的阻尼参数确定*/
void disFuel_damping_argv_system(T_FUEL_CHANGE_ARGV *data)
{
	static U32 tk_no_spd;
	static U8  flag_add_oil;

	//测量的AD值取平均
	actual_fuel_ad = calc_avg_without_maxmin(&m_adbuf2);
	
	if(app_get_fact_spd()== 0)
	{
		/*停车后IGN ON状态 每30s检测一次是否加油*/ 
		if(hal_timer_passed(tk_no_spd)>=3000)
		{
			if(has_added_fuel(disp_fuel_ad)==FUEL_STATE_ADD)
			{
				flag_add_oil = 1;
			}
			tk_no_spd = hal_timer_get_tick();
		}
		/*加完油*/
		if(Absolute(actual_fuel_ad-disp_fuel_ad)<= data->step)
		{
			flag_add_oil = 0;
		}
		/*如果加油*/
		if(flag_add_oil == 1)
		{
			data->sec = 10; 
			data->step = 6;
		}
		else if(app_get_rpm()==0) //没车速，没加油，没转速
		{
			//nothing to do
			data->sec = 2400;
			data->step = 0;
		}	
		else //有转速，没车速,没加油
		{
			/* 怠速时 只掉不涨  */
			if(disp_fuel_ad > actual_fuel_ad)
			{
				data->sec = 2400; 
				data->step= 0;
			}
			else
			{
				data->sec = 900; //9s更新一次
				data->step= 1;
			}
		}
	}
	else //有车速 不加油
	{
		flag_add_oil = 0;
		/* 行车中，油量降的快升的慢 */
		if(disp_fuel_ad > actual_fuel_ad)
		{
			data->sec = 2400; //24s更新一次
			data->step= 1;
		}
		else
		{
			if(actual_fuel_ad<1460)//超过半箱油时18s更新一次
			{
				data->sec = 1800; 
			}
			else
			{
				data->sec = 900; //9s更新一次
			}
			data->step= 1;
		}
		tk_no_spd = hal_timer_get_tick();
	}
}

/* sec*10ms增加或减少step值*/
void clac_disp_fuel_ad(T_FUEL_CHANGE_ARGV *data)
{
	static U32 tk;
//	dbg_string("sec:%d step:%d\n",data->sec, data->step);
	
	if(hal_timer_passed(tk)>= data->sec)
	{
		actual_fuel_ad = calc_avg_without_maxmin(&m_adbuf2);			
		if(actual_fuel_ad == disp_fuel_ad)
		{
			//nothing to do
		}
		else if(actual_fuel_ad - disp_fuel_ad >= data->step)
		{
			disp_fuel_ad += data->step;
		}
		else if(disp_fuel_ad - actual_fuel_ad >= data->step)
		{
			disp_fuel_ad -= data->step;
		}
		tk = hal_timer_get_tick();
	}
//	dbg_string("act_ad:%d disp_ad:%d\n",actual_fuel_ad,disp_fuel_ad);
}
// AD-> 燃油量 单位：ML
static U16 ad_to_volm(U16 ad)
{
	U16 ad_tmp;
	ad_tmp = (U16)(ad);
	
	return linear_interpolation(fuel_ad_2_volm,
		ARRAY_SIZE(fuel_ad_2_volm),SOURCE_UP_ORDER,ad_tmp);
}

U16 get_fuel_volum()
{	
	return fuel_volum;
}

U8 get_fuel_percent3()
{	
	return fuel_percent3;
}

/*与cmp_fuel_ad对比，来判断加油或漏油*/
E_FUEL_STATE has_added_fuel(U16 cmp_fuel_ad)
{
	U16 actual_fuel_ad;
	U16 actual_fuel;
	U16 cmp_fuel;
	E_FUEL_STATE result;

	actual_fuel_ad = calc_avg_without_maxmin(&m_adbuf1);
	
	actual_fuel = ad_to_volm(actual_fuel_ad);
	cmp_fuel    = ad_to_volm(cmp_fuel_ad);
//	dbg_string("actual_fuel:%d cmp_fuel:%d diff:%d\n",actual_fuel, cmp_fuel, actual_fuel-cmp_fuel);

	if(cmp_fuel>actual_fuel)
	{
		/*上次显示的大于实测的12L,认为漏油或被偷油*/
		if(cmp_fuel - actual_fuel>REFUEL_THRESHOLD)
		{
			result = FUEL_STATE_LEAK;
		}
		else
		{
			result = FUEL_STATE_NO_ADD;
		}
	}
	else
	{
		if(actual_fuel-cmp_fuel>REFUEL_THRESHOLD)
		{
			result = FUEL_STATE_ADD;
		}
		else
		{
			result = FUEL_STATE_NO_ADD;
		}	
	}
//	dbg_string("FUEL_STATE:%d\n",result);
	return result;
}

void save_fuel_ad(void)
{
	write_fuel_tank_ad(disp_fuel_ad);
	
	dbg_string("Save disp_fuel_ad:%d\n",disp_fuel_ad);
}


void set_this_trip_used_fuel(U32 val)
{
	this_trip_used_fuel = val;
}
U32 get_this_trip_used_fuel(void)
{
	return this_trip_used_fuel;
}
void set_this_trip_distance(U32 val)
{
	this_trip_distance = val;
}
U32 get_this_trip_distance(void)
{
	return this_trip_distance;
}

void this_trip_data_init()
{
	this_trip_used_fuel_bak = 0;
	read_this_trip_used_fuel(&prev_trip_used_fuel);
	read_this_trip_distance(&prev_trip_distance);
	set_this_trip_used_fuel(0);
	set_this_trip_distance(0);

	this_trip_time_off2on = 0;

	dbg_string("prev_fuel:%d prev_dist:%d\n"
		,prev_trip_used_fuel,prev_trip_distance);
	if(prev_trip_distance < 50)//上次路程小于5Km,按10L/100Km计算
	{
		prev_trip_distance = 100; //100*0.1Km
		prev_trip_used_fuel = 1000000; //1000000*uL
	}
}

void total_used_fuel_init()
{
	read_total_used_fuel(&total_used_fuel_ign_off2on);
	dbg_string("total_used_fuel_ign_off2on:%d\n",total_used_fuel_ign_off2on);
}



void this_trip_data_saved()
{
	U32 trip = get_this_trip_distance();
	U32 fuel = get_this_trip_used_fuel(); /*存储单位为uL*/
	dbg_string("++++++save trip:%d fuel:%d\n",trip, fuel);
	/*只有本次行驶大于5Km才存储数据，小于5Km使用上次的数据*/
	if(trip >= 50)
	{
		write_this_trip_used_fuel(fuel);
		write_this_trip_distance(trip);
		dbg_string("this trip data saved\n");
	}
}

void app_calc_this_trip_AFE_task()
{
	static U8 afe_ignSts_bak = OFF;
	static U8 rpm_300_cnt = 0;
	U16 the_afe;
	U32 this_afe_used_fuel_total = 0;
	U32 this_afe_distance_total = 0;
	/*本次行驶时间*/
	this_trip_time_off2on++;
	
	/*IGN ON -> OFF*/
	if(g_u8IgnSts == OFF)
	{
		if(afe_ignSts_bak == ON)
		{					
			this_trip_data_saved();	
			write_total_used_fuel(total_used_fuel);
			afe_ignSts_bak = g_u8IgnSts;			
		}
		return ;
	}
	/*IGN OFF -> ON*/
	if(g_u8IgnSts == ON)
	{
		if(afe_ignSts_bak == OFF)
		{
			this_trip_data_init();
			total_used_fuel_init();
		}
	}
	
	if(app_get_rpm() > 300)
	{
		rpm_300_cnt ++;
		rpm_300_cnt = (rpm_300_cnt>3) ? 3 : rpm_300_cnt;
	}
	else
	{
		rpm_300_cnt = 0;
	}
	/*转速大于300，3s后开始累积本次油量*/
	if(rpm_300_cnt >= 3)
	{
		this_trip_used_fuel_bak = get_this_trip_used_fuel();
	}
	else
	{
		set_this_trip_used_fuel(this_trip_used_fuel_bak);
	}
	this_afe_used_fuel_total = prev_trip_used_fuel + get_this_trip_used_fuel();
	this_afe_distance_total  = prev_trip_distance  + get_this_trip_distance();
//	dbg_string("prev_fuel:%d prev_dist:%d the_used_fuel:%d the_dist:%d\n",
//		prev_trip_used_fuel,prev_trip_distance,get_this_trip_used_fuel(),get_this_trip_distance());

	the_afe = this_afe_used_fuel_total/(this_afe_distance_total*100);//单位0.1L/100Km

	push_ad_to_adbuf(&m_afebuf, the_afe);
	this_trip_AFE = calc_avg_without_maxmin(&m_afebuf);
	/*总耗油量*/
	total_used_fuel = total_used_fuel_ign_off2on + (get_this_trip_used_fuel()+500)/1000; //显示ml
		
//	dbg_string("this_trip_AFE:%d total_used_fuel:%d\n",this_trip_AFE,total_used_fuel);


	afe_ignSts_bak = g_u8IgnSts;
	
}

U16 get_this_trip_AFE()
{
	return this_trip_AFE;
}

U32 get_total_used_fuel()
{
	return total_used_fuel;
}
void clear_total_used_fuel()
{
	total_used_fuel_ign_off2on = 0;
	set_this_trip_used_fuel(0);
}

U16 get_drive_time()
{
	return this_trip_time_off2on;
}

U8 get_test_data_U8_ad_saved()
{
	U8 ret;

	ret = (U8)(test_data_ad_saved/10);

	return ret;
}


U16 get_actual_fuel_ad()
{
	return actual_fuel_ad;
}
U16 get_disp_fuel_ad()
{
	return disp_fuel_ad;
}




