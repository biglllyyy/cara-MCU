#include <string.h>
#include "app_key.h"
#include "io_cfg.h"
#include "app_uart_cmd.h"
#include "app_io.h"
#include "hal_wdg.h"
#include "app_power.h"
#include "app_info.h"
#include "app_trip.h"

extern uint8_t g_u8IgnSts;

key_t key[KEY_NUM_MAX];


//static key_status_e convert_key_status(key_status_e status)
//{
//	if(status==PRESSED)
//	{
//		status=NOT_PRESSED;
//	}
//	else
//	{
//		status=PRESSED;
//	}
//	
//	return status;
//}
//
//void app_key_init(void)
//{
//	memset(&key,0,sizeof(key));
//}
//
//void app_key_scan_task_100ms(void)
//{
//	U8 i;
//	if(g_u8IgnSts==ON)
//	{
//		//读IO状态
//		key[0].status_in_from_pin=(key_status_e)(pin_filter_in[PIN_IN_KEY2].result&0x01);
//		key[1].status_in_from_pin=(key_status_e)(pin_filter_in[PIN_IN_KEY1].result&0x01);
//		key[2].status_in_from_pin=(key_status_e)(pin_filter_in[PIN_IN_KEY0].result&0x01);
//		
//		//
//		for(i=0;i<KEY_NUM_MAX;i++)
//		{
//			//特殊处理
//			if(i==2)//确认按键不做翻转处理
//			{
//				key[i].status_out_to_core=key[i].status_in_from_pin;
//				continue;//不再执行下面的语句，重新开始一次循环
//			}
//			
//			
//			if(key[i].status_in_from_pin==PRESSED)
//			{
//				if(key[i].time_cnt<0xff)//计时
//				{
//					key[i].time_cnt++;
//				}
//				
//				if((key[i].time_cnt%CONST_TIME_200MS)==0)//每隔200ms翻转一次
//				{
//					key[i].status_out_to_core=convert_key_status(key[i].status_out_to_core);
//				}
//				
//				if(key[i].time_cnt>=CONST_LONG_PRESSED_TIME)//判断长按
//				{
//					key[i].status_handle=LONG_PRESSED;
//				}
//				
//			}
//			else
//			{
//				key[i].status_out_to_core=NOT_PRESSED;
//				
//				if(key[i].time_cnt>=CONST_LONG_PRESSED_TIME)//判断长按
//				{
//					key[i].status_handle=LONG_PRESSED;
//				}
//				else if((key[i].time_cnt>=CONST_SHORT_PRESSED_TIME)&&(key[i].time_cnt<CONST_LONG_PRESSED_TIME))//判断短按
//				{
//					key[i].status_handle=SHORT_PRESSED;
//				}
//				else
//				{
//				}
//				
//				key[i].time_cnt=0;
//			}
//		}
//	}
//	else
//	{
//		memset(key,0,sizeof(key));
//	}
//}
extern U8  g_u8_A20_power_sts;
static void update_mode_judge(void)
{
	static U8 time_cnt=0;
	
	if(g_u8_pre_sleep_flag==OFF)
	{
		key[PIN_OK_KEY].time_cnt=0;
	}
	else
	{
		key[PIN_OK_KEY].status_in_from_pin=(key_status_e)(pin_filter_in[PIN_IN_KEY0].result&0x01);
		
		if(key[PIN_OK_KEY].status_in_from_pin==PRESSED)
		{
			if(time_cnt<0xff)
			{
				time_cnt++;
			}
			
			if(time_cnt>=CONST_TIME_10S)
			{
				while(1);//等待wdg reset
			}
		}
	}
}

void app_key_init(void)
{
	memset(&key,0,sizeof(key));
}

void app_key_scan_task_100ms(void)
{
	U8 i;
	if(/*g_u8IgnSts==*/ON)  //for 206 for test
	{
		//读IO状态
		key[PIN_LEFT_KEY].status_in_from_pin=(key_status_e)(!(pin_filter_in[PIN_IN_KEY2].result&0x01));
		key[PIN_RIGHT_KEY].status_in_from_pin=(key_status_e)(!(pin_filter_in[PIN_IN_KEY1].result&0x01));
		key[PIN_OK_KEY].status_in_from_pin=(key_status_e)(!(pin_filter_in[PIN_IN_KEY0].result&0x01));
		key[PIN_CANCEL_KEY].status_in_from_pin=(key_status_e)(!(pin_filter_in[PIN_IN_KEY3].result&0x01));
		
		//判断按键状态
		for(i=0;i<KEY_NUM_MAX;i++)
		{
			if(key[i].status_in_from_pin==PRESSED)
			{
				if(key[i].time_cnt<0xff)//计时
				{
					key[i].time_cnt++;
				}
				
				if(key[i].time_cnt>=CONST_TIME_3S)//判断长按
				{
					key[i].status_handle=LONG_PRESSED;
				}
				else
				{
					key[i].status_handle=SHORT_PRESSED;
				}
			}
			else
			{
				key[i].status_handle=NO_PRESSED;
				key[i].time_cnt=0;
			}
		}
		
		//转换成串口数据
		for(i=0;i<KEY_NUM_MAX;i++)
		{
			switch(i)
			{
				case PIN_LEFT_KEY:
					switch(key[PIN_LEFT_KEY].status_handle)
					{
						case NO_PRESSED:
							key_info_value	&= 0xFD;
							break;
						case SHORT_PRESSED:
							key_info_value	|= 0x02;
							break;
						case LONG_PRESSED:
							key_info_value	|= 0x02;
							break;
						default:
							key_info_value	&= 0xFD;
							break;
					}
					break;
				case PIN_RIGHT_KEY:
					switch(key[PIN_RIGHT_KEY].status_handle)
					{
						case NO_PRESSED:
							key_info_value	&= 0xFB;
							break;
						case SHORT_PRESSED:
							key_info_value	|= 0x04;
							break;
						case LONG_PRESSED:
							key_info_value	|= 0x04;
							break;
						default:
							key_info_value	&= 0xFB;
							break;
					}
					break;
				case PIN_OK_KEY:
					switch(key[PIN_OK_KEY].status_handle)
					{
						case NO_PRESSED:
							key_info_value	&= 0xFE;
							break;
						case SHORT_PRESSED:
							key_info_value	|= 0x01;
							break;
						case LONG_PRESSED:
							key_info_value	|= 0x01;
							break;
						default:
							key_info_value	&= 0xFE;
							break;
					}
					break;
				case PIN_CANCEL_KEY:
					switch(key[PIN_CANCEL_KEY].status_handle)
					{
						case NO_PRESSED:
							key_info_value	&= ~0x08;
							break;
						case SHORT_PRESSED:
							key_info_value	|= 0x08;
							break;
						case LONG_PRESSED:
							key_info_value	|= 0x08;
							app_sub_trip1_clear();
							break;
						default:
							key_info_value	&= ~0x08;
							break;
					}
					break;	
			}
		}
		uart_general_data.generalInfo.key3 = 0;             
	}
	else
	{
		update_mode_judge();
		memset(key,0,sizeof(key));
	}
}
