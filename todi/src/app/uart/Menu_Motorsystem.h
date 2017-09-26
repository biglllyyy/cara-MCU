#define MENU_MOTORSYSTEM_DATA_LENGTH (sizeof(EngineSystemInfoFrame))
#define MENU_MOTORSYSTEM_FRAME_TYPE  ENGINE_SYSTEM_INFO_TYPE


/* 发动机系统信息,对应ENGINE_SYSTEM_INFO_TYPE */
typedef struct {
    U8 engine_fuel_consumption[2];         //发动机油耗, 0.00 L/H 12.8L/h /bit

    U8 engine_load;					    //发动机负荷, 0 %
    //int32_t urea_level;					//尿素液位, 0.0 %,通用帧已存在该变量
    U8 engine_target_throttle[4];			//发动机目标油门, 0.0 %
    U8 engine_actual_throttle[4];			//发动机实际油门, 0.0 % 0.4%/bit
    U8 engine_oil_pressure[4];			//发动机机油压力, Kpa 4kPa/bit
    U8 engine_inlet_temp[2];				//发动机进气温度, 0 ℃

} EngineSystemInfoFrame;
static EngineSystemInfoFrame s_engine_para;
void get_engine_system(void)
{
	U32 u32temp;
	memset(&s_engine_para,0,MENU_CONTROLSYSTEM_DATA_LENGTH);
	u32temp = Fule_rate;WORD_WRITE(s_engine_para.engine_fuel_consumption,u32temp);
	
	s_engine_para.engine_load = Engine_Fuhe;
	u32temp = Accelerator;DWORD_WRITE(s_engine_para.engine_target_throttle,u32temp);
	u32temp = Accelerator_Shiji;DWORD_WRITE(s_engine_para.engine_actual_throttle,u32temp);
	u32temp = Engine_oil_press;DWORD_WRITE(s_engine_para.engine_oil_pressure,u32temp);
	u32temp = ambient_air_temperature;WORD_WRITE(s_engine_para.engine_inlet_temp,u32temp);
}
void send_engine_system(void)
{
	U8 data[MENU_MOTORSYSTEM_DATA_LENGTH + A20_MCU_DATA_LENTH];
	U32 parse_len;
    parse_len = app_uart_arm_send_parse(data,(void*)&s_engine_para,MENU_MOTORSYSTEM_FRAME_TYPE,MENU_MOTORSYSTEM_DATA_LENGTH);
	sent_data(UART_A20_CHN, data, parse_len); /* data sent */
}

void menu_engine_system_Task(void)
{
	get_engine_system();
	send_engine_system();
}

