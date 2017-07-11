#include "app_air_pump.h"
#include "app_can.h"
#include "app_uart_cmd.h"
#include "hal_uart.h"
#include "string.h"
#include "crc16.h"
#include "mid_string.h"
#include "uart_cfg.h"

#define AIR_PUMP_DATA_LENGTH 20
#define AIR_PUMP_FRAME_TYPE  MENU_AIRPUMP_TYPE

static U8 air_pump_data[AIR_PUMP_DATA_LENGTH] =
{ 0 };

void air_pump_get_data(void)
{
	U8 i = 0;
	U16 u16tempdata = 0;
	//U8 u8tempdata = 0;
	air_pump_data[i++] = AIRDCAC_Status; //DCAC工作状态
	air_pump_data[i++] = AIRDCAC_Output_Cut; //DCAC 输出切断
	air_pump_data[i++] = AIRDCAC_intput_Cut; //DCAC 输入切断
	air_pump_data[i++] = AIRDCAC_Fault_Code; //DCAC故障代码

	air_pump_data[i++] = AIRDCAC_Level_Fault; //DCAC故障等级
	air_pump_data[i++] = AIRDCAC_Temp_Warn; //DCDC温度报警
	air_pump_data[i++] = AIRDCAC_Fault_Warn; //DCDC故障报警
	air_pump_data[i++] = AIRDCAC_Temp; //DCAC模块温度

	u16tempdata = AIRDCAC_V_Voltage; //DCAC输出V电压
	if (u16tempdata > 10000)
	{
		u16tempdata = 10000;
	}
	u16tempdata = u16tempdata / 10;
	air_pump_data[i++] = (u16tempdata & 0xff);
	air_pump_data[i++] = (U8) (u16tempdata >> 8) & 0xff;

	u16tempdata = AIRDCAC_V_Current; //DCAC输出V电流
	if (u16tempdata > 5000)
	{
		u16tempdata = 5000;
	}
	u16tempdata = u16tempdata / 10;
	air_pump_data[i++] = (u16tempdata & 0xff);
	air_pump_data[i++] = (U8) (u16tempdata >> 8) & 0xff;

	u16tempdata = AIRDCAC_W_Voltage; //DCAC输出W电压
	if (u16tempdata > 10000)
	{
		u16tempdata = 10000;
	}
	u16tempdata = u16tempdata / 10;
	air_pump_data[i++] = (u16tempdata & 0xff);
	air_pump_data[i++] = (U8) (u16tempdata >> 8) & 0xff;

	u16tempdata = AIRDCAC_W_Current; //DCAC输出W电流
	if (u16tempdata > 5000)
	{
		u16tempdata = 5000;
	}
	u16tempdata = u16tempdata / 10;
	air_pump_data[i++] = (u16tempdata & 0xff);
	air_pump_data[i++] = (U8) (u16tempdata >> 8) & 0xff;

	u16tempdata = AIRDCAC_U_Voltage; //DCAC输出U电压
	if (u16tempdata > 10000)
	{
		u16tempdata = 10000;
	}
	u16tempdata = u16tempdata / 10;
	air_pump_data[i++] = (u16tempdata & 0xff);
	air_pump_data[i++] = (U8) (u16tempdata >> 8) & 0xff;

	u16tempdata = AIRDCAC_U_Current; //DCAC输出U电流
	if (u16tempdata > 5000)
	{
		u16tempdata = 5000;
	}
	u16tempdata = u16tempdata / 10;
	air_pump_data[i++] = (u16tempdata & 0xff);
	air_pump_data[i++] = (U8) (u16tempdata >> 8) & 0xff;
}
void air_pump_send_data(void)
{

	U16 temp = 0;
	U8 maindata[AIR_PUMP_DATA_LENGTH + 6] =
	{ 0 };

	maindata[0] = FRAME_HEAD; /* load frame head */
	maindata[1] = AIR_PUMP_DATA_LENGTH + 3; /* include data segment and crc segment */
	maindata[2] = AIR_PUMP_FRAME_TYPE;

	memcpy(&maindata[3], &air_pump_data, AIR_PUMP_DATA_LENGTH);

	/*-------------------------CRC check------------------------*/
	temp = api_cal_crc16(&maindata[3], AIR_PUMP_DATA_LENGTH); /* crc check */

	maindata[3 + AIR_PUMP_DATA_LENGTH] = temp & 0xff;
	maindata[3 + AIR_PUMP_DATA_LENGTH + 1] = (temp & 0xff00) >> 8;

	maindata[3 + AIR_PUMP_DATA_LENGTH + 1 + 1] = FRAME_REAR; /* frame rear */

	sent_data(UART_A20_CHN, maindata, (AIR_PUMP_DATA_LENGTH + 6)); /* data sent */

}
