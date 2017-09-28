

#define MENU_CONTROLSYSTEM_DATA_LENGTH (sizeof(ControlSystemInfoFrame))
#define MENU_CONTROLSYSTEM_FRAME_TYPE  CONTROL_SYSTEM_INFO_TYPE


/* 控制系统信息,对应CONTROL_SYSTEM_INFO_TYPE */
typedef struct {
    U8 motor_in_voltage[4];			//电机输入电压, 0 V 0～999V，-10000，0.1V/位
    U8 motor_in_current[4];			//电机输入电流, 0 A -999～999A，-10000，0.1A/位
    U8 motor_temp[2];					//电机温度, 0 ℃ 1℃/bit，-40
    U8 motor_controller_temp[2];		//电机控制器温度, 0 ℃ 
    U8 car_mode[4];					//整车模式, 0
    U8 version[4];					//程序版本, 0.00
    U8 car_Life[4];					//整车控制器Life, 0
    U8 accel_pedal_percent[4];	    //加速踏板百分比,0.0 %
    U8 brake_pedal_percent[4];	    //制动踏板百分比,0.0 %
    U8 din1[4];						//数字量输入1, 00000000（2进制显示）
    U8 din2[4];						//数字量输入2, 00000000（2进制显示）
    U8 din3[4];						//数字量输入3, 00000000（2进制显示）
    U8 dout1[4];						//数字量输出1, 00000000（2进制显示）
    U8 dout2[4];						//数字量输出2, 00000000（2进制显示）
    U8 dout3[4];						//数字量输出3, 00000000（2进制显示）
} ControlSystemInfoFrame;
static ControlSystemInfoFrame s_control_para;
void get_control_system(void)
{
	U32 u32temp;
	memset(&s_control_para,0,MENU_CONTROLSYSTEM_DATA_LENGTH);
	u32temp = moto_voltage;DWORD_WRITE(s_control_para.motor_in_voltage,u32temp);
	u32temp = moto_current;DWORD_WRITE(s_control_para.motor_in_current,u32temp);
	u32temp = Motor_Temperature;WORD_WRITE(s_control_para.motor_temp,u32temp);
	u32temp = Inverter_t;WORD_WRITE(s_control_para.motor_controller_temp,u32temp);
	u32temp = Car_mode;DWORD_WRITE(s_control_para.car_mode,u32temp);
	u32temp = Program;DWORD_WRITE(s_control_para.version,u32temp);
	u32temp = Car_LIFE;DWORD_WRITE(s_control_para.car_Life,u32temp);
	u32temp = Speed_percent;DWORD_WRITE(s_control_para.accel_pedal_percent,u32temp);
	u32temp = Brake_percent;DWORD_WRITE(s_control_para.brake_pedal_percent,u32temp);
	u32temp = 0;DWORD_WRITE(s_control_para.din1,u32temp);
	u32temp = 0;DWORD_WRITE(s_control_para.din2,u32temp);
	u32temp = Digit_input3;DWORD_WRITE(s_control_para.din3,u32temp);
	u32temp = CAN_KEY[2].byte;DWORD_WRITE(s_control_para.dout1,u32temp);
	u32temp = 0;DWORD_WRITE(s_control_para.dout2,u32temp);
	u32temp = 0;DWORD_WRITE(s_control_para.dout3,u32temp);

}
void send_control_system(void)
{
	U8 data[MENU_CONTROLSYSTEM_DATA_LENGTH + A20_MCU_DATA_LENTH];
	U32 parse_len;
    parse_len = app_uart_arm_send_parse(data,(void*)&s_control_para,MENU_CONTROLSYSTEM_FRAME_TYPE,MENU_CONTROLSYSTEM_DATA_LENGTH);
	sent_data(UART_A20_CHN, data, parse_len); /* data sent */
}

void menu_control_system_Task(void)
{
	get_control_system();
	send_control_system();
}



