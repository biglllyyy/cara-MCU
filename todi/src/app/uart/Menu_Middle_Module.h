#define MENU_MIDDLE_MODULE_DATA_LENGTH (sizeof(MiddleModuleFrame))
#define MENU_MIDDLE_MODULE_FRAME_TYPE  MIDDLE_MODULE_TYPE


/* 中模块,对应MIDDLE_MODULE_TYPE */
typedef struct {
    U8 middle_current[15][4];
        //0,广告灯箱, 0.0 A
        //1,右转向灯, 0.0 A
        //2,车内指示灯电源, 0.0 A
        //3,制动灯开关, 0.0 A
        //4,前门开门电磁阀, 0.0 A
        //5,前路牌, 0.0 A
        //6,前门关门电磁阀, 0.0 A
        //7,侧路牌, 0.0 A
        //8,中门开门电磁阀, 0.0 A
        //9,后路牌, 0.0 A
        //10,中门关门电磁阀, 0.0 A
        //11,电子滚动屏, 0.0 A
        //12,小灯电源, 0.0 A
        //13,后广告屏, 0.0 A
        //14,左转向灯, 0.0 A
    U8
		N: 2,                //预留
		middle_switch6: 1,	//中门踏步灯开关
		middle_switch5: 1,	//前门踏步灯开关
		middle_switch4: 1,	//地址选择2
		middle_switch3: 1,	//地址选择1
		middle_switch2: 1,	//应急阀开启报警     
		middle_switch1: 1;	//门铃开关

       
} MiddleModuleFrame;



static MiddleModuleFrame s_middle_module_para;
void get_middle_module_system(void)
{
	U32 u32temp;
	memset(&s_middle_module_para,0,MENU_MIDDLE_MODULE_DATA_LENGTH);
	u32temp = mpcur[0 ];DWORD_WRITE(s_middle_module_para.middle_current[0 ],u32temp);        //0,广告灯箱, 0.0 A        
	u32temp = mpcur[1 ];DWORD_WRITE(s_middle_module_para.middle_current[1 ],u32temp);        //1,右转向灯, 0.0 A        
	u32temp = mpcur[2 ];DWORD_WRITE(s_middle_module_para.middle_current[2 ],u32temp);        //2,车内指示灯电源, 0.0 A  
	u32temp = mpcur[3 ];DWORD_WRITE(s_middle_module_para.middle_current[3 ],u32temp);        //3,制动灯开关, 0.0 A      
	u32temp = mpcur[4 ];DWORD_WRITE(s_middle_module_para.middle_current[4 ],u32temp);        //4,前门开门电磁阀, 0.0 A  
	u32temp = mpcur[5 ];DWORD_WRITE(s_middle_module_para.middle_current[5 ],u32temp);        //5,前路牌, 0.0 A          
	u32temp = mpcur[6 ];DWORD_WRITE(s_middle_module_para.middle_current[6 ],u32temp);        //6,前门关门电磁阀, 0.0 A  
	u32temp = mpcur[7 ];DWORD_WRITE(s_middle_module_para.middle_current[7 ],u32temp);        //7,侧路牌, 0.0 A          
	u32temp = mpcur[8 ];DWORD_WRITE(s_middle_module_para.middle_current[8 ],u32temp);        //8,中门开门电磁阀, 0.0 A  
	u32temp = mpcur[9 ];DWORD_WRITE(s_middle_module_para.middle_current[9 ],u32temp);        //9,后路牌, 0.0 A          
	u32temp = mpcur[10];DWORD_WRITE(s_middle_module_para.middle_current[10],u32temp);        //10,中门关门电磁阀, 0.0 A 
	u32temp = mpcur[11];DWORD_WRITE(s_middle_module_para.middle_current[11],u32temp);        //11,电子滚动屏, 0.0 A     
	u32temp = mpcur[12];DWORD_WRITE(s_middle_module_para.middle_current[12],u32temp);        //12,小灯电源, 0.0 A       
	u32temp = mpcur[13];DWORD_WRITE(s_middle_module_para.middle_current[13],u32temp);        //13,后广告屏, 0.0 A       
	u32temp = mpcur[14];DWORD_WRITE(s_middle_module_para.middle_current[14],u32temp);        //14,左转向灯, 0.0 A   

	s_middle_module_para.middle_switch6 = mKEY.BITS.kl1;      //中门踏步灯开关        
	s_middle_module_para.middle_switch5 = mKEY.BITS.kl15;      //前门踏步灯开关        
	s_middle_module_para.middle_switch4 = mKEY.BITS.kl14;      //地址选择2             
	s_middle_module_para.middle_switch3 = mKEY.BITS.kl13;      //地址选择1             
	s_middle_module_para.middle_switch2 = mKEY.BITS.kl4;      //应急阀开启报警        
	s_middle_module_para.middle_switch1 = mKEY.BITS.kl2;      //门铃开关              
}
void send_middle_module_system(void)
{
	U8 data[MENU_MOTORSYSTEM_DATA_LENGTH + A20_MCU_DATA_LENTH];
	U32 parse_len;
    parse_len = app_uart_arm_send_parse(data,(void*)&s_middle_module_para,MENU_MIDDLE_MODULE_FRAME_TYPE,MENU_MIDDLE_MODULE_DATA_LENGTH);
	sent_data(UART_A20_CHN, data, parse_len); /* data sent */
}

void menu_middle_module_system_Task(void)
{
	get_middle_module_system();
	send_middle_module_system();
}






