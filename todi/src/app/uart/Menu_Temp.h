#define MENU_TEMP_DATA_LENGTH (sizeof(BatteryTemperater))
#define MENU_TEMP_FRAME_TYPE  TEMP_INFO_TYPE

void send_temp_system(void)
{
	U8 data[MENU_TEMP_DATA_LENGTH + A20_MCU_DATA_LENTH];
	U8 page;
	U32 idx,len;
	U32 parse_len;

	page = g_tUart1Rec.u8BattBoxNum;
	if (page == 0)
		page = 1;
	bat_temp_msg.pager = page;
	memcpy(&bat_temp_msg.Temperater[0],&temperater[32*(page-1)],32);
    parse_len = app_uart_arm_send_parse(data,(void*)&bat_temp_msg,MENU_TEMP_FRAME_TYPE,MENU_TEMP_DATA_LENGTH);
	sent_data(UART_A20_CHN, data, parse_len); /* data sent */
}

void menu_temp_system_Task(void)
{
	send_temp_system();
}



