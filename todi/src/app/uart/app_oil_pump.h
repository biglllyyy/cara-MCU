#ifndef APP_OIL_PUMP_H
#define APP_OIL_PUMP_H
#include "comm_typedef.h"

typedef struct
{
	U8 OILDCAC_Status; //DCAC工作状态
	U8 OILDCAC_Output_Cut; //DCAC 输出切断
	U8 OILDCAC_intput_Cut; //DCAC 输入切断
	U8 OILDCAC_Fault_Code; //DCAC故障代码

	U8 OILDCAC_Level_Fault; //DCAC故障等级
	U8 OILDCAC_Temp_Warn; //DCDC温度报警
	U8 OILDCAC_Fault_Warn; //DCDC故障报警
	U8 OILDCAC_Temp; //DCAC模块温度

	U16 OILDCAC_V_Voltage; //DCAC输出V电压
	U16 OILDCAC_V_Current; //DCAC输出V电流

	U16 OILDCAC_W_Voltage; //DCAC输出W电压
	U16 OILDCAC_W_Current; //DCAC输出W电流

	U16 OILDCAC_U_Voltage; //DCAC输出U电压
	U16 OILDCAC_U_Current; //DCAC输出U电流

} Oil_Pump_Data_Struct;

void oil_pump_get_data(void);
void oil_pump_send_data(void);
#endif
