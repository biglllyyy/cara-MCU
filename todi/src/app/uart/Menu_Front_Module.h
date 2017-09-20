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

