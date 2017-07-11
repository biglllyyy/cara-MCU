#include "app_tm.h"
#include "app_can.h"
#include "app_uart_cmd.h"
#include "hal_uart.h"
#include "string.h"
#include "crc16.h"
#include "mid_string.h"
#include "uart_cfg.h"


#define TM_DATA_LENGTH 12
#define TM_FRAME_TYPE  MENU_MOTOR_TYPE
static U8 tm_data_t[TM_DATA_LENGTH] ={ 0 };


void tm_get_data(void)
{
	U8 i = 0;
	U16 tempdata = 0;
	//U8 u8tempdata = 0;
	tm_data_t[i++] = TM_Number;
	tm_data_t[i++] = TM_Serial;
	tm_data_t[i++] = TM_WorkStatus;
	tm_data_t[i++] = VCU_Status_Flag1.byte;

	tempdata = TM_Voltage_DC;
	if(tempdata>60000)
	{
		tempdata = 60000;
	}
	tempdata = tempdata/10;
	tm_data_t[i++] = (tempdata)&0xff;
	tm_data_t[i++] = (U8) (tempdata >> 8) & 0xff;

	
	tempdata = TM_Current_DC;
	if(tempdata>20000)
	{
		tempdata = 20000;
	}
	tempdata = tempdata/10;
	tm_data_t[i++] = (tempdata)&0xff;
	tm_data_t[i++] = (U8) (tempdata >> 8) & 0xff;  //收到后减去1000


	tm_data_t[i++] =  VCU_Life; //整车控制器LIFE
	tm_data_t[i++] =  VCU_Code; //整车故障码
	tm_data_t[i++] =  VCU_TM_Brake_percent; //制动踏板开度
	tm_data_t[i++] =  VCU_TM_Speed_percent; //加速踏板开度
	
	
}
void tm_send_data(void)
{

	U16 temp = 0;
	U8 maindata[TM_DATA_LENGTH + 6] =
	{ 0 };

	maindata[0] = FRAME_HEAD; /* load frame head */
	maindata[1] = TM_DATA_LENGTH + 3; /* include data segment and crc segment */
	maindata[2] = TM_FRAME_TYPE;

	memcpy(&maindata[3], &tm_data_t, TM_DATA_LENGTH);

	/*-------------------------CRC check------------------------*/
	temp = api_cal_crc16(&maindata[3], TM_DATA_LENGTH); /* crc check */

	maindata[3 + TM_DATA_LENGTH] = temp & 0xff;
	maindata[3 + TM_DATA_LENGTH + 1] = (temp & 0xff00) >> 8;

	maindata[3 + TM_DATA_LENGTH + 1 + 1] = FRAME_REAR; /* frame rear */

	sent_data(UART_A20_CHN, maindata, (TM_DATA_LENGTH + 6)); /* data sent */

}

