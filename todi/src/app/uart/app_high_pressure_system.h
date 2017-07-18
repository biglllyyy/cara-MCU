#ifndef  APP_HIGH_PRESSURE_SYSTEM_H
#define  APP_HIGH_PRESSURE_SYSTEM_H
#include "comm_typedef.h"
#include "app_can.h"

typedef struct
{

	DATA_DOUBLE ICS01_Status1; //高压采集状态1
	DATA_DOUBLE ICS01_Status2; //高压采集状态2
	DATA_DOUBLE ICS01_Status3; //高压采集状态3
	DATA_DOUBLE ICS01_Status4; //高压采集状态4

	U16  ICS01_Voltage_Front; //前端电压
	U16  ICS01_Voltage_Rear; //后端电压
	
	U8 ICS01_LIFE; //LIFE
	U8 Reversed1;
	U16 Reversed2;
}High_Pressure_System_Struct;

void high_pressure_get_data(void);
void high_pressure_send_data(void);

#endif

