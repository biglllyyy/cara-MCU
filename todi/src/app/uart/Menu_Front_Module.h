#define MENU_FRONT_MODULE_DATA_LENGTH (sizeof(FrontModuleFrame))
#define MENU_FRONT_MODULE_FRAME_TYPE  FRONT_MODULE_TYPE


/* ǰģ��,��ӦFRONT_MODULE_TYPE */
typedef struct {
    U8 front_current[15][4];
		//0, ��ο쵵���, 0.0 A
        //1, ����������, 0.0 A
        //2, ��ˮ���, 0.0 A
        //3, ��ǰ���, 0.0 A
        //4, ������, 0.0 A
        //5, ��ת���, 0.0 A
        //6, ��ת���, 0.0 A
        //7, Һλ��ʾ��Դ, 0.0 A
        //8, ��Զ���, 0.0 A
        //9, ��ǰ���, 0.0 A
        //10, ��Զ���, 0.0 A
        //11, �г���¼�ǵ�Դ, 0.0 A
        //12, ������, 0.0 A
        //13, λ�õ�, 0.0 A
        //14, �ҽ����, 0.0 A
    U8
		N: 2,                //Ԥ��
		front_switch6: 1,	//�����ƿ���
		front_switch5: 1,	//��ַѡ��2
		front_switch4: 1,	//��ַѡ��1
		front_switch3: 1,	//�ƶ��ƿ���
		front_switch2: 1,	 //��ǰĦ��Ƭ��������		 
		front_switch1: 1;	//��ǰĦ��Ƭ��������
}FrontModuleFrame;


static FrontModuleFrame s_front_module_para;
void get_front_module_system(void)
{
	U32 u32temp;
	memset(&s_front_module_para,0,MENU_FRONT_MODULE_DATA_LENGTH);
	u32temp = fpcur[0];DWORD_WRITE(s_front_module_para.front_current[0 ],u32temp);	//0, ��ο쵵���, 0.0 
	u32temp = fpcur[1];DWORD_WRITE(s_front_module_para.front_current[1 ],u32temp);	//1, ����������, 0.0 
	u32temp = fpcur[2];DWORD_WRITE(s_front_module_para.front_current[2 ],u32temp);	//2, ��ˮ���, 0.0 A   
	u32temp = fpcur[3];DWORD_WRITE(s_front_module_para.front_current[3 ],u32temp);	//3, ��ǰ���, 0.0 A   
	u32temp = fpcur[4];DWORD_WRITE(s_front_module_para.front_current[4 ],u32temp);	//4, ������, 0.0 A	   
	u32temp = fpcur[5];DWORD_WRITE(s_front_module_para.front_current[5 ],u32temp);	//5, ��ת���, 0.0 A   
	u32temp = fpcur[6];DWORD_WRITE(s_front_module_para.front_current[6 ],u32temp);	//6, ��ת���, 0.0 A   
	u32temp = fpcur[7];DWORD_WRITE(s_front_module_para.front_current[7 ],u32temp);	//7, Һλ��ʾ��Դ, 0.0 
	u32temp = fpcur[8];DWORD_WRITE(s_front_module_para.front_current[8 ],u32temp);	//8, ��Զ���, 0.0 A   
	u32temp = fpcur[9];DWORD_WRITE(s_front_module_para.front_current[9 ],u32temp);	//9, ��ǰ���, 0.0 A   
	u32temp = fpcur[10];DWORD_WRITE(s_front_module_para.front_current[10],u32temp);	//10, ��Զ���, 0.0 A  
	u32temp = fpcur[11];DWORD_WRITE(s_front_module_para.front_current[11],u32temp);	//11, �г���¼�ǵ�Դ, 0
	u32temp = fpcur[12];DWORD_WRITE(s_front_module_para.front_current[12],u32temp);	//12, ������, 0.0 A  
	u32temp = fpcur[13];DWORD_WRITE(s_front_module_para.front_current[13],u32temp);	//13, λ�õ�, 0.0 A    
	u32temp = fpcur[14];DWORD_WRITE(s_front_module_para.front_current[14],u32temp);	//14, �ҽ����, 0.0 A  

	s_front_module_para.front_switch6 = fKEY.BITS.kl1; //�����ƿ���      
	s_front_module_para.front_switch5 = fKEY.BITS.kl14; //��ַѡ��2       
	s_front_module_para.front_switch4 = fKEY.BITS.kl13; //��ַѡ��1       
	s_front_module_para.front_switch3 = fKEY.BITS.kl6; //�ƶ��ƿ���      
	s_front_module_para.front_switch2 = fKEY.BITS.kl5; //��ǰĦ��Ƭ������
	s_front_module_para.front_switch1 = fKEY.BITS.kl4; //��ǰĦ��Ƭ������
	

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

