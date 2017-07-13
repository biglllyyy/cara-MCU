#ifndef  APP_DCDC_H
#define  APP_DCDC_H
#include "comm_typedef.h"

typedef struct
{
	U16  DCDC_Voltage; //DCDC�����ѹ
	U16  DCDC_Current; //DCDC�������

	U8 DCDC_Status; //DCDC����״̬
	U8 DCDC_Output_Cut; //DCDC ����ж�
	U8 DCDC_intput_Cut; //DCDC �����ж�
	U8 DCDC_Fault_Code; //DCDC������

	U8 DCDC_Level_Fault; //DCDC���ϵȼ�
	U8 DCDC_Temp_Warn; //DCDC�¶ȱ���
	U8 DCDC_Fault_Warn; //DCDC���ϱ���
	U8 DCDC_Temp; //DCDCģ���¶�
}DCDC_Data_Struct;

void dcdc_get_data(void);
void dcdc_send_data(void);
#endif
