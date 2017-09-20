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
	//!<TODO:按协议填充信息
}

#define WARNING_DATA_LENGTH   (sizeof(WarningDataStruct))
#define WARNING_FRAME_TYPE    WARNING_TYPE

#if 0
extern U8 testdata[100];
extern U8 testindex;
#endif
void warning_send_data(void)
{


	U8 data[WARNING_DATA_LENGTH + 6];
	U32 parse_len;
    parse_len = app_uart_arm_send_parse(data,(void*)&warning_data,WARNING_FRAME_TYPE,WARNING_DATA_LENGTH);
	sent_data(UART_A20_CHN, data, parse_len); /* data sent */
}

