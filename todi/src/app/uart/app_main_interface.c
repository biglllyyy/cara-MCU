#include "app_main_interface.h"
#include "app_can.h"
#include "app_uart_cmd.h"
#include "hal_uart.h"
#include "string.h"
#include "crc16.h"
#include "mid_string.h"
#include "app_can.h"
#include "app_moudle.h"
#include "app_info.h"
#include "mid_can.h"
#include "mb91520.h"
#include "uart_cfg.h"
#include "app_trip.h"
#include "g_variable.h"
#include "mid_filter.h"



Main_Interface_Data_Struct main_interface_data =
{ 0 };

#define MAIN_INTERFACE_DATA_LENGTH (sizeof(Main_Interface_Data_Struct))
#define MAIN_INTERFACE_FRAME_TYPE  CAR_INFO_TYPE

extern U16 ad_data;
U32 ad_data_group[4];

void main_interface_get_data(void)
{
	U16 buf1, buf2;
	U32 temp = 0;
	U16 temp1 = 0;
	U8  temp2 = 0;
	U8  tempSpeed = 0;
	//����
	main_interface_data.key.u8KeyState = key_info_value; /* ����״̬ */

	//����


	tempSpeed = pSpeed;  //���ٵ�λ1KM/H
	if (tempSpeed > 140)
	{
		tempSpeed = 140;
	}
	main_interface_data.speed = tempSpeed; /* ����0-140km/h */


	//���ת��
	temp1 = moto_speed;
	WORD_WRITE(main_interface_data.rpm,temp1);
	//�ܵ�ѹ
	buf1 = BMS_V;  //�ܵ�ѹ
	WORD_WRITE(main_interface_data.totalVoltage,buf1);

	//�ܵ���
	buf1 = BMS_A;
	WORD_WRITE(main_interface_data.totalCurrent,buf1);
	//��λ
	main_interface_data.gear = Current_Gear;  /*0000:�յ� 0001: 1 �� 0010: 2 �� 0011: 3 �� 0100: 4 �� 0101: 5 �� 0110: 6 �� 0111: 7����*/
	//�����
	temp = (info.Odo); 
	DWORD_WRITE(main_interface_data.odo,temp);

	
	//С�����
	buf1 = info.Trip1;
	WORD_WRITE(main_interface_data.trip,buf1);

	//SOC
	temp2 = BMS_SOC;
	if(temp2>250)
	{
		temp2 = 250;
	}
	main_interface_data.soc = temp2; /* Soc �ٷ�֮0-100  */


	//��ѹ ��

	main_interface_data.airPressure1 = PRESS[0]; /* ��ѹ1    */
	main_interface_data.airPressure1 = PRESS[1]; /* ��ѹ1	 */

	


	//����������¶�
	{
		main_interface_data.moter_control_temp = Inverter_t;
	}


	//����¶�
	{
		main_interface_data.moter_temp = Motor_Temperature;
	}

	//���زɼ�
	{
		//!<��Ҫ��ѯӲ���˿�

 		main_interface_data.control_IN01 = MCU_IN4;  //��ת��
		main_interface_data.control_IN02 = 0;        //����    
		main_interface_data.control_IN03 = 0;        //����    
		main_interface_data.control_IN04 = MCU_IN1 ; //Կ��ST��
		main_interface_data.control_IN05 = MCU_IN5 ; //��ת��
		main_interface_data.control_IN06 = MCU_IN6 ; //С�ƿ���
		main_interface_data.control_IN07 = MCU_IN7 ; //Զ��ƿ�
		main_interface_data.control_IN08 = MCU_IN8 ; //����ƿ�
		
		main_interface_data.control_IN09 = MCU_IN9 ; //ǰ��ƿ�
		main_interface_data.control_IN10 = MCU_IN10; //����ƿ�
		main_interface_data.control_IN11 = 0;        //����    
		main_interface_data.control_IN12 = MCU_IN12; //���ƿ�
		main_interface_data.control_IN13 = 0;        //����    
		main_interface_data.control_IN14 = MCU_IN14; //��ε͵�
		main_interface_data.control_IN15 = MCU_IN15; //��μ�Ъ
		main_interface_data.control_IN16 = MCU_IN16; //��θߵ�
		
		main_interface_data.control_IN17 = 0;        //����    
		main_interface_data.control_IN18 = 0;        //����    
		main_interface_data.control_IN19 = MCU_IN23; //·�ƿ���
		main_interface_data.control_IN20 = MCU_IN24; //ǰ�ſ���
		main_interface_data.control_IN21 = MCU_IN17; //�����ˮ
		main_interface_data.control_IN22 = 0;        //����    
		main_interface_data.control_IN23 = 0;        //����    
		main_interface_data.control_IN24 = 0;        //���� 
		
		main_interface_data.control_IN25 = 0;        //����    
		main_interface_data.control_IN26 = MCU_IN26; //���Źؿ�
		main_interface_data.control_IN27 = MCU_IN27; //���ſ���
		main_interface_data.control_IN28 = MCU_IN25; //ǰ�Źؿ�
		main_interface_data.control_IN29 = 0;        //ȼ������
		main_interface_data.control_IN30 = 0;        //����    
		main_interface_data.control_IN31 = 0;        //����    
		main_interface_data.control_IN32 = 0;        //����    
		
		main_interface_data.control_IN33 = 0;        //Σ�վ���
		main_interface_data.control_IN34 = 0;        //��Դ����
		main_interface_data.control_IN35 = 0;        //Կ��ON��	
		
	}
	main_interface_data.diagnostic_mode = Diagnosis;
	main_interface_data.climbing_mode = Mode_S;
	main_interface_data.HV_overhaul = High_Voltage;
	main_interface_data.main_power = M_ON;//!<����ź�
	main_interface_data.defroster = 0;//!<δ����
	main_interface_data.back_door = rKL15;                                                                 
	main_interface_data.park_brake=;		   //פ���ƶ�����Ƭ                                                                   
	main_interface_data.brake_light=;		   //ɲ���Ʒ���Ƭ                                                                                                                                                                                                                                                         
	main_interface_data.ABS=;				       //ABS����Ƭ                                                                          
	main_interface_data.dryer=;			       //����������Ƭ                                                                         
	main_interface_data.Kt=Battery_Kt;				       //������Ӵ���                                                                         
	main_interface_data.power_mode=Electric;		   //Powerģʽ����Ƭ(���綯ģʽ or �춯ģʽ or ���ģʽ)                              
	main_interface_data.charge_confirm=Charge_Check;	//���ȷ�Ϸ���Ƭ                                                                 
	main_interface_data.AC=AC_SWITCH;				      //AC�յ�����Ƭ
	temp = TCU_level;
	DWORD_WRITE(main_interface_data.fault_level,temp);
	temp = TCU_code;
	DWORD_WRITE(main_interface_data.fault_code,temp);
	temp = tcu_spn;
	DWORD_WRITE(main_interface_data.SPN,temp);
	temp = Niaosu;
	DWORD_WRITE(main_interface_data.urea_level,temp);
	temp = g_u32_utcTime;
	DWORD_WRITE(main_interface_data.dateTime,temp);
	temp = 0;
	DWORD_WRITE(main_interface_data.battery,temp);
}
void main_interface_send_data(void)
{
	U8 data[MAIN_INTERFACE_DATA_LENGTH + 6];
	U32 parse_len;
    parse_len = app_uart_arm_send_parse(data,(void*)&main_interface_data,CAR_INFO_TYPE,MAIN_INTERFACE_DATA_LENGTH);
	sent_data(UART_A20_CHN, data, parse_len); /* data sent */
}
