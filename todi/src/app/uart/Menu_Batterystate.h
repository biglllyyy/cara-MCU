#define MENU_BATTERY_INFO_DATA_LENGTH (sizeof(BatteryInfoFrame))
#define MENU_BATTERY_INFO_FRAME_TYPE  BATTERY_INFO_TYPE


/* 电池信息,对应BATTERY_INFO_TYPE */
typedef struct {
    U8 bat_voltage[10][4];	//电池电压, 0.00 V  0.01V/ bit，-10000
    U8 bat_temp[10][4];		//电池温度, 0 ℃    1℃/bit，-40
} BatteryInfoFrame;



static BatteryInfoFrame s_battery_info_para;
void get_battery_info_system(void)
{
	U32 u32temp;
	U8 i;
	memset(&s_battery_info_para,0,MENU_BATTERY_INFO_DATA_LENGTH);
	for(i=0; i<10; i++)
	{
		u32temp = Battery_voltage[i];
		DWORD_WRITE(s_battery_info_para.bat_voltage[i],u32temp);
	}
	for(i=0; i<10; i++)
	{
		u32temp = Battery_temp[i];
		DWORD_WRITE(s_battery_info_para.bat_temp[i],u32temp);
	}
}
void send_battery_info_system(void)
{
	U8 data[MENU_MOTORSYSTEM_DATA_LENGTH + A20_MCU_DATA_LENTH];
	U32 parse_len;
    parse_len = app_uart_arm_send_parse(data,(void*)&s_battery_info_para,MENU_BATTERY_INFO_FRAME_TYPE,MENU_BATTERY_INFO_DATA_LENGTH);
	sent_data(UART_A20_CHN, data, parse_len); /* data sent */
}

void menu_battery_info_system_Task(void)
{
	get_battery_info_system();
	send_battery_info_system();
}




