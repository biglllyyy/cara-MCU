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
	memset(&s_middle_module_para,0,MENU_MIDDLE_MODULE_DATA_LENGTH);
}
void send_middle_module_system(void)
{
	U8 data[MENU_MOTORSYSTEM_DATA_LENGTH + 6];
	U32 parse_len;
    parse_len = app_uart_arm_send_parse(data,(void*)&s_middle_module_para,MENU_MIDDLE_MODULE_FRAME_TYPE,MENU_MIDDLE_MODULE_DATA_LENGTH);
	sent_data(UART_A20_CHN, data, parse_len); /* data sent */
}

void menu_middle_module_system_Task(void)
{
	get_middle_module_system();
	send_middle_module_system();
}






