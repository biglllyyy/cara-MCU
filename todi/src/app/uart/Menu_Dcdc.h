#define MENU_DCDC_DATA_LENGTH (sizeof(DcdcControl))
#define MENU_DCDC_FRAME_TYPE  DCDC_INFO_TYPE

void send_dcdc_system(void)
{
	U8 data[MENU_DCDC_DATA_LENGTH + A20_MCU_DATA_LENTH];
	U32 parse_len;
    parse_len = app_uart_arm_send_parse(data,(void*)&dcdc_msg,MENU_DCDC_FRAME_TYPE,MENU_DCDC_DATA_LENGTH);
	sent_data(UART_A20_CHN, data, parse_len); /* data sent */
}

void menu_dcdc_system_Task(void)
{
	send_dcdc_system();
}
