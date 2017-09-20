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
#include "app_info.h"
#include "app_warning.h"
#include "app_main_interface.h"
#include "app_big_moudle.h"
#include "app_warning.h"
#include "app_trip.h"
#include <time.h>




GeneralFrame	uart_general_data;
SpecialFrame	uart_special_data;

SettingsGeneralFrame mcu_rec_general;
SettingsSpecialFrame mcu_rec_special;

U8		rec_queue_buf[SETTINGS_FRAME_LEN_MAX]= {0};	//uesd to process core to mcu data buf
UartQueue mcu_frame_rec = {0};

//FRAMEA20TOMCU_t  g_tUart1Rec = {0};


/* 记录上一次收到UART数据时的tick */
static U32 last_uart_tick;

/*正常模式 工程模式 演示模式*/
static E_SCREEN_MODE screen_mode;

/* globle varialbe */
void check_program_versions(void);
void excute_boot(void);


/* 该函数就上电的时候初始化一次。*/
void app_uart_frame_init(void)
{
	hal_uart_init();//初始化队列
	#if 1
	uart_queue_init(&mcu_frame_rec,rec_queue_buf,sizeof(rec_queue_buf)/sizeof(rec_queue_buf[0]));
	uart_clear_queue(&mcu_frame_rec);
	memset(&g_tUart1Rec, 0, sizeof(FRAMEA20TOMCU_t)); /* init mcu receive  data */  //add for 206
	memset(&uart_general_data,0,sizeof(uart_general_data));
	memset(&uart_special_data,0,sizeof(uart_special_data));
	memset(&mcu_rec_general,0,sizeof(mcu_rec_general));
	memset(&mcu_rec_special,0,sizeof(mcu_rec_special));
	
	/* 有些数据默认不是发送0的，需要特殊设置 */
	uart_special_data.specialInfo.MainDisp.first.bit.DispMode=3;
	uart_special_data.specialInfo.MainDisp.first.bit.NotDisp=3;
	mcu_rec_special.settingsInfo.CarNumInfo.val = 0xff;	/* 没有读到Flash中存储的车型代码前，不能发送0，因为0是第一种车型 */
	mcu_rec_general.settingsInfo.fourth.bits.themeID = 0xF; /* 没有读到Flash中存储的主题ID前，不能发送0，因为0是第一种主题 */
	#endif

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

/*
 * 函数功能:普通模式任务函数，100ms发送一次工程模式数据框架
 */
void app_frame_sent_task(void)
{
	app_main_farme_sent_task();
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


void app_frame_get_task20(void)
{
	U8 u8Char = 0;
	static Bool bHeadOK = FALSE, bGetLen = FALSE;
	static U8 dataindex = 0, datalen = 0;
	if (uart_queue_not_empty(&uart_recv_queue)) /* 如果串口有数据 */
	{
		u8Char = uart_de_queue(&uart_recv_queue);			// 出队列
	}
	else
	{
		return;
	}

	if (bHeadOK == FALSE && u8Char == FRAME_HEAD)
	{
		bHeadOK = TRUE;
		uart_clear_queue(&mcu_frame_rec);
		uart_en_queue(&mcu_frame_rec, u8Char);
		return;
	}
	if (bHeadOK == TRUE && bGetLen == FALSE)
	{
		datalen = u8Char;
		bGetLen = TRUE;
		dataindex = 0;
		uart_en_queue(&mcu_frame_rec, u8Char);
		return;
	}
	if (bHeadOK == TRUE && bGetLen == TRUE)
	{
		if (dataindex < datalen + 2)
		{
			dataindex++;
			uart_en_queue(&mcu_frame_rec, u8Char);
			if (dataindex >= datalen + 2)
			{
				
				uart_clear_queue(&uart_recv_queue);
				if (mcu_frame_rec.queue[mcu_frame_rec.rear - 1] == FRAME_REAR)
				{
					uart_data_parse(&mcu_frame_rec);
					bHeadOK = FALSE;
					bGetLen = FALSE;
					datalen = 0;
				}
				else
				{
					bHeadOK = FALSE;
					bGetLen = FALSE;
					datalen = 0;
				}
			}
		}
	}
}


/*================================================================================
 **         Copyright (c) 2014, Wuhan AUTOROCK R&D
 ** \name		uart_data_parse
 ** \brief		used for autorock uart parse
 ** \input		UartQueue
 ** \output		void
 ** \author		haitaoh
 ** \version 	0.1
 ** \data
 ** \notes
 **
 ================================================================================
 */



void uart_data_parse(UartQueue *p)
{

	U16 data_len = 0;
	U8 i = 0, frametype = 0;
	U8 data_point = 0;
	static U8 dat[20] =
	{ 0 };

	data_len = p->queue[p->front + 1];
	frametype = p->queue[p->front + 2];
	data_point = (p->front + 3) % A20_MCU_DATA_BUF_LENGTH;
	for (i = 0; i < data_len; i++)
	{
		dat[i] = p->queue[data_point++ % A20_MCU_DATA_BUF_LENGTH];
	}
	if ((A20_MCU_DATA_LENTH + 2) != data_len)
	{
#ifdef WIN32
		DBG("data len err\n");
		printf("it should be %d,but you fill is %d\n",A20_MCU_DATA_LENTH+2,data_len);
#endif
	}
	g_tUart1Rec.u8TripClear = dat[0] & 0x01;
	g_tUart1Rec.time_set_enable = dat[0] & 0x02;
	g_tUart1Rec.u8MenuNum = dat[1];
	g_tUart1Rec.u8BattBoxNum = dat[2];
	memcpy(&g_tUart1Rec.u32UTCTime,&dat[3],4);
	//g_tUart1Rec.u32UTCTime = dat[3]|dat[4]<<8

	if (g_tUart1Rec.u8TripClear)
		app_sub_trip1_clear();
		
	if (0 != g_tUart1Rec.u8MenuNum)
	{
		MidSchAddTask(app_frame_sent_sub, 1000);			//发送界面数据
	}
	else
	{
		MidSchDeleteTask(app_frame_sent_sub);			//停止发送界面数据
	}
	//g_nMcuUartToJadeEnable = TRUE;

}

U8 oushucisend = 0;

void app_main_farme_sent_task(void)
{
	main_interface_get_data();
	main_interface_send_data();
	

	if ((oushucisend % 2) == 0)
	{
		warning_get_data();
		warning_send_data();

	}

	oushucisend++;

	if (oushucisend == 0xff)
	{
		oushucisend = 0;
	}

}

/*================================================================================
 **
 ** \name		app_frame_sent_main
 ** \brief	 串口发送主界面数据
 ** \input		void
 ** \output	    1：ERR 0:OK
 ** \author
 ** \data
 ** \notes
 **
 ================================================================================*/

//extern void oil_pump_send_data(void);
typedef void (*p_send_analyse)(void);
typedef struct
{
	FRAME_TYPES frame_type;
	p_send_analyse f_handle;
}
SEND_FRAME_TYPE;

#include "Menu_Controlsystem.h"
#include "Menu_Motorsystem.h"
#include "Menu_Auxiliarysystem.h" 
#include "Menu_Tcusystem.h"	
#include "Menu_Batterymanagesystem.h" 
#include "Menu_Batterystate.h"
#include "Menu_Aircontrolsystem.h"
#include "Menu_Front_Module.h"
#include "Menu_Middle_Module.h"
#include "Menu_Back_Module.h"



SEND_FRAME_TYPE send_frame_table[] = 
{
	{MENU_CONTROLSYSTEM,menu_control_system_Task},
	{MENU_MOTORSYSTEM,menu_engine_system_Task},
	{MENU_AUXILIARYSYSTEM,menu_assist_system_Task},
	{MENU_TCUSYSTEM,menu_TCU_system_Task},
	{MENU_BATTERYMANAGESYSTEM,menu_bms_info_system_Task},
	{MENU_BATTERYSTATE,menu_battery_info_system_Task},
	{MENU_AIRCONTROLSYSTEM,menu_AC_system_Task},
	{MENU_FRONT_MODULE,menu_front_module_system_Task},
	{MENU_MIDDLE_MODULE,menu_middle_module_system_Task},
	{MENU_BACK_MODULE,menu_back_module_system_Task},
	{-1,NULL},
};


U8 app_frame_sent_sub(void)
{
	U32 i;
	for (i=0;send_frame_table[i].frame_type != -1;i++)
	{
		if (g_tUart1Rec.u8MenuNum == send_frame_table[i].frame_type )
		{
			if (NULL == send_frame_table[i].f_handle)
			{
				dbg_printf("err: send frame is null \n");
			}
			else{
				send_frame_table[i].f_handle();
			}
			break;
		}
	}
}


//!<新协议封装
/**
 * 帧协议: 小端模式
 *   | 帧 头 | 总长度 | 命 令 | 数 据 | CRC | 帧 尾 |
 *   |   1   |    1   |   1   |   n   |  2  |   1   |(byte)
 *   |  AAh  |   n+6  |  cmd  |  ...  |  x  |  55h  |
 */
/** 
 * @brief  封装数据
 * @param dest: 封装后的数据存放地址
 * @param data: 数据源
 * @param cmd: 命令类型
 * @param len: 数据长度 
 * @retval : 封装后数据长度
 */

U32 app_uart_arm_send_parse(U8 *dest,const void *data,U8 cmd,U8 len)
{
	U16 temp;
	assert_param(dest == NULL);
	assert_param(data == NULL);
	assert_param(len >= 250);
	dest[0] = FRAME_HEAD;
	dest[1] = len + 6;
	dest[2] = cmd;
	memcpy(&dest[3],data,len);
	/*-------------------------CRC check------------------------*/
	temp = api_cal_crc16((U8 *)data, len); /* crc check */
	dest[len + 3] = temp & 0xFF;
	dest[len + 4] = (temp>>8) & 0xFF;
	dest[len + 5] = cmd;
	dest[len + 6] = FRAME_REAR;
	return (U32)dest[1];
}


/** 
 * @brief  查询软件版本
 * @param dest: None
 * @retval : None
 */
#define CHECK_ID_DATA_LEN     8      //参考升级协议
#define CAN_BL_APP            1
void check_program_versions(void)
{
	U8 data[CHECK_ID_DATA_LEN];
	U8 buffer[CHECK_ID_DATA_LEN+6];
	U32 parse_len;
	memset(data,0,sizeof(data));
	data[0] = (MCU_VERSION_H>>8)&0xFF;
	data[1] = MCU_VERSION_H&0xFF;
	data[2] = (MCU_VERSION_M>>8)&0xFF;
	data[3] = MCU_VERSION_M&0xFF;	
	data[4] = 0;
	data[5] = 0;
	data[6] = 0;
	data[7] = CAN_BL_APP;
	parse_len = app_uart_arm_send_parse(buffer,data,OnlineCheckCMD,CHECK_ID_DATA_LEN);
	sent_data(UART_A20_CHN, data, parse_len); /* data sent */
}
void excute_boot(void)
{
	__DI();
	hal_soft_reset();
}


