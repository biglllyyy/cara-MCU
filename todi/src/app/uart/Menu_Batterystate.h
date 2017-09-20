#define MENU_BATTERY_INFO_DATA_LENGTH (sizeof(BatteryInfoFrame))
#define MENU_BATTERY_INFO_FRAME_TYPE  BATTERY_INFO_TYPE


/* 电池信息,对应BATTERY_INFO_TYPE */
typedef struct {
    U8 bat_voltage[10][4];	//电池电压, 0.00 V
    U8 bat_temp[10][4];		//电池温度, 0 ℃
} BatteryInfoFrame;



static BatteryInfoFrame s_battery_info_para;
void get_battery_info_system(void)
{
	memset(&s_battery_info_para,0,MENU_BATTERY_INFO_DATA_LENGTH);
}
void send_battery_info_system(void)
{
	U8 data[MENU_MOTORSYSTEM_DATA_LENGTH + 6];
	U32 parse_len;
    parse_len = app_uart_arm_send_parse(data,(void*)&s_battery_info_para,MENU_BATTERY_INFO_FRAME_TYPE,MENU_BATTERY_INFO_DATA_LENGTH);
	sent_data(UART_A20_CHN, data, parse_len); /* data sent */
}

void menu_battery_info_system_Task(void)
{
	get_battery_info_system();
	send_battery_info_system();
}




