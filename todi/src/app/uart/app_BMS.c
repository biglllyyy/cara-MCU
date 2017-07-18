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

	bms_data.BMS_Mode = BMS_Mode; 			//��س�ŵ�ģʽ 4
	bms_data.BMS_Kt = BMS_Kt; 				//��ѹ�̵���״̬ 5
	bms_data.BMS_Status = BMS_Status; 		//���״̬ 6(����״̬)
	bms_data.BMS_Number = BMS_Number; 		//�����ѹ���� 7

	bms_data.BMS_Temp_Number = BMS_Temp_Number; 		//�����¶����� 8
	if(BMS_System_Unit>5)
	{
		BMS_System_Unit = 5;
	}
	bms_data.BMS_System_Unit = BMS_System_Unit; 		//ϵͳ���� 9
	bms_data.BMS_System_Number = BMS_System_Number;		//ϵͳ�� 10
	bms_data.Reversed1 = 1;

	//U16 BAT_Single_H; //��ص���ߵ�ѹ  main  11
	bms_data.BAT_Single_H_Number = BAT_Single_H_Number; 		//��ص���ߵ�ѹλ�� ���11
	bms_data.BAT_Single_H_Case = BAT_Single_H_Case; 			//��ص���ߵ�ѹ��� 11
	//U16 BAT_Single_L; //��ص���͵�ѹ  main 12
	bms_data.BAT_Single_L_Number = BAT_Single_L_Number; 		//��ص���͵�ѹλ�� 12
	bms_data.BAT_Single_L_Case = BAT_Single_L_Case; 			//��ص���͵�ѹ��� 12


	//bms_data.BAT_Temp_H; //��ص�������¶�  main 13
	bms_data.BAT_Temp_H_Number = BAT_Temp_H_Number; 			//��ص�����¶���� 13
	bms_data.BAT_Temp_H_Case = BAT_Temp_H_Case ; 				//��ص�����¶���� 13
	//U8 BAT_Temp_L; //��ص�������¶�  main 14
	bms_data.BAT_Temp_L_Number = BAT_Temp_L_Number; 			//��ص�����¶���� 14
	bms_data.BAT_Temp_L_Case = BAT_Temp_L_Case; 				//��ص�����¶���� 14

	bms_data.Reversed2 = 2;
	if(BAT_Temp_Average>250)
	{
		BAT_Temp_Average = 250;

	}
	bms_data.BAT_Temp_Average = BAT_Temp_Average; 				//���ƽ���¶� 15  �յ����ȥ40
	u16temp = BMS_Ohm;
	if(u16temp>50000)
	{
	  u16temp = 50000;
	}
	byte_order_change(((U8*)&(u16temp)),2); 
	bms_data.BMS_Ohm = u16temp; 								//��Ե��ֵ 16


	if(BAT_Temp_H_Limit>250)
	{
		BAT_Temp_H_Limit = 250;
	}
	bms_data.BAT_Temp_H_Limit = BAT_Temp_H_Limit; 				//��������ص����¶� 2.1  �յ����ȥ40
	if(BAT_Temp_L_Limit>250)
	{
		BAT_Temp_L_Limit = 250;
	}
	bms_data.BAT_Temp_L_Limit = BAT_Temp_L_Limit; 				//��������ص����¶� 2.2  �յ����ȥ40
	if(BMS_SOC_L_Limit>100)
	{
		BMS_SOC_L_Limit = 100;
	}
	bms_data.BMS_SOC_L_Limit = BMS_SOC_L_Limit; 				//�������SOCֵ  2.3
	bms_data.Reversed3 = 3;

	buf1 = BAT_Current_Discharge_Limit;
	if(buf1>64255)
	{
		buf1 = 64255;
	}
	buf1 = buf1/20;

	byte_order_change(((U8*)&(buf1)),2); 
	bms_data.BAT_Current_Discharge_Limit = buf1; 				//�������ŵ���� 2.4  �յ����ȥ1600


	buf1 = BAT_Current_Charge_Limit;
	if(buf1>64255)
	{
		buf1 = 64255;
	}
	buf1 = buf1/20;

	byte_order_change(((U8*)&(buf1)),2); 
	bms_data.BAT_Current_Charge_Limit = buf1; 				//������������ 2.5 �յ����ȥ1600

//IRM01
	u32temp = IRM_Ohm_Positive;
	if(u32temp>100000)
	{
		u32temp=100000;
	}
	byte_order_change(((U8*)&(u32temp)),3); 
	bms_data.IRM_Ohm_Positive = u32temp;    				//������Ե��ֵ 2.6


	
	u32temp = IRM_Ohm_Negative;
	if(u32temp>100000)
	{
		u32temp=100000;
	}
	byte_order_change(((U8*)&(u32temp)),3); 
	bms_data.IRM_Ohm_Negative = u32temp;					//�ܸ���Ե��ֵ 2.7

	bms_data.IRM_Fault_Level = IRM_Fault_Level; 				//�豸���ϵȼ�	2.8
	bms_data.IRM_Insulation_Level = IRM_Insulation_Level; 		//��Ե�ȼ�  2.9
	bms_data.IRM_Life = IRM_Life; 								//��ԵLIFE 2.10
	bms_data.BAT_Voltage_Fault = BAT_Voltage_Fault; 			//��ѹ���� 2.11

	bms_data.BAT_Single_Fault = BAT_Single_Fault; 				//��ѹ���� 2.12
	bms_data.BAT_Temp_Fault = BAT_Temp_Fault; 					//�¶ȹ���  2.13
	bms_data.BAT_Insulation_Fault = BAT_Insulation_Fault; 		//��Ե���� 2.14
	bms_data.BAT_Consistency_Fault = BAT_Consistency_Fault; 	//���һ���Թ��� 2.15


	bms_data.BAT_SOC_Fault = BAT_SOC_Fault; 					//SOC���� 2.16
	bms_data.BAT_Current_Fault = BAT_Current_Fault; 			//�������� 2.17
	bms_data.BAT_Lock_Fault = BAT_Lock_Fault; 					//��ѹ�������� 2.18
	bms_data.BMS_Comm_Fault = BMS_Comm_Fault; 					//BMSͨѶ���� 2.19


	bms_data.BMS_System_Fault = BMS_System_Fault; 				//BMSϵͳ���� 2.20
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

