#ifndef APP_WARNING_H
#define APP_WARNING_H
#include "comm_typedef.h"

typedef struct
{
    U8 low_water_level_warn;			//ˮλ�ͱ���
    U8 low_ap1_warn;                    //����ѹ�ͱ���
    U8 low_ap2_warn;                    //����ѹ�ͱ���
    U8 water_temp_sensor_warn;			//ˮ�´���������
    U8 oil_quantity_sensor_warn;		//��������������
    U8 oil_pressure_sensor_warn;		//����ѹ������������
    U8 bat_voltage_sensor_warn;		    //��ص�ѹ����������
    U8 ap1_sensor_warn;				    //��ѹһ����������
    U8 ap2_sensor_warn;				    //��ѹ������������
    U8 j1939_comm_fault_warn;			//J1939ͨ�Ź���
    U8 low_oil_pressure_warn;			//����ѹ����
                                            //�����ͱ�������������������ֵ����15��3��ʱ, ��ɫ�����Ƴ�����
                                            //ˮ�¸߱�����ˮ���������ߡ����߹���ʱ��˸����,ˮ�¸���98��ʱ������
    U8 left_brake_shoe_warn;			//��ɲ����Ƭ����
    U8 right_brake_shoe_warn;			//��ɲ����Ƭ����
    U8 air_filter_warn;				    //���˱���
    U8 after_treat_warn;				//�������
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
