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
	U32 u32temp;
	memset(&s_front_module_para,0,MENU_FRONT_MODULE_DATA_LENGTH);
	u32temp = fpcur[0];DWORD_WRITE(s_front_module_para.front_current[0 ],u32temp);	//0, 雨刮快档电机, 0.0 
	u32temp = fpcur[1];DWORD_WRITE(s_front_module_para.front_current[1 ],u32temp);	//1, 雨刮慢档电机, 0.0 
	u32temp = fpcur[2];DWORD_WRITE(s_front_module_para.front_current[2 ],u32temp);	//2, 喷水电机, 0.0 A   
	u32temp = fpcur[3];DWORD_WRITE(s_front_module_para.front_current[3 ],u32temp);	//3, 左前雾灯, 0.0 A   
	u32temp = fpcur[4];DWORD_WRITE(s_front_module_para.front_current[4 ],u32temp);	//4, 倒车灯, 0.0 A	   
	u32temp = fpcur[5];DWORD_WRITE(s_front_module_para.front_current[5 ],u32temp);	//5, 右转向灯, 0.0 A   
	u32temp = fpcur[6];DWORD_WRITE(s_front_module_para.front_current[6 ],u32temp);	//6, 左转向灯, 0.0 A   
	u32temp = fpcur[7];DWORD_WRITE(s_front_module_para.front_current[7 ],u32temp);	//7, 液位显示电源, 0.0 
	u32temp = fpcur[8];DWORD_WRITE(s_front_module_para.front_current[8 ],u32temp);	//8, 左远光灯, 0.0 A   
	u32temp = fpcur[9];DWORD_WRITE(s_front_module_para.front_current[9 ],u32temp);	//9, 右前雾灯, 0.0 A   
	u32temp = fpcur[10];DWORD_WRITE(s_front_module_para.front_current[10],u32temp);	//10, 右远光灯, 0.0 A  
	u32temp = fpcur[11];DWORD_WRITE(s_front_module_para.front_current[11],u32temp);	//11, 行车记录仪电源, 0
	u32temp = fpcur[12];DWORD_WRITE(s_front_module_para.front_current[12],u32temp);	//12, 左近光灯, 0.0 A  
	u32temp = fpcur[13];DWORD_WRITE(s_front_module_para.front_current[13],u32temp);	//13, 位置灯, 0.0 A    
	u32temp = fpcur[14];DWORD_WRITE(s_front_module_para.front_current[14],u32temp);	//14, 右近光灯, 0.0 A  

	s_front_module_para.front_switch6 = fKEY.BITS.kl1; //倒车灯开关      
	s_front_module_para.front_switch5 = fKEY.BITS.kl14; //地址选择2       
	s_front_module_para.front_switch4 = fKEY.BITS.kl13; //地址选择1       
	s_front_module_para.front_switch3 = fKEY.BITS.kl6; //制动灯开关      
	s_front_module_para.front_switch2 = fKEY.BITS.kl5; //右前摩擦片报警开
	s_front_module_para.front_switch1 = fKEY.BITS.kl4; //左前摩擦片报警开
	

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

