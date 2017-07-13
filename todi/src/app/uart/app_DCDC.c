#include "app_DCDC.h"
#include "app_can.h"
#include "app_uart_cmd.h"
#include "hal_uart.h"
#include "string.h"
#include "crc16.h"
#include "mid_string.h"
#include "uart_cfg.h"




#define DCDC_DATA_LENGTH  12
#define DCDC_FRAME_TYPE  MENU_DCDC_TYPE
static U8 dcdc_data[DCDC_DATA_LENGTH] = {0};


void dcdc_get_data(void)
{
	U8 i = 0;
	U16 u16tempdata = 0;
	

	u16tempdata = DCDC_Voltage; //DCDC输出电压
	if (u16tempdata > 10000)
	{
		u16tempdata = 10000;
	}
	u16tempdata = u16tempdata / 10;
	dcdc_data[i++] = (u16tempdata & 0xff);
	dcdc_data[i++] = (U8) (u16tempdata >> 8) & 0xff;

	u16tempdata = DCDC_Current; //DCDC输出电流
	if (u16tempdata > 5000)
	{
		u16tempdata = 5000;
	}
	u16tempdata = u16tempdata / 10;
	dcdc_data[i++] = (u16tempdata & 0xff);
	dcdc_data[i++] = (U8) (u16tempdata >> 8) & 0xff;

	dcdc_data[i++] =  DCDC_Status; //DCDC工作状态
	dcdc_data[i++] =  DCDC_Output_Cut; //DCDC 输出切断
	dcdc_data[i++] =  DCDC_intput_Cut; //DCDC 输入切断
	dcdc_data[i++] =  DCDC_Fault_Code; //DCDC故障码

	dcdc_data[i++] =  DCDC_Level_Fault; //DCDC故障等级
	dcdc_data[i++] =  DCDC_Temp_Warn; //DCDC温度报警
	dcdc_data[i++] =  DCDC_Fault_Warn; //DCDC故障报警
	dcdc_data[i++] =  DCDC_Temp; //DCDC模块温度

}
void dcdc_send_data(void)
{
	U16 temp = 0;
	U8 maindata[DCDC_DATA_LENGTH + 6] =
	{ 0 };

	maindata[0] = FRAME_HEAD; /* load frame head */
	maindata[1] = DCDC_DATA_LENGTH + 3; /* include data segment and crc segment */
	maindata[2] = DCDC_FRAME_TYPE;

	memcpy(&maindata[3], &dcdc_data, DCDC_DATA_LENGTH);

	/*-------------------------CRC check------------------------*/
	temp = api_cal_crc16(&maindata[3], DCDC_DATA_LENGTH); /* crc check */

	maindata[3 + DCDC_DATA_LENGTH] = temp & 0xff;
	maindata[3 + DCDC_DATA_LENGTH + 1] = (temp & 0xff00) >> 8;

	maindata[3 + DCDC_DATA_LENGTH + 1 + 1] = FRAME_REAR; /* frame rear */

	sent_data(UART_A20_CHN, maindata, (DCDC_DATA_LENGTH + 6)); /* data sent */

}
