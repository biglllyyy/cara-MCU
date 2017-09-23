#define MENU_AUXILIARYSYSTEM_DATA_LENGTH (sizeof(AssistSystemInfoFrame))
#define MENU_AUXILIARYSYSTEM_FRAME_TYPE  ASSIST_SYSTEM_INFO_TYPE


/* 辅助系统信息,对应ASSIST_SYSTEM_INFO_TYPE */
typedef struct {
    U8 oilpump_dcac_w_output_current[4];		//油泵DC/AC W相输出电流, 0 A 0.1A/bit，-10000
    U8 oilpump_dcac_v_output_current[4];		//油泵DC/AC V相输出电流, 0 A 0.1A/bit，-10000
    U8 oilpump_dcac_u_output_current[4];		//油泵DC/AC U相输出电流, 0 A 0.1A/bit，-10000
    int8_t radiator_temp;						//散热器温度, 0 ℃ 1℃/bit，-40
    //int32_t fault_code;						//故障代码, 0,通用帧已存在该变量
} AssistSystemInfoFrame;

static AssistSystemInfoFrame s_assist_para;
void get_assist_system(void)
{
	U32 u32temp;
	memset(&s_assist_para,0,MENU_AUXILIARYSYSTEM_DATA_LENGTH);
	u32temp = DCAC_W;
	DWORD_WRITE(s_assist_para.oilpump_dcac_w_output_current,u32temp);
	u32temp = DCAC_V;
	DWORD_WRITE(s_assist_para.oilpump_dcac_v_output_current,u32temp);
	u32temp = DCAC_U;
	DWORD_WRITE(s_assist_para.oilpump_dcac_u_output_current,u32temp);
	s_assist_para.radiator_temp = Sanreqi_t;
}
void send_assist_system(void)
{
	U8 data[MENU_MOTORSYSTEM_DATA_LENGTH + 6];
	U32 parse_len;
    parse_len = app_uart_arm_send_parse(data,(void*)&s_assist_para,MENU_AUXILIARYSYSTEM_FRAME_TYPE,MENU_AUXILIARYSYSTEM_DATA_LENGTH);
	sent_data(UART_A20_CHN, data, parse_len); /* data sent */
}

void menu_assist_system_Task(void)
{
	get_assist_system();
	send_assist_system();
}


