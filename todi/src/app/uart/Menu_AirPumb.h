#define MENU_AIR_PUMB_DATA_LENGTH (sizeof(AirPump))
#define MENU_AIR_PUMB_FRAME_TYPE  AIR_PUMB_INFO_TYPE

void send_airpumb_system(void)
{
	U8 data[MENU_AIR_PUMB_DATA_LENGTH + A20_MCU_DATA_LENTH];
	U32 parse_len;
    parse_len = app_uart_arm_send_parse(data,(void*)&air_pump_msg,MENU_AIR_PUMB_FRAME_TYPE,MENU_AIR_PUMB_DATA_LENGTH);
	sent_data(UART_A20_CHN, data, parse_len); /* data sent */
}

void menu_airpumb_system_Task(void)
{
	send_airpumb_system();
}



