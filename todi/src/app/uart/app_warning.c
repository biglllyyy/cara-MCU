#include "app_warning.h"
#include "app_can.h"
#include "app_uart_cmd.h"
#include "hal_uart.h"
#include "string.h"
#include "crc16.h"
#include "mid_string.h"
#include "app_moudle.h"
#include "uart_cfg.h"



WarningDataStruct warning_data = { 0 };
void warning_get_data(void)
{
	warning_data.BAT_Voltage_Fault = BAT_Voltage_Fault; 					//×ÜÑ¹¹ÊÕÏ
	warning_data.BAT_Single_Fault = BAT_Single_Fault; 						//µ¥Ñ¹¹ÊÕÏ
	warning_data.BAT_Temp_Fault = BAT_Temp_Fault; 							//ÎÂ¶È¹ÊÕÏ
	warning_data.BAT_Insulation_Fault = BAT_Insulation_Fault; 				//¾øÔµ¹ÊÕÏ
	
	warning_data.BAT_Consistency_Fault = BAT_Consistency_Fault; 			//µç³ØÒ»ÖÂÐÔ¹ÊÕÏ
	warning_data.BAT_SOC_Fault = BAT_SOC_Fault; 							//SOC¹ÊÕÏ
	warning_data.BAT_Current_Fault = BAT_Current_Fault; 					//µçÁ÷¹ÊÕÏ
	warning_data.BAT_Lock_Fault = BAT_Lock_Fault; 							//¸ßÑ¹»¥Ëø¹ÊÕÏ
	
	warning_data.BMS_Comm_Fault = BMS_Comm_Fault; 							//BMSÍ¨Ñ¶¹ÊÕÏ
	warning_data.BMS_System_Fault = BMS_System_Fault;						//BMSÏµÍ³¹ÊÕÏ
	warning_data.DCDC_Temp_Warn = DCDC_Temp_Warn; 							//DCDCÎÂ¶È±¨¾¯
	warning_data.DCDC_Fault_Warn = DCDC_Fault_Warn; 						//DCDC¹ÊÕÏ±¨¾¯
	
	warning_data.OILDCAC_Temp_Warn = OILDCAC_Temp_Warn; 					//DCDCÎÂ¶È±¨¾¯
	warning_data.OILDCAC_Fault_Warn = OILDCAC_Fault_Warn; 					//DCDC¹ÊÕÏ±¨¾¯
	warning_data.AIRDCAC_Temp_Warn = AIRDCAC_Temp_Warn; 					//DCDCÎÂ¶È±¨¾¯
	warning_data.AIRDCAC_Fault_Warn = AIRDCAC_Fault_Warn; 					//DCDC¹ÊÕÏ±¨¾¯
}

#define WARNING_DATA_LENGTH   16
#define WARNING_FRAME_TYPE    WARNING_TYPE

#if 0
extern U8 testdata[100];
extern U8 testindex;
#endif
void warning_send_data(void)
{

	U16 temp = 0;
	U8 maindata[WARNING_DATA_LENGTH + 6] =
	{ 0 };
	//U8 testindexa[3]={0};

	maindata[0] = FRAME_HEAD; /* load frame head */
	maindata[1] = WARNING_DATA_LENGTH + 3; /* include data segment and crc segment */
	maindata[2] = WARNING_FRAME_TYPE;

	memcpy(&maindata[3], &warning_data, WARNING_DATA_LENGTH);

	/*-------------------------CRC check------------------------*/
	temp = api_cal_crc16(&maindata[3], WARNING_DATA_LENGTH); /* crc check */

	maindata[3 + WARNING_DATA_LENGTH] = temp & 0xff;
	maindata[3 + WARNING_DATA_LENGTH + 1] = (temp & 0xff00) >> 8;

	maindata[3 + WARNING_DATA_LENGTH + 1 + 1] = FRAME_REAR; /* frame rear */

	sent_data(UART_A20_CHN, maindata, (WARNING_DATA_LENGTH + 6)); /* data sent */

#if 0
	sent_data(1,testdata,100);
	testindexa[0] = 0xaa;
	testindexa[1] = testindex;
	testindexa[2] = 0xbb;
	sent_data(1,&testindexa,3);
#endif	

}

