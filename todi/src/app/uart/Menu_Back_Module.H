#define MENU_BACK_MODULE_DATA_LENGTH (sizeof(BackModuleFrame))
#define MENU_BACK_MODULE_FRAME_TYPE  BACK_MODULE_TYPE


/* 后模块,对应BACK_MODULE_TYPE */
typedef struct {
    U8 back_current[15][2];	
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
}BackModuleFrame;

static BackModuleFrame s_back_module_para;
void get_back_module_system(void)
{
    int i = 0;
	for (i = 0; i < 15; ++i)
	{
		WORD_WRITE(s_back_module_para.back_current[i],rpcur[i]);
	}
	s_back_module_para.cur_status01 = rPF[0];
	s_back_module_para.cur_status02 = rPF[1];
	s_back_module_para.cur_status03 = rPF[2];
	s_back_module_para.cur_status04 = rPF[3];
	s_back_module_para.cur_status05 = rPF[4];
	s_back_module_para.cur_status06 = rPF[5];
	s_back_module_para.cur_status07 = rPF[6];
	s_back_module_para.cur_status08 = rPF[7];
	s_back_module_para.cur_status09 = rPF[8];
	s_back_module_para.cur_status10 = rPF[9];
	s_back_module_para.cur_status11 = rPF[10];
	s_back_module_para.cur_status12 = rPF[11];
	s_back_module_para.cur_status13 = rPF[12];
	s_back_module_para.cur_status14 = rPF[13];
	s_back_module_para.cur_status15 = rPF[14];
}
void send_back_module_system(void)
{
	U8 data[MENU_BACK_MODULE_DATA_LENGTH + A20_MCU_DATA_LENTH];
	U32 parse_len;
    parse_len = app_uart_arm_send_parse(data,(void*)&s_back_module_para,MENU_BACK_MODULE_FRAME_TYPE,MENU_BACK_MODULE_DATA_LENGTH);
	sent_data(UART_A20_CHN, data, parse_len); /* data sent */
}

void menu_back_module_system_Task(void)
{
	get_back_module_system();
	send_back_module_system();
}


