#define MENU_MIDDLE_MODULE_DATA_LENGTH (sizeof(MiddleModuleFrame))
#define MENU_MIDDLE_MODULE_FRAME_TYPE  MIDDLE_MODULE_TYPE


/* ��ģ��,��ӦMIDDLE_MODULE_TYPE */
typedef struct {
    U8 middle_current[15][4];
        //0,������, 0.0 A
        //1,��ת���, 0.0 A
        //2,����ָʾ�Ƶ�Դ, 0.0 A
        //3,�ƶ��ƿ���, 0.0 A
        //4,ǰ�ſ��ŵ�ŷ�, 0.0 A
        //5,ǰ·��, 0.0 A
        //6,ǰ�Ź��ŵ�ŷ�, 0.0 A
        //7,��·��, 0.0 A
        //8,���ſ��ŵ�ŷ�, 0.0 A
        //9,��·��, 0.0 A
        //10,���Ź��ŵ�ŷ�, 0.0 A
        //11,���ӹ�����, 0.0 A
        //12,С�Ƶ�Դ, 0.0 A
        //13,������, 0.0 A
        //14,��ת���, 0.0 A
    U8
		N: 2,                //Ԥ��
		middle_switch6: 1,	//����̤���ƿ���
		middle_switch5: 1,	//ǰ��̤���ƿ���
		middle_switch4: 1,	//��ַѡ��2
		middle_switch3: 1,	//��ַѡ��1
		middle_switch2: 1,	//Ӧ������������     
		middle_switch1: 1;	//���忪��

       
} MiddleModuleFrame;



static MiddleModuleFrame s_middle_module_para;
void get_middle_module_system(void)
{
	U32 u32temp;
	memset(&s_middle_module_para,0,MENU_MIDDLE_MODULE_DATA_LENGTH);
	u32temp = mpcur[0 ];DWORD_WRITE(s_middle_module_para.middle_current[0 ],u32temp);        //0,������, 0.0 A        
	u32temp = mpcur[1 ];DWORD_WRITE(s_middle_module_para.middle_current[1 ],u32temp);        //1,��ת���, 0.0 A        
	u32temp = mpcur[2 ];DWORD_WRITE(s_middle_module_para.middle_current[2 ],u32temp);        //2,����ָʾ�Ƶ�Դ, 0.0 A  
	u32temp = mpcur[3 ];DWORD_WRITE(s_middle_module_para.middle_current[3 ],u32temp);        //3,�ƶ��ƿ���, 0.0 A      
	u32temp = mpcur[4 ];DWORD_WRITE(s_middle_module_para.middle_current[4 ],u32temp);        //4,ǰ�ſ��ŵ�ŷ�, 0.0 A  
	u32temp = mpcur[5 ];DWORD_WRITE(s_middle_module_para.middle_current[5 ],u32temp);        //5,ǰ·��, 0.0 A          
	u32temp = mpcur[6 ];DWORD_WRITE(s_middle_module_para.middle_current[6 ],u32temp);        //6,ǰ�Ź��ŵ�ŷ�, 0.0 A  
	u32temp = mpcur[7 ];DWORD_WRITE(s_middle_module_para.middle_current[7 ],u32temp);        //7,��·��, 0.0 A          
	u32temp = mpcur[8 ];DWORD_WRITE(s_middle_module_para.middle_current[8 ],u32temp);        //8,���ſ��ŵ�ŷ�, 0.0 A  
	u32temp = mpcur[9 ];DWORD_WRITE(s_middle_module_para.middle_current[9 ],u32temp);        //9,��·��, 0.0 A          
	u32temp = mpcur[10];DWORD_WRITE(s_middle_module_para.middle_current[10],u32temp);        //10,���Ź��ŵ�ŷ�, 0.0 A 
	u32temp = mpcur[11];DWORD_WRITE(s_middle_module_para.middle_current[11],u32temp);        //11,���ӹ�����, 0.0 A     
	u32temp = mpcur[12];DWORD_WRITE(s_middle_module_para.middle_current[12],u32temp);        //12,С�Ƶ�Դ, 0.0 A       
	u32temp = mpcur[13];DWORD_WRITE(s_middle_module_para.middle_current[13],u32temp);        //13,������, 0.0 A       
	u32temp = mpcur[14];DWORD_WRITE(s_middle_module_para.middle_current[14],u32temp);        //14,��ת���, 0.0 A   

	s_middle_module_para.middle_switch6 = mKEY.BITS.kl1;      //����̤���ƿ���        
	s_middle_module_para.middle_switch5 = mKEY.BITS.kl15;      //ǰ��̤���ƿ���        
	s_middle_module_para.middle_switch4 = mKEY.BITS.kl14;      //��ַѡ��2             
	s_middle_module_para.middle_switch3 = mKEY.BITS.kl13;      //��ַѡ��1             
	s_middle_module_para.middle_switch2 = mKEY.BITS.kl4;      //Ӧ������������        
	s_middle_module_para.middle_switch1 = mKEY.BITS.kl2;      //���忪��              
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






