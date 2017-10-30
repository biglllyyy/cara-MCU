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
#include "app_ad_capture.h"
#include "io_cfg.h"
#include "mid_switch.h"


Main_Interface_Data_Struct main_interface_data =
{ 0 };

#define MAIN_INTERFACE_DATA_LENGTH (sizeof(Main_Interface_Data_Struct))
#define MAIN_INTERFACE_FRAME_TYPE  CAR_INFO_TYPE

extern U16 ad_data;
U32 ad_data_group[4];

extern pin_filter_t    pin_filter_in[];

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
	Speed_rmp = vcu_msg.ID_0C008980_byte78[0] + vcu_msg.ID_0C008980_byte78[1]*256;

	temp1 = (unsigned int) (Speed_rmp * 0.446 * 2 * 60 * 3.14159 / 1000 / 22.66 * 1.05); //�뾶*2*��*60/1000/���ٱ�

//	tempSpeed = pSpeed;  //���ٵ�λ1KM/H
	if (temp1 > 140)
	{
		temp1 = 140;
	}
	pSpeed = temp1;
	main_interface_data.speed = temp1; /* ����0-140km/h */

	//���ת��
	temp1 = vcu_msg.ID_0C008980_byte78[0] + vcu_msg.ID_0C008980_byte78[1]*256;
	if (temp1 > 4000)
	{
		temp1 = 4000;
	}
	WORD_WRITE(main_interface_data.rpm,temp1);
	
	//�ܵ�ѹ
	buf1 = bms_msg.ID_180689F4_byte12[0]+bms_msg.ID_180689F4_byte12[1]*256;  //�ܵ�ѹ
	WORD_WRITE(main_interface_data.totalVoltage,buf1);

	//�ܵ���
	buf1 = bms_msg.ID_180689F4_byte34[0]+bms_msg.ID_180689F4_byte34[1]*256;  //�ܵ���
	WORD_WRITE(main_interface_data.totalCurrent,buf1);

	//��λ
	main_interface_data.gear = vcu_msg.ID_0C018980_byte4.bit02;	//01ǰ�� 02�յ� ��04���� 1���Ǳ���Ƶ������� 0���Ǳ���Ƶ�������

	//�����
	temp = (info.Odo); 
	DWORD_WRITE(main_interface_data.odo,temp);

	//С�����
	buf1 = info.Trip1;
	WORD_WRITE(main_interface_data.trip,buf1);

	//SOC
	temp2 = bms_msg.ID_180689F4_byte5;
	if(temp2>250)
	{
		temp2 = 250;
	}
	main_interface_data.soc = temp2; /* Soc �ٷ�֮0-100  */

	WORD_WRITE(main_interface_data.airPressure1,fADR[0]);
	WORD_WRITE(main_interface_data.airPressure2,fADR[1]);

	//����������¶�
	{
		temp = vcu_msg.ID_0C008980_byte4;
		WORD_WRITE(main_interface_data.moter_control_temp,temp);
	}

	//����¶�
	{
		temp = vcu_msg.ID_0C008980_byte3;
		WORD_WRITE(main_interface_data.moter_temp,temp);
	}


	//���زɼ�
	{
		//!<��Ҫ��ѯӲ���˿�
#if 0
		main_interface_data.control_IN01 = MCU_IN1;  //��ת�򿪹�
		main_interface_data.control_IN02 = 0;        //����
		main_interface_data.control_IN03 = 0;        //����
		main_interface_data.control_IN04 = MCU_IN4 ; //Կ��ON����
		main_interface_data.control_IN05 = MCU_IN5 ; //��ת�򿪹�
		main_interface_data.control_IN06 = MCU_IN6 ; //С�ƿ���
		main_interface_data.control_IN07 = MCU_IN7 ; //Զ��ƿ�
		main_interface_data.control_IN08 = MCU_IN8 ; //����ƿ�
		
		main_interface_data.control_IN09 = MCU_IN9 ; //ǰ��ƿ���
		main_interface_data.control_IN10 = MCU_IN10; //����ƿ���
		main_interface_data.control_IN11 = 0;        //����
		main_interface_data.control_IN12 = 0;		 //����
		main_interface_data.control_IN13 = 0;        //����
		main_interface_data.control_IN14 = MCU_IN14; //��ε͵�����
		main_interface_data.control_IN15 = MCU_IN15; //��μ�Ъ������
		main_interface_data.control_IN16 = MCU_IN16; //��θߵ�����
		                                                                                                                               
		main_interface_data.control_IN17 = 0;		 //����
		main_interface_data.control_IN18 = 0;        //����
		main_interface_data.control_IN19 = 0; 		 //����
		main_interface_data.control_IN20 = MCU_IN20; //ǰ�ſ�����
		main_interface_data.control_IN21 = MCU_IN21; //�����ˮ����
		main_interface_data.control_IN22 = 0;        //����
		main_interface_data.control_IN23 = 0;        //����
		main_interface_data.control_IN24 = 0;		 //����

		main_interface_data.control_IN25 = 0; 		 //����
		main_interface_data.control_IN26 = MCU_IN26; //���Źؿ���
		main_interface_data.control_IN27 = MCU_IN27; //���ſ�����
		main_interface_data.control_IN28 = MCU_IN28; //ǰ�Źؿ���
		main_interface_data.control_IN29 = 0;		 //����
		main_interface_data.control_IN30 = 0;        //����
		main_interface_data.control_IN31 = 0;        //����
		main_interface_data.control_IN32 = 0;        //����
		
		main_interface_data.control_IN33 = 0;        //����
		main_interface_data.control_IN34 = 0;        //����
		main_interface_data.control_IN35 = 0;	     //����


#endif
 		main_interface_data.control_IN01 = MCU_IN1;  //��ת�򿪹�
		main_interface_data.control_IN02 = 0;        //����
		main_interface_data.control_IN03 = 0;        //����
		main_interface_data.control_IN04 = MCU_IN4 ; //Կ��ON����
		main_interface_data.control_IN05 = MCU_IN5 ; //��ת�򿪹�
		main_interface_data.control_IN06 = MCU_IN6 ; //С�ƿ���
		main_interface_data.control_IN07 = MCU_IN7 ; //Զ��ƿ�
		main_interface_data.control_IN08 = MCU_IN8 ; //����ƿ�
		
		main_interface_data.control_IN09 = MCU_IN9 ; //ǰ��ƿ���
		main_interface_data.control_IN10 = MCU_IN10; //����ƿ���
		main_interface_data.control_IN11 = 1;        //����
		main_interface_data.control_IN12 = 1;		 //����
		main_interface_data.control_IN13 = 1;        //����
		main_interface_data.control_IN14 = MCU_IN14; //��ε͵�����
		main_interface_data.control_IN15 = MCU_IN15; //��μ�Ъ������
		main_interface_data.control_IN16 = MCU_IN16; //��θߵ�����
		                                                                                                                               
		main_interface_data.control_IN17 = 1; 		 //����
		main_interface_data.control_IN18 = 1;        //����
		main_interface_data.control_IN19 = 1; 		 //����
		main_interface_data.control_IN20 = MCU_IN20; //ǰ�ſ�����
		main_interface_data.control_IN21 = MCU_IN21; //�����ˮ����
		main_interface_data.control_IN22 = 1;        //����
		main_interface_data.control_IN23 = 1;        //����
		main_interface_data.control_IN24 = 1; 		 //����

		main_interface_data.control_IN25 = 1; 		 //����
		main_interface_data.control_IN26 = MCU_IN26; //���Źؿ���
		main_interface_data.control_IN27 = MCU_IN27; //���ſ�����
		main_interface_data.control_IN28 = MCU_IN28; //ǰ�Źؿ���
		main_interface_data.control_IN29 = 1;		 //����
		main_interface_data.control_IN30 = 1;        //����
		main_interface_data.control_IN31 = 1;        //����
		main_interface_data.control_IN32 = 1;        //����
		
		main_interface_data.control_IN33 = 1;        //����
		main_interface_data.control_IN34 = 1;        //����
		main_interface_data.control_IN35 = 1;	     //����

	}
	temp1 = vcu_msg.ID_0C018980_byte67[0]+vcu_msg.ID_0C018980_byte67[1]*256;
	WORD_WRITE(main_interface_data.systemCode,temp1);
	memcpy(&main_interface_data.charge_status , &bms_msg.ID_180789F4_byte7,1);
	temp = g_u32_utcTime;
	DWORD_WRITE(main_interface_data.dateTime,temp);
	{
		temp = ADV[MCU_VBAT];
		temp = temp * 169 /10000;
		if (temp<16)
		{
			temp = 16;
		}
		else if (temp>32)
		{
			temp = 32;
		}
//		dbg_printf("battery = %d,%d\n",temp,ADV[MCU_VBAT]);
		DWORD_WRITE(main_interface_data.battery,temp);
	}
	main_interface_data.version = 100; //�汾��1.00

	{
		main_interface_data.front_moudle.switch23 = fKH1;
		main_interface_data.front_moudle.switch24 = fKL1;
		main_interface_data.front_moudle.switch25 = fKL2;
		main_interface_data.front_moudle.switch26 = fKL3;

		main_interface_data.front_moudle.switch27 = fKL4;
		main_interface_data.front_moudle.switch28 = fKL5;
		main_interface_data.front_moudle.switch29 = fKL6;
		main_interface_data.front_moudle.switch30 = fKL7;

		main_interface_data.front_moudle.switch31 = fKL8;
		main_interface_data.front_moudle.switch32 = fKL9;
		main_interface_data.front_moudle.switch33 = fKL10;
		main_interface_data.front_moudle.switch34 = fKL11;
		
		main_interface_data.front_moudle.switch35 = fKL12;
		main_interface_data.front_moudle.switch36 = fKL13;
		main_interface_data.front_moudle.switch37 = fKL14;
		main_interface_data.front_moudle.switch38 = fKL15;

		main_interface_data.front_moudle.switch39 = fADD;

		memcpy(&main_interface_data.front_moudle.out1_8.byte,&fPOUT.BYTES[0],1);
		memcpy(&main_interface_data.front_moudle.out9_16.byte,&fPOUT.BYTES[1],1);
		main_interface_data.front_moudle.version = Fversion;
	}

	{
		main_interface_data.middle_moudle.switch23 = mKH1;
		main_interface_data.middle_moudle.switch24 = mKL1;
		main_interface_data.middle_moudle.switch25 = mKL2;
		main_interface_data.middle_moudle.switch26 = mKL3;
		
		main_interface_data.middle_moudle.switch27 = mKL4;
		main_interface_data.middle_moudle.switch28 = mKL5;
		main_interface_data.middle_moudle.switch29 = mKL6;
		main_interface_data.middle_moudle.switch30 = mKL7;

		main_interface_data.middle_moudle.switch31 = mKL8;
		main_interface_data.middle_moudle.switch32 = mKL9;
		main_interface_data.middle_moudle.switch33 = mKL10;
		main_interface_data.middle_moudle.switch34 = mKL11;

		main_interface_data.middle_moudle.switch35 = mKL12;
		main_interface_data.middle_moudle.switch36 = mKL13;
		main_interface_data.middle_moudle.switch37 = mKL14;
		main_interface_data.middle_moudle.switch38 = mKL15;

		main_interface_data.middle_moudle.switch39 = mADD;

		memcpy(&main_interface_data.middle_moudle.out1_8.byte,&mPOUT.BYTES[0],1);
		memcpy(&main_interface_data.middle_moudle.out9_16.byte,&mPOUT.BYTES[1],1);
		main_interface_data.middle_moudle.version = Mversion;
	}

	{
		main_interface_data.back_moudle.switch23 = rKH1;
		main_interface_data.back_moudle.switch24 = rKL1;
		main_interface_data.back_moudle.switch25 = rKL2;
		main_interface_data.back_moudle.switch26 = rKL3;

		main_interface_data.back_moudle.switch27 = rKL4;
		main_interface_data.back_moudle.switch28 = rKL5;
		main_interface_data.back_moudle.switch29 = rKL6;
		main_interface_data.back_moudle.switch30 = rKL7;

		main_interface_data.back_moudle.switch31 = rKL8;
		main_interface_data.back_moudle.switch32 = rKL9;
		main_interface_data.back_moudle.switch33 = rKL10;
		main_interface_data.back_moudle.switch34 = rKL11;
		
		main_interface_data.back_moudle.switch35 = rKL12;
		main_interface_data.back_moudle.switch36 = rKL13;
		main_interface_data.back_moudle.switch37 = rKL14;
		main_interface_data.back_moudle.switch38 = rKL15;

		main_interface_data.back_moudle.switch38 = rADD;

		memcpy(&main_interface_data.back_moudle.out1_8.byte,&rPOUT.BYTES[0],1);
		memcpy(&main_interface_data.back_moudle.out9_16.byte,&rPOUT.BYTES[1],1);
		main_interface_data.back_moudle.version = Rversion;
	}
	

}
void main_interface_send_data(void)
{
	U8 data[MAIN_INTERFACE_DATA_LENGTH + A20_MCU_DATA_LENTH];
	U32 parse_len;
    parse_len = app_uart_arm_send_parse(data,(void*)&main_interface_data,MAIN_INTERFACE_FRAME_TYPE,MAIN_INTERFACE_DATA_LENGTH);
	sent_data(UART_A20_CHN, data, parse_len); /* data sent */
}
