#ifndef APP_AIR_PUMP_H
#define APP_AIR_PUMP_H

#include "comm_typedef.h"
typedef struct
{
	U8 AIRDCAC_Status; //DCAC工作状态
	U8 AIRDCAC_Output_Cut; //DCAC 输出切断
	U8 AIRDCAC_intput_Cut; //DCAC 输入切断
	U8 AIRDCAC_Fault_Code; //DCAC故障代码

	U8 AIRDCAC_Level_Fault; //DCAC故障等级
	U8 AIRDCAC_Temp_Warn; //DCDC温度报警
	U8 AIRDCAC_Fault_Warn; //DCDC故障报警
	U8 AIRDCAC_Temp; //DCAC模块温度

	U16 AIRDCAC_V_Voltage; //DCAC输出V电压
	U16 AIRDCAC_V_Current; //DCAC输出V电流

	U16 AIRDCAC_W_Voltage; //DCAC输出W电压
	U16 AIRDCAC_W_Current; //DCAC输出W电流

	U16 AIRDCAC_U_Voltage; //DCAC输出U电压
	U16 AIRDCAC_U_Current; //DCAC输出U电流

} Air_Pump_Data_Struct;

void air_pump_get_data(void);
void air_pump_send_data(void);

#endif
