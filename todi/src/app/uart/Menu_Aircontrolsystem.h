#define MENU_ACSYSTEM_DATA_LENGTH (sizeof(ACSystemInfoFrame))
#define MENU_ACSYSTEM_FRAME_TYPE  AC_SYSTEM_INFO_TYPE


/* 空调系统信息,对应AC_SYSTEM_INFO_TYPE */
typedef struct {
    U8 ac_fault_code[4];		//空调故障代码, 00000000（2进制显示）
    U8 ac_fan_status[4];		//空调风机状态, 00000000（2进制显示）
    U8 ac_work_status[4];		//空调工作状态, 00000000（2进制显示）
    U8 ac_Life[4];			//空调Life, 0
    U8 ac_out_temp[2];			//车外实际温度, 0 ℃0.5℃/bit，-30
    U8 ac_in_temp[2];			//车内实际温度, 0 ℃0.5℃/bit，-30
    U8 ac_set_temp[2];			//设定温度, 0 ℃ 0.5℃/bit，-30
    U8 ac_refrigere_req[4];	//空调制冷请求, 00000000（2进制显示）
} ACSystemInfoFrame;


static ACSystemInfoFrame s_AC_para;
void get_AC_system(void)
{
	U32 u32temp;
	memset(&s_AC_para,0,MENU_ACSYSTEM_DATA_LENGTH);
	u32temp = AC_Warn_code;
	DWORD_WRITE(s_AC_para.ac_fault_code,u32temp);
	u32temp = AC_KEY[0].byte;
	DWORD_WRITE(s_AC_para.ac_fan_status,u32temp);
	u32temp = AC_KEY[1].byte;
	DWORD_WRITE(s_AC_para.ac_work_status,u32temp);
	u32temp = AC_LIFE;
	DWORD_WRITE(s_AC_para.ac_Life,u32temp);
	u32temp = Outdoor_t;WORD_WRITE(s_AC_para.ac_out_temp,u32temp);
	u32temp = Indoor_t;WORD_WRITE(s_AC_para.ac_in_temp,u32temp);
	u32temp = Set_t;WORD_WRITE(s_AC_para.ac_set_temp,u32temp);
	u32temp = AC_req;DWORD_WRITE(s_AC_para.ac_refrigere_req,u32temp);
}
void send_AC_system(void)
{
	U8 data[MENU_ACSYSTEM_DATA_LENGTH + A20_MCU_DATA_LENTH];
	U32 parse_len;
    parse_len = app_uart_arm_send_parse(data,(void*)&s_AC_para,MENU_ACSYSTEM_FRAME_TYPE,MENU_ACSYSTEM_DATA_LENGTH);
	sent_data(UART_A20_CHN, data, parse_len); /* data sent */
}

void menu_AC_system_Task(void)
{
	get_AC_system();
	send_AC_system();
}


