#include <string.h>
#include "app_IPconfig.h"
#include "app_uart_cmd.h"
#include "hal_wdg.h"
#include "app_can.h"
#include "app_trip.h"
#include "app_uart_cmd.h"
#include "app_data_store.h"
#include "g_variable.h"
#include "fw_version.h"
#include "mid_flash.h"
#include "app_service.h"
#include "app_system.h"
#include "app_car_type.h" 

/************************globle variable***********************/
IPconfig_T IPconfig;
/**************************************************************/

static U8 IsIPconfigValid(void)
{
	U8 activated;
	
	if (read_activation_flag(&activated) != TRUE) return 0;
	
	if(activated == 1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
static void SetIPconfigValid(void)
{
	U8 activated = 1;
	
	write_activation_flag(activated);
}
static void ClearIPconfigValid(void)
{
	U8 activated = 0;
	write_activation_flag(activated);
	write_total_trip(0);
}

/* �ϵ�֮��ִ��һ�� */
static void app_get_ee_car_num(void)
{
	U8 car_num;

	if(IPconfig.Sts)
	{
		uart_special_data.specialInfo.LedWarning.third.bit.IPconfig=2;
		read_car_num((U8*)&car_num);
		IPconfig.CarNum = car_num;	/* ���ʹ��� */
		IPconfig.protocol = (HAVAL_CAR_TYPE_T)app_get_protocol(car_num);	/* Э���� */
		dbg_string("Activated, car_type=%d, protocol:%d\n", IPconfig.CarNum, IPconfig.protocol);
	}
	else
	{
		uart_special_data.specialInfo.LedWarning.third.bit.IPconfig=1;
		dbg_string("NOT activated, car_num=%d, protocol=%d\n", IPconfig.CarNum, IPconfig.protocol);
	}
}

void app_IPconfig_init(void)
{
	memset(&IPconfig,0,sizeof(IPconfig));

	/*** ǿ��д�����־������������ ***/
	//write_activation_flag(1);
	/*** ǿ�ưѳ��ͱ��д�룬���������� ***/
	//write_car_num(17);

	/*ǿ��д�뱣����̺ͱ�������*/
	//write_service_mile(2000);
	//write_service_num(1);
	//write_total_trip(41308);
	//write_sub_trip1(9950);
	
	rel_printf("Todi %d.%d.%d, Build date:%s,%s\n", MCU_VERSION_H, MCU_VERSION_M, MCU_VERSION_L, 
	__DATE__, __TIME__);
	/*�������־λ*/
	IPconfig.Sts=IsIPconfigValid();
	/*������*/
	app_get_ee_car_num();
	/* ������ID */
	read_theme_id(&IPconfig.themeID);
}

/* �ϵ�֮��1sִ��һ�� */
void app_IPconfig_pro_1000ms(void)
{
	static U8 step=0;
	static U8 timecnt=0;
	static U8 carNum = 0;		/*���ͣ�0ΪĬ��ֵ*/
	static U8 srvNum = 0;		/*��������,0ΪĬ��ֵ*/
	static U16 srvMil= 0;		/*������̣�0ΪĬ��ֵ*/
	U8 themeID;
	
	switch(step)
	{
		case 0:/*�ж��Ƿ񼤻�*/
			if(IPconfig.Sts==1)//�Ǳ��Ѿ�����
			{
				/* Ԥ�� ���� IPCONFIGVALID�Ĺ��ܣ����ţ�
				   ʵ�ַ�������IPCONFIGVALIDΪ1������һ֡���Ϊ0xffffff�����ݣ���������
				*/
				if(IPconfig.Odo==0x00ffffff)													
				{
					ClearIPconfigValid();
					
					app_soft_reset();	/* ��λ */
				}
				
				/*
					�Ƿ�ͨ�������޸��˳��ͣ�
				*/
				carNum = app_uart_get_car_num();
				if ((IPconfig.CarNum != carNum) && (carNum != 0xff))
				{
					//dbg_string("New CarNum=%d, old CarNum:%d\n", carNum, IPconfig.CarNum);
					write_car_num(carNum);
					step = 3;
				}
				
				/* ͨ�������޸������� */
				themeID = app_uart_get_theme_id();
				if ((IPconfig.themeID != themeID) && (themeID != 0xF))
				{
					//dbg_string("New themeID=%d, old themeID:%d\n", themeID, IPconfig.themeID);
					write_theme_id(themeID);
					IPconfig.themeID= themeID;
				}
			}
			else/*�Ǳ�δ����,�������ý���*/
			{
				if(uart_special_data.specialInfo.LedWarning.third.bit.IPconfig==1)
				{
					step=1;
				}
			}
			break;
		case 1:/*����ԭ���Ӧ�ĳ��͡�������̡���������*/
				
			carNum = app_uart_get_car_num();
			srvNum = app_uart_get_car_srv_num();
			srvMil = app_uart_get_car_srv_mil();	
			/*
			carNum = 5;
			srvNum = 1;
			srvMil = 2500;	
			IPconfig.Odo = 1;		
			*/
			
			/*���߶�������*/
			if(0!=srvNum && 0!= srvMil)
			{
				step = 2;		
				IPconfig.CarNum = carNum; /*���³��ͣ�ȥ�������*/
				IPconfig.protocol = (HAVAL_CAR_TYPE_T)app_get_protocol(carNum);
				dbg_string("carNum:%d srvNum:%d srvMil:%d \n",carNum,srvNum,srvMil);
			}		
			break;
		case 2:/*�������ݵ�flash*/
			/* �������ԭ�����Ҳ��ܷ�������� */
			if((0!=IPconfig.Odo)&&(IPconfig.Odo!=0x00ffffff))
			{
				if(IPconfig.Odo >= MAX_SUM_ODO)
				{
					IPconfig.Odo=MAX_SUM_ODO;
				}
				/*���������������*/
				switch(srvNum)
				{
					case FIRST_SERVICE_NUM: 
						if(3000 <= srvMil)
							srvMil = 3000;
					break;

					case SECOND_SERVICE_NUM:
						if(6000 <= srvMil)
							srvMil = 6000;
					break;

					case THIRD_SERVICE_NUM:
						if(5000 <= srvMil)
							srvMil = 5000;
					break;
					default :
					break;
				}
				write_car_num(carNum); 			/* ���泵�ͱ�� */
				write_service_num(srvNum);		/* ���汣������ */
				write_service_mile(srvMil+IPconfig.Odo); /* ���汣�����  */
				write_total_trip(IPconfig.Odo); /* ��������� */
				SetIPconfigValid(); 			/*�Ǳ����־λ��λ */	
				/*��������Ҫ��flash*/
				app_sub_trip1_clear();
				app_sub_trip2_clear();
				write_afe_fuel(0);
				write_afe_distance(0);
				write_none_display_info(0);
				write_display_mode(0);
				write_drive_time(0, 0);
			
				mid_flash_sync();				/* ͬ��Flash, ��ΪҪ�ϵ��� */
				uart_special_data.specialInfo.LedWarning.third.bit.IPconfig=2; /* ���ߺ��İ壬������� */
				dbg_string("Actived!!! reset now \n");
				step = 3;
			} 
			/*���û�����������Ϣ����ʱ5S������ */
			if(timecnt>=5) 
			{	
				step = 3;
				dbg_string("Odo invalid!!! reset now \n");
			}
			timecnt++;
			
			break;	
		case 3:/*����*/	
			app_soft_reset();//��λ
			break;
		default:
			app_soft_reset();//��λ
			break;
	}
}
