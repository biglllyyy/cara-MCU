#ifndef APP_WARNING_H
#define APP_WARNING_H
#include "comm_typedef.h"

typedef struct
{
    U8 low_water_level_warn;			//水位低报警
    U8 low_ap1_warn;                    //左气压低报警
    U8 low_ap2_warn;                    //右气压低报警
    U8 water_temp_sensor_warn;			//水温传感器报警
    U8 oil_quantity_sensor_warn;		//油量传感器报警
    U8 oil_pressure_sensor_warn;		//机油压力传感器报警
    U8 bat_voltage_sensor_warn;		    //电池电压传感器报警
    U8 ap1_sensor_warn;				    //气压一传感器报警
    U8 ap2_sensor_warn;				    //气压二传感器报警
    U8 j1939_comm_fault_warn;			//J1939通信故障
    U8 low_oil_pressure_warn;			//机油压力低
                                            //油量低报警（油量传感器电阻值低于15±3Ω时, 红色报警灯常亮）
                                            //水温高报警（水温来自总线。总线故障时闪烁报警,水温高于98度时常亮）
    U8 left_brake_shoe_warn;			//左刹车蹄片报警
    U8 right_brake_shoe_warn;			//右刹车蹄片报警
    U8 air_filter_warn;				    //空滤报警
    U8 after_treat_warn;				//后处理故障
	U8 bat_status1;				//电池状态1, 00000000（2进制显示）
								//-8	 电池不均衡故障
								//-7	 总电压过压
								//-6	 总电压欠压
								//-5	 内部通讯失败
								//-4	 SOC过低
								//-3	 SOC过高
								//-2	 单体模块电压过低
								//-1	 单体模块电压过高
    U8 bat_status2;				//电池状态2, 00000000（2进制显示）
							     //-8	温度差过大
								 //-7	温度报警
								 //-6	温度过高
								 //-5	充电电流过大
								 //-4	放电电流过大
								 //-3	绝缘报警
								 //-2	DCDC故障报警（1故障，0正常）
								 //-1	电池热失控
    U8 bat_status3;				//电池状态3, 00000000（2进制显示）
							    //-8	保留
								//-7	
								//-6	
								//-5	
								//-4	有容量偏小电池
								//-3	有内阻偏大电池
								//-2	内部接触器信号
								//-1	 
} WarningDataStruct;




extern WarningDataStruct warning_data;
void warning_get_data(void);
void warning_send_data(void);
#endif
