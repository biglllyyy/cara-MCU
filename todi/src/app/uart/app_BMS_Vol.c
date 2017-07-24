#include "app_BMS_Vol.h"
#include "app_can.h"
#include "app_uart_cmd.h"
#include "hal_uart.h"
#include "string.h"
#include "crc16.h"
#include "mid_string.h"
#include "uart_cfg.h"
#include "app_info.h"




#define BMS_VOL_DATA_LENGTH  40
#define BMS_VOL_FRAME_TYPE   MENU_BATSTATUS_TYPE



static U16 bms_vol_data[20] = {0};
void bms_vol_get_data(void)
{
	//U8 temp = 0;
	U16 i = 0,j=0;
	U16 temp;
	memset(&bms_vol_data[0],0,40);
	temp = CurrentMenu;
	dbg_printf("  >>CurrentMenu is %d\n",temp);
	if (temp !=  0)
	{
		temp -= 1;
	}
	
	for((i=20*temp);i<(20*(temp+1));i++)
	{
		bms_vol_data[j] = BAT_Cell_Voltage[i];
		dbg_printf("->id%d = %d,%d;",i,bms_vol_data[j],temp);
		j++;
	}

	for(i=0;i<20;i++)
	{
		//if(i%2==0)
		{
			byte_order_change(((U8*)&(bms_vol_data[i])),2); 
		}
	}
}
void bms_vol_send_data(void)
{

	U16 temp = 0;
	U8 maindata[BMS_VOL_DATA_LENGTH + 6] =
	{ 0 };

	maindata[0] = FRAME_HEAD; /* load frame head */
	maindata[1] = BMS_VOL_DATA_LENGTH + 3; /* include data segment and crc segment */
	maindata[2] = BMS_VOL_FRAME_TYPE;

	memcpy(&maindata[3], &bms_vol_data, BMS_VOL_DATA_LENGTH);

	/*-------------------------CRC check------------------------*/
	temp = api_cal_crc16(&maindata[3], BMS_VOL_DATA_LENGTH); /* crc check */

	maindata[3 + BMS_VOL_DATA_LENGTH] = temp & 0xff;
	maindata[3 + BMS_VOL_DATA_LENGTH + 1] = (temp & 0xff00) >> 8;

	maindata[3 + BMS_VOL_DATA_LENGTH + 1 + 1] = FRAME_REAR; /* frame rear */

	sent_data(UART_A20_CHN, maindata, (BMS_VOL_DATA_LENGTH + 6)); /* data sent */

}

