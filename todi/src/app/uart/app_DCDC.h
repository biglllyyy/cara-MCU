#ifndef  APP_DCDC_H
#define  APP_DCDC_H
#include "comm_typedef.h"

typedef struct
{
	U16  DCDC_Voltage; //DCDC输出电压
	U16  DCDC_Current; //DCDC输出电流

	U8 DCDC_Status; //DCDC工作状态
	U8 DCDC_Output_Cut; //DCDC 输出切断
	U8 DCDC_intput_Cut; //DCDC 输入切断
	U8 DCDC_Fault_Code; //DCDC故障码

	U8 DCDC_Level_Fault; //DCDC故障等级
	U8 DCDC_Temp_Warn; //DCDC温度报警
	U8 DCDC_Fault_Warn; //DCDC故障报警
	U8 DCDC_Temp; //DCDC模块温度
}DCDC_Data_Struct;

void dcdc_get_data(void);
void dcdc_send_data(void);
#endif
