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

typedef struct
{
	union {
		struct {
			U8
			out_status8 :1,
			out_status7 :1,
			out_status6 :1,
			out_status5 :1,
			out_status4 :1,
			out_status3 :1,
			out_status2 :1,
			out_status1 :1;
		}bits;
		U8 byte;
	}out1_8;

	union {
		struct {
			U8
			out_resever :1,
			out_status15 :1,
			out_status14 :1,
			out_status13 :1,
			out_status12 :1,
			out_status11 :1,
			out_status10 :1,
			out_status09 :1;
		}bits;
		U8 byte;
	}out9_16;

U8
	switch23: 1,
	switch22: 1,
	switch21: 1,
	switch20: 1,
	switch19: 1,
	switch18: 1,
	switch17: 1,
	switch16: 1;

U8
	switch31: 1,
	switch30: 1,
	switch29: 1,
	switch28: 1,
	switch27: 1,
	switch26: 1,
	switch25: 1,
	switch24: 1;

U8
	switch39: 1,
	switch38: 1,
	switch37: 1,
	switch36: 1,
	switch35: 1,
	switch34: 1,
	switch33: 1,
	switch32: 1;

U8
	switch_reserve: 5,
	switch42: 1,
	switch41: 1,
	switch40: 1;

U8  version;
}moudle_struct;

//!<小端
typedef struct
{
	Key_Status key;					//key1
//key0
	U8 speed;							//车速	1km/h
	U8 rpm[2];							//转速	1r/min
	U8 totalVoltage[2];                 //总电压 0.1V/bit，-10000
	U8 totalCurrent[2];				    //总电流 0.1A/bit，-10000
	U8 gear;							/*0000:空挡 0001: 1 档 0010: 2 档 0011: 3 档 0100: 4 档 0101: 5 档 0110: 6 档 0111: 7倒档*/
	U8 odo[4];							
	U8 trip[2];							//里程小计	0-9999 <0.1>
	U8 soc;							//系统SOC  0.4%/bit
	U8 airPressure1[2];					//气压1	10 kpa/bit，0
	U8 airPressure2[2];					//气压2	10 kpa/bit，0
	U8	moter_control_temp[2];        //电机控制器温度, 1℃/bit，-40 
	U8	moter_temp[2];                //电机温度, 1℃/bit，-40
    U8
        control_IN08:1, //近光灯开关
		control_IN07:1, //远光灯开关
        control_IN06:1, //小灯开关
		control_IN05:1, //右转向开关
        control_IN04:1, //钥匙ON开关
        control_IN03:1, //保留
        control_IN02:1, //保留
		control_IN01:1; //左转向开关
    U8
		control_IN16:1, //雨刮高档开关
        control_IN15:1, //雨刮间歇档开关
		control_IN14:1, //雨刮低档开关
        control_IN13:1, //保留
		control_IN12:1, //保留
		control_IN11:1, //保留
		control_IN10:1, //后雾灯开关
        control_IN09:1; //前雾灯开关
	U8
        control_IN24:1, //保留
		control_IN23:1, //保留
        control_IN22:1, //保留
		control_IN21:1, //雨刮喷水开关
        control_IN20:1, //前门开开关
        control_IN19:1, //保留
        control_IN18:1, //保留
		control_IN17:1; //保留
	U8
		control_IN32:1, //保留
        control_IN31:1, //保留
        control_IN30:1, //保留
        control_IN29:1, //保留
        control_IN28:1, //前门关开关
		control_IN27:1, //中门开开关
		control_IN26:1, //中门关开关
		control_IN25:1; //保留
    U8
		N1: 5,			//保留
		control_IN35:1, //保留
        control_IN34:1, //保留
		control_IN33:1; //保留
	U8 systemCode[2];	//系统代码
	U8 charge_status;	//充电状态
    U8 dateTime[4];		//时间
    U8 battery[4];      //蓄电池电压
	U8 version;			//版本号

	moudle_struct front_moudle;
	moudle_struct middle_moudle;
	moudle_struct back_moudle;
} Main_Interface_Data_Struct;

extern Main_Interface_Data_Struct main_interface_data;

void main_interface_get_data(void);
void main_interface_send_data(void);
#endif
