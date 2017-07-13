#ifndef APP_TM_H
#define APP_TM_H
#include "comm_typedef.h"
#include "app_can.h"

typedef struct
{
	// VCU2TERMINAL_MOTOR01
	U8 TM_Number; //�������
	U8 TM_Serial; //������
	U8 TM_WorkStatus; //TM�������״̬
	DATA_BIT VCU_Status_Flag1; //״̬1

	//U8 TM_Control_Temp = 40; //TM����������¶� main  main���еĲ��ٷ���
	//U16  TM_Feedback_RPM = 20000; //TM���ת�� main
	//U16  TM_Feedback_NM = 20000; //TM���ת�� main
	// VCU2TERMINAL_MOTOR02
	//U8 TM_Temp = 40; //TM����¶�   main
	U16 TM_Voltage_DC; //TM���ֱ����ѹ
	U16 TM_Current_DC; //TM���ֱ������

	U8 VCU_Life; //����������LIFE
	U8 VCU_Code; //����������
	U8 VCU_TM_Brake_percent; //�ƶ�̤�忪��
	U8 VCU_TM_Speed_percent; //����̤�忪��
} TM_Data_Struct;

//TM_Data_Struct tm_data;
void tm_get_data(void);
void tm_send_data(void);
#endif
