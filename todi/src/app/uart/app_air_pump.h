#ifndef APP_AIR_PUMP_H
#define APP_AIR_PUMP_H

#include "comm_typedef.h"
typedef struct
{
	U8 AIRDCAC_Status; //DCAC����״̬
	U8 AIRDCAC_Output_Cut; //DCAC ����ж�
	U8 AIRDCAC_intput_Cut; //DCAC �����ж�
	U8 AIRDCAC_Fault_Code; //DCAC���ϴ���

	U8 AIRDCAC_Level_Fault; //DCAC���ϵȼ�
	U8 AIRDCAC_Temp_Warn; //DCDC�¶ȱ���
	U8 AIRDCAC_Fault_Warn; //DCDC���ϱ���
	U8 AIRDCAC_Temp; //DCACģ���¶�

	U16 AIRDCAC_V_Voltage; //DCAC���V��ѹ
	U16 AIRDCAC_V_Current; //DCAC���V����

	U16 AIRDCAC_W_Voltage; //DCAC���W��ѹ
	U16 AIRDCAC_W_Current; //DCAC���W����

	U16 AIRDCAC_U_Voltage; //DCAC���U��ѹ
	U16 AIRDCAC_U_Current; //DCAC���U����

} Air_Pump_Data_Struct;

void air_pump_get_data(void);
void air_pump_send_data(void);

#endif
