

#define MENU_CONTROLSYSTEM_DATA_LENGTH (sizeof(ControlSystemInfoFrame))
#define MENU_CONTROLSYSTEM_FRAME_TYPE  CONTROL_SYSTEM_INFO_TYPE


/* ����ϵͳ��Ϣ,��ӦCONTROL_SYSTEM_INFO_TYPE */
typedef struct {
    U8 motor_in_voltage[4];			//��������ѹ, 0 V
    U8 motor_in_current[4];			//����������, 0 A
    U8 motor_temp;					//����¶�, 0 ��
    U8 motor_controller_temp;		//����������¶�, 0 ��
    U8 car_mode[4];					//����ģʽ, 0
    U8 version[4];					//����汾, 0.00
    U8 car_Life[4];					//����������Life, 0
    U8 accel_pedal_percent[4];	    //����̤��ٷֱ�,0.0 %
    U8 brake_pedal_percent[4];	    //�ƶ�̤��ٷֱ�,0.0 %
    U8 din1[4];						//����������1, 00000000��2������ʾ��
    U8 din2[4];						//����������2, 00000000��2������ʾ��
    U8 din3[4];						//����������3, 00000000��2������ʾ��
    U8 dout1[4];						//���������1, 00000000��2������ʾ��
    U8 dout2[4];						//���������2, 00000000��2������ʾ��
    U8 dout3[4];						//���������3, 00000000��2������ʾ��
} ControlSystemInfoFrame;
static ControlSystemInfoFrame s_control_para;
void get_control_system(void)
{
	memset(&s_control_para,0,MENU_CONTROLSYSTEM_DATA_LENGTH);
}
void send_control_system(void)
{
	U8 data[MENU_CONTROLSYSTEM_DATA_LENGTH + 6];
	U32 parse_len;
    parse_len = app_uart_arm_send_parse(data,(void*)&s_control_para,MENU_CONTROLSYSTEM_FRAME_TYPE,MENU_CONTROLSYSTEM_DATA_LENGTH);
	sent_data(UART_A20_CHN, data, parse_len); /* data sent */
}

void menu_control_system_Task(void)
{
	get_control_system();
	send_control_system();
}



