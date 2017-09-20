#include "app_arm_comm.h"





//!<新协议封装
/**
 * 帧协议: 小端模式
 *   | 帧 头 | 总长度 | 命 令 | 数 据 | CRC | 帧 尾 |
 *   |   1   |    1   |   1   |   n   |  2  |   1   |(byte)
 *   |  AAh  |   n+6  |  cmd  |  ...  |  x  |  55h  |
 */
/** 
 * @brief  封装数据
 * @param dest: 封装后的数据存放地址
 * @param data: 数据源
 * @param cmd: 命令类型
 * @param len: 数据长度 
 * @retval : 封装后数据长度
 */

U32 app_uart_arm_send_parse(U8 *dest,const void *data,U8 cmd,U8 len)
{
	U16 temp;
	assert_param(dest == NULL);
	assert_param(data == NULL);
	assert_param(len >= 250);
	dest[0] = FRAME_HEAD;
	dest[1] = len + 6;
	dest[2] = cmd;
	memcpy(&dest[3],data,len);
	/*-------------------------CRC check------------------------*/
	temp = api_cal_crc16((U8 *)data, len); /* crc check */
	dest[len + 3] = temp & 0xFF;
	dest[len + 4] = (temp>>8) & 0xFF;
	dest[len + 5] = cmd;
	dest[len + 6] = FRAME_REAR;
	return (U32)dest[1];
}




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
	if (TM_Feedback_RPM > 20000)
	{
 		temp1 = TM_Feedback_RPM - 20000;
 	}
	else
	{
		temp1 = 20000 - TM_Feedback_RPM;
	}

	tempSpeed = (U8) (temp1 * 0.382 * 2 * 60 * 3.14159 / 1000 / 5.571 * 1.05);  //车速单位1KM/H
	if (tempSpeed > 140)
	{
		tempSpeed = 140;
	}
	main_interface_data.speed = tempSpeed; /* 车速0-140km/h */
	pSpeed = tempSpeed;


	//电机转速
	buf1 = TM_Feedback_RPM;
	
	byte_order_change((U8*)&buf1,2);

	main_interface_data.rpm = buf1; /* 收到后减去20000 单位r/min */

	//总里程
	temp = (info.Odo); 

	byte_order_change((U8*)&temp,4);
	main_interface_data.odo = temp;

	//总电压
	buf1 = (BMS_Voltage/10);  //总电压
	if(buf1>=800)
	{
		buf1 = 800;
	}
	byte_order_change((U8*)&buf1,2);
	main_interface_data.totalVoltage = buf1;

	//档位
	main_interface_data.gear = VCU_Gear;  /*0000:空挡 0001: 1 档 0010: 2 档 0011: 3 档 0100: 4 档 0101: 5 档 0110: 6 档 1101: 13倒档*/
	main_interface_data.reversed1 = VCU_Status_Flag1.byte;

	//总电流
	buf1 = BMS_Current;
	if(buf1>64255)
	{
		buf1 = 64255;
	}
	buf1 = buf1/20;

	byte_order_change(((U8*)&(buf1)),2); //总电流  收到后减去1600
	main_interface_data.totalCurrent = buf1; 

	//小计里程
	buf1 = info.Trip1;
	byte_order_change((U8*)&(buf1), 2);
	main_interface_data.trip = buf1; /* trip  */

	//SOC
	temp2 = BMS_SOC;
	if(temp2>100)
	{
		temp2 = 100;
	}
	main_interface_data.soc = temp2; /* Soc 百分之0-100  */


	//ready 信号
	if(mid_can_lost_sts_get(ID_100017EF) == 0 )
	{
		main_interface_data.ready_signal = READY; /*ready 信号*/
	}
	else
	{
		main_interface_data.ready_signal = 0xff; //can丢失 ready 信号无效 
	}

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
	
	/* 单体最高温 */
	if(BAT_Temp_H > 200)
	{
		BAT_Temp_H = 200;
	}
	main_interface_data.batteryTH = BAT_Temp_H;  //收到后减去40

	/* 单体最低温 */
	if(BAT_Temp_L > 200)
	{
		BAT_Temp_L = 200;
	}
	main_interface_data.batteryTL = BAT_Temp_L; //收到后减去40

	//电池单体高电压
	buf1 = BAT_Single_H;
	if(buf1 > 15000)
	{
		buf1 = 15000;
	}
	
	byte_order_change((U8*)&buf1,2);
	main_interface_data.batteryVH = buf1;  //收到后除以1000 保留3位精度
	
	//电池单体低电压
	buf2 = BAT_Single_L;
	if(buf2 > 15000)
	{
		buf2 = 15000;
	}
	byte_order_change((U8*)&buf2,2);
	main_interface_data.batteryVL = buf2; //收到后除以1000 保留3位精度

	//电机控制器温度
	if(TM_Control_Temp > 250)  //需要QT -40
	{
		main_interface_data.moter_control_temp = 250;
	}
	else
	{
		main_interface_data.moter_control_temp = TM_Control_Temp; //需要QT -40
	}

	//电机温度
	if(TM_Temp >= 250)
	{
		main_interface_data.moter_temp = 250;
	}
	else
	{
		main_interface_data.moter_temp = TM_Temp; //需要QT -40
	}
	//开关采集
	{
		extern pin_filter_t    pin_filter_in[];
		temp = 0;
		temp = (MCU_IN1<<31)|(MCU_IN2<<30)|((MCU_IN3)<<29)|((MCU_IN4)<<28)|(MCU_IN5<<27)|(MCU_IN6<<26)|(MCU_IN7<<25)|(MCU_IN8<<24)|
			   (MCU_IN9<<23)|(MCU_IN10<<22)|(MCU_IN11<<21)|(MCU_IN12<<20)|(MCU_IN13<<19)|(MCU_IN14<<18)|(MCU_IN15<<17)|(MCU_IN16<<16)|
			   (MCU_IN17<<15)|(MCU_IN18<<14)|(MCU_IN19<<13)|(MCU_IN20<<12)|(MCU_IN21<<11)|(MCU_IN22<<10)|(MCU_IN23<<9)|(MCU_IN24<<8)|
			   (MCU_IN25<<7)|(MCU_IN26<<6)|(MCU_IN27<<5)|(MCU_IN28<<4)|(MCU_IN29<<3)|(pin_filter_in[0].result<<2)|(pin_filter_in[1].result<<1)|(pin_filter_in[2].result);
		dbg_printf("io_temp = %x\n",temp);
		byte_order_change((U8*)&temp,4);
	}
	main_interface_data.switch_capture.u32_switch_capture = temp;
	dbg_printf("io = %x\n",main_interface_data.switch_capture.u32_switch_capture);
	//电机转矩
	buf1 = (TM_Feedback_NM/10);
	byte_order_change((U8*)&buf1,2);
	main_interface_data.tm_zhuanju_nm = buf1;  //电机转矩 收到后减去2000
	
	//预充超时 
	if(mid_can_lost_sts_get(ID_100017EF) == 0 )
	{
		main_interface_data.charge_overtime = CHARGE_OVERTIME; /*预充超时*/
	}
	else
	{
		main_interface_data.charge_overtime = 0xff; //can丢失 预充超时 信号无效 
	}

	//整车系统故障
	if(mid_can_lost_sts_get(ID_100017EF) == 0 )
	{
		main_interface_data.vcu_fault = VCU_FAULT; /*整车系统故障*/
	}
	else
	{
		main_interface_data.vcu_fault = 0xff; //can丢失 整车系统故障 信号无效 
	}
	temp = g_u32_utcTime;
	byte_order_change((U8*)&temp,4);
	main_interface_data.utc_time_second = temp;     //
	{
		U32 u32tem;
		u32tem = ADR[6];
		u32tem = u32tem * 169 /10000;
		u32tem = u32tem + 4;
		if (u32tem<16)
		{
			u32tem = 16;
		}
		else if (u32tem>32)
		{
			u32tem = 32;
		}
		byte_order_change((U8*)&u32tem,4);
		main_interface_data.vol = u32tem;
		//dbg_printf("!!!!!----->>>> %d\n",ADR[6]);
	}
}







