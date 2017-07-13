#ifndef APP_OIL_PUMP_H
#define APP_OIL_PUMP_H
#include "comm_typedef.h"

typedef struct
{
	U8 OILDCAC_Status; //DCAC����״̬
	U8 OILDCAC_Output_Cut; //DCAC ����ж�
	U8 OILDCAC_intput_Cut; //DCAC �����ж�
	U8 OILDCAC_Fault_Code; //DCAC���ϴ���

	U8 OILDCAC_Level_Fault; //DCAC���ϵȼ�
	U8 OILDCAC_Temp_Warn; //DCDC�¶ȱ���
	U8 OILDCAC_Fault_Warn; //DCDC���ϱ���
	U8 OILDCAC_Temp; //DCACģ���¶�

	U16 OILDCAC_V_Voltage; //DCAC���V��ѹ
	U16 OILDCAC_V_Current; //DCAC���V����

	U16 OILDCAC_W_Voltage; //DCAC���W��ѹ
	U16 OILDCAC_W_Current; //DCAC���W����

	U16 OILDCAC_U_Voltage; //DCAC���U��ѹ
	U16 OILDCAC_U_Current; //DCAC���U����

} Oil_Pump_Data_Struct;

void oil_pump_get_data(void);
void oil_pump_send_data(void);
#endif
