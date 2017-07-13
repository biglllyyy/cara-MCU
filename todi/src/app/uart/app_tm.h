#ifndef APP_TM_H
#define APP_TM_H
#include "comm_typedef.h"
#include "app_can.h"

typedef struct
{
	// VCU2TERMINAL_MOTOR01
	U8 TM_Number; //电机个数
	U8 TM_Serial; //电机序号
	U8 TM_WorkStatus; //TM驱动电机状态
	DATA_BIT VCU_Status_Flag1; //状态1

	//U8 TM_Control_Temp = 40; //TM电机控制器温度 main  main已有的不再发送
	//U16  TM_Feedback_RPM = 20000; //TM电机转速 main
	//U16  TM_Feedback_NM = 20000; //TM电机转矩 main
	// VCU2TERMINAL_MOTOR02
	//U8 TM_Temp = 40; //TM电机温度   main
	U16 TM_Voltage_DC; //TM电机直流电压
	U16 TM_Current_DC; //TM电机直流电流

	U8 VCU_Life; //整车控制器LIFE
	U8 VCU_Code; //整车故障码
	U8 VCU_TM_Brake_percent; //制动踏板开度
	U8 VCU_TM_Speed_percent; //加速踏板开度
} TM_Data_Struct;

//TM_Data_Struct tm_data;
void tm_get_data(void);
void tm_send_data(void);
#endif
