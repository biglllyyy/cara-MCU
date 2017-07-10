#include <string.h>
#include "comm_typedef.h"
#include "hal_flash.h"
#include "app_display.h"
#include "mid_pin.c"
#include "mid_string.h"
#include "app_io.h"
#include "io_cfg.h"
#include "app_uart_cmd.h"
#include "uart_cfg.h"
#include "fw_version.h"
#include"g_variable.h"
#include "app_CarKey.h"
#include "app_trip.h"
#include "app_spd.h"
#include "app_fuel_method2.h"
#include "app_service.h"
#include "app_IPconfig.h"
#include "app_fuel.h"
#include "crc16.h"
#include "app_hw_version.h"
#include "app_UserSetting.h"

#define DEFAULT_OIL        (75)

#define ADDR_BOOT_LOADER   (0x33E000)
#define ADDR_PROJECT_MODE  (0x33E004)
#define ADDR_DISPLAY_MODE  (0x33E008)

E_SCREEN_MODE screen_mode;

struct T_SCREEN_MODE_BUFF
{
	U16 flag_boot_loader;
	U16 reseve1;
	U16 count_project_mode;
	U16 reseve2;
	U16 count_display_mode;
	U16 reseve3;
};
static  struct T_SCREEN_MODE_BUFF screen_mode_buff;


static U16 demo_rpm;
static U32 demo_speed;
static U8 demo_TpmsFrontLeftTyre;
static U8 demo_TpmsFrontRightTyre;
static U8 demo_TpmsRearLeftTyre;
static U8 demo_TpmsRearRightTyre;
static U8 demo_TtmsFrontLeftTyre;
static U8 demo_TtmsFrontRightTyre;
static U8 demo_TtmsRearLeftTyre;
static U8 demo_TtmsRearRightTyre;
static U8 demo_temp;

static U16 get_data_from_addr(U32 addr);
static void get_saved_screen_mode_data();
static void save_screen_mode_data();
static void set_screen_mode(E_SCREEN_MODE data);
E_SCREEN_MODE get_screen_mode();
void judge_which_screen_mode();

E_SCREEN_MODE get_screen_mode()
{
	return screen_mode;
}
void set_screen_mode(E_SCREEN_MODE data)
{
	screen_mode = data;
}

void judge_which_screen_mode()
{
	get_saved_screen_mode_data();
	/*演示模式标志位*/
	if(screen_mode_buff.count_display_mode == 0xFFFE)
	{
		set_screen_mode(MODE_DISPLAY);
		dbg_string("flag:0xFFFE Entry display mode!\n");
		return ;
	}
	/*按键右键 演示模式*/
	if(mid_get_io_sts(&pin_io_in[PIN_IN_KEY1]) == OFF)	
	{
		if((screen_mode_buff.count_display_mode!=0)&&(screen_mode_buff.count_display_mode!=0xffff))
		{			
			screen_mode_buff.count_display_mode -= 1;
			set_screen_mode(MODE_DISPLAY);
			dbg_string("Entry Display mode! have %d times\n",screen_mode_buff.count_display_mode);
			save_screen_mode_data();
			return ;
		}
	}
	/*按下按键左键 工程模式*/
	if(mid_get_io_sts(&pin_io_in[PIN_IN_KEY2]) == OFF)
	{
		if((screen_mode_buff.count_project_mode!=0)&&(screen_mode_buff.count_project_mode!=0xffff))
		{
			
			screen_mode_buff.count_project_mode -= 1;
			set_screen_mode(MODE_PROJECT);
			dbg_string("Entry Project mode! have %d times\n",screen_mode_buff.count_project_mode);
			save_screen_mode_data();
			return;
		}
	}
	
	set_screen_mode(MODE_NORMAL);
}

/*从Workflash中得到各种模式的计数值*/
U16 get_data_from_addr(U32 addr)
{
	U16 data = 0;
	
	data = *((U16 *)addr);

	return data;
}

void get_saved_screen_mode_data()
{
	memset(&screen_mode_buff,0xff,sizeof(screen_mode_buff));
	screen_mode_buff.flag_boot_loader = get_data_from_addr(ADDR_BOOT_LOADER);
	screen_mode_buff.count_display_mode = get_data_from_addr(ADDR_DISPLAY_MODE);
	screen_mode_buff.count_project_mode = get_data_from_addr(ADDR_PROJECT_MODE);
}

/*保存各模式的计数值*/
void save_screen_mode_data()
{
	hal_data_flash_sect_erase(ADDR_BOOT_LOADER); 			/* 开始擦除 */
	while(hal_check_erasing_status(ADDR_BOOT_LOADER));		/* 等待擦除完毕 */
	hal_data_flash_sector_programing(ADDR_BOOT_LOADER,(U16 *)&screen_mode_buff, \
		0,sizeof(screen_mode_buff)/sizeof(U16));
}

/*函数功能: 处理工程模式随机数产生值*/
void app_display_rand_data_task(void)
{
	U8 temp;
	demo_speed = rand()%(120 - 1);

	demo_rpm = rand()%(8000 - 1);
	demo_TpmsFrontLeftTyre = rand()%(255 - 1);
	demo_TpmsFrontRightTyre = demo_TpmsFrontLeftTyre;
	demo_TpmsRearLeftTyre = demo_TpmsFrontLeftTyre;
	demo_TpmsRearRightTyre = demo_TpmsFrontLeftTyre;
	demo_TtmsFrontLeftTyre = rand()%(255 - 1);
	demo_TtmsFrontRightTyre = demo_TtmsFrontLeftTyre;
	demo_TtmsRearLeftTyre = demo_TtmsFrontLeftTyre;
	demo_TtmsRearRightTyre =demo_TtmsFrontLeftTyre;
	/*最高水温不超过108防止水温报警*/
	temp = rand()%70 + 50;
	if(temp<108) demo_temp =temp;	
}

/*函数功能: 工程模式通用帧框架*/
void app_mcu_core_general_demo(void)
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
	temp16 = demo_rpm;
	byte_order_change((U8*)&temp16,2);
	uart_general_data.generalInfo.rpm = temp16;
	
	/*RTC*/
	temp32 = g_u32_utcTime;
	byte_order_change((U8*)&temp32,4);
	uart_general_data.generalInfo.time =temp32;
	
	/*SPEED*/
	uart_general_data.generalInfo.speed=demo_speed;		

    /*水温*/
	uart_general_data.generalInfo.temprature=demo_temp;

	/*IGN 状态 */
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
	byte_order_change((U8*)&temp16,2);
	uart_general_data.generalInfo.remainMileage= temp16;
	
	/*trip1*/
	temp32 = app_get_sub_trip1();
	byte_order_change((U8*)&temp32,4);
	uart_general_data.generalInfo.trip1= (temp32>>8);

	/* fuel */
	/*如果演示模式，关掉所有报警动画*/
	if (get_screen_mode() == MODE_DISPLAY)
	{
		uart_general_data.generalInfo.fuel = DEFAULT_OIL;
	}
	else
	{
		uart_general_data.generalInfo.fuel =get_fuel_percent3();
	}
		
	/*trip2*/
	temp32 = app_get_sub_trip2();
	byte_order_change((U8*)&temp32,4);
	uart_general_data.generalInfo.trip2= (temp32>>8);
	
	/*保养里程*/
	temp16 = (U16)CarService.rest_service_mile;
	byte_order_change((U8*)&temp16,2);
	uart_general_data.generalInfo.maintenanceMileage= temp16;
	
	/*车内温度*/
	//uart_general_data.generalInfo.inCarTemprature= default_value;
	
	/*车外温度*/
	uart_general_data.generalInfo.outCarTemprature= default_value;
	
	/*平均油耗*/
	uart_general_data.generalInfo.averageFuel.unit = 1;  //L/100km 
	uart_general_data.generalInfo.averageFuel.val = app_get_avg_fuel_consump(); 
	temp16 = uart_general_data.generalInfo.averageFuel.val+(uart_general_data.generalInfo.averageFuel.unit<<15);
	byte_order_change((U8*)&temp16,2);
	*((U16 *)(&uart_general_data.generalInfo.averageFuel))=temp16;

	/*瞬时油耗*/
	uart_general_data.generalInfo.InstantaneousFuel.unit = 1;  //L/100km 
	uart_general_data.generalInfo.InstantaneousFuel.val = app_get_ins_fuel_consump();
	temp16= uart_general_data.generalInfo.InstantaneousFuel.val+(uart_general_data.generalInfo.InstantaneousFuel.unit<<15);
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

	/* 模式选择*/
	uart_general_data.generalInfo.mode_choice = get_screen_mode();

	/*frame rear*/
	uart_general_data.tail= FRAME_REAR;
	temp16 = api_cal_crc16((uint8_t *)&uart_general_data.magic,GENERAL_FRAME_LEN-5);
	byte_order_change((U8*)&temp16,2);
	uart_general_data.crc16 = temp16;
	
	sent_data(UART_A20_CHN,(uint8_t *)&uart_general_data,GENERAL_FRAME_LEN);
	
}


/*函数功能: 工程模式专用帧框架*/

void app_mcu_core_special_demo(void)
{
	U32 temp32;
	U16 temp16;
	
	U16 ver;
	U8  ver_High;
	U8  ver_Mid;
	U8  ver_Low;
	uart_special_data.head = FRAME_HEAD;
	uart_special_data.length = (U8)SPECIAL_FRAME_LEN-9;	//len
	
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
	
	
	/*如果演示模式，关掉所有报警动画*/
	if (get_screen_mode() == MODE_DISPLAY)
	{
		uart_special_data.specialInfo.LedWarning.first.val = 0;
		uart_special_data.specialInfo.LedWarning.second.val = 0;
		uart_special_data.specialInfo.LedWarning.third.val = 0;
	}
	
	if(IPconfig.Sts)
	{
		uart_special_data.specialInfo.LedWarning.third.bit.IPconfig=2;
	}
	else
	{
		uart_special_data.specialInfo.LedWarning.third.bit.IPconfig=1;
	}	
	uart_special_data.specialInfo.Tpms.FrontLeftTyre=demo_TpmsFrontLeftTyre;
	uart_special_data.specialInfo.Tpms.FrontRightTyre=demo_TpmsFrontRightTyre;
	uart_special_data.specialInfo.Tpms.RearLeftTyre=demo_TpmsRearLeftTyre;
	uart_special_data.specialInfo.Tpms.RearRightTyre=demo_TpmsRearRightTyre;
	uart_special_data.specialInfo.Ttms.FrontLeftTyre=demo_TtmsFrontLeftTyre;
	uart_special_data.specialInfo.Ttms.FrontRightTyre=demo_TtmsFrontRightTyre;
	uart_special_data.specialInfo.Ttms.RearLeftTyre=demo_TtmsRearLeftTyre;
	uart_special_data.specialInfo.Ttms.RearRightTyre=demo_TtmsRearRightTyre;
	uart_special_data.specialInfo.MainDisp.first.bit.DispMode=UserSetting.disp_mode_setting&0x03;
	uart_special_data.specialInfo.MainDisp.first.bit.NotDisp=UserSetting.not_disp_info_setting&0x03;
	uart_special_data.specialInfo.CarNum.first.val=IPconfig.CarNum;  

	uart_special_data.tail =FRAME_REAR;

	/* crc */
	temp16 = api_cal_crc16((U8*)&uart_special_data.magic,SPECIAL_FRAME_LEN-5);//crc
	byte_order_change((U8*)&temp16,2);
	uart_special_data.crc = temp16;
	sent_data(UART_A20_CHN,(U8 *)&uart_special_data,SPECIAL_FRAME_LEN);	
}


