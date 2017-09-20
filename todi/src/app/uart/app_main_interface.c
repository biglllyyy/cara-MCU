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



Main_Interface_Data_Struct main_interface_data =
{ 0 };

#define MAIN_INTERFACE_DATA_LENGTH (sizeof(Main_Interface_Data_Struct))
#define MAIN_INTERFACE_FRAME_TYPE  CAR_INFO_TYPE

extern U16 ad_data;
U32 ad_data_group[4];

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


	tempSpeed = pSpeed;  //车速单位1KM/H
	if (tempSpeed > 140)
	{
		tempSpeed = 140;
	}
	main_interface_data.speed = tempSpeed; /* 车速0-140km/h */


	//电机转速
	temp1 = moto_speed;
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


	//气压 电路分压10倍，
	{
		U32 u32temp;
		ADV[4] = (ADR[4]*3300*11/4096);
		if (ADV[4]>2100)
		{
			ADV[4] += 500;
		}
		else if (ADV[4]>1600)
		{
			ADV[4] += 400;
		}
		else
		{
			ADV[4] +=200;
		}
		u32temp = ADV[4]/100;

		if (u32temp>=33)
		{
			u32temp = 33;
		}
		main_interface_data.airPressure1 = (U8)u32temp;//(U8)(ADR[4]*330/4096);//(U8)((ADR[4]/(4095)*3.3)*10); /* 气压1 电压值扩大了10倍   */
		dbg_printf("ad1 = %d\n",ADR[4]);
	}
	{
		U32 u32temp;
		ADV[3] = (ADR[3]*3300*11/4096);
		if (ADV[3]>2100)
		{
			ADV[3] += 500;
		}
		else if (ADV[4]>1600)
		{
			ADV[3] += 400;
		}
		else
		{
			ADV[3] +=200;
		}
		u32temp = ADV[3]/100;
		if (u32temp>=33)
		{
			u32temp = 33;
		}
		main_interface_data.airPressure2 = (U8)u32temp;//(U8)(ADR[4]*330/4096);//(U8)((ADR[4]/(4095)*3.3)*10); /* 气压1 电压值扩大了10倍   */
		
		dbg_printf("ad2 = %d\n",ADR[3]);
	}
	


	//电机控制器温度
	{
		main_interface_data.moter_control_temp = 250;
	}


	//电机温度
	{
		main_interface_data.moter_temp = 250;
	}

	//开关采集
	{
		//!<需要查询硬件端口
	}
	
}
void main_interface_send_data(void)
{
	U8 data[MAIN_INTERFACE_DATA_LENGTH + 6];
	U32 parse_len;
    parse_len = app_uart_arm_send_parse(data,(void*)&main_interface_data,CAR_INFO_TYPE,MAIN_INTERFACE_DATA_LENGTH);
	sent_data(UART_A20_CHN, data, parse_len); /* data sent */
}
