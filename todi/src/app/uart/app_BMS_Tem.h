#ifndef  APP_BMS_TEM_H
#define  APP_BMS_TEM_H
#include "comm_typedef.h"

typedef struct
{
	U8  temp_value1; //һ��ҳ��16��U8��wendu ��λ��1mV/Bit   yigong 8 ye
	U8  temp_value2;
	//..............
	U8  remp_value16;
}Bms_Temp_Struct;


void bms_tem_get_data(void);
void bms_tem_send_data(void);
#endif
