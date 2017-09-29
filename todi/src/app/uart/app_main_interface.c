#include "app_main_interface.h"
#include "app_can.h"
#include "app_uart_cmd.h"
#include "hal_uart.h"
#include "string.h"
#include "crc16.h"
#include "mid_string.h"
#include "app_can.h"
#include "app_moudle.h"
#include "app_info.h"
#include "mid_can.h"
#include "mb91520.h"
#include "uart_cfg.h"
#include "app_trip.h"
#include "g_variable.h"
#include "mid_filter.h"
#include "app_ad_capture.h"
#include "io_cfg.h"
#include "mid_switch.h"


Main_Interface_Data_Struct main_interface_data =
{ 0 };

#define MAIN_INTERFACE_DATA_LENGTH (sizeof(Main_Interface_Data_Struct))
#define MAIN_INTERFACE_FRAME_TYPE  CAR_INFO_TYPE

extern U16 ad_data;
U32 ad_data_group[4];

extern pin_filter_t    pin_filter_in[];

void main_interface_get_data(void)
{
	U16 buf1, buf2;
	U32 temp = 0;
	U16 temp1 = 0;
	U8  temp2 = 0;
	U8  tempSpeed = 0;
	//按键
	main_interface_data.key.u8KeyState = key_info_value; /* 按键状态 */

	//车速

	temp1 = (unsigned int) (Speed_rmp * 0.508 * 2 * 60 * 3.14159 / 1000 / 5.63 * 1.05); //半径*2*π*60/1000/减速比

//	tempSpeed = pSpeed;  //车速单位1KM/H
	if (temp1 > 140)
	{
		temp1 = 140;
	}
	pSpeed = temp1;
	main_interface_data.speed = temp1; /* 车速0-140km/h */


	//电机转速
	temp1 = moto_speed;
	if (temp1 > 4000)
	{
		temp1 = 4000;
	}
	WORD_WRITE(main_interface_data.rpm,temp1);
	//总电压
	buf1 = BMS_V;  //总电压
	WORD_WRITE(main_interface_data.totalVoltage,buf1);

	//总电流
	buf1 = BMS_A;
	WORD_WRITE(main_interface_data.totalCurrent,buf1);
	//档位
	main_interface_data.gear = Current_Gear;  /*0000:空挡 0001: 1 档 0010: 2 档 0011: 3 档 0100: 4 档 0101: 5 档 0110: 6 档 0111: 7倒档*/
	//总里程
	temp = (info.Odo); 
	DWORD_WRITE(main_interface_data.odo,temp);

	
	//小计里程
	buf1 = info.Trip1;
	WORD_WRITE(main_interface_data.trip,buf1);

	//SOC
	temp2 = BMS_SOC;
	if(temp2>250)
	{
		temp2 = 250;
	}
	main_interface_data.soc = temp2; /* Soc 百分之0-100  */


	//气压 ，
//	temp = ADV[MCU_SER1]*3300/4096;
//	PRESS[0] = temp;
//	temp = ADR[MCU_SER2]*3300/4096;
//	PRESS[1] = temp;
//	temp = PRESS[0];WORD_WRITE(main_interface_data.airPressure1,temp);
//	temp = PRESS[1];WORD_WRITE(main_interface_data.airPressure2,temp);
	if ( ADV[MCU_SER1] == 4096)
	{
		ADR[MCU_SER1] = 0XFFFF;
	}
	else
	{
		temp=ADV[MCU_SER1]*100/(4096-ADV[MCU_SER1]);
		ADR[MCU_SER1] = temp;
	}
	if (ADR[MCU_SER1] < 10)
	{
		ADR[MCU_SER1] = 10;
	}
	temp = (ADR[MCU_SER1] - 10)*1000 / 174;
	if (temp>1000)
	{
		temp = 1000;
	}
	PRESS[0] = temp;





	if ( ADV[MCU_SER2] == 4096)
	{
		ADR[MCU_SER2] = 0XFFFF;
	}
	else
	{
		temp=ADV[MCU_SER2]*100/(4096-ADV[MCU_SER2]);
		ADR[MCU_SER2] = temp;
	}
	if (ADR[MCU_SER2] < 10)
	{
		ADR[MCU_SER2] = 10;
	}
	temp = (ADR[MCU_SER2] - 10)*1000 / 174;
	if (temp>1000)
	{
		temp = 1000;
	}
	PRESS[1] = temp;
	dbg_printf("airPressure1 = %d\n", PRESS[0]);
	dbg_printf("airPressure1 = %d\n", PRESS[1]);
	WORD_WRITE(main_interface_data.airPressure1,PRESS[0]);
	WORD_WRITE(main_interface_data.airPressure2,PRESS[1]);

	
	//电机控制器温度
	{
		temp = Inverter_t;WORD_WRITE(main_interface_data.moter_control_temp,temp);
	}


	//电机温度
	{
		temp = Motor_Temperature;WORD_WRITE(main_interface_data.moter_temp,temp);
	}


	//开关采集
	{
		//!<需要查询硬件端口

 		main_interface_data.control_IN01 = MCU_IN1;  //左转向开
		main_interface_data.control_IN02 = 0;        //保留    
		main_interface_data.control_IN03 = 0;        //保留    
		main_interface_data.control_IN04 = MCU_IN4 ; //钥匙ST开
		main_interface_data.control_IN05 = MCU_IN5 ; //右转向开
		main_interface_data.control_IN06 = MCU_IN6 ; //小灯开关
		main_interface_data.control_IN07 = MCU_IN7 ; //远光灯开
		main_interface_data.control_IN08 = MCU_IN8 ; //近光灯开
		
		main_interface_data.control_IN09 = MCU_IN9 ; //前雾灯开
		main_interface_data.control_IN10 = MCU_IN10; //后雾灯开
		main_interface_data.control_IN11 = 0;        //保留    
		main_interface_data.control_IN12 = MCU_IN12; //广告灯开
		main_interface_data.control_IN13 = 0;        //保留    
		main_interface_data.control_IN14 = MCU_IN14; //雨刮低档
		main_interface_data.control_IN15 = MCU_IN15; //雨刮间歇
		main_interface_data.control_IN16 = MCU_IN16; //雨刮高档
		                                                                                                                               
		main_interface_data.control_IN17 = 0;        //保留    
		main_interface_data.control_IN18 = 0;        //保留    
		main_interface_data.control_IN19 = MCU_IN19; //路牌开关
		main_interface_data.control_IN20 = MCU_IN20; //前门开开
		main_interface_data.control_IN21 = MCU_IN21; //雨刮喷水
		main_interface_data.control_IN22 = 0;        //保留    
		main_interface_data.control_IN23 = 0;        //保留    
		main_interface_data.control_IN24 = 0;        //保留 
		
		main_interface_data.control_IN25 = 0;        //保留    
		main_interface_data.control_IN26 = MCU_IN26; //中门关开
		main_interface_data.control_IN27 = MCU_IN27; //中门开开
		main_interface_data.control_IN28 = MCU_IN28; //前门关开
		main_interface_data.control_IN29 = MCU_IN3;  //燃油量过
		main_interface_data.control_IN30 = 0;        //保留    
		main_interface_data.control_IN31 = 0;        //保留    
		main_interface_data.control_IN32 = 0;        //保留    
		
		main_interface_data.control_IN33 = mid_get_io_filtered(pin_filter_in, PIN_IN_WAKEUP1);        //危险警报
		main_interface_data.control_IN34 = mid_get_io_filtered(pin_filter_in, PIN_IN_WAKEUP2);        //电源唤醒
		main_interface_data.control_IN35 = mid_get_io_filtered(pin_filter_in, PIN_IN_WAKEUP3);	/*获取滤波后IGN输入*/        //钥匙ON开	
		
	}
	main_interface_data.diagnostic_mode = Diagnosis;
	main_interface_data.climbing_mode = Mode_S;
	main_interface_data.HV_overhaul = High_Voltage;
	main_interface_data.main_power = M_ON;     //!<点火信号
	main_interface_data.defroster = 0;         //!<未定义
	main_interface_data.back_door = rKL15;                                                                 
	main_interface_data.park_brake=rKEY.BITS.kl8;		   //驻车制动符号片                                                                   
	main_interface_data.brake_light=!rKEY.BITS.kl6;		   //刹车灯符号片                                                                                                                                                                                                                                                         
	main_interface_data.ABS=0;				       //ABS符号片                                                                          
	main_interface_data.dryer=0;			       //干燥器符号片                                                                         
	main_interface_data.Kt=Battery_Kt;				       //电池主接触器                                                                         
	main_interface_data.power_mode=(CAN_KEY[0].byte>>5)&0x07;		   //Power模式符号片(纯电动模式 or 混动模式 or 插电模式)                              
	main_interface_data.charge_confirm=Charge_Check;	//充电确认符号片                                                                 
	main_interface_data.AC=AC_SWITCH;				      //AC空调符号片
	temp = water_temp;WORD_WRITE(main_interface_data.engine_water_temp,temp);
	temp = TCU_level;
	DWORD_WRITE(main_interface_data.fault_level,temp);
	temp = TCU_code;
	DWORD_WRITE(main_interface_data.fault_code,temp);
	temp = tcu_spn;
	DWORD_WRITE(main_interface_data.SPN,temp);
	temp = Niaosu;
	DWORD_WRITE(main_interface_data.urea_level,temp);
	temp = g_u32_utcTime;
	DWORD_WRITE(main_interface_data.dateTime,temp);
	{
		temp = ADV[MCU_VBAT];
		temp = temp * 169 /10000;
		if (temp<16)
		{
			temp = 16;
		}
		else if (temp>32)
		{
			temp = 32;
		}
		dbg_printf("battery = %d,%d\n",temp,ADV[MCU_VBAT]);
		DWORD_WRITE(main_interface_data.battery,temp);
	}
	
	
}
void main_interface_send_data(void)
{
	U8 data[MAIN_INTERFACE_DATA_LENGTH + A20_MCU_DATA_LENTH];
	U32 parse_len;
    parse_len = app_uart_arm_send_parse(data,(void*)&main_interface_data,CAR_INFO_TYPE,MAIN_INTERFACE_DATA_LENGTH);
	sent_data(UART_A20_CHN, data, parse_len); /* data sent */
}
