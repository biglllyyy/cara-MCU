#define MENU_TCUSYSTEM_DATA_LENGTH (sizeof(TCUSystemInfoFrame))
#define MENU_TCUSYSTEM_FRAME_TYPE  TCU_SYSTEM_INFO_TYPE


/* TCUϵͳ��Ϣ,��ӦTCU_SYSTEM_INFO_TYPE */
typedef struct {
    U8 actual_clutch_position;		//���ʵ��λ��, 0
    //int8_t  gear;               		//��λ��λ, 0,ͨ��֡�Ѵ��ڸñ���
    U8 TCU_fault_code[4];				//TCU������Ϣ, 00000000��2������ʾ��
    U8 axis_rpm[2];					//�����������ת��, 0 RPM
    U8 FMI[4];						//FMI, 0
    //int32_t SPN;						//SPN, 0��16������ʾ��,ͨ��֡�Ѵ��ڸñ���
    U8 CM[4];							//CM, 0
    U8 OC[4];							//OC, 0��16������ʾ��
}TCUSystemInfoFrame;


static TCUSystemInfoFrame s_TCU_para;
void get_TCU_system(void)
{
	U32 u32temp;
	memset(&s_TCU_para,0,MENU_TCUSYSTEM_DATA_LENGTH);
	s_TCU_para.actual_clutch_position = TCU_Position;
	u32temp = TCU_warn;DWORD_WRITE(s_TCU_para.TCU_fault_code,u32temp);
	u32temp = Speed_rmp;WORD_WRITE(s_TCU_para.axis_rpm,u32temp);
	u32temp = tcu_fmi;DWORD_WRITE(s_TCU_para.FMI,u32temp);
	u32temp = tcu_cm;DWORD_WRITE(s_TCU_para.CM,u32temp);
	u32temp = tcu_oc;DWORD_WRITE(s_TCU_para.OC,u32temp);
}
void send_TCU_system(void)
{
	U8 data[MENU_MOTORSYSTEM_DATA_LENGTH + 6];
	U32 parse_len;
    parse_len = app_uart_arm_send_parse(data,(void*)&s_TCU_para,MENU_TCUSYSTEM_FRAME_TYPE,MENU_TCUSYSTEM_DATA_LENGTH);
	sent_data(UART_A20_CHN, data, parse_len); /* data sent */
}

void menu_TCU_system_Task(void)
{
	get_TCU_system();
	send_TCU_system();
}


