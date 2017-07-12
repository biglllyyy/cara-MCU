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
				right_key :1,						//key3
				cancel_key :1,						//key4
				keyRemain :4;						//key0
	} bits;
} Key_Status;

typedef union
{
	U32 data;
	struct
	{
		U32 odo :20,						//总里程	0-999999
				gear :2,							//档位	0：P档，1：R档，2：N档，3：D档
				totalVoltage :10;					//总电压 0-1000

	} bits;
} Odo_Gear_TotalVol;

typedef union
{
	U32 can_valid;
		struct
		{
			U32 CAN_ID_REVERSED1 :1,    //0无效 1有效
				CAN_ID_REVERSED2 :1,
				CAN_ID_18AA28F3 :1,
				CAN_ID_1818D0F3 :1,
				CAN_ID_1819D0F3 :1,
				CAN_ID_181AD0F3 :1,
				CAN_ID_181BD0F3 :1,
				CAN_ID_181CD0F3 :1,
				CAN_ID_181DD0F3 :1,
				CAN_ID_181ED0F3 :1,
				CAN_ID_181FD0F3 :1,
				/*BMS Interaction*/
				//CAN_ID_1800F328,  send remove else will crash
				CAN_ID_180028F3 :1,
				CAN_ID_180028F4 :1,
				/*电机*/
				//CAN_ID_1882BBAB,  send remove else will crash
				CAN_ID_1002FF1E :1,
				CAN_ID_1003FF1E :1,
				CAN_ID_1004FF1E :1,
				/*气泵*/
				//CAN_ID_18FF1103,
				CAN_ID_1429289B :1,
				CAN_ID_142A289B :1,
				//CAN_ID_18AB9B28,   send remove else will crash
				/*仪表与远程通讯*/
				//CAN_ID_18FFA017,    send remove else will crash
				/*DC-DC电源*/
				CAN_ID_1828272B :1,
				/*油泵控制器*/
				CAN_ID_142A2895 :1,
				CAN_ID_reversed:12;

		} bits;



} Can_Valid_Bits;

typedef union
{
	U32 u32_switch_capture;
		struct
		{
		   U32 MCU_IN1:1,     //左转向开关 4
			   MCU_IN2:1,     //保留开关   3
			   MCU_IN3:1,     //中门开状态 2
			   MCU_IN4 :1,   //前门开状态 (测试板上的1)
			   MCU_IN5 :1,   //右转向开关
			   MCU_IN6 :1,   //小灯开关
			   MCU_IN7 :1,   //远光灯开关
			   MCU_IN8 :1,   //近光灯开关
			   MCU_IN9 :1,   //前雾灯开关
			   MCU_IN10:1,    //后雾灯开关
			   MCU_IN11:1,    //前气压报警开关（仪表蜂鸣提醒并图标显示）
			   MCU_IN12:1,    //后气压报警开关（仪表蜂鸣提醒并图标显示）
			   MCU_IN13:1,   //保留
			   MCU_IN14:1,    //雨刮低档开关
			   MCU_IN15:1,    //雨刮间歇档开关
			   MCU_IN16:1,    //雨刮高档开关
			   MCU_IN17:1,   //雨刮喷水档开关
			   MCU_IN18:1,    //
			   MCU_IN19:1,    //厢灯1开关
			   MCU_IN20:1,    //厢灯2开关
			   MCU_IN21:1,    //
			   MCU_IN22:1,    //司机风扇开关
			   MCU_IN23:1,   //路牌开关
			   MCU_IN24:1,    //
			   MCU_IN25:1,    //电控排污阀开关
			   MCU_IN26:1,    //强制行车开关
			   MCU_IN27:1,    //手制动信号
			   MCU_IN28:1,   //司机灯开关
			   MCU_IN29:1,   //
			   MCU_IN30:1,    //
			   MCU_IN31:1,    //安全带开关
			   MCU_IN32:1;     //
	} bits;

}Switch_Capture;

typedef union
{
	U8 u8MotorContStatus;
	struct
	{
		U8 bit1to3 :3,						//key1
				bit4to5 :2,						//key2
				bit6to8 :3;					//key3
	} bits;
}MotorContStatus;


typedef struct
{
	Key_Status key;					//key1
//key0
	U8 speed;							//车速	0-140
	U16 rpm;							//转速	-20000 ~ 45531


	U32 odo;	
	
	U16 totalVoltage;
	U8  gear;
	U8  reversed1;
	

	//Odo_Gear_TotalVol Odo_gear_totalVol;

	U16 totalCurrent;                  //总电流	0-1000 <-500>
	U16 trip;							//里程小计	0-9999 <0.1>

	U8 soc;							//系统SOC  0-100
	U8 ready_signal;						//ready
	U8 airPressure1;					//气压1	0-100
	U8 airPressure2;					//气压2	0-100

	U8 batteryTH;						//单体电池最高温度	0-240 <-40>
	U8 batteryTL;						//单体电池最低温度	0-240 <-40>
	U16 batteryVH;						//单体电池最高电压	0-50 <0.1>

	U16 batteryVL;						//单体电池最低电压	0-50 <0.1>
	U8  moter_control_temp;
	U8  moter_temp;
	
	Switch_Capture switch_capture;
	
	U16  tm_zhuanju_nm;
	U16  reversed2;

	

} Main_Interface_Data_Struct;

extern Main_Interface_Data_Struct main_interface_data;

void main_interface_get_data(void);
void main_interface_send_data(void);
#endif
