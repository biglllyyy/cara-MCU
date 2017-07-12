#include "app_BMS.h"
#include "app_can.h"
#include "app_uart_cmd.h"
#include "hal_uart.h"
#include "string.h"
#include "crc16.h"
#include "mid_string.h"
#include "uart_cfg.h"


#define BMS_DATA_LENGTH 52
#define BMS_FRAME_TYPE  MENU_BATMANAGE_TYPE

static Bms_Data_Struct bms_data =
{ 0 };

void bms_get_data(void)
{
	U16 u16temp = 0;
	U16 buf1 = 0;
	U32 u32temp = 0;

	bms_data.BMS_Mode = BMS_Mode; 			//电池充放电模式 4
	bms_data.BMS_Kt = BMS_Kt; 				//高压继电器状态 5
	bms_data.BMS_Status = BMS_Status; 		//电池状态 6(故障状态)
	bms_data.BMS_Number = BMS_Number; 		//单体电压数量 7

	bms_data.BMS_Temp_Number = BMS_Temp_Number; 		//单体温度数量 8
	if(BMS_System_Unit>5)
	{
		BMS_System_Unit = 5;
	}
	bms_data.BMS_System_Unit = BMS_System_Unit; 		//系统数量 9
	bms_data.BMS_System_Number = BMS_System_Number;		//系统号 10
	bms_data.Reversed1 = 1;

	//U16 BAT_Single_H; //电池单体高电压  main  11
	bms_data.BAT_Single_H_Number = BAT_Single_H_Number; 		//电池单体高电压位置 编号11
	bms_data.BAT_Single_H_Case = BAT_Single_H_Case; 			//电池单体高电压箱号 11
	//U16 BAT_Single_L; //电池单体低电压  main 12
	bms_data.BAT_Single_L_Number = BAT_Single_L_Number; 		//电池单体低电压位置 12
	bms_data.BAT_Single_L_Case = BAT_Single_L_Case; 			//电池单体低电压箱号 12


	//bms_data.BAT_Temp_H; //电池单体最高温度  main 13
	bms_data.BAT_Temp_H_Number = BAT_Temp_H_Number; 			//电池单体高温度序号 13
	bms_data.BAT_Temp_H_Case = BAT_Temp_H_Case ; 				//电池单体高温度箱号 13
	//U8 BAT_Temp_L; //电池单体最低温度  main 14
	bms_data.BAT_Temp_L_Number = BAT_Temp_L_Number; 			//电池单体低温度序号 14
	bms_data.BAT_Temp_L_Case = BAT_Temp_L_Case; 				//电池单体低温度箱号 14

	bms_data.Reversed2 = 2;
	if(BAT_Temp_Average>250)
	{
		BAT_Temp_Average = 250;

	}
	bms_data.BAT_Temp_Average = BAT_Temp_Average; 				//电池平均温度 15  收到后减去40
	u16temp = BMS_Ohm;
	if(u16temp>50000)
	{
	  u16temp = 50000;
	}
	byte_order_change(((U8*)&(u16temp)),2); 
	bms_data.BMS_Ohm = u16temp; 								//绝缘阻值 16


	if(BAT_Temp_H_Limit>250)
	{
		BAT_Temp_H_Limit = 250;
	}
	bms_data.BAT_Temp_H_Limit = BAT_Temp_H_Limit; 				//最高允许电池单体温度 2.1  收到后减去40
	if(BAT_Temp_L_Limit>250)
	{
		BAT_Temp_L_Limit = 250;
	}
	bms_data.BAT_Temp_L_Limit = BAT_Temp_L_Limit; 				//最低允许电池单体温度 2.2  收到后减去40
	if(BMS_SOC_L_Limit>100)
	{
		BMS_SOC_L_Limit = 100;
	}
	bms_data.BMS_SOC_L_Limit = BMS_SOC_L_Limit; 				//最低允许SOC值  2.3
	bms_data.Reversed3 = 3;

	buf1 = BAT_Current_Discharge_Limit;
	if(buf1>64255)
	{
		buf1 = 64255;
	}
	buf1 = buf1/20;

	byte_order_change(((U8*)&(buf1)),2); 
	bms_data.BAT_Current_Discharge_Limit = buf1; 				//最高允许放电电流 2.4  收到后减去1600


	buf1 = BAT_Current_Charge_Limit;
	if(buf1>64255)
	{
		buf1 = 64255;
	}
	buf1 = buf1/20;

	byte_order_change(((U8*)&(buf1)),2); 
	bms_data.BAT_Current_Charge_Limit = buf1; 				//最高允许充电电流 2.5 收到后减去1600

//IRM01
	u32temp = IRM_Ohm_Positive;
	if(u32temp>100000)
	{
		u32temp=100000;
	}
	byte_order_change(((U8*)&(u32temp)),3); 
	bms_data.IRM_Ohm_Positive = u32temp;    				//总正绝缘阻值 2.6


	
	u32temp = IRM_Ohm_Negative;
	if(u32temp>100000)
	{
		u32temp=100000;
	}
	byte_order_change(((U8*)&(u32temp)),3); 
	bms_data.IRM_Ohm_Negative = u32temp;					//总负绝缘阻值 2.7

	bms_data.IRM_Fault_Level = IRM_Fault_Level; 				//设备故障等级	2.8
	bms_data.IRM_Insulation_Level = IRM_Insulation_Level; 		//绝缘等级  2.9
	bms_data.IRM_Life = IRM_Life; 								//绝缘LIFE 2.10
	bms_data.BAT_Voltage_Fault = BAT_Voltage_Fault; 			//总压故障 2.11

	bms_data.BAT_Single_Fault = BAT_Single_Fault; 				//单压故障 2.12
	bms_data.BAT_Temp_Fault = BAT_Temp_Fault; 					//温度故障  2.13
	bms_data.BAT_Insulation_Fault = BAT_Insulation_Fault; 		//绝缘故障 2.14
	bms_data.BAT_Consistency_Fault = BAT_Consistency_Fault; 	//电池一致性故障 2.15


	bms_data.BAT_SOC_Fault = BAT_SOC_Fault; 					//SOC故障 2.16
	bms_data.BAT_Current_Fault = BAT_Current_Fault; 			//电流故障 2.17
	bms_data.BAT_Lock_Fault = BAT_Lock_Fault; 					//高压互锁故障 2.18
	bms_data.BMS_Comm_Fault = BMS_Comm_Fault; 					//BMS通讯故障 2.19


	bms_data.BMS_System_Fault = BMS_System_Fault; 				//BMS系统故障 2.20
	bms_data.Reversed4 = 4;
	bms_data.Reversed5 = 5;

}
void bms_send_data(void)
{

	U16 temp = 0;
	U8 maindata[BMS_DATA_LENGTH + 6] =
	{ 0 };

	maindata[0] = FRAME_HEAD; /* load frame head */
	maindata[1] = BMS_DATA_LENGTH + 3; /* include data segment and crc segment */
	maindata[2] = BMS_FRAME_TYPE;

	memcpy(&maindata[3], &bms_data, BMS_DATA_LENGTH);

	/*-------------------------CRC check------------------------*/
	temp = api_cal_crc16(&maindata[3], BMS_DATA_LENGTH); /* crc check */

	maindata[3 + BMS_DATA_LENGTH] = temp & 0xff;
	maindata[3 + BMS_DATA_LENGTH + 1] = (temp & 0xff00) >> 8;

	maindata[3 + BMS_DATA_LENGTH + 1 + 1] = FRAME_REAR; /* frame rear */

	sent_data(UART_A20_CHN, maindata, (BMS_DATA_LENGTH + 6)); /* data sent */

}

