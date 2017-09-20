#define MENU_FRONT_MODULE_DATA_LENGTH (sizeof(FrontModuleFrame))
#define MENU_FRONT_MODULE_FRAME_TYPE  FRONT_MODULE_TYPE


/* 前模块,对应FRONT_MODULE_TYPE */
typedef struct {
    U8 front_current[15][4];
		//0, 雨刮快档电机, 0.0 A
        //1, 雨刮慢档电机, 0.0 A
        //2, 喷水电机, 0.0 A
        //3, 左前雾灯, 0.0 A
        //4, 倒车灯, 0.0 A
        //5, 右转向灯, 0.0 A
        //6, 左转向灯, 0.0 A
        //7, 液位显示电源, 0.0 A
        //8, 左远光灯, 0.0 A
        //9, 右前雾灯, 0.0 A
        //10, 右远光灯, 0.0 A
        //11, 行车记录仪电源, 0.0 A
        //12, 左近光灯, 0.0 A
        //13, 位置灯, 0.0 A
        //14, 右近光灯, 0.0 A
    U8
		N: 2,                //预留
		front_switch6: 1,	//倒车灯开关
		front_switch5: 1,	//地址选择2
		front_switch4: 1,	//地址选择1
		front_switch3: 1,	//制动灯开关
		front_switch2: 1,	 //右前摩擦片报警开关		 
		front_switch1: 1;	//左前摩擦片报警开关
}FrontModuleFrame;


static FrontModuleFrame s_front_module_para;
void get_front_module_system(void)
{
	memset(&s_front_module_para,0,MENU_FRONT_MODULE_DATA_LENGTH);
}
void send_front_module_system(void)
{
	U8 data[MENU_MOTORSYSTEM_DATA_LENGTH + 6];
	U32 parse_len;
    parse_len = app_uart_arm_send_parse(data,(void*)&s_front_module_para,MENU_FRONT_MODULE_FRAME_TYPE,MENU_FRONT_MODULE_DATA_LENGTH);
	sent_data(UART_A20_CHN, data, parse_len); /* data sent */
}

void menu_front_module_system_Task(void)
{
	get_front_module_system();
	send_front_module_system();
}

