#ifndef  APP_BMS_H
#define  APP_BMS_H
#include "comm_typedef.h"

typedef struct
{
	//U16 BMS_Current; //���ϵͳ���� main 1
	//U16 BMS_Voltage; //���ϵͳ���ܵ�ѹ  main 2
	//U8 BMS_SOC; //SOC   main 3
	U8 BMS_Mode; //��س�ŵ�ģʽ 4
	U8 BMS_Kt; //��ѹ�̵���״̬ 5
	U8 BMS_Status; //���״̬ 6(����״̬)
	U8 BMS_Number; //�����ѹ���� 7

	U8 BMS_Temp_Number; //�����¶����� 8
	U8 BMS_System_Unit; //ϵͳ���� 9
	U8 BMS_System_Number; //ϵͳ�� 10
	U8 Reversed1;

	//U16 BAT_Single_H; //��ص���ߵ�ѹ  main  11
	U8 BAT_Single_H_Number; //��ص���ߵ�ѹλ�� ���11
	U8 BAT_Single_H_Case; //��ص���ߵ�ѹ��� 11
	//U16 BAT_Single_L; //��ص���͵�ѹ  main 12
	U8 BAT_Single_L_Number; //��ص���͵�ѹλ�� 12
	U8 BAT_Single_L_Case; //��ص���͵�ѹ��� 12


	//U8 BAT_Temp_H; //��ص�������¶�  main 13
	U8 BAT_Temp_H_Number; //��ص�����¶���� 13
	U8 BAT_Temp_H_Case; //��ص�����¶���� 13
	//U8 BAT_Temp_L; //��ص�������¶�  main 14
	U8 BAT_Temp_L_Number; //��ص�����¶���� 14
	U8 BAT_Temp_L_Case; //��ص�����¶���� 14

	U8 Reversed2;
	U8 BAT_Temp_Average; //���ƽ���¶� 15
	U16 BMS_Ohm; //��Ե��ֵ 16



	U8 BAT_Temp_H_Limit; //��������ص����¶� 2.1
	U8 BAT_Temp_L_Limit; //��������ص����¶� 2.2
	U8 BMS_SOC_L_Limit; //�������SOCֵ  2.3
	U8 Reversed3;

	U16 BAT_Current_Discharge_Limit; //�������ŵ���� 2.4
	U16 BAT_Current_Charge_Limit; //������������ 2.5

//IRM01
	U32 IRM_Ohm_Positive; //������Ե��ֵ 2.6

	U32 IRM_Ohm_Negative; //�ܸ���Ե��ֵ 2.7

	U8 IRM_Fault_Level; //�豸���ϵȼ�	2.8
	U8 IRM_Insulation_Level; //��Ե�ȼ�  2.9
	U8 IRM_Life; //��ԵLIFE 2.10
	U8 BAT_Voltage_Fault; //��ѹ���� 2.11

	U8 BAT_Single_Fault; //��ѹ���� 2.12
	U8 BAT_Temp_Fault; //�¶ȹ���  2.13
	U8 BAT_Insulation_Fault; //��Ե���� 2.14
	U8 BAT_Consistency_Fault; //���һ���Թ��� 2.15


	U8 BAT_SOC_Fault; //SOC���� 2.16
	U8 BAT_Current_Fault; //�������� 2.17
	U8 BAT_Lock_Fault; //��ѹ�������� 2.18
	U8 BMS_Comm_Fault; //BMSͨѶ���� 2.19


	U8 BMS_System_Fault; //BMSϵͳ���� 2.20
	U8 Reversed4;
	U16 Reversed5;

} Bms_Data_Struct;

void bms_get_data(void);
void bms_send_data(void);
#endif
