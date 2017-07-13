#include "app_big_moudle.h"
#include "string.h"
#include "app_can.h"
#include "app_uart_cmd.h"
#include "hal_uart.h"
#include "crc16.h"
#include "mid_string.h"
#include "uart_cfg.h"


#if 0
#define BIG_MOUDLE_DATA_LENGTH  (88)


void big_moudle_front_send_data(void)
{

	U16 temp = 0;
	U8 maindata[BIG_MOUDLE_DATA_LENGTH + 6] =
	{ 0 };

	maindata[0] = FRAME_HEAD; /* load frame head */
	maindata[1] = BIG_MOUDLE_DATA_LENGTH + 3; /* include data segment and crc segment */
	maindata[2] = MENU_FRONT_MOUDLE;

	memcpy(&maindata[3], &big_moudle_can.ID_700, BIG_MOUDLE_DATA_LENGTH);

	/*-------------------------CRC check------------------------*/
	temp = api_cal_crc16(0x0000, &maindata[3], BIG_MOUDLE_DATA_LENGTH); /* crc check */

	maindata[3 + BIG_MOUDLE_DATA_LENGTH] = temp & 0xff;
	maindata[3 + BIG_MOUDLE_DATA_LENGTH + 1] = (temp & 0xff00) >> 8;

	maindata[3 + BIG_MOUDLE_DATA_LENGTH + 1 + 1] = FRAME_REAR; /* frame rear */

	sent_data(UART_A20_CHN, maindata, (BIG_MOUDLE_DATA_LENGTH + 6)); /* data sent */

}


void big_moudle_middle_send_data(void)
{

	U16 temp = 0;
	U8 maindata[BIG_MOUDLE_DATA_LENGTH + 6] =
	{ 0 };

	maindata[0] = FRAME_HEAD; /* load frame head */
	maindata[1] = BIG_MOUDLE_DATA_LENGTH + 3; /* include data segment and crc segment */
	maindata[2] = MENU_MIDDLE_MOUDLE;

	memcpy(&maindata[3], &big_moudle_can.ID_701, BIG_MOUDLE_DATA_LENGTH);

	/*-------------------------CRC check------------------------*/
	temp = api_cal_crc16(0x0000, &maindata[3], BIG_MOUDLE_DATA_LENGTH); /* crc check */

	maindata[3 + BIG_MOUDLE_DATA_LENGTH] = temp & 0xff;
	maindata[3 + BIG_MOUDLE_DATA_LENGTH + 1] = (temp & 0xff00) >> 8;

	maindata[3 + BIG_MOUDLE_DATA_LENGTH + 1 + 1] = FRAME_REAR; /* frame rear */

	sent_data(UART_A20_CHN, maindata, (BIG_MOUDLE_DATA_LENGTH + 6)); /* data sent */

}



void big_moudle_back_send_data(void)
{

	U16 temp = 0;
	U8 maindata[BIG_MOUDLE_DATA_LENGTH + 6] =
	{ 0 };

	maindata[0] = FRAME_HEAD; /* load frame head */
	maindata[1] = BIG_MOUDLE_DATA_LENGTH + 3; /* include data segment and crc segment */
	maindata[2] = MENU_BACK_MOUDLE;

	memcpy(&maindata[3], &big_moudle_can.ID_702, BIG_MOUDLE_DATA_LENGTH);

	/*-------------------------CRC check------------------------*/
	temp = api_cal_crc16(0x0000, &maindata[3], BIG_MOUDLE_DATA_LENGTH); /* crc check */

	maindata[3 + BIG_MOUDLE_DATA_LENGTH] = temp & 0xff;
	maindata[3 + BIG_MOUDLE_DATA_LENGTH + 1] = (temp & 0xff00) >> 8;

	maindata[3 + BIG_MOUDLE_DATA_LENGTH + 1 + 1] = FRAME_REAR; /* frame rear */

	sent_data(UART_A20_CHN, maindata, (BIG_MOUDLE_DATA_LENGTH + 6)); /* data sent */

}

#endif

