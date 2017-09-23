#ifndef APP_MAIN_INTERFACE_H_
#define APP_MAIN_INTERFACE_H_

#include "comm_typedef.h"
typedef union
{
	U8 u8KeyState;
	struct
	{
		U8 ok_key :1,						//key1
		   left_key :1,						//key2
           right_key :1,					//key3
           cancel_key :1,					//key4
           keyRemain :4;					//key0
	} bits;
} Key_Status;
//!<小端数据
#define WORD_WRITE(dest,data)	do{dest[0] = data&0xFF;(dest[1]=data>>8)&0xFF;}while(0)
#define DWORD_WRITE(dest,data)	do{dest[0] = data&0xFF;(dest[1]=data>>8)&0xFF;(dest[2]=data>>16)&0xFF;(dest[3]=data>>24)&0xFF;}while(0)

//!<小端
typedef struct
{
	Key_Status key;					//key1
//key0
	U8 speed;							//车速	1km/h
	U8 rpm[2];							//转速	1r/min
	U8 totalVoltage[2];                 // 0.1V/bit，-10000
	U8 totalCurrent[2];				    //0.1A/bit，-10000
	U8	gear;							/*0000:空挡 0001: 1 档 0010: 2 档 0011: 3 档 0100: 4 档 0101: 5 档 0110: 6 档 0111: 7倒档*/
	U8 odo[4];							//0.4%/bit
	U8 trip[2];							//里程小计	0-9999 <0.1>
	U8 soc;							//系统SOC  0-100
	U8 airPressure1;					//气压1	10 kpa/bit，0
	U8 airPressure2;					//气压2	10 kpa/bit，0
	U8	moter_control_temp;        //电机控制器温度, 1℃/bit，-40 
	U8	moter_temp;                //电机温度, 1℃/bit，-40
	U8
		control_IN08:1, //近光灯开关
		control_IN07:1, //远光灯开关
		control_IN06:1, //小灯开关
		control_IN05:1, //右转向开关
		control_IN04:1, //钥匙ST开关
		control_IN03:1, //保留
		control_IN02:1, //保留
		control_IN01:1; //左转向开关
    U8
		control_IN16:1, //雨刮高档开关
		control_IN15:1, //雨刮间歇档开关
		control_IN14:1, //雨刮低档开关
		control_IN13:1, //保留
		control_IN12:1, //广告灯开关
		control_IN11:1, //保留
		control_IN10:1, //后雾灯开关		
		control_IN09:1; //前雾灯开关
				
	U8
		control_IN24:1, //保留
		control_IN23:1, //保留
        control_IN22:1, //保留
        control_IN21:1, //雨刮喷水开关
        control_IN20:1, //前门开开关
        control_IN19:1, //路牌开关
        control_IN18:1, //保留		
		control_IN17:1; //保留
	U8
        control_IN32:1, //保留
        control_IN31:1, //保留
        control_IN30:1, //保留
        control_IN29:1, //燃油量过低报警
        control_IN28:1, //前门关开关
        control_IN27:1, //中门开开关
        control_IN26:1, //中门关开关
		control_IN25:1; //保留
    U8  		
		N1: 5,			//保留
        control_IN35:1, //钥匙ON开关
        control_IN34:1, //电源唤醒开关
		control_IN33:1; //危险警报开关
	U8
		diagnostic_mode:1,	//诊断模式符号片
        climbing_mode:1,	//爬坡模式符号片
        HV_overhaul:1,		//高压检修指示符号片
        main_power:1,		//总电源指示符号片
        defroster:1,		//除霜器符号片   未定义
        back_door:1,		//后舱门开符号片
        park_brake:1,		//驻车制动符号片
		brake_light:1;		//刹车灯符号片
	U8  
		N2: 2,		        //保留
        ABS:1,				//ABS符号片
        dryer:1,			//干燥器符号片
        Kt:1,				//电池主接触器
        power_mode:1,		//Power模式符号片(纯电动模式 or 混动模式 or 插电模式)
        charge_confirm:1,	//充电确认符号片
		AC:1;				//AC空调符号片
	U8 fault_level[4];	//故障等级	
	U8 fault_code[4];		//故障代码（界面中的系统代码,以10进制显示,代码颜色需随故障等级变化,相见车厂协议文档）
	U8 SPN[4];			//SPN（16进制显示）
	U8 urea_level[4];		//尿素液位, 0.4 %
    U8 dateTime[4];		//时间
    U8 battery[4];           //蓄电池电压
} Main_Interface_Data_Struct;

extern Main_Interface_Data_Struct main_interface_data;

void main_interface_get_data(void);
void main_interface_send_data(void);
#endif
