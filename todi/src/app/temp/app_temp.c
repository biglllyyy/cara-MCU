#include <stdlib.h>
#include "app_temp.h"
#include "temp_cfg.h"
#include "string.h"
#include "app_can.h"
#include "app_IPconfig.h"
#include "app_fuel.h"
#include "G_variable.h"
#include "Mid_dbg.h"
#define 	TEMP_BUFSIZE  	10
static uint8_t temp_src=0;
static uint8_t g_temp = 0;
static U8 temp_buf[TEMP_BUFSIZE] ={0};
static U8 temp_count =0;
static U16 total_temp =0;
void app_init_temp(void)
{
	temp_src=0;
	g_temp=0;
}
/*
1��������ʾ���ֵ��ʱ��С��2S������ԭֵ������2S���������ֵ
2��С����Сֵ��ά��ʱ��С��2S������ԭֵ������2S����ʾ��Сֵ��
3�����յ���Чֵ��2S ��ָ����ʾ��Сֵ,2S��Ϊԭֵ��
4��CAN�źŶ�ʧ��2S��ָʾ����ʾ��Сֵ��
*/
#if 0
void guage_process(gauge_t *gauge,U16 period)
{
	if(gauge->signal_lost_flg)
	{
		gauge->signal_lost_cnt++;
		if(gauge->signal_lost_cnt >= TimeMs(2000,period))
		{
			gauge->signal_lost_cnt = TimeMs(2000,period);
			gauge->gauge_value = TEMP_DIS_MIN;
		}
		gauge->invalid_time_cnt = 0;
		gauge->less_min_time_cnt = 0;
		gauge->over_max_time_cnt = 0;
	}
	else
	{
		if(gauge->can_value == gauge->invalid_can_value)
		{
			gauge->invalid_time_cnt++;
			if(gauge->invalid_time_cnt >= TimeMs(2000,period))
			{
				gauge->invalid_time_cnt = TimeMs(2000,period);
				gauge->gauge_value = TEMP_DIS_MIN;
			}
			gauge->less_min_time_cnt = 0;
			gauge->over_max_time_cnt = 0;
			gauge->signal_lost_cnt = 0;
		}
		else if(gauge->can_value > gauge->max_dis_can_value)
		{
			gauge->over_max_time_cnt++;
			if(gauge->over_max_time_cnt > TimeMs(2000,period))
			{
				gauge->over_max_time_cnt = TimeMs(2000,period);
				gauge->gauge_value = TEMP_DIS_MAX;
			}
			gauge->less_min_time_cnt = 0;
			gauge->signal_lost_cnt = 0;
			gauge->invalid_time_cnt= 0;
		}
		else if(gauge->can_value < gauge->min_dis_can_value)
		{
			gauge->less_min_time_cnt++;
			if(gauge->less_min_time_cnt >= TimeMs(2000,period))
			{
				gauge->less_min_time_cnt = TimeMs(2000,period);
				gauge->gauge_value = TEMP_DIS_MIN;
			}
			gauge->over_max_time_cnt = 0;
			gauge->signal_lost_cnt = 0;
			gauge->invalid_time_cnt= 0;
		}
		else
		{
			gauge->gauge_value = (U8)(gauge->can_value * gauge->scale) + gauge->offset;
			gauge->less_min_time_cnt = 0;
			gauge->over_max_time_cnt = 0;
			gauge->signal_lost_cnt = 0;
			gauge->invalid_time_cnt= 0;
		}
	}
}
#endif

void app_set_temp(U8 temp)
{
	temp_src=temp;
}

#if 0
U8 app_get_temp_percent(void)
{
	U8 temp = 0;

	temp = (temp_gauge.gauge_value - TEMP_DIS_MIN) *100/(TEMP_DIS_MAX - TEMP_DIS_MIN);
	
	return temp; 
}
#endif
U8  app_get_temp_value(void)
{
	return (U8)g_temp;
}

U8   ap_cal_aver_temp(U8 temp)
{
	U8 result;
	U8 i;

	if(OFF == get_g_u8IgnSts())
	{
		temp_count = 0;
		total_temp = 0;
		//set_g_u8IgnSts_bak(OFF);
		return 0;
	}
	result = temp - 48;
	temp_buf[(temp_count++)%TEMP_BUFSIZE]=result;
	//dbg_string("tempbuf:%d\n",result);
	total_temp = 0;
	for(i=0;i<TEMP_BUFSIZE;i++)/*���ȡƽ��ֵ*/
	{
		if((ON == get_g_u8IgnSts())&&(OFF==get_g_u8IgnSts_bak()))
		{
			temp_buf[i] = result;
		}
		total_temp = total_temp + temp_buf[i];
	}
	result = total_temp/TEMP_BUFSIZE;
	
	if((result>=80)&&(result<=105))/*��ʵ��ˮ��80-105֮�䣬ˮ��ָʾ��85��*/
	{
		result = 85;
		return result;
	}
	//
	if(result>105)
	{		
		return result*451/200-150;/*��������Ǵ�����ϵ�*/
	}
	return result;
}
void app_pro_temp_task(void)
{
#if 0
	U8 temp;
	temp=temp_src;
	switch(IPconfig.protocol)
	{
		case HAVAL_2013_2P4L_AT_2WD:
			/*����ʵ��ˮ��*/
			temp=(U16)temp*3/4;
			if(temp<TEMP_DIS_MIN+48)
			{
				temp=TEMP_DIS_MIN+48;
			}
			else if(temp>TEMP_DIS_MAX+48)
			{
				temp=TEMP_DIS_MAX+48;
			}
			else
			{
				
			}
			
			/*2��CANˮ���źŶ�ʧ��ˮ����ʾ���ֵ*/
			if(can_id_271_lost_timecnt>CAN_LOST_TIME)/*CAN��ʧ2.5s,ˮ��ָʾ��ߣ�������*/
			{
				temp=TEMP_DIS_MAX+48;
			}
			//////////////////////////
			temp = ap_cal_aver_temp(temp);
			g_temp=temp;
			break;
		case HAVAL_2013_1P5T_MT_2WD:
			/*����ʵ��ˮ��*/
			temp=(U16)temp*3/4;
			if(temp<TEMP_DIS_MIN+48)
			{
				temp=TEMP_DIS_MIN+48;
			}
			else if(temp>TEMP_DIS_MAX+48)
			{
				temp=TEMP_DIS_MAX+48;
			}
			else
			{
				
			}
			
			/*2��CANˮ���źŶ�ʧ��ˮ����ʾ���ֵ*/
			if(can_id_371_lost_timecnt>CAN_LOST_TIME)/*CAN��ʧ2.5s,ˮ��ָʾ��ߣ�������*/
			{
				temp=TEMP_DIS_MAX+48;
			}
			//////////////////////////
			temp = ap_cal_aver_temp(temp);
			g_temp=temp;
			break;
		case HAVAL_2012_1P5T_MT_2WD:
			/*����ʵ��ˮ��*/
			temp=(U16)temp*3/4;
			if(temp<TEMP_DIS_MIN+48)
			{
				temp=TEMP_DIS_MIN+48;
			}
			else if(temp>TEMP_DIS_MAX+48)
			{
				temp=TEMP_DIS_MAX+48;
			}
			else
			{
				
			}

			/*2��CANˮ���źŶ�ʧ��ˮ����ʾ���ֵ*/
			if(can_id_371_lost_timecnt>CAN_LOST_TIME)/*CAN��ʧ2.5s,ˮ��ָʾ��ߣ�������*/
			{
				temp=TEMP_DIS_MAX+48;
			}
			//////////////////////////
			temp = ap_cal_aver_temp(temp);
			g_temp=temp;
			break;
		case HAVAL_2015_1P5T_MT_2WD: 
		case HAVAL_2015_1P5T_6AT_2WD:
			/*����ʵ��ˮ��*/
			temp=(U16)temp*3/4;
			if(temp<TEMP_DIS_MIN+48)
			{
				temp=TEMP_DIS_MIN+48;
			}
			else if(temp>TEMP_DIS_MAX+48)
			{
				temp=TEMP_DIS_MAX+48;
			}
			else
			{
				
			}
			
			/*2��CANˮ���źŶ�ʧ��ˮ����ʾ���ֵ*/
			if(can_id_371_lost_timecnt>CAN_LOST_TIME)/*CAN��ʧ2.5s,ˮ��ָʾ��ߣ�������*/
			{
				temp=TEMP_DIS_MAX+48;
			}
			//////////////////////////
			temp = ap_cal_aver_temp(temp);
			g_temp=temp;
			break;
		case HAVAL_2011_2P0T_MT_2WD:
			/*����ʵ��ˮ��*/
			temp=(U16)temp*3/4;
			if(temp<TEMP_DIS_MIN+48)
			{
				temp=TEMP_DIS_MIN+48;
			}
			else if(temp>TEMP_DIS_MAX+48)
			{
				temp=TEMP_DIS_MAX+48;
			}
			else
			{
				
			}

			/*2��CANˮ���źŶ�ʧ��ˮ����ʾ���ֵ*/
			if(can_id_371_lost_timecnt>CAN_LOST_TIME)/*CAN��ʧ2.5s,ˮ��ָʾ��ߣ�������*/
			{
				temp=TEMP_DIS_MAX+48;
			}
			//////////////////////////
			temp = ap_cal_aver_temp(temp);
			g_temp=temp;
			break;
		case HAVAL_2013_1P5T_MT_4WD:
			/*����ʵ��ˮ��*/
			temp=(U16)temp*3/4;
			if(temp<TEMP_DIS_MIN+48)
			{
				temp=TEMP_DIS_MIN+48;
			}
			else if(temp>TEMP_DIS_MAX+48)
			{
				temp=TEMP_DIS_MAX+48;
			}
			else
			{
				
			}
			
			/*2��CANˮ���źŶ�ʧ��ˮ����ʾ���ֵ*/
			if(can_id_371_lost_timecnt>CAN_LOST_TIME)/*CAN��ʧ2.5s,ˮ��ָʾ��ߣ�������*/
			{
				temp=TEMP_DIS_MAX+48;
			}
			//////////////////////////
			temp = ap_cal_aver_temp(temp);
			g_temp=temp;
			break;
	}
	#endif
}
