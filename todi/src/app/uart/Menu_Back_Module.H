#define MENU_BACK_MODULE_DATA_LENGTH (sizeof(BackModuleFrame))
#define MENU_BACK_MODULE_FRAME_TYPE  BACK_MODULE_TYPE


/* 后模块,对应BACK_MODULE_TYPE */
typedef struct {
    U8 back_current[9][4];
        //0,制动灯 0.0 A
        //1,后雾灯, 0.0 A
        //2,左转向灯, 0.0 A
        //3,右转向灯, 0.0 A
        //4,干燥器电源, 0.0 A
        //5,倒车灯, 0.0 A
        //6,前门关门电磁阀, 0.0 A
        //7,位置灯, 0.0 A
        //8,中门踏步灯开关, 0.0 A
    U8
		back_switch8:1, //地址开关1
		back_switch7:1, //低气压2报警开关
		back_switch6:1, //低气压1报警开关
		back_switch5:1, //水温报警开关
		back_switch4:1, //驻车制动开关
		back_switch3:1, //制动灯开关
		back_switch2:1, //空滤阻塞报警开关   
		back_switch1:1; //水位传感器开关
    U8
		N:6,              //预留
		back_switch10:1, //后舱门开关
        back_switch9:1; //地址开关2

}BackModuleFrame;




static BackModuleFrame s_back_module_para;
void get_back_module_system(void)
{
	memset(&s_back_module_para,0,MENU_BACK_MODULE_DATA_LENGTH);
	
}
void send_back_module_system(void)
{
	U8 data[MENU_MOTORSYSTEM_DATA_LENGTH + 6];
	U32 parse_len;
    parse_len = app_uart_arm_send_parse(data,(void*)&s_back_module_para,MENU_BACK_MODULE_FRAME_TYPE,MENU_BACK_MODULE_DATA_LENGTH);
	sent_data(UART_A20_CHN, data, parse_len); /* data sent */
}

void menu_back_module_system_Task(void)
{
	get_back_module_system();
	send_back_module_system();
}


