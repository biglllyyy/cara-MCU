#define MENU_MIDDLE_MODULE_DATA_LENGTH (sizeof(MiddleModuleFrame))
#define MENU_MIDDLE_MODULE_FRAME_TYPE  MIDDLE_MODULE_TYPE


/* 中模块,对应MIDDLE_MODULE_TYPE */
typedef struct {
    U8 middle_current[15][2];
	U8
	cur_status04: 2,
	cur_status03: 2,
	cur_status02: 2,
	cur_status01: 2;

	U8
	cur_status08: 2,
	cur_status07: 2,
	cur_status06: 2,
	cur_status05: 2;

	U8
	cur_status12: 2,
	cur_status11: 2,
	cur_status10: 2,
	cur_status09: 2;
	
	U8
	cur_reserve: 2,
	cur_status15: 2,
	cur_status14: 2,
	cur_status13: 2;
} MiddleModuleFrame;



static MiddleModuleFrame s_middle_module_para;
void get_middle_module_system(void)
{
    int i = 0;
	for (i = 0; i < 15; ++i)
	{
		WORD_WRITE(s_middle_module_para.middle_current[i],mpcur[i]);
	}
	s_middle_module_para.cur_status01 = mPF[0];
	s_middle_module_para.cur_status02 = mPF[1];
	s_middle_module_para.cur_status03 = mPF[2];
	s_middle_module_para.cur_status04 = mPF[3];
	s_middle_module_para.cur_status05 = mPF[4];
	s_middle_module_para.cur_status06 = mPF[5];
	s_middle_module_para.cur_status07 = mPF[6];
	s_middle_module_para.cur_status08 = mPF[7];
	s_middle_module_para.cur_status09 = mPF[8];
	s_middle_module_para.cur_status10 = mPF[9];
	s_middle_module_para.cur_status11 = mPF[10];
	s_middle_module_para.cur_status12 = mPF[11];
	s_middle_module_para.cur_status13 = mPF[12];
	s_middle_module_para.cur_status14 = mPF[13];
	s_middle_module_para.cur_status15 = mPF[14];	
}
void send_middle_module_system(void)
{
	U8 data[MENU_MIDDLE_MODULE_DATA_LENGTH + A20_MCU_DATA_LENTH];
	U32 parse_len;
    parse_len = app_uart_arm_send_parse(data,(void*)&s_middle_module_para,MENU_MIDDLE_MODULE_FRAME_TYPE,MENU_MIDDLE_MODULE_DATA_LENGTH);
	sent_data(UART_A20_CHN, data, parse_len); /* data sent */
}

void menu_middle_module_system_Task(void)
{
	get_middle_module_system();
	send_middle_module_system();
}






