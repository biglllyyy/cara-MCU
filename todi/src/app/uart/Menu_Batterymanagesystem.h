#define MENU_BMS_INFO_DATA_LENGTH (sizeof(BmsInfoFrame))
#define MENU_BMS_INFO_FRAME_TYPE  BMS_INFO_TYPE


/* 电池管理系统信息,对应BMS_INFO_TYPE */
typedef struct {
    //int16_t totalVoltage;				//电池总电压, 0.0 V,通用帧已存在该变量
    U8 bat_max_charge_current[2];	    //最大允许充电电流,, 0.0 A
    U8 bat_max_discharge_current[2];	//最大允许放电电流,, 0.0 A
    U8 bat_average_voltage[4];		//单体平均电压, 0.00 V
    U8 bat_status1[4];				//电池状态1, 00000000（2进制显示）
    U8 bat_status2[4];				//电池状态2, 00000000（2进制显示）
    U8 bat_status3[4];				//电池状态3, 00000000（2进制显示）
    U8 irm_ohm_positive[4];			//总正绝缘阻值, 0 KΩ
    U8 irm_ohm_negative[4];			//总负绝缘阻值, 0 KΩ
    U8 bat_max_voltage[4];			//单体最高电压, 0.00 V
    U8 bat_min_voltage[4];			//单体最低电压, 0.00 V
    U8 batteryTH[4];					//单体最高温度, 0.0 ℃
    //int16_t totalCurrent;				//充放电电流, 0 A,通用帧已存在该变量
} BmsInfoFrame;



static BmsInfoFrame s_bms_info_para;
void get_bms_info_system(void)
{
	memset(&s_bms_info_para,0,MENU_BMS_INFO_DATA_LENGTH);
}
void send_bms_info_system(void)
{
	U8 data[MENU_MOTORSYSTEM_DATA_LENGTH + 6];
	U32 parse_len;
    parse_len = app_uart_arm_send_parse(data,(void*)&s_bms_info_para,MENU_BMS_INFO_FRAME_TYPE,MENU_BMS_INFO_DATA_LENGTH);
	sent_data(UART_A20_CHN, data, parse_len); /* data sent */
}

void menu_bms_info_system_Task(void)
{
	get_bms_info_system();
	send_bms_info_system();
}



