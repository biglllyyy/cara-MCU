#include "app_high_pressure_system.h"
#include "app_can.h"
#include "app_uart_cmd.h"
#include "hal_uart.h"
#include "string.h"
#include "crc16.h"
#include "mid_string.h"
#include "uart_cfg.h"


#define HIGH_PRESSURE_DATA_LENGTH  12
#define HIGH_PRESSURE_FRAME_TYPE  MENU_HIGH_PRESSURE

static High_Pressure_System_Struct high_pressure_data = { 0 };


void high_pressure_get_data(void)
{
	U16 u16temp = 0;
	high_pressure_data.ICS01_Status1.byte = ICS01_Status1.byte;
	high_pressure_data.ICS01_Status2.byte = ICS01_Status2.byte;
	high_pressure_data.ICS01_Status3.byte = ICS01_Status3.byte;
	high_pressure_data.ICS01_Status4.byte = ICS01_Status4.byte;
	
	u16temp = ICS01_Voltage_Front;
	if(u16temp>1000)
	{
		u16temp = 1000;
	}
	byte_order_change(((U8*)&(u16temp)),2); 
	high_pressure_data.ICS01_Voltage_Front = u16temp;

	
	u16temp = ICS01_Voltage_Rear;
	if(u16temp>1000)
	{
		u16temp = 1000;
	}
	byte_order_change(((U8*)&(u16temp)),2); 
	high_pressure_data.ICS01_Voltage_Rear = u16temp;

	high_pressure_data.ICS01_LIFE = ICS01_LIFE;
}
void high_pressure_send_data(void)
{

	U16 temp = 0;
	U8 maindata[HIGH_PRESSURE_DATA_LENGTH + 6] = { 0 };

	maindata[0] = FRAME_HEAD; /* load frame head */
	maindata[1] = HIGH_PRESSURE_DATA_LENGTH + 3; /* include data segment and crc segment */
	maindata[2] = HIGH_PRESSURE_FRAME_TYPE;

	memcpy(&maindata[3], &high_pressure_data, HIGH_PRESSURE_DATA_LENGTH);

	/*-------------------------CRC check------------------------*/
	temp = api_cal_crc16(&maindata[3], HIGH_PRESSURE_DATA_LENGTH); /* crc check */

	maindata[3 + HIGH_PRESSURE_DATA_LENGTH] = temp & 0xff;
	maindata[3 + HIGH_PRESSURE_DATA_LENGTH + 1] = (temp & 0xff00) >> 8;

	maindata[3 + HIGH_PRESSURE_DATA_LENGTH + 1 + 1] = FRAME_REAR; /* frame rear */

	sent_data(UART_A20_CHN, maindata, (HIGH_PRESSURE_DATA_LENGTH + 6)); /* data sent */

}




