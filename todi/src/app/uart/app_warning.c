#include "app_warning.h"
#include "app_can.h"
#include "app_uart_cmd.h"
#include "hal_uart.h"
#include "string.h"
#include "crc16.h"
#include "mid_string.h"
#include "app_moudle.h"
#include "uart_cfg.h"



WarningDataStruct warning_data = { 0 };
void warning_get_data(void)
{
	//!<TODO:按协议填充信息
	warning_data.low_water_level_warn     = 0;   //水位低报警                                                       
	warning_data.low_ap1_warn             = 0;   //左气压低报警                                                     
	warning_data.low_ap2_warn             = 0;   //右气压低报警                                                     
	warning_data.water_temp_sensor_warn   = Vcan_timeout_cfg;   //水温传感器报警                                                   
	warning_data.oil_quantity_sensor_warn = 0;   //油量传感器报警                                                   
	warning_data.oil_pressure_sensor_warn = 0;   //机油压力传感器报警                                               
	warning_data.bat_voltage_sensor_warn  = 0;   //电池电压传感器报警                                               
	warning_data.ap1_sensor_warn          = 0;   //气压一传感器报警                                                 
	warning_data.ap2_sensor_warn          = 0;   //气压二传感器报警                                                 
	warning_data.j1939_comm_fault_warn    = 0;   //J1939通信故障                                                    
	warning_data.low_oil_pressure_warn    = ((Engine_oil_press * 4 < 100) && (Engine_Speed > 0) && M_ON);   //机油压力低                                                       
	                                             //油量低报警（油量传感器电阻值低于15±3Ω时, 红色报警灯常亮）      
	                                             //水温高报警（水温来自总线。总线故障时闪烁报警,水温高于98度时常亮）
	warning_data.left_brake_shoe_warn     = 0;   //左刹车蹄片报警                                                   
	warning_data.right_brake_shoe_warn    = 0;   //右刹车蹄片报警                                                   
	warning_data.air_filter_warn          = 0;   //空滤报警                                                         
	warning_data.after_treat_warn         = 0;   //后处理故障 

	warning_data.bat_status1 = Status_Flag1;
	warning_data.bat_status2 = Status_Flag2;
	warning_data.bat_status3 = Status_Flag3;
}

#define WARNING_DATA_LENGTH   (sizeof(WarningDataStruct))
#define WARNING_FRAME_TYPE    WARNING_TYPE

#if 0
extern U8 testdata[100];
extern U8 testindex;
#endif
void warning_send_data(void)
{


	U8 data[WARNING_DATA_LENGTH + A20_MCU_DATA_LENTH];
	U32 parse_len;
    parse_len = app_uart_arm_send_parse(data,(void*)&warning_data,WARNING_FRAME_TYPE,WARNING_DATA_LENGTH);
	sent_data(UART_A20_CHN, data, parse_len); /* data sent */
}

