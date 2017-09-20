#define MENU_BMS_INFO_DATA_LENGTH (sizeof(BmsInfoFrame))
#define MENU_BMS_INFO_FRAME_TYPE  BMS_INFO_TYPE


/* ��ع���ϵͳ��Ϣ,��ӦBMS_INFO_TYPE */
typedef struct {
    //int16_t totalVoltage;				//����ܵ�ѹ, 0.0 V,ͨ��֡�Ѵ��ڸñ���
    U8 bat_max_charge_current[2];	    //������������,, 0.0 A
    U8 bat_max_discharge_current[2];	//�������ŵ����,, 0.0 A
    U8 bat_average_voltage[4];		//����ƽ����ѹ, 0.00 V
    U8 bat_status1[4];				//���״̬1, 00000000��2������ʾ��
    U8 bat_status2[4];				//���״̬2, 00000000��2������ʾ��
    U8 bat_status3[4];				//���״̬3, 00000000��2������ʾ��
    U8 irm_ohm_positive[4];			//������Ե��ֵ, 0 K��
    U8 irm_ohm_negative[4];			//�ܸ���Ե��ֵ, 0 K��
    U8 bat_max_voltage[4];			//������ߵ�ѹ, 0.00 V
    U8 bat_min_voltage[4];			//������͵�ѹ, 0.00 V
    U8 batteryTH[4];					//��������¶�, 0.0 ��
    //int16_t totalCurrent;				//��ŵ����, 0 A,ͨ��֡�Ѵ��ڸñ���
} BmsInfoFrame;



static BmsInfoFrame s_bms_info_para;
void get_bms_info_system(void)
{
	memset(&s_bms_info_para,0,MENU_BMS_INFO_DATA_LENGTH);
}
void send_bms_info_system(void)
{
	U8 data[MENU_MOTORSYSTEM_DATA_LENGTH + 6];
	U32 parse_len;
    parse_len = app_uart_arm_send_parse(data,(void*)&s_bms_info_para,MENU_BMS_INFO_FRAME_TYPE,MENU_BMS_INFO_DATA_LENGTH);
	sent_data(UART_A20_CHN, data, parse_len); /* data sent */
}

void menu_bms_info_system_Task(void)
{
	get_bms_info_system();
	send_bms_info_system();
}



