#include "app_BMS_Tem.h"
#include "app_can.h"
#include "app_uart_cmd.h"
#include "hal_uart.h"
#include "string.h"
#include "crc16.h"
#include "mid_string.h"
#include "uart_cfg.h"
#include "app_info.h"



#define BMS_TEM_DATA_LENGTH 20
#define BMS_TEM_FRAME_TYPE  MENU_BATTEMPERATURE_TYPE

extern U8 CurrentMenu;


static U8 bms_tem_data[BMS_TEM_DATA_LENGTH] = { 0 };

void bms_tem_get_data(void)
{
	U8 i = 0,j=0;
	U16 temp;
	memset(&bms_tem_data[0],0,20);
	if (temp != 0)
	{
		temp--;
	}
	
	for((i=20*temp);i<(20*(temp+1));i++)
	{
		bms_tem_data[j] = BAT_Cell_Temperature[i];
		j++;
	}

}

void bms_tem_send_data(void)
{

	U16 temp = 0;
	U8 maindata[BMS_TEM_DATA_LENGTH + 6] =
	{ 0 };

	maindata[0] = FRAME_HEAD; /* load frame head */
	maindata[1] = BMS_TEM_DATA_LENGTH + 3; /* include data segment and crc segment */
	maindata[2] = BMS_TEM_FRAME_TYPE;

	memcpy(&maindata[3], &bms_tem_data, BMS_TEM_DATA_LENGTH);

	/*-------------------------CRC check------------------------*/
	temp = api_cal_crc16(&maindata[3], BMS_TEM_DATA_LENGTH); /* crc check */

	maindata[3 + BMS_TEM_DATA_LENGTH] = temp & 0xff;
	maindata[3 + BMS_TEM_DATA_LENGTH + 1] = (temp & 0xff00) >> 8;

	maindata[3 + BMS_TEM_DATA_LENGTH + 1 + 1] = FRAME_REAR; /* frame rear */

	sent_data(UART_A20_CHN, maindata, (BMS_TEM_DATA_LENGTH + 6)); /* data sent */

}

