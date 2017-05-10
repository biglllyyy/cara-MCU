#include <string.h>
#include"can_cfg.h"
#include "app_can.h"
#include"mid_buzzer.h"
#include"app_uart_cmd.h"
#include "app_service.h"
#include "g_variable.h"
#include "mid_ds1302.h"
#include "app_buzzer.h"
#include "mid_filter.h"
#include "app_trip.h"
#include "hal_wdg.h"
#include "mid_string.h"
#include "app_IPconfig.h"

U32 service_mile;
CarService_T CarService;
U8 CarServiceFlag=0;
static void app_get_ee_service(void);

void app_service_init(void)
{
	memset(&CarService,0,sizeof(CarService));
	app_get_ee_service();

	dbg_string("srvMil:%d srvNum: %d\n",CarService.service_mile,CarService.service_num);
}

static void app_get_ee_service(void)
{
	wdg_feed();
		
	if(read_service_mile(&service_mile) == TRUE)
	{
		CarService.service_mile = service_mile;	
		CarService.service_num = read_service_num();	 
	}
	else
	{
		switch(IPconfig.protocol)
		{
			case HAVAL_2013_2P4L_AT_2WD:
			case HAVAL_2013_1P5T_MT_2WD:
			case HAVAL_2012_1P5T_MT_2WD:
			case HAVAL_2015_1P5T_MT_2WD: 
			case HAVAL_2015_1P5T_6AT_2WD:
			case HAVAL_2011_2P0T_MT_2WD:
			case HAVAL_2013_1P5T_MT_4WD:
				write_service_mile(3000); /* 写入首保里程 */
				break;
		}
		write_service_num(FIRST_SERVICE_NUM);
	} 
}

//调度保养里程任务
void app_get_rest_service_task_1000ms(void)
{
	static uint8_t service_flag_bak=0;
	
	//get data from pc to mcu
	CarService.service_flag=CarServiceFlag;
	
	CarService.current_mile=info.Odo;
	
	if(CarService.service_mile>=CarService.current_mile)
	{
		CarService.rest_service_mile=CarService.service_mile-CarService.current_mile;
	}
	else
	{
		CarService.rest_service_mile=0;
	}
	if((CarService.service_flag==1)&&(service_flag_bak==0))//0-1的跳变，认为是一次有效的汽车保养信号
	{
		switch(CarService.service_num)
		{
			case 1://首保结束，开始二保
				switch(IPconfig.protocol)
				{
					case HAVAL_2013_2P4L_AT_2WD:
					case HAVAL_2013_1P5T_MT_2WD:
					case HAVAL_2012_1P5T_MT_2WD:
					case HAVAL_2015_1P5T_MT_2WD: 
					case HAVAL_2015_1P5T_6AT_2WD:
					case HAVAL_2011_2P0T_MT_2WD:
					case HAVAL_2013_1P5T_MT_4WD:
						CarService.service_mile=CarService.current_mile+6000;
						break;
				}
				CarService.service_num = SECOND_SERVICE_NUM;
				break;
			default:
				switch(IPconfig.protocol)
				{
					case HAVAL_2013_2P4L_AT_2WD:
						CarService.service_mile=CarService.current_mile+5000;
						break;
					case HAVAL_2013_1P5T_MT_2WD:
						CarService.service_mile=CarService.current_mile+5000;
						break;
					case HAVAL_2012_1P5T_MT_2WD:
						CarService.service_mile=CarService.current_mile+5000;
						break;
					case HAVAL_2015_1P5T_MT_2WD: 
					case HAVAL_2015_1P5T_6AT_2WD:
						CarService.service_mile=CarService.current_mile+5000;
						break;
					case HAVAL_2011_2P0T_MT_2WD:
						CarService.service_mile=CarService.current_mile+5000;
						break;
					case HAVAL_2013_1P5T_MT_4WD:
						CarService.service_mile=CarService.current_mile+5000;
				}
				if(CarService.service_num<0xff)
				{
					CarService.service_num++;
				}
				break;
		}
		write_service_mile(CarService.service_mile);
		write_service_num(CarService.service_num);
		
		service_flag_bak=CarService.service_flag;
	}
}

void app_service_clean_service()
{
	CarServiceFlag = 1;
}

