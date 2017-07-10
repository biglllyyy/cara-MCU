/**************************
 
 ��λ��Ϣ����
 
 *************************/
#include "app_gear.h"
#include <string.h>
#include "app_IPconfig.h"
#include "app_uart_cmd.h"
#include "app_can.h"
#include "io_cfg.h"
#include "mid_pin.h"
#include "app_io.h"

void app_gear_init(void)
{
	uart_general_data.generalInfo.gear_mode=0;
	uart_general_data.generalInfo.gear=0;
}

void app_gear_pro_100ms(void)
{/*
	switch(IPconfig.protocol)
	{
		case HAVAL_2013_2P4L_AT_2WD:
			uart_general_data.generalInfo.gear_mode=1;//��ʾ��λ��Ϣ
			if(can_id_131_lost_timecnt<CAN_LOST_TIME)
			{
	 			if(can0.id_131.TCU_GearLeverPos==8)//�ֶ�ģʽ
	 			{
	 				if(can0.id_131.TCU_GearSts<=4)
	 				{
	 					uart_general_data.generalInfo.gear=can0.id_131.TCU_GearSts+9;
	 				}
	 				else
	 				{
	 					uart_general_data.generalInfo.gear=9;
	 				}
	 			}
	 			else//�Զ�ģʽ
	 			{
	 				if(can0.id_131.TCU_GearLeverPos<8)
	 				{
	 					uart_general_data.generalInfo.gear=can0.id_131.TCU_GearLeverPos;
	 				}
	 				else
	 				{
	 					uart_general_data.generalInfo.gear=0;
	 				}
	 			}   
	 		}
	 		else
	 		{
	 			uart_general_data.generalInfo.gear=0;//P��
	 		}
			break;
		case HAVAL_2013_1P5T_MT_2WD:
			uart_general_data.generalInfo.gear_mode=0;//����ʾ��λ��Ϣ
					
			if(mid_get_io_sts(&pin_io_in[PIN_IN_REVERSE_SIGNAL])==ON)//����Ч
			{
				uart_general_data.generalInfo.gear=7;
			}
			else
			{
				uart_general_data.generalInfo.gear=1;//�˴����ܷ���0����Ϊ0��Ϊ��P����������ʾ�״��ź�
			}
			break;
		case HAVAL_2012_1P5T_MT_2WD:
			uart_general_data.generalInfo.gear_mode=0;//����ʾ��λ��Ϣ
					
			if(mid_get_io_sts(&pin_io_in[PIN_IN_REVERSE_SIGNAL])==ON)//����Ч
			{
				uart_general_data.generalInfo.gear=7;
			}
			else
			{
				uart_general_data.generalInfo.gear=1;//�˴����ܷ���0����Ϊ0��Ϊ��P����������ʾ�״��ź�
			}
			break;
		case HAVAL_2015_1P5T_MT_2WD: 
			uart_general_data.generalInfo.gear_mode=0;//����ʾ��λ��Ϣ
			if(mid_get_io_sts(&pin_io_in[PIN_IN_REVERSE_SIGNAL])==ON)//����Ч
			{
				uart_general_data.generalInfo.gear=7;
			}
			else
			{
				uart_general_data.generalInfo.gear=1;//�˴����ܷ���0����Ϊ0��Ϊ��P����������ʾ�״��ź�
			}
			break;
			
		case HAVAL_2015_1P5T_6AT_2WD:
			uart_general_data.generalInfo.gear_mode=1;//��ʾ��λ��Ϣ
			if(can_id_221_lost_timecnt<CAN_LOST_TIME)
			{
				switch(can3.id_221.gear&0x0f)
				{
					case 0://P
						uart_general_data.generalInfo.gear=0;	
						break;
					case 5://D
						uart_general_data.generalInfo.gear=1;
						break;
					case 6://N
						uart_general_data.generalInfo.gear=6;
						break;
					case 7://R
						uart_general_data.generalInfo.gear=7;
						break;
					case 8://M
						switch(can3.id_221.gear)
						{
							case 0x28: M1
								uart_general_data.generalInfo.gear=10;
								break;
							case 0x48: M2
								uart_general_data.generalInfo.gear=11;
								break;
							case 0x68: M3
								uart_general_data.generalInfo.gear=12;
								break;
							case 0x88: M4
								uart_general_data.generalInfo.gear=13;
								break;
							case 0xa8: M5
								uart_general_data.generalInfo.gear=14;
								break;
							case 0xC8: M6
								uart_general_data.generalInfo.gear=15;
								break;
						}
						break;
				}
			}
			else
			{
				uart_general_data.generalInfo.gear=0;	
			}
			break;
		case HAVAL_2011_2P0T_MT_2WD:
			uart_general_data.generalInfo.gear_mode=0;//����ʾ��λ��Ϣ
					
			if(mid_get_io_sts(&pin_io_in[PIN_IN_REVERSE_SIGNAL])==ON)//����Ч
			{
				uart_general_data.generalInfo.gear=7;
			}
			else
			{
				uart_general_data.generalInfo.gear=1;//�˴����ܷ���0����Ϊ0��Ϊ��P����������ʾ�״��ź�
			}
			break;
		case HAVAL_2013_1P5T_MT_4WD:
			uart_general_data.generalInfo.gear_mode=0;//����ʾ��λ��Ϣ
					
			if(mid_get_io_sts(&pin_io_in[PIN_IN_REVERSE_SIGNAL])==ON)//����Ч
			{
				uart_general_data.generalInfo.gear=7;
			}
			else
			{
				uart_general_data.generalInfo.gear=1;//�˴����ܷ���0����Ϊ0��Ϊ��P����������ʾ�״��ź�
			}
			break;
	}
*/}
