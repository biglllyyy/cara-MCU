#include "app_oil_pump.h"
#include "app_can.h"
#include "app_uart_cmd.h"
#include "hal_uart.h"
#include "string.h"
#include "crc16.h"
#include "mid_string.h"
#include "uart_cfg.h"




#define OIL_PUMP_DATA_LENGTH 20
#define OIL_PUMP_FRAME_TYPE  MENU_OILPUMP_TYPE

static U8 oil_pump_data[OIL_PUMP_DATA_LENGTH] ={0};


void oil_pump_get_data(void)
{
	U8 i = 0;
	U16 u16tempdata = 0;
	//U8 u8tempdata = 0;
	oil_pump_data[i++] = OILDCAC_Status; //DCAC����״̬
	oil_pump_data[i++] = OILDCAC_Output_Cut; //DCAC ����ж�
	oil_pump_data[i++] = OILDCAC_intput_Cut; //DCAC �����ж�
	oil_pump_data[i++] = OILDCAC_Fault_Code; //DCAC���ϴ���

	oil_pump_data[i++] = OILDCAC_Level_Fault; //DCAC���ϵȼ�
	oil_pump_data[i++] = OILDCAC_Temp_Warn; //DCDC�¶ȱ���
	oil_pump_data[i++] = OILDCAC_Fault_Warn; //DCDC���ϱ���
	oil_pump_data[i++] = OILDCAC_Temp; //DCACģ���¶�

	u16tempdata = OILDCAC_V_Voltage; //DCAC���V��ѹ
	if (u16tempdata > 10000)
	{
		u16tempdata = 10000;
	}
	u16tempdata = u16tempdata / 10;
	oil_pump_data[i++] = (u16tempdata & 0xff);
	oil_pump_data[i++] = (U8) (u16tempdata >> 8) & 0xff;

	u16tempdata = OILDCAC_V_Current; //DCAC���V����
	if (u16tempdata > 5000)
	{
		u16tempdata = 5000;
	}
	u16tempdata = u16tempdata / 10;
	oil_pump_data[i++] = (u16tempdata & 0xff);
	oil_pump_data[i++] = (U8) (u16tempdata >> 8) & 0xff;

	u16tempdata = OILDCAC_W_Voltage; //DCAC���W��ѹ
	if (u16tempdata > 10000)
	{
		u16tempdata = 10000;
	}
	u16tempdata = u16tempdata / 10;
	oil_pump_data[i++] = (u16tempdata & 0xff);
	oil_pump_data[i++] = (U8) (u16tempdata >> 8) & 0xff;

	u16tempdata = OILDCAC_W_Current; //DCAC���W����
	if (u16tempdata > 5000)
	{
		u16tempdata = 5000;
	}
	u16tempdata = u16tempdata / 10;
	oil_pump_data[i++] = (u16tempdata & 0xff);
	oil_pump_data[i++] = (U8) (u16tempdata >> 8) & 0xff;

	u16tempdata = OILDCAC_U_Voltage; //DCAC���U��ѹ
	if (u16tempdata > 10000)
	{
		u16tempdata = 10000;
	}
	u16tempdata = u16tempdata / 10;
	oil_pump_data[i++] = (u16tempdata & 0xff);
	oil_pump_data[i++] = (U8) (u16tempdata >> 8) & 0xff;

	u16tempdata = OILDCAC_U_Current; //DCAC���U����
	if (u16tempdata > 5000)
	{
		u16tempdata = 5000;
	}
	u16tempdata = u16tempdata / 10;
	oil_pump_data[i++] = (u16tempdata & 0xff);
	oil_pump_data[i++] = (U8) (u16tempdata >> 8) & 0xff;
}
void oil_pump_send_data(void)
{

	U16 temp = 0;
	U8 maindata[OIL_PUMP_DATA_LENGTH + 6] =
	{ 0 };

	maindata[0] = FRAME_HEAD; /* load frame head */
	maindata[1] = OIL_PUMP_DATA_LENGTH + 3; /* include data segment and crc segment */
	maindata[2] = OIL_PUMP_FRAME_TYPE;

	memcpy(&maindata[3], &oil_pump_data, OIL_PUMP_DATA_LENGTH);

	/*-------------------------CRC check------------------------*/
	temp = api_cal_crc16(&maindata[3], OIL_PUMP_DATA_LENGTH); /* crc check */

	maindata[3 + OIL_PUMP_DATA_LENGTH] = temp & 0xff;
	maindata[3 + OIL_PUMP_DATA_LENGTH + 1] = (temp & 0xff00) >> 8;

	maindata[3 + OIL_PUMP_DATA_LENGTH + 1 + 1] = FRAME_REAR; /* frame rear */

	sent_data(UART_A20_CHN, maindata, (OIL_PUMP_DATA_LENGTH + 6)); /* data sent */


}
