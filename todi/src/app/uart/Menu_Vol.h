#define MENU_VOL_DATA_LENGTH (sizeof(BatteryVoltage))
#define MENU_VOL_FRAME_TYPE  VOL_INFO_TYPE

void send_vol_system(void)
{
	U8 data[MENU_VOL_DATA_LENGTH + A20_MCU_DATA_LENTH];
	U16 page;
	U32 parse_len;
	int i;
	page = g_tUart1Rec.u8BattBoxNum;
	if (page == 0)
	page = 1;
	bat_vol_msg.pager = page;
	for(i = 0; i < 32; ++i){
		WORD_WRITE(bat_vol_msg.voltage[i], vol[(page-1)*32+i]);
		dbg_printf("%d %d %d,%d\n", i,bat_vol_msg.voltage[i][0],bat_vol_msg.voltage[i][1], vol[(page-1)*32+i]);
	}
    parse_len = app_uart_arm_send_parse(data,(void*)&bat_vol_msg,MENU_VOL_FRAME_TYPE,MENU_VOL_DATA_LENGTH);
	sent_data(UART_A20_CHN, data, parse_len); /* data sent */
}

void menu_vol_system_Task(void)
{
	send_vol_system();
}



