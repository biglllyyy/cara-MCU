#ifndef  APP_BMS_VOL_H
#define  APP_BMS_VOL_H
#include "comm_typedef.h"

typedef struct
{
	U16  vol_value1; //һ��ҳ��50��U16�ĵ�ѹ ��λ��1mV/Bit 
	U16  vol_value2;
	//..............
	U16  vol_value50;
}Bms_Vol_Struct;

void bms_vol_get_data(void);
void bms_vol_send_data(void);
#endif
