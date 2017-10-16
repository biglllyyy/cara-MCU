#include <string.h>
#include "app_spd.h"
#include "spd_cfg.h"
#include "comm_typedef.h"
#include "app_rpm.h"
#include "hal_reload_timer.h"
#include "app_trip.h"
#include "app_can.h"

#include "app_uart_cmd.h"
#include "app_moudle.h"

#define  MAX_SHIFT_ENLARGE  16
/**********************************************************************************/
U8			g_u8Spd = 0;				/* this spd used as other module  spd */
U8		g_u8DispSpd=0;				/**/
U16  s_g_u16_speed_src;				/* this spd used in this module */
U8		    g_u8_avg_speed = 0;
avg_spd_info_t avg_spg_info;
/**********************************************************************************/
//functon declare
static void app_cal_avg_speed(void);
/**********************************************************************************/
extern uint8_t g_u8IgnSts;
void app_init_spd(void)
{
	//ָʾ����
	s_g_u16_speed_src = 0;
	g_u8Spd = 0;
	
	//ƽ������
	memset(&avg_spg_info,0,sizeof(avg_spg_info));
	g_u8_avg_speed=0;
}

void  app_set_spd(U16 spd)
{
	s_g_u16_speed_src = spd;
}
static void app_cal_fact_speed(void)
{/*
	U16 g_u16Spd = 0;
	if(g_u8IgnSts == ON)
	{
		����CAN���ݺ�ϵ��0.05625����ʵ�ʳ��٣�����������
		//g_u8Spd = ((U32)s_g_u16_speed_src*90+800)/1600;
		g_u16Spd = ((U32)s_g_u16_speed_src*90+800)/1600;
		if(g_u16Spd > 0xff)
			g_u8Spd = 0xff;
		else
			g_u8Spd = g_u16Spd;
		if(can_id_265_lost_timecnt>CAN_LOST_TIME)CAN��ʧʱ�䳬2.5s,����Ϊ0
		{
			g_u8Spd=0;
		}
	}
	else
	{
		s_g_u16_speed_src=0;
		g_u8Spd=0;
	}
*/}
static void app_cal_disp_speed(void)
{
	static  struct
	{
		U16 spd_total;
		U16 times_cnt;
	}disp_spd={0,0};
	
	U32 temp32;
	
	/***********************************/
	//������ʾ����,ԭ��ʽ:y=-0.0001X*X+1.0832x-0.0901
	//�������
	/*
	temp32=(U32)g_u8Spd*10832;
	temp32-=(U32)g_u8Spd*g_u8Spd*1;
	*/
	//Ϊ���������������������ϲ�Ϊһ��
	//temp32-=901; temp32+=10000/2;
	/*
	temp32+=4099;
	temp32/=10000;
	*/
	/*����ԭ��ʽ: y=0.00005X*X+1.0202x+0.1581 */
	temp32=(U32)g_u8Spd*102020;
	temp32 += (U32)g_u8Spd*g_u8Spd*5;
	temp32 += 15810;
	temp32 = temp32/100000;
	
	if(temp32 >= SPD_DIS_MAX)
	{
		temp32 = SPD_DIS_MAX;
	}
	/**********************************/
	
	/**********************************/
	//��ʾ����5��ȡƽ��
	disp_spd.spd_total+=(U8)temp32;//�ۼӳ���
	disp_spd.times_cnt++;//�ۼӴ���
	if(disp_spd.times_cnt>=5)//�ۼӵ�10��֮��
	{
		g_u8DispSpd=disp_spd.spd_total/disp_spd.times_cnt;//ȡƽ��ֵ
		
		disp_spd.spd_total=0;//��ʼ���ۼӳ���
		disp_spd.times_cnt=0;//��ʼ���ۼӴ���
	}
	/***********************************/
}

U8  app_get_fact_spd(void)
{
	return(pSpeed);
}

U8  app_get_dis_spd(void)
{
	return(g_u8DispSpd);
}

void app_process_spd_task(void)
{
	//����ʵ�ʳ���
	app_cal_fact_speed();
	//������ʾ����
	app_cal_disp_speed();
	//����ƽ������
	app_cal_avg_speed();
}

U8  app_get_avg_spd(void)
{
	return (g_u8_avg_speed);
}


static void app_cal_avg_speed(void)
{
	U8 spd=uart_general_data.generalInfo.speed;
	
	if(g_u8IgnSts == ON)//���
	{
		if(spd)//�г��ٵ������
		{
			if(avg_spg_info.spd_total<=(0xffffffff-240))//û�г��������ۼƳ���
			{
				avg_spg_info.cnt_total++;
				avg_spg_info.spd_total+=spd;
				
				g_u8_avg_speed=avg_spg_info.spd_total/avg_spg_info.cnt_total;
			}
			else
			{
				g_u8_avg_speed=g_u8_avg_speed;
			} 
		}  
	}
	else
	{
		memset(&avg_spg_info,0,sizeof(avg_spg_info));
		g_u8_avg_speed=0;
	}
}

void app_clear_avg_spd(void)
{
	memset(&avg_spg_info,0,sizeof(avg_spg_info));
	g_u8_avg_speed=0;
}
