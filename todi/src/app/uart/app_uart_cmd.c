#include <string.h>
#include"app_uart_cmd.h"

#include"can_cfg.h"
#include"app_rpm.h"
#include"app_spd.h"
#include"app_temp.h"

#include"app_fuel_method2.h"
#include "app_fuel.h"
#include "app_can.h"
#include "app_trip.h"
#include "uart_queue.h"
#include "app_power.h"
#include"g_variable.h"
#include"io_cfg.h"
#include"hal_uart.h"
#include"crc16.h"
#include "mid_scheduler.h"
#include "app_trip.h"
#include "app_lin.h"

#include "mid_can.h"
#include "mid_string.h"
#include "mid_buzzer.h"
#include "mid_time.h"
#include "uart_cfg.h"
#include "mid_switch.h"
#include "app_service.h"
#include "app_key.h"
#include "app_drivetime.h"
#include "app_UserSetting.h"
#include "app_CarKey.h"
#include "app_IPconfig.h"
#include "fw_version.h"
#include "app_buzzer.h"
#include "mid_pin.h"
#include "io_cfg.h"
#include "mid_dbg.h"
#include "app_io.h"
#include "app_backlight.h"
#include "Comm_typedef.h"
#include "app_buzzer.h"
#include "CarTemp.h"
#include "app_hw_version.h"
#include "fuel_cfg.h"
#include "mid_adc.h"

#include "app_display.h"

GeneralFrame	uart_general_data;
SpecialFrame	uart_special_data;

SettingsGeneralFrame mcu_rec_general;
SettingsSpecialFrame mcu_rec_special;

U8		rec_queue_buf[SETTINGS_FRAME_LEN_MAX]= {0};	//uesd to process core to mcu data buf
UartQueue mcu_frame_rec = {0};

/* 记录上一次收到UART数据时的tick */
static U32 last_uart_tick;

/*正常模式 工程模式 演示模式*/
static E_SCREEN_MODE screen_mode;

/* globle varialbe */
static void app_uart_rec_process(UartQueue *p);

/* 该函数就上电的时候初始化一次。*/
void app_uart_frame_init(void)
{
	hal_uart_init();//初始化队列
	uart_queue_init(&mcu_frame_rec,rec_queue_buf,sizeof(rec_queue_buf)/sizeof(rec_queue_buf[0]));
	uart_clear_queue(&mcu_frame_rec);
	memset(&uart_general_data,0,sizeof(uart_general_data));
	memset(&uart_special_data,0,sizeof(uart_special_data));
	memset(&mcu_rec_general,0,sizeof(mcu_rec_general));
	memset(&mcu_rec_special,0,sizeof(mcu_rec_special));
	
	/* 有些数据默认不是发送0的，需要特殊设置 */
	uart_special_data.specialInfo.MainDisp.first.bit.DispMode=3;
	uart_special_data.specialInfo.MainDisp.first.bit.NotDisp=3;
	mcu_rec_special.settingsInfo.CarNumInfo.val = 0xff;	/* 没有读到Flash中存储的车型代码前，不能发送0，因为0是第一种车型 */
	mcu_rec_general.settingsInfo.fourth.bits.themeID = 0xF; /* 没有读到Flash中存储的主题ID前，不能发送0，因为0是第一种主题 */
}


/*
 * 函数功能: 普通模式通用帧框架
 */
static void app_mcu_core_general(void)
{
	U16 temp16 = 0;
	U32 temp32 = 0;
		
	uart_general_data.head=FRAME_HEAD;
	uart_general_data.length = GENERAL_FRAME_LEN - 9; 
	temp32 = (U32)MAGIC_GENERAL_UP;
	byte_order_change((U8*)&temp32,4);
	uart_general_data.magic= temp32;
	
	/*软件版本*/
	temp16=((U16)MCU_VERSION_H<<13)+((U16)MCU_VERSION_M<<8)+((U16)MCU_VERSION_L);
	byte_order_change((U8*)&temp16,2);
	uart_general_data.generalInfo.hwVer=temp16>>13;
	uart_general_data.generalInfo.releaseVer=temp16>>8;
	uart_general_data.generalInfo.betaVer=temp16;
	
	/*转速*/
	temp16 = app_get_rpm();
	byte_order_change((U8*)&temp16,2);
	uart_general_data.generalInfo.rpm = temp16;
	/*RTC*/
	temp32 = g_u32_utcTime;
	byte_order_change((U8*)&temp32,4);
	uart_general_data.generalInfo.time =temp32;
	/*SPEED*/
	uart_general_data.generalInfo.speed=app_get_dis_spd();
    /*水温*/
	uart_general_data.generalInfo.temprature=app_get_temp_value();
	//dbg_string("waterTemp:%d\n",uart_general_data.generalInfo.temprature);/*打印显示的水温数据*/
	/*KEY*///放在key.c赋值
	//uart_general_data.generalInfo.key2 = key[2].status_out_to_core;
	//uart_general_data.generalInfo.key1 = key[1].status_out_to_core;
	//uart_general_data.generalInfo.key0 = key[0].status_out_to_core;
	//uart_general_data.generalInfo.key3 = default_value;
	//uart_general_data.generalInfo.key4 = default_value;
	//uart_general_data.generalInfo.key5 = default_value;
	//uart_general_data.generalInfo.key6 = default_value;
	//IGN STS
	uart_general_data.generalInfo.ign_sts = get_g_u8IgnSts();
	/*档位*/
	 //此处信息放在CAN/LIN
	/*钥匙状态*/
	uart_general_data.generalInfo.key_status = app_get_car_key_sts();
	/*门状态*/
	 //此处信息放在CAN/LIN
	
	/* total trip */
	temp32 = app_get_total_trip();
	byte_order_change((U8*)&temp32,4);
	temp32 >>= 8;
	uart_general_data.generalInfo.odo = temp32;
	
	/*平均车速*/
	uart_general_data.generalInfo.averageSpeed =app_get_avg_spd();
	/*续驶里程*/
	temp16 = app_rest_mile_get();
	//temp16=0xffff;
	byte_order_change((U8*)&temp16,2);
	uart_general_data.generalInfo.remainMileage= temp16;
	
	/*trip1*/
	temp32 = app_get_sub_trip1();
	byte_order_change((U8*)&temp32,4);
	uart_general_data.generalInfo.trip1= (temp32>>8);
	
	/* fuel */
	uart_general_data.generalInfo.fuel =get_fuel_percent3();

	/*trip2*/
	temp32 = app_get_sub_trip2();
	byte_order_change((U8*)&temp32,4);
	uart_general_data.generalInfo.trip2= (temp32>>8);
	/*保养里程*/
	temp16 = (U16)CarService.rest_service_mile;
	byte_order_change((U8*)&temp16,2);
	uart_general_data.generalInfo.maintenanceMileage= temp16;
	/*车内温度*/
	//uart_general_data.generalInfo.inCarTemprature= app_get_inCarTemp();
	/*车外温度*/
	temp16 = app_get_outCarTemp();
	byte_order_change((U8*)&temp16,2);	
	uart_general_data.generalInfo.outCarTemprature= temp16;	
	
	/*平均油耗*/
	uart_general_data.generalInfo.averageFuel.unit = 1;  //L/100km 
	uart_general_data.generalInfo.averageFuel.val = app_get_avg_fuel_consump();
	temp16 = *((U16 *)(&uart_general_data.generalInfo.averageFuel));
	byte_order_change((U8*)&temp16,2);
	*((U16 *)(&uart_general_data.generalInfo.averageFuel)) = temp16;

	/*瞬时油耗*/
	uart_general_data.generalInfo.InstantaneousFuel.unit = 1;  //L/100km 
	uart_general_data.generalInfo.InstantaneousFuel.val = app_get_ins_fuel_consump();	
	temp16= *((U16 *)(&uart_general_data.generalInfo.InstantaneousFuel));
	byte_order_change((U8*)&temp16,2);
	*((U16 *)(&uart_general_data.generalInfo.InstantaneousFuel))=temp16;

	/* 主题ID*/
	uart_general_data.generalInfo.themeID = IPconfig.themeID;
	/* soc */
	uart_general_data.generalInfo.soc =default_value;
	/*动力电池电流*/
	uart_general_data.generalInfo.batteryCurrent=default_value;
	/*动力电池电压*/
	uart_general_data.generalInfo.batteryVoltage=default_value;

	/* 模式选择,普通模式时，标志位必须为0 */
	uart_general_data.generalInfo.mode_choice = get_screen_mode();	
	
	/*frame rear*/
	uart_general_data.tail= FRAME_REAR;
	temp16 = api_cal_crc16((uint8_t *)&uart_general_data.magic,GENERAL_FRAME_LEN-5);
	byte_order_change((U8*)&temp16,2);
	uart_general_data.crc16 = temp16;
	
	sent_data(UART_A20_CHN,(uint8_t *)&uart_general_data,GENERAL_FRAME_LEN);
}


/*
 * 函数功能: 普通模式专用帧框架
 */
static void app_mcu_core_special(void)
{
	U32 ver;
	U8  ver_High;
	U8  ver_Mid;
	U8  ver_Low; 

	U32 temp32;
	U16 temp16;

	uart_special_data.head = FRAME_HEAD;
	uart_special_data.length = (uint8_t)SPECIAL_FRAME_LEN-9;	//len
	
	temp32 = (U32)MAGIC_SPECIAL_UP;
	byte_order_change((U8*)&temp32,4);
	uart_special_data.magic= temp32;

	/*硬件版本*/
	ver = get_HW_version();
	ver_High = (U8)(ver>>16);
	ver_Mid  = (U8)(ver>> 8);
	ver_Low  = (U8)(ver);

	temp16=((U16)ver_High<<13)+((U16)ver_Mid<<8)+((U16)ver_Low);
	byte_order_change((U8*)&temp16,2);
	uart_special_data.specialInfo.hw_ver.first.val=temp16;
	
	///IPconfig.Sts = 1;///Qt test
	if(IPconfig.Sts)
	{
		uart_special_data.specialInfo.LedWarning.third.bit.IPconfig=2;
	}
	else
	{
		uart_special_data.specialInfo.LedWarning.third.bit.IPconfig=1;
	}
	
#if	TEST_MODE
	uart_special_data.specialInfo.LedWarning.third.bit.TestMode=1;
#else
	uart_special_data.specialInfo.LedWarning.third.bit.TestMode=0;
#endif
	
	uart_special_data.specialInfo.MainDisp.first.bit.DispMode=UserSetting.disp_mode_setting&0x03;
	uart_special_data.specialInfo.MainDisp.first.bit.NotDisp=UserSetting.not_disp_info_setting&0x03;
	uart_special_data.specialInfo.CarNum.first.val=IPconfig.CarNum;  

/*以下是测试用数据*/
	temp16 = get_this_trip_distance();
	byte_order_change((U8*)&temp16,2);
	uart_special_data.specialInfo.this_trip_distance  = temp16;
	temp16 = get_drive_time();
	byte_order_change((U8*)&temp16,2);
	uart_special_data.specialInfo.this_trip_time = temp16;
	uart_special_data.specialInfo.oil_precent = get_fuel_percent3();
	uart_special_data.specialInfo.ad_saved= get_test_data_U8_ad_saved();
	uart_special_data.specialInfo.temp = app_get_inCarTemp();
	uart_special_data.specialInfo.water_temp = app_get_temp_value();

	temp32 = get_total_used_fuel();
	byte_order_change((U8*)&temp32,4);
	uart_special_data.specialInfo.total_used_fuel = temp32;

	//uart_special_data.specialInfo.this_trip_used_fuel = (U16)(get_this_trip_used_fuel()/1000);
	temp16 = get_this_trip_used_fuel()/1000;
	byte_order_change((U8*)&temp16,2);
	uart_special_data.specialInfo.this_trip_used_fuel = temp16;

	temp16 = get_this_trip_AFE();
	byte_order_change((U8*)&temp16,2);
	uart_special_data.specialInfo.this_trip_AFE = temp16;

	uart_special_data.tail =FRAME_REAR;
	/* crc */
	temp16 = api_cal_crc16((U8*)&uart_special_data.magic,SPECIAL_FRAME_LEN-5);//crc
	byte_order_change((U8*)&temp16,2);
	uart_special_data.crc = temp16;
	
	sent_data(UART_A20_CHN,(uint8_t *)&uart_special_data,SPECIAL_FRAME_LEN);	
}


void app_uart_rec_process(UartQueue *p)
{
	uint32_t  magic;
	uint16_t  check_sum;
	const    uint16_t  buf_len = SETTINGS_FRAME_LEN_MAX;
	uint8_t   i =0;
	uint8_t   data_buf[SETTINGS_FRAME_LEN_MAX] = {0};
	uint8_t   data_len;					//record the segment data len
	const U8  len_offset = 1,crc_offset = 2, magic_offset = 4,data_offset =  8;
	
	static U8 trip1Clean_last=0;
	static U8 trip2Clean_last=0;
	static U8 averageFuelClean_last=0;
	static U8 averageSpdClean_last=0;
	static U8 DriveTimeClean_last=0;
	static U8 WriteMainDispMode_last=0;
	static U8 WriteMainNotDisp_last=0;
//	static U8 CarService_last=0;
	static U8 uart_recv_count = 0;
	
	last_uart_tick = hal_timer_get_tick();	/* 保存一下收到UART的tick*/

	do 
	{
		//data move,if not move,not convenience for crc check
		for(i=0;i<buf_len;i++)
		{
			data_buf[i] = p->queue[(p->front+1+i)%(buf_len)];
		}
		data_len  = data_buf[len_offset];
		check_sum = data_buf[crc_offset] | ((U16)data_buf[crc_offset+1]<<8);
		magic     = data_buf[magic_offset]|
			(U32)data_buf[magic_offset+1]<<8|
			(U32)data_buf[magic_offset+2]<<16|
			(U32)data_buf[magic_offset+3]<<24;

		if(magic==MAGIC_GENERAL_DOWN)
		{
			if(data_len !=  SETTINGS_GENERAL_FRAME_LEN-9 )
			{
				break;  //len err
			}
			if(check_sum != api_cal_crc16(&data_buf[magic_offset],SETTINGS_GENERAL_FRAME_LEN-5))
			{
				break;	//check sum err
			}
			uart_clear_queue(&mcu_frame_rec);			// clear queue      
		}
		else if(magic==MAGIC_SPECIAL_DOWN)
		{
			
			if(data_len !=  SETTINGS_SPECIAL_FRAME_LEN-9)
			{
				break;  //len err
			}
			if(check_sum != api_cal_crc16(&data_buf[magic_offset],SETTINGS_SPECIAL_FRAME_LEN-5))
			{
				break;	//check sum err
			}
			uart_clear_queue(&mcu_frame_rec);			// clear queue      
		}
		else
		{
			break;//magic error
		}
		
		g_U8_core_start_work = 1;//收到CORE的串口数据，说明CORE开始工作
		
		if(magic==MAGIC_GENERAL_DOWN)
		{
			mcu_rec_general.settingsInfo .first.val   =	data_buf[data_offset];
			mcu_rec_general.settingsInfo .warningSoundAndInterfaceSync=data_buf[data_offset+1];
			
			mcu_rec_general.settingsInfo .SoundAndInterfaceSync=data_buf[data_offset+2];
			mcu_rec_general.settingsInfo .fourth.val =data_buf[data_offset+3];
			//dbg_string("themeID:%d\n", mcu_rec_general.settingsInfo .fourth.bits.themeID);
			mcu_rec_general.settingsInfo .Time.u8Time[3]  = data_buf[data_offset+4]; 
			mcu_rec_general.settingsInfo .Time.u8Time[2]  = data_buf[data_offset+5]; 
			mcu_rec_general.settingsInfo .Time.u8Time[1]  = data_buf[data_offset+6]; 
			mcu_rec_general.settingsInfo .Time.u8Time[0]  = data_buf[data_offset+7]; 
		}
		else if(magic==MAGIC_SPECIAL_DOWN)
		{
			mcu_rec_special.settingsInfo.first.val=data_buf[data_offset];
			mcu_rec_special.settingsInfo.Backlight=data_buf[data_offset+1];
			mcu_rec_special.settingsInfo.TotalTrip.u8TotalTrip[3]=data_buf[data_offset+2];
			mcu_rec_special.settingsInfo.TotalTrip.u8TotalTrip[2]=data_buf[data_offset+3];
			mcu_rec_special.settingsInfo.TotalTrip.u8TotalTrip[1]=data_buf[data_offset+4];
			mcu_rec_special.settingsInfo.TotalTrip.u8TotalTrip[0]=data_buf[data_offset+5];
			mcu_rec_special.settingsInfo.MainDisp.val=data_buf[data_offset+6];
			mcu_rec_special.settingsInfo.CarNumInfo.val=data_buf[data_offset+7];
			mcu_rec_special.settingsInfo.serviceMileage.u8srvMil[1]=data_buf[data_offset+8];
			mcu_rec_special.settingsInfo.serviceMileage.u8srvMil[0]=data_buf[data_offset+9];
		}		
	} while (0);

	/********************************************************************************************/
	//这位为1表示bootload烧写程序成功
	if(mcu_rec_general.settingsInfo.first.bits.load_success)
	{
		mcu_rec_general.settingsInfo.first.bits.load_success = 0;
		if(uart_recv_count == 0)
		{
			app_buz_load_success();
			uart_recv_count = 1;
		}
	}
	//以下是界面对MCU数据的清零操作，相关标志位从0->1的跳变执行一次有效的清零操作
	if((mcu_rec_general.settingsInfo .first.bits.trip1Clean)&&(trip1Clean_last==0))
	{
		app_sub_trip1_clear();
		clear_total_used_fuel();
		uart_special_data.specialInfo.LedWarning.third.bit.ClearTrip1=1;//点火时只能清一次小计
	}
	if((mcu_rec_special.settingsInfo.first.bits.trip2Clean)&&(trip2Clean_last==0))
	{
		app_sub_trip2_clear();
		uart_special_data.specialInfo.LedWarning.third.bit.ClearTrip2=1;//点火时只能清一次小计
	}
	if((mcu_rec_general.settingsInfo .first.bits.averageFuelClean)&&(averageFuelClean_last==0))
	{
		app_clear_avg_fuel_consump();
	}  
	if((mcu_rec_general.settingsInfo .first.bits.averageSpdClean)&&(averageSpdClean_last==0))
	{
		app_clear_avg_spd();
	} 
	if((mcu_rec_special.settingsInfo.first.bits.DriveTimeClean)&&(DriveTimeClean_last==0))
	{
		drive_time_clear();
	}
	/********************************************************************************************/
	//配置主界面显示内容,相关标志位从0->1的跳变执行一次有效写操作
	if((mcu_rec_special.settingsInfo.MainDisp.bit.WriteMainDispMode)&&(WriteMainDispMode_last==0))
	{
		UserSetting.disp_mode_setting=mcu_rec_special.settingsInfo.MainDisp.bit.MainDispMode;
		app_user_setting_save();
	}
	
	if((mcu_rec_special.settingsInfo.MainDisp.bit.WriteMainNotDisp)&&(WriteMainNotDisp_last==0))
	{
		UserSetting.not_disp_info_setting=mcu_rec_special.settingsInfo.MainDisp.bit.MainNotDisp;
		app_user_setting_save();
	}
	/********************************************************************************************/
	//以下内容待定
	if((mcu_rec_special.settingsInfo.first.bits.IPconfigInfo)
		&&(uart_special_data.specialInfo.LedWarning.third.bit.IPconfig==0))
	{
		//TBD
		//uart_special_data.specialInfo.LedWarning.third.bit.IPconfig=1;
	}
	if(mcu_rec_special.settingsInfo.first.bits.DriveSystem)
	{
		//TBD
	}
	/*******************************************************************************************/
	
	if(mcu_rec_special.settingsInfo.first.bits.CarService)
	{
		app_service_clean_service();
	}

	
	if(g_u8_uart_req_bak != mcu_rec_general.settingsInfo.first.bits.commandReq)
	{
		g_u8_uart_req_bak = mcu_rec_general.settingsInfo .first.bits .commandReq;
		
		if(mcu_rec_general.settingsInfo.first.bits.commandReq) /* 收到了来自核心板的第一帧 */
		{
			judge_which_screen_mode();	
			/*获得显示模式*/
			screen_mode = get_screen_mode();
		}
	}

	/************************************************************************/
	//保留上一次的状态
	trip1Clean_last=mcu_rec_general.settingsInfo .first.bits.trip1Clean;                                       
	trip2Clean_last=mcu_rec_special.settingsInfo .first.bits.trip2Clean;                                       
	averageFuelClean_last=mcu_rec_general.settingsInfo .first.bits.averageFuelClean;                                 
	averageSpdClean_last=mcu_rec_general.settingsInfo .first.bits.averageSpdClean;                                  
	DriveTimeClean_last=mcu_rec_special.settingsInfo.first.bits.DriveTimeClean;                                   
	WriteMainDispMode_last=mcu_rec_special.settingsInfo.MainDisp.bit.WriteMainDispMode;                                
	WriteMainNotDisp_last=mcu_rec_special.settingsInfo.MainDisp.bit.WriteMainNotDisp;                                 
//	CarService_last=mcu_rec_special.settingsInfo.first.bits.CarService;                                       

}


/* 获取是否收到的ARM发给MCU的第一帧 */
unsigned char get_uart_first_frame(void)
{
	return mcu_rec_general.settingsInfo.first.bits.commandReq;
}

/* 获取是否收到的ARM发给MCU的关屏请求 */
unsigned char get_uart_close_lcd_req(void)
{
	return mcu_rec_general.settingsInfo.first.bits.closeReq;
}

/* 获取是否超速报警 */
unsigned char get_uart_Overspeed(void)
{
	if(app_uart_get_sound_sync()==SPD_120KM)
		return 1;
	else
		return 0;
}

void app_frame_get_task(void)
{
	
	uint8_t i = SETTINGS_FRAME_LEN_MAX;
	uint8_t ch;

	while(i--)
	{
		if(uart_queue_not_empty(&uart_recv_queue))				/* 如果串口有数据 */
		{
			ch = (uint8_t)uart_de_queue(&uart_recv_queue);		// 出队列
		}
		else
		{
			break;
		}
		if(uart_en_queue(&mcu_frame_rec,ch))			   // 入队列失败，说明当前队列已满
		{
				uart_de_queue(&mcu_frame_rec);				// out queue
				uart_en_queue(&mcu_frame_rec,ch);  
				
				if((mcu_frame_rec.queue[mcu_frame_rec.front+1] == FRAME_HEAD) 
					&&(mcu_frame_rec.queue[mcu_frame_rec.rear] == FRAME_REAR))
				{			                                                           
					app_uart_rec_process(&mcu_frame_rec);		// data parse     
				}                                                           
		}
		else
		{
			if((mcu_frame_rec.queue[mcu_frame_rec.front+1] == FRAME_HEAD)
				&&(mcu_frame_rec.queue[mcu_frame_rec.rear] == FRAME_REAR))
			{
				app_uart_rec_process(&mcu_frame_rec);		// data parse
			}
		}
	}
}

/*
 * 函数功能:普通模式任务函数，100ms发送一次工程模式数据框架
 */
void app_frame_sent_task(void)
{
	if(get_uart_first_frame()==ENABLE)
	{
		switch (screen_mode)
		{
			case MODE_NORMAL:
				app_mcu_core_general();
				app_mcu_core_special();
				break;
			case MODE_DISPLAY:
			case MODE_PROJECT:	
				app_mcu_core_general_demo(); 
				app_mcu_core_special_demo();
				break;
			default:
				dbg_string("screen mode ERROR!\n");
		}
	}
	else
	{
	//	dbg_string("not get first frame \n");
	}
}

U8 app_uart_get_car_num(void)
{
	return (mcu_rec_special.settingsInfo.CarNumInfo.val);
}

U8 app_uart_get_car_srv_num(void)
{
	return (mcu_rec_special.settingsInfo.MainDisp.bit.serviceNum);
}

U16 app_uart_get_car_srv_mil(void)
{
	return (mcu_rec_special.settingsInfo.serviceMileage.u16srvMil);
}

U8 app_uart_get_theme_id(void)
{
	return (mcu_rec_general.settingsInfo.fourth.bits.themeID);
}

void uart_set_heartbeat_tick(U32 tk)
{
	last_uart_tick = tk;
}
U8 app_uart_get_sound_sync(void)
{
	//dbg_string("sound sync:%d\n",mcu_rec_general.settingsInfo .warningSoundAndInterfaceSync);
	return mcu_rec_general.settingsInfo .warningSoundAndInterfaceSync;
}

/* 检查ARM是否活着 */
void app_uart_arm_alive(void)
{
	/*调试模式下，5s未通讯也不给ARM掉电*/
#ifndef __DEBUG__
		if ((hal_timer_passed(last_uart_tick) >= 5*HZ))
		{
			dbg_string("5S NO communication, Reboot ARM!\n");
			app_backlight_off();
			msleep(100);
			A20_POWER_OFF();
			msleep(100);
			A20_POWER_ON();
			mid_set_io_sts(&BL_GMA_SD,ON);/* open LCD data EN */
			msleep(200);		
			app_backlight_init();
			
			last_uart_tick = hal_timer_get_tick();
		}
#endif

}

