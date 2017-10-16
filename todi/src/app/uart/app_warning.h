#ifndef APP_WARNING_H
#define APP_WARNING_H
#include "comm_typedef.h"

typedef struct
{
    uint8_t 
			ap1_sensor_warn:1,				//��ѹһ����������
			bat_voltage_sensor_warn:1,	   //��ص�ѹ����������
			oil_pressure_sensor_warn:1, 	//����ѹ������������
			oil_quantity_sensor_warn:1, 	   //��������������
			water_temp_sensor_warn:1,			//ˮ�´���������
			low_ap2_warn:1, 				  //����ѹ�ͱ���
			low_ap1_warn:1, 				  //����ѹ�ͱ���
			low_water_level_warn:1;			//ˮλ�ͱ���
                
    uint8_t 
			water_temp_miss:1,			   //ˮ����Ϣ��ʧ
		    after_treat_warn:1, 			//�������
			air_filter_warn:1,				//���˱���
			right_brake_shoe_warn:1,			//��ɲ����Ƭ����
			left_brake_shoe_warn:1, 		//��ɲ����Ƭ����
			low_oil_pressure_warn:1,			//����ѹ����
												  //�����ͱ�������������������ֵ����15��3��ʱ, ��ɫ�����Ƴ�����
												  //ˮ�¸߱�����ˮ���������ߡ����߹���ʱ��˸����,ˮ�¸���98��ʱ������
		    j1939_comm_fault_warn:1,			//J1939ͨ�Ź���
			ap2_sensor_warn:1;			    //��ѹ������������
            
    uint8_t  N :6,
			 bus_comm_fault:1,                //����ͨ�Ź���
			 oil_pressure_data_miss:1;       //����ѹ�����ݶ�ʧ
             


	
	U8 bat_status1;				//���״̬1, 00000000��2������ʾ��
								//-8	 ��ز��������
								//-7	 �ܵ�ѹ��ѹ
								//-6	 �ܵ�ѹǷѹ
								//-5	 �ڲ�ͨѶʧ��
								//-4	 SOC����
								//-3	 SOC����
								//-2	 ����ģ���ѹ����
								//-1	 ����ģ���ѹ����
    U8 bat_status2;				//���״̬2, 00000000��2������ʾ��
							     //-8	�¶Ȳ����
								 //-7	�¶ȱ���
								 //-6	�¶ȹ���
								 //-5	����������
								 //-4	�ŵ��������
								 //-3	��Ե����
								 //-2	DCDC���ϱ�����1���ϣ�0������
								 //-1	�����ʧ��
    U8 bat_status3;				//���״̬3, 00000000��2������ʾ��
							    //-8	����
								//-7	
								//-6	
								//-5	
								//-4	������ƫС���
								//-3	������ƫ����
								//-2	�ڲ��Ӵ����ź�
								//-1	 
} WarningDataStruct;




extern WarningDataStruct warning_data;
void warning_get_data(void);
void warning_send_data(void);
#endif
