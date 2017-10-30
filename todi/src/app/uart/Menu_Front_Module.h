#define MENU_FRONT_MODULE_DATA_LENGTH (sizeof(FrontModuleFrame))
#define MENU_FRONT_MODULE_FRAME_TYPE  FRONT_MODULE_TYPE


/* 前模块,对应FRONT_MODULE_TYPE */
typedef struct {
    U8 front_current[15][2];
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
}FrontModuleFrame;

static FrontModuleFrame s_front_module_para;
void get_front_module_system(void)
{
	int i = 0;
	for (i = 0; i < 15; ++i)
	{
		WORD_WRITE(s_front_module_para.front_current[i],fpcur[i]);
	}
	s_front_module_para.cur_status01 = fPF[0];
	s_front_module_para.cur_status02 = fPF[1];
	s_front_module_para.cur_status03 = fPF[2];
	s_front_module_para.cur_status04 = fPF[3];
	s_front_module_para.cur_status05 = fPF[4];
	s_front_module_para.cur_status06 = fPF[5];
	s_front_module_para.cur_status07 = fPF[6];
	s_front_module_para.cur_status08 = fPF[7];
	s_front_module_para.cur_status09 = fPF[8];
	s_front_module_para.cur_status10 = fPF[9];
	s_front_module_para.cur_status11 = fPF[10];
	s_front_module_para.cur_status12 = fPF[11];
	s_front_module_para.cur_status13 = fPF[12];
	s_front_module_para.cur_status14 = fPF[13];
	s_front_module_para.cur_status15 = fPF[14];
}
void send_front_module_system(void)
{
	U8 data[MENU_FRONT_MODULE_DATA_LENGTH + A20_MCU_DATA_LENTH];
	U32 parse_len;
    parse_len = app_uart_arm_send_parse(data,(void*)&s_front_module_para,MENU_FRONT_MODULE_FRAME_TYPE,MENU_FRONT_MODULE_DATA_LENGTH);
	sent_data(UART_A20_CHN, data, parse_len); /* data sent */
}

void menu_front_module_system_Task(void)
{
	get_front_module_system();
	send_front_module_system();
}

