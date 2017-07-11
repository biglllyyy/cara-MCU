#ifndef  APP_BMS_H
#define  APP_BMS_H
#include "comm_typedef.h"

typedef struct
{
	//U16 BMS_Current; //电池系统电流 main 1
	//U16 BMS_Voltage; //电池系统内总电压  main 2
	//U8 BMS_SOC; //SOC   main 3
	U8 BMS_Mode; //电池充放电模式 4
	U8 BMS_Kt; //高压继电器状态 5
	U8 BMS_Status; //电池状态 6(故障状态)
	U8 BMS_Number; //单体电压数量 7

	U8 BMS_Temp_Number; //单体温度数量 8
	U8 BMS_System_Unit; //系统数量 9
	U8 BMS_System_Number; //系统号 10
	U8 Reversed1;

	//U16 BAT_Single_H; //电池单体高电压  main  11
	U8 BAT_Single_H_Number; //电池单体高电压位置 编号11
	U8 BAT_Single_H_Case; //电池单体高电压箱号 11
	//U16 BAT_Single_L; //电池单体低电压  main 12
	U8 BAT_Single_L_Number; //电池单体低电压位置 12
	U8 BAT_Single_L_Case; //电池单体低电压箱号 12


	//U8 BAT_Temp_H; //电池单体最高温度  main 13
	U8 BAT_Temp_H_Number; //电池单体高温度序号 13
	U8 BAT_Temp_H_Case; //电池单体高温度箱号 13
	//U8 BAT_Temp_L; //电池单体最低温度  main 14
	U8 BAT_Temp_L_Number; //电池单体低温度序号 14
	U8 BAT_Temp_L_Case; //电池单体低温度箱号 14

	U8 Reversed2;
	U8 BAT_Temp_Average; //电池平均温度 15
	U16 BMS_Ohm; //绝缘阻值 16



	U8 BAT_Temp_H_Limit; //最高允许电池单体温度 2.1
	U8 BAT_Temp_L_Limit; //最低允许电池单体温度 2.2
	U8 BMS_SOC_L_Limit; //最低允许SOC值  2.3
	U8 Reversed3;

	U16 BAT_Current_Discharge_Limit; //最高允许放电电流 2.4
	U16 BAT_Current_Charge_Limit; //最高允许充电电流 2.5

//IRM01
	U32 IRM_Ohm_Positive; //总正绝缘阻值 2.6

	U32 IRM_Ohm_Negative; //总负绝缘阻值 2.7

	U8 IRM_Fault_Level; //设备故障等级	2.8
	U8 IRM_Insulation_Level; //绝缘等级  2.9
	U8 IRM_Life; //绝缘LIFE 2.10
	U8 BAT_Voltage_Fault; //总压故障 2.11

	U8 BAT_Single_Fault; //单压故障 2.12
	U8 BAT_Temp_Fault; //温度故障  2.13
	U8 BAT_Insulation_Fault; //绝缘故障 2.14
	U8 BAT_Consistency_Fault; //电池一致性故障 2.15


	U8 BAT_SOC_Fault; //SOC故障 2.16
	U8 BAT_Current_Fault; //电流故障 2.17
	U8 BAT_Lock_Fault; //高压互锁故障 2.18
	U8 BMS_Comm_Fault; //BMS通讯故障 2.19


	U8 BMS_System_Fault; //BMS系统故障 2.20
	U8 Reversed4;
	U16 Reversed5;

} Bms_Data_Struct;

void bms_get_data(void);
void bms_send_data(void);
#endif
