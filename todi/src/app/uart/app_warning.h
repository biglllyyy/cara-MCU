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
} WarningDataStruct;



extern WarningDataStruct warning_data;
void warning_get_data(void);
void warning_send_data(void);
#endif
