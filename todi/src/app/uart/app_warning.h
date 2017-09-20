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
} WarningDataStruct;



extern WarningDataStruct warning_data;
void warning_get_data(void);
void warning_send_data(void);
#endif
