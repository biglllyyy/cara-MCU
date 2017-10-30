#define MENU_OILPUMB_DATA_LENGTH (sizeof(OilPump))
#define MENU_OILPUMB_FRAME_TYPE  OIL_PUMB_INFO_TYPE

void send_oilpumb_system(void)
{
	U8 data[MENU_OILPUMB_DATA_LENGTH + A20_MCU_DATA_LENTH];
	U32 parse_len;
    parse_len = app_uart_arm_send_parse(data,(void*)&oil_pump_msg,MENU_OILPUMB_FRAME_TYPE,MENU_OILPUMB_DATA_LENGTH);
	sent_data(UART_A20_CHN, data, parse_len); /* data sent */
}

void menu_oilpumb_system_Task(void)
{
	send_oilpumb_system();
}



