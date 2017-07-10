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

/* 上电之后，执行一次 */
static void app_get_ee_car_num(void)
{
	U8 car_num;

	if(IPconfig.Sts)
	{
		uart_special_data.specialInfo.LedWarning.third.bit.IPconfig=2;
		read_car_num((U8*)&car_num);
		IPconfig.CarNum = car_num;	/* 车型代码 */
		IPconfig.protocol = (HAVAL_CAR_TYPE_T)app_get_protocol(car_num);	/* 协议编号 */
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

	/*** 强制写激活标志，仅做调试用 ***/
	//write_activation_flag(1);
	/*** 强制把车型编号写入，仅做调试用 ***/
	//write_car_num(17);

	/*强制写入保养里程和保养次数*/
	//write_service_mile(2000);
	//write_service_num(1);
	//write_total_trip(41308);
	//write_sub_trip1(9950);
	
	rel_printf("Todi %d.%d.%d, Build date:%s,%s\n", MCU_VERSION_H, MCU_VERSION_M, MCU_VERSION_L, 
	__DATE__, __TIME__);
	/*读激活标志位*/
	IPconfig.Sts=IsIPconfigValid();
	/*读车型*/
	app_get_ee_car_num();
	/* 读主题ID */
	read_theme_id(&IPconfig.themeID);
}

/* 上电之后1s执行一次 */
void app_IPconfig_pro_1000ms(void)
{
	static U8 step=0;
	static U8 timecnt=0;
	static U8 carNum = 0;		/*车型，0为默认值*/
	static U8 srvNum = 0;		/*保养次数,0为默认值*/
	static U16 srvMil= 0;		/*保养里程，0为默认值*/
	U8 themeID;
	
	switch(step)
	{
		case 0:/*判断是否激活*/
			if(IPconfig.Sts==1)//仪表已经激活
			{
				/* 预留 清零 IPCONFIGVALID的功能（后门）
				   实现方法：当IPCONFIGVALID为1，发送一帧里程为0xffffff的数据，即可清零
				*/
				if(IPconfig.Odo==0x00ffffff)													
				{
					ClearIPconfigValid();
					
					app_soft_reset();	/* 复位 */
				}
				
				/*
					是否通过界面修改了车型？
				*/
				carNum = app_uart_get_car_num();
				if ((IPconfig.CarNum != carNum) && (carNum != 0xff))
				{
					//dbg_string("New CarNum=%d, old CarNum:%d\n", carNum, IPconfig.CarNum);
					write_car_num(carNum);
					step = 3;
				}
				
				/* 通过界面修改了主题 */
				themeID = app_uart_get_theme_id();
				if ((IPconfig.themeID != themeID) && (themeID != 0xF))
				{
					//dbg_string("New themeID=%d, old themeID:%d\n", themeID, IPconfig.themeID);
					write_theme_id(themeID);
					IPconfig.themeID= themeID;
				}
			}
			else/*仪表未激活,进入设置界面*/
			{
				if(uart_special_data.specialInfo.LedWarning.third.bit.IPconfig==1)
				{
					step=1;
				}
			}
			break;
		case 1:/*设置原表对应的车型、保养里程、保养次数*/
				
			carNum = app_uart_get_car_num();
			srvNum = app_uart_get_car_srv_num();
			srvMil = app_uart_get_car_srv_mil();	
			/*
			carNum = 5;
			srvNum = 1;
			srvMil = 2500;	
			IPconfig.Odo = 1;		
			*/
			
			/*三者都设置完*/
			if(0!=srvNum && 0!= srvMil)
			{
				step = 2;		
				IPconfig.CarNum = carNum; /*更新车型，去读总里程*/
				IPconfig.protocol = (HAVAL_CAR_TYPE_T)app_get_protocol(carNum);
				dbg_string("carNum:%d srvNum:%d srvMil:%d \n",carNum,srvNum,srvMil);
			}		
			break;
		case 2:/*保存数据到flash*/
			/* 必须插上原表，并且不能发后门里程 */
			if((0!=IPconfig.Odo)&&(IPconfig.Odo!=0x00ffffff))
			{
				if(IPconfig.Odo >= MAX_SUM_ODO)
				{
					IPconfig.Odo=MAX_SUM_ODO;
				}
				/*保养里程做个限制*/
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
				write_car_num(carNum); 			/* 保存车型编号 */
				write_service_num(srvNum);		/* 保存保养次数 */
				write_service_mile(srvMil+IPconfig.Odo); /* 保存保养里程  */
				write_total_trip(IPconfig.Odo); /* 保存总里程 */
				SetIPconfigValid(); 			/*仪表激活标志位置位 */	
				/*初次设置要清flash*/
				app_sub_trip1_clear();
				app_sub_trip2_clear();
				write_afe_fuel(0);
				write_afe_distance(0);
				write_none_display_info(0);
				write_display_mode(0);
				write_drive_time(0, 0);
			
				mid_flash_sync();				/* 同步Flash, 因为要断电了 */
				uart_special_data.specialInfo.LedWarning.third.bit.IPconfig=2; /* 告诉核心板，激活完成 */
				dbg_string("Actived!!! reset now \n");
				step = 3;
			} 
			/*如果没读到总里程信息，延时5S，重启 */
			if(timecnt>=5) 
			{	
				step = 3;
				dbg_string("Odo invalid!!! reset now \n");
			}
			timecnt++;
			
			break;	
		case 3:/*重启*/	
			app_soft_reset();//复位
			break;
		default:
			app_soft_reset();//复位
			break;
	}
}
