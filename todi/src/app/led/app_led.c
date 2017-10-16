#include <string.h>
#include "app_led.h"
#include "app_io.h"
#include "mid_pin.h"
#include "mid_time.h"
#include "g_variable.h"
#include "mid_trigger.h"
#include "app_can.h"
#include "mid_switch.h"
#include "mid_led.h"
#include "app_rpm.h"
#include "buzzer_cfg.h"
#include "app_buzzer.h"
#include "app_IPconfig.h"
#include "app_uart_cmd.h"
#include "hal_reload_timer.h"
#include "hal_wdg.h"
#include "mid_dbg.h"



#include "app_lin.h"
#include "app_power.h"
#include "app_CarKey.h"

/*******************************************************************************/
led_check_t  led_check = {0,0};
U16 led_check_time=0;
U8 led_selfcheck_enable=0;
/********************************************************************************/

void app_init_led(void)
{
	mid_init_led();
	//register the blink function,to let led  blink
	//mid_led_blink_register();
	//mid_led_blink_register(led_esp_blink,LED_ESP,F_2HZ);

	led_check.is_less_3s_sts = 0;
	led_check.is_less_6s_sts = 0;
	
	//app_leds_all_on_test();//for test 2015/7/21 15:57:55
	
}

/* ���� */
void led_airbag_ctl(void)
{
#if 0
	switch(IPconfig.protocol)
	{
		case HAVAL_2013_2P4L_AT_2WD:
			if(ON == g_u8IgnSts)
			{
				if(can_id_351_lost_timecnt<CAN_LOST_TIME)
				{
					uart_special_data.specialInfo.LedWarning.first.bit.SRSError=can0.id_351.ABM_AirBagFault;
				}
				else
				{
					uart_special_data.specialInfo.LedWarning.first.bit.SRSError=1;
				}
			}
			else
			{
				uart_special_data.specialInfo.LedWarning.first.bit.SRSError=0;
			}
			break;
		case HAVAL_2013_1P5T_MT_2WD:
			if(ON == g_u8IgnSts)
			{
				if(can_id_351_lost_timecnt<CAN_LOST_TIME)
				{
					uart_special_data.specialInfo.LedWarning.first.bit.SRSError=can1.id_351.ABM_AirBagFault;
				}
				else
				{
					uart_special_data.specialInfo.LedWarning.first.bit.SRSError=1;
				}
			}
			else
			{
				uart_special_data.specialInfo.LedWarning.first.bit.SRSError=0;
			}
			break;
		case HAVAL_2012_1P5T_MT_2WD:
			if(ON == g_u8IgnSts)
			{
				if(can_id_351_lost_timecnt<CAN_LOST_TIME)
				{
					uart_special_data.specialInfo.LedWarning.first.bit.SRSError=can2.id_351.ABM_AirBagFault;
				}
				else
				{
					uart_special_data.specialInfo.LedWarning.first.bit.SRSError=1;
				}
			}
			else
			{
				uart_special_data.specialInfo.LedWarning.first.bit.SRSError=0;
			}
			break;
		case HAVAL_2015_1P5T_MT_2WD: 
		case HAVAL_2015_1P5T_6AT_2WD:
			if(ON == g_u8IgnSts)
			{
				if(can_id_351_lost_timecnt<CAN_LOST_TIME)
				{
					uart_special_data.specialInfo.LedWarning.first.bit.SRSError=can3.id_351.ABM_AirBagFault;
				}
				else
				{
					uart_special_data.specialInfo.LedWarning.first.bit.SRSError=1;
				}
			}
			else
			{
				uart_special_data.specialInfo.LedWarning.first.bit.SRSError=0;
			}
			break;
		case HAVAL_2011_2P0T_MT_2WD:
			if(ON == g_u8IgnSts)
			{
				if(can_id_351_lost_timecnt<CAN_LOST_TIME)
				{
					uart_special_data.specialInfo.LedWarning.first.bit.SRSError=can4.id_351.ABM_AirBagFault;
				}
				else
				{
					uart_special_data.specialInfo.LedWarning.first.bit.SRSError=1;
				}
			}
			else
			{
				uart_special_data.specialInfo.LedWarning.first.bit.SRSError=0;
			}
			break;
		case HAVAL_2013_1P5T_MT_4WD:
			if(ON == g_u8IgnSts)
			{
				if(can_id_351_lost_timecnt<CAN_LOST_TIME)
				{
					uart_special_data.specialInfo.LedWarning.first.bit.SRSError=can5.id_351.ABM_AirBagFault;
				}
				else
				{
					uart_special_data.specialInfo.LedWarning.first.bit.SRSError=1;
				}
			}
			else
			{
				uart_special_data.specialInfo.LedWarning.first.bit.SRSError=0;
			}
			break;
	}
	#endif
}
 
void led_turn_ctl(void)
{
#if 0
	switch(IPconfig.protocol)                               
	{                                                     
		case HAVAL_2013_2P4L_AT_2WD:                            
			if(ON == g_u8IgnSts)//���ʱ����CAN����
			{
				if(can_id_311_lost_timecnt<CAN_LOST_TIME)/*CAN��ʧʱ��С��2.5s*/
				{
					mid_led_ctl(LED_LEFTTURNLIGHT,(LED_STS_e)(can0.id_311.BCM_LeftTurnSignalCmd));
					mid_led_ctl(LED_RIGHTTURNLIGHT,(LED_STS_e)(can0.id_311.BCM_RightTurnSignalCmd)); 
				}
				else/*CAN��ʧʱ�����2.5s*/
				{
					mid_led_ctl(LED_LEFTTURNLIGHT,LED_OFF);
					mid_led_ctl(LED_RIGHTTURNLIGHT,LED_OFF);
				}
			}
			else//Ϩ��ʱ����LIN����
			{
				if(lin_pid_0x42_lost_timecnt<LIN_LOST_TIME)
				{                                                     
					mid_led_ctl(LED_LEFTTURNLIGHT,(LED_STS_e)(lin0_msg.pid_0x42.LeftTurnLampSts));
					mid_led_ctl(LED_RIGHTTURNLIGHT,(LED_STS_e)(lin0_msg.pid_0x42.RightTurnLampSts));
				}
				else
				{
					mid_led_ctl(LED_LEFTTURNLIGHT,LED_OFF);
					mid_led_ctl(LED_RIGHTTURNLIGHT,LED_OFF);
				}
			}                                            
			break;                                            
		case HAVAL_2013_1P5T_MT_2WD:                            
			if(ON == g_u8IgnSts)//���ʱ����CAN����
			{
				if(can_id_311_lost_timecnt<CAN_LOST_TIME)/*CAN��ʧʱ��С��2.5s*/
				{
					mid_led_ctl(LED_LEFTTURNLIGHT,(LED_STS_e)(can1.id_311.BCM_LeftTurnSignalCmd));
					mid_led_ctl(LED_RIGHTTURNLIGHT,(LED_STS_e)(can1.id_311.BCM_RightTurnSignalCmd)); 
				}
				else/*CAN��ʧʱ�����2.5s*/
				{
					mid_led_ctl(LED_LEFTTURNLIGHT,LED_OFF);
					mid_led_ctl(LED_RIGHTTURNLIGHT,LED_OFF);
				}
			}
			else//Ϩ��ʱ����LIN����
			{
				if(lin_pid_0x42_lost_timecnt<LIN_LOST_TIME)
				{                                                     
					mid_led_ctl(LED_LEFTTURNLIGHT,(LED_STS_e)(lin1_msg.pid_0x42.LeftTurnLampSts));
					mid_led_ctl(LED_RIGHTTURNLIGHT,(LED_STS_e)(lin1_msg.pid_0x42.RightTurnLampSts));
				}
				else
				{
					mid_led_ctl(LED_LEFTTURNLIGHT,LED_OFF);
					mid_led_ctl(LED_RIGHTTURNLIGHT,LED_OFF);
				}
			}                                             
			break;                                            
		case HAVAL_2012_1P5T_MT_2WD:                            
			if(ON == g_u8IgnSts)//���ʱ����CAN����
			{
				if(can_id_311_lost_timecnt<CAN_LOST_TIME)/*CAN��ʧʱ��С��2.5s*/
				{
					mid_led_ctl(LED_LEFTTURNLIGHT,(LED_STS_e)(can2.id_311.BCM_LeftTurnSignalCmd));
					mid_led_ctl(LED_RIGHTTURNLIGHT,(LED_STS_e)(can2.id_311.BCM_RightTurnSignalCmd)); 
				}
				else/*CAN��ʧʱ�����2.5s*/
				{
					mid_led_ctl(LED_LEFTTURNLIGHT,LED_OFF);
					mid_led_ctl(LED_RIGHTTURNLIGHT,LED_OFF);
				}
			}
			else//Ϩ��ʱ����LIN����
			{
				if(lin_pid_0x42_lost_timecnt<LIN_LOST_TIME)
				{                                                     
					mid_led_ctl(LED_LEFTTURNLIGHT,(LED_STS_e)(lin2_msg.pid_0x42.LeftTurnLampSts));
					mid_led_ctl(LED_RIGHTTURNLIGHT,(LED_STS_e)(lin2_msg.pid_0x42.RightTurnLampSts));
				}
				else
				{
					mid_led_ctl(LED_LEFTTURNLIGHT,LED_OFF);
					mid_led_ctl(LED_RIGHTTURNLIGHT,LED_OFF);
				}
			}                                             
			break;   
		case HAVAL_2015_1P5T_MT_2WD: 
		case HAVAL_2015_1P5T_6AT_2WD:                           
			if(ON == g_u8IgnSts)//���ʱ����CAN����
			{
				if(can_id_311_lost_timecnt<CAN_LOST_TIME)/*CAN��ʧʱ��С��2.5s*/
				{
					mid_led_ctl(LED_LEFTTURNLIGHT,(LED_STS_e)(can3.id_311.BCM_LeftTurnSignalCmd));
					mid_led_ctl(LED_RIGHTTURNLIGHT,(LED_STS_e)(can3.id_311.BCM_RightTurnSignalCmd)); 
				}
				else/*CAN��ʧʱ�����2.5s*/
				{
					mid_led_ctl(LED_LEFTTURNLIGHT,LED_OFF);
					mid_led_ctl(LED_RIGHTTURNLIGHT,LED_OFF);
				}
			}
			else//Ϩ��ʱ����LIN����
			{
				if(lin_pid_0x42_lost_timecnt<LIN_LOST_TIME)
				{                                                     
					mid_led_ctl(LED_LEFTTURNLIGHT,(LED_STS_e)(lin3_msg.pid_0x42.LeftTurnLampSts));
					mid_led_ctl(LED_RIGHTTURNLIGHT,(LED_STS_e)(lin3_msg.pid_0x42.RightTurnLampSts));
				}
				else
				{
					mid_led_ctl(LED_LEFTTURNLIGHT,LED_OFF);
					mid_led_ctl(LED_RIGHTTURNLIGHT,LED_OFF);
				}
			}                                        
			break; 
		case HAVAL_2011_2P0T_MT_2WD:                            
			if(ON == g_u8IgnSts)//���ʱ����CAN����
			{
				if(can_id_311_lost_timecnt<CAN_LOST_TIME)/*CAN��ʧʱ��С��2.5s*/
				{
					mid_led_ctl(LED_LEFTTURNLIGHT,(LED_STS_e)(can4.id_311.BCM_LeftTurnSignalCmd));
					mid_led_ctl(LED_RIGHTTURNLIGHT,(LED_STS_e)(can4.id_311.BCM_RightTurnSignalCmd)); 
				}
				else/*CAN��ʧʱ�����2.5s*/
				{
					mid_led_ctl(LED_LEFTTURNLIGHT,LED_OFF);
					mid_led_ctl(LED_RIGHTTURNLIGHT,LED_OFF);
				}
			}
			else//Ϩ��ʱ����LIN����
			{
				if(lin_pid_0x42_lost_timecnt<LIN_LOST_TIME)
				{                                                     
					mid_led_ctl(LED_LEFTTURNLIGHT,(LED_STS_e)(lin4_msg.pid_0x42.LeftTurnLampSts));
					mid_led_ctl(LED_RIGHTTURNLIGHT,(LED_STS_e)(lin4_msg.pid_0x42.RightTurnLampSts));
				}
				else
				{
					mid_led_ctl(LED_LEFTTURNLIGHT,LED_OFF);
					mid_led_ctl(LED_RIGHTTURNLIGHT,LED_OFF);
				}
			}                                             
			break;  
		case HAVAL_2013_1P5T_MT_4WD:                            
			if(ON == g_u8IgnSts)//���ʱ����CAN����
			{
				if(can_id_311_lost_timecnt<CAN_LOST_TIME)/*CAN��ʧʱ��С��2.5s*/
				{
					mid_led_ctl(LED_LEFTTURNLIGHT,(LED_STS_e)(can5.id_311.BCM_LeftTurnSignalCmd));
					mid_led_ctl(LED_RIGHTTURNLIGHT,(LED_STS_e)(can5.id_311.BCM_RightTurnSignalCmd)); 
				}
				else/*CAN��ʧʱ�����2.5s*/
				{
					mid_led_ctl(LED_LEFTTURNLIGHT,LED_OFF);
					mid_led_ctl(LED_RIGHTTURNLIGHT,LED_OFF);
				}
			}
			else//Ϩ��ʱ����LIN����
			{
				if(lin_pid_0x42_lost_timecnt<LIN_LOST_TIME)
				{                                                     
					mid_led_ctl(LED_LEFTTURNLIGHT,(LED_STS_e)(lin5_msg.pid_0x42.LeftTurnLampSts));
					mid_led_ctl(LED_RIGHTTURNLIGHT,(LED_STS_e)(lin5_msg.pid_0x42.RightTurnLampSts));
				}
				else
				{
					mid_led_ctl(LED_LEFTTURNLIGHT,LED_OFF);
					mid_led_ctl(LED_RIGHTTURNLIGHT,LED_OFF);
				}
			}                                             
			break;                                          
	}
	#endif
}

/* ����� */
void led_rearfoglamp_ctl(void)
{
#if 0
	switch(IPconfig.protocol)                               
	{                                                     
		case HAVAL_2013_2P4L_AT_2WD:                            
			if(ON == g_u8IgnSts)
			{
				if(can_id_311_lost_timecnt<CAN_LOST_TIME)
				{
					mid_led_ctl(LED_REARFOGLAMP,(LED_STS_e)(can0.id_311.BCM_RearFogLmpSts));     
				}
				else
				{
					mid_led_ctl(LED_REARFOGLAMP,LED_OFF);
				}
			}
			else
			{
				if(lin_pid_0x42_lost_timecnt<LIN_LOST_TIME)
				{
					mid_led_ctl(LED_REARFOGLAMP,(LED_STS_e)(lin0_msg.pid_0x42.RearFogLampSts));
				}
				else
				{
					mid_led_ctl(LED_REARFOGLAMP,LED_OFF);
				}
			}                                           
			break;                                            
		case HAVAL_2013_1P5T_MT_2WD:                            
			if(ON == g_u8IgnSts)
			{
				if(can_id_311_lost_timecnt<CAN_LOST_TIME)
				{
					mid_led_ctl(LED_REARFOGLAMP,(LED_STS_e)(can1.id_311.BCM_RearFogLmpSts));     
				}
				else
				{
					mid_led_ctl(LED_REARFOGLAMP,LED_OFF);
				}
			}
			else
			{
				if(lin_pid_0x42_lost_timecnt<LIN_LOST_TIME)
				{
					mid_led_ctl(LED_REARFOGLAMP,(LED_STS_e)(lin1_msg.pid_0x42.RearFogLampSts));
				}
				else
				{
					mid_led_ctl(LED_REARFOGLAMP,LED_OFF);
				}
			}                                             
			break;                                            
		case HAVAL_2012_1P5T_MT_2WD:                            
			if(ON == g_u8IgnSts)
			{
				if(can_id_311_lost_timecnt<CAN_LOST_TIME)
				{
					mid_led_ctl(LED_REARFOGLAMP,(LED_STS_e)(can2.id_311.BCM_RearFogLmpSts));     
				}
				else
				{
					mid_led_ctl(LED_REARFOGLAMP,LED_OFF);
				}
			}
			else
			{
				if(lin_pid_0x42_lost_timecnt<LIN_LOST_TIME)
				{
					mid_led_ctl(LED_REARFOGLAMP,(LED_STS_e)(lin2_msg.pid_0x42.RearFogLampSts));
				}
				else
				{
					mid_led_ctl(LED_REARFOGLAMP,LED_OFF);
				}
			}                                             
			break;       
		case HAVAL_2015_1P5T_MT_2WD: 
		case HAVAL_2015_1P5T_6AT_2WD:                           
			if(ON == g_u8IgnSts)
			{
				if(can_id_311_lost_timecnt<CAN_LOST_TIME)
				{
					mid_led_ctl(LED_REARFOGLAMP,(LED_STS_e)(can3.id_311.BCM_RearFogLmpSts));     
				}
				else
				{
					mid_led_ctl(LED_REARFOGLAMP,LED_OFF);
				}
			}
			else
			{
				if(lin_pid_0x42_lost_timecnt<LIN_LOST_TIME)
				{
					mid_led_ctl(LED_REARFOGLAMP,(LED_STS_e)(lin3_msg.pid_0x42.RearFogLampSts));
				}
				else
				{
					mid_led_ctl(LED_REARFOGLAMP,LED_OFF);
				}
			}                                        
			break;
		case HAVAL_2011_2P0T_MT_2WD:                            
			if(ON == g_u8IgnSts)
			{
				if(can_id_311_lost_timecnt<CAN_LOST_TIME)
				{
					mid_led_ctl(LED_REARFOGLAMP,(LED_STS_e)(can4.id_311.BCM_RearFogLmpSts));     
				}
				else
				{
					mid_led_ctl(LED_REARFOGLAMP,LED_OFF);
				}
			}
			else
			{
				if(lin_pid_0x42_lost_timecnt<LIN_LOST_TIME)
				{
					mid_led_ctl(LED_REARFOGLAMP,(LED_STS_e)(lin4_msg.pid_0x42.RearFogLampSts));
				}
				else
				{
					mid_led_ctl(LED_REARFOGLAMP,LED_OFF);
				}
			}                                             
			break;
		case HAVAL_2013_1P5T_MT_4WD:                            
			if(ON == g_u8IgnSts)
			{
				if(can_id_311_lost_timecnt<CAN_LOST_TIME)
				{
					mid_led_ctl(LED_REARFOGLAMP,(LED_STS_e)(can5.id_311.BCM_RearFogLmpSts));     
				}
				else
				{
					mid_led_ctl(LED_REARFOGLAMP,LED_OFF);
				}
			}
			else
			{
				if(lin_pid_0x42_lost_timecnt<LIN_LOST_TIME)
				{
					mid_led_ctl(LED_REARFOGLAMP,(LED_STS_e)(lin5_msg.pid_0x42.RearFogLampSts));
				}
				else
				{
					mid_led_ctl(LED_REARFOGLAMP,LED_OFF);
				}
			}                                             
			break;                                            
	}  
	#endif
}

/* ǰ��� */
void led_frontfoglamp_ctl(void)
{
#if 0
	switch(IPconfig.protocol)                               
	{                                                     
		case HAVAL_2013_2P4L_AT_2WD:                            
			if(ON == g_u8IgnSts)
			{
				if(can_id_311_lost_timecnt<CAN_LOST_TIME)
				{
					switch(can0.id_311.BCM_FrontFogLmpSts)
					{
						case 0:
							mid_led_ctl(LED_FRONTFOGLAMP,LED_OFF);
							break;
						case 1:
							mid_led_ctl(LED_FRONTFOGLAMP,LED_ON);
							break;
						case 3:
							mid_led_ctl(LED_FRONTFOGLAMP,LED_OFF);
							break;
						default:
							mid_led_ctl(LED_FRONTFOGLAMP,LED_OFF);
							break;
					}
					mid_led_ctl(LED_FRONTFOGLAMP,(LED_STS_e)(can0.id_311.BCM_FrontFogLmpSts));
				}
				else
				{
					mid_led_ctl(LED_FRONTFOGLAMP,LED_OFF);
				}
			}
			else
			{
				if(lin_pid_0x42_lost_timecnt<LIN_LOST_TIME)
				{
					mid_led_ctl(LED_FRONTFOGLAMP,(LED_STS_e)(lin0_msg.pid_0x42.FrontFogLampSts));
				}
				else/*����2.5Sû���յ���ص�LIN����*/
				{
					mid_led_ctl(LED_FRONTFOGLAMP,LED_OFF);//�ر�LED��
				}
			}                                            
			break;                                            
		case HAVAL_2013_1P5T_MT_2WD:                            
			if(ON == g_u8IgnSts)
			{
				if(can_id_311_lost_timecnt<CAN_LOST_TIME)
				{
					switch(can1.id_311.BCM_FrontFogLmpSts)
					{
						case 0:
							mid_led_ctl(LED_FRONTFOGLAMP,LED_OFF);
							break;
						case 1:
							mid_led_ctl(LED_FRONTFOGLAMP,LED_ON);
							break;
						case 3:
							mid_led_ctl(LED_FRONTFOGLAMP,LED_OFF);
							break;
						default:
							mid_led_ctl(LED_FRONTFOGLAMP,LED_OFF);
							break;
					}
					mid_led_ctl(LED_FRONTFOGLAMP,(LED_STS_e)(can1.id_311.BCM_FrontFogLmpSts));
				}
				else
				{
					mid_led_ctl(LED_FRONTFOGLAMP,LED_OFF);
				}
			}
			else
			{
				if(lin_pid_0x42_lost_timecnt<LIN_LOST_TIME)
				{
					mid_led_ctl(LED_FRONTFOGLAMP,(LED_STS_e)(lin1_msg.pid_0x42.FrontFogLampSts));
				}
				else/*����2.5Sû���յ���ص�LIN����*/
				{
					mid_led_ctl(LED_FRONTFOGLAMP,LED_OFF);//�ر�LED��
				}
			}                                             
			break;                                            
		case HAVAL_2012_1P5T_MT_2WD:                            
			if(ON == g_u8IgnSts)
			{
				if(can_id_311_lost_timecnt<CAN_LOST_TIME)
				{
					switch(can2.id_311.BCM_FrontFogLmpSts)
					{
						case 0:
							mid_led_ctl(LED_FRONTFOGLAMP,LED_OFF);
							break;
						case 1:
							mid_led_ctl(LED_FRONTFOGLAMP,LED_ON);
							break;
						case 3:
							mid_led_ctl(LED_FRONTFOGLAMP,LED_OFF);
							break;
						default:
							mid_led_ctl(LED_FRONTFOGLAMP,LED_OFF);
							break;
					}
					mid_led_ctl(LED_FRONTFOGLAMP,(LED_STS_e)(can2.id_311.BCM_FrontFogLmpSts));
				}
				else
				{
					mid_led_ctl(LED_FRONTFOGLAMP,LED_OFF);
				}
			}
			else
			{
				if(lin_pid_0x42_lost_timecnt<LIN_LOST_TIME)
				{
					mid_led_ctl(LED_FRONTFOGLAMP,(LED_STS_e)(lin2_msg.pid_0x42.FrontFogLampSts));
				}
				else/*����2.5Sû���յ���ص�LIN����*/
				{
					mid_led_ctl(LED_FRONTFOGLAMP,LED_OFF);//�ر�LED��
				}
			}                                             
			break;  
		case HAVAL_2015_1P5T_MT_2WD: 
		case HAVAL_2015_1P5T_6AT_2WD:                           
			if(ON == g_u8IgnSts)
			{
				if(can_id_311_lost_timecnt<CAN_LOST_TIME)
				{
					switch(can3.id_311.BCM_FrontFogLmpSts)
					{
						case 0:
							mid_led_ctl(LED_FRONTFOGLAMP,LED_OFF);
							break;
						case 1:
							mid_led_ctl(LED_FRONTFOGLAMP,LED_ON);
							break;
						case 3:
							mid_led_ctl(LED_FRONTFOGLAMP,LED_OFF);
							break;
						default:
							mid_led_ctl(LED_FRONTFOGLAMP,LED_OFF);
							break;
					}
					mid_led_ctl(LED_FRONTFOGLAMP,(LED_STS_e)(can3.id_311.BCM_FrontFogLmpSts));
				}
				else
				{
					mid_led_ctl(LED_FRONTFOGLAMP,LED_OFF);
				}
			}
			else
			{
				if(lin_pid_0x42_lost_timecnt<LIN_LOST_TIME)
				{
					mid_led_ctl(LED_FRONTFOGLAMP,(LED_STS_e)(lin3_msg.pid_0x42.FrontFogLampSts));
				}
				else/*����2.5Sû���յ���ص�LIN����*/
				{
					mid_led_ctl(LED_FRONTFOGLAMP,LED_OFF);//�ر�LED��
				}
			}                                        
			break; 
		case HAVAL_2011_2P0T_MT_2WD:                            
			if(ON == g_u8IgnSts)
			{
				if(can_id_311_lost_timecnt<CAN_LOST_TIME)
				{
					switch(can4.id_311.BCM_FrontFogLmpSts)
					{
						case 0:
							mid_led_ctl(LED_FRONTFOGLAMP,LED_OFF);
							break;
						case 1:
							mid_led_ctl(LED_FRONTFOGLAMP,LED_ON);
							break;
						case 3:
							mid_led_ctl(LED_FRONTFOGLAMP,LED_OFF);
							break;
						default:
							mid_led_ctl(LED_FRONTFOGLAMP,LED_OFF);
							break;
					}
					mid_led_ctl(LED_FRONTFOGLAMP,(LED_STS_e)(can4.id_311.BCM_FrontFogLmpSts));
				}
				else
				{
					mid_led_ctl(LED_FRONTFOGLAMP,LED_OFF);
				}
			}
			else
			{
				if(lin_pid_0x42_lost_timecnt<LIN_LOST_TIME)
				{
					mid_led_ctl(LED_FRONTFOGLAMP,(LED_STS_e)(lin4_msg.pid_0x42.FrontFogLampSts));
				}
				else/*����2.5Sû���յ���ص�LIN����*/
				{
					mid_led_ctl(LED_FRONTFOGLAMP,LED_OFF);//�ر�LED��
				}
			}                                             
			break; 
		case HAVAL_2013_1P5T_MT_4WD:                            
			if(ON == g_u8IgnSts)
			{
				if(can_id_311_lost_timecnt<CAN_LOST_TIME)
				{
					switch(can5.id_311.BCM_FrontFogLmpSts)
					{
						case 0:
							mid_led_ctl(LED_FRONTFOGLAMP,LED_OFF);
							break;
						case 1:
							mid_led_ctl(LED_FRONTFOGLAMP,LED_ON);
							break;
						case 3:
							mid_led_ctl(LED_FRONTFOGLAMP,LED_OFF);
							break;
						default:
							mid_led_ctl(LED_FRONTFOGLAMP,LED_OFF);
							break;
					}
					mid_led_ctl(LED_FRONTFOGLAMP,(LED_STS_e)(can5.id_311.BCM_FrontFogLmpSts));
				}
				else
				{
					mid_led_ctl(LED_FRONTFOGLAMP,LED_OFF);
				}
			}
			else
			{
				if(lin_pid_0x42_lost_timecnt<LIN_LOST_TIME)
				{
					mid_led_ctl(LED_FRONTFOGLAMP,(LED_STS_e)(lin5_msg.pid_0x42.FrontFogLampSts));
				}
				else/*����2.5Sû���յ���ص�LIN����*/
				{
					mid_led_ctl(LED_FRONTFOGLAMP,LED_OFF);//�ر�LED��
				}
			}                                             
			break;                                           
	}  
	#endif
}

/* Զ��� */
void led_highbeam_ctl(void)
{
#if 0
	switch(IPconfig.protocol)                               
	{                                                     
		case HAVAL_2013_2P4L_AT_2WD:                            
			if(ON == g_u8IgnSts)
			{
				if(can_id_311_lost_timecnt<CAN_LOST_TIME)
				{
					mid_led_ctl(LED_HIGHBEAM,(LED_STS_e)(can0.id_311.BCM_HighBeamSts));       
				}
				else
				{
					mid_led_ctl(LED_HIGHBEAM,LED_OFF);
				}
			}
			else
			{
				if(lin_pid_0x42_lost_timecnt<LIN_LOST_TIME)
				{
					mid_led_ctl(LED_HIGHBEAM,(LED_STS_e)(lin0_msg.pid_0x42.HighBeamCmdSts));
				}
				else
				{
					mid_led_ctl(LED_HIGHBEAM,LED_OFF);
				}
			}                                            
			break;                                            
		case HAVAL_2013_1P5T_MT_2WD:                            
			if(ON == g_u8IgnSts)
			{
				if(can_id_311_lost_timecnt<CAN_LOST_TIME)
				{
					mid_led_ctl(LED_HIGHBEAM,(LED_STS_e)(can1.id_311.BCM_HighBeamSts));       
				}
				else
				{
					mid_led_ctl(LED_HIGHBEAM,LED_OFF);
				}
			}
			else
			{
				if(lin_pid_0x42_lost_timecnt<LIN_LOST_TIME)
				{
					mid_led_ctl(LED_HIGHBEAM,(LED_STS_e)(lin1_msg.pid_0x42.HighBeamCmdSts));
				}
				else
				{
					mid_led_ctl(LED_HIGHBEAM,LED_OFF);
				}
			}                                             
			break;                                            
		case HAVAL_2012_1P5T_MT_2WD:                            
			if(ON == g_u8IgnSts)
			{
				if(can_id_311_lost_timecnt<CAN_LOST_TIME)
				{
					mid_led_ctl(LED_HIGHBEAM,(LED_STS_e)(can2.id_311.BCM_HighBeamSts));       
				}
				else
				{
					mid_led_ctl(LED_HIGHBEAM,LED_OFF);
				}
			}
			else
			{
				if(lin_pid_0x42_lost_timecnt<LIN_LOST_TIME)
				{
					mid_led_ctl(LED_HIGHBEAM,(LED_STS_e)(lin2_msg.pid_0x42.HighBeamCmdSts));
				}
				else
				{
					mid_led_ctl(LED_HIGHBEAM,LED_OFF);
				}
			}                                             
			break;   
		case HAVAL_2015_1P5T_MT_2WD: 
		case HAVAL_2015_1P5T_6AT_2WD:                           
			if(ON == g_u8IgnSts)
			{
				if(can_id_311_lost_timecnt<CAN_LOST_TIME)
				{
					mid_led_ctl(LED_HIGHBEAM,(LED_STS_e)(can3.id_311.BCM_HighBeamSts));       
				}
				else
				{
					mid_led_ctl(LED_HIGHBEAM,LED_OFF);
				}
			}
			else
			{
				if(lin_pid_0x42_lost_timecnt<LIN_LOST_TIME)
				{
					mid_led_ctl(LED_HIGHBEAM,(LED_STS_e)(lin3_msg.pid_0x42.HighBeamCmdSts));
				}
				else
				{
					mid_led_ctl(LED_HIGHBEAM,LED_OFF);
				}
			}                                        
			break; 
		case HAVAL_2011_2P0T_MT_2WD:                            
			if(ON == g_u8IgnSts)
			{
				if(can_id_311_lost_timecnt<CAN_LOST_TIME)
				{
					mid_led_ctl(LED_HIGHBEAM,(LED_STS_e)(can4.id_311.BCM_HighBeamSts));       
				}
				else
				{
					mid_led_ctl(LED_HIGHBEAM,LED_OFF);
				}
			}
			else
			{
				if(lin_pid_0x42_lost_timecnt<LIN_LOST_TIME)
				{
					mid_led_ctl(LED_HIGHBEAM,(LED_STS_e)(lin4_msg.pid_0x42.HighBeamCmdSts));
				}
				else
				{
					mid_led_ctl(LED_HIGHBEAM,LED_OFF);
				}
			}                                             
			break;  
		case HAVAL_2013_1P5T_MT_4WD:                            
			if(ON == g_u8IgnSts)
			{
				if(can_id_311_lost_timecnt<CAN_LOST_TIME)
				{
					mid_led_ctl(LED_HIGHBEAM,(LED_STS_e)(can5.id_311.BCM_HighBeamSts));       
				}
				else
				{
					mid_led_ctl(LED_HIGHBEAM,LED_OFF);
				}
			}
			else
			{
				if(lin_pid_0x42_lost_timecnt<LIN_LOST_TIME)
				{
					mid_led_ctl(LED_HIGHBEAM,(LED_STS_e)(lin5_msg.pid_0x42.HighBeamCmdSts));
				}
				else
				{
					mid_led_ctl(LED_HIGHBEAM,LED_OFF);
				}
			}                                             
			break;                                              
	}  
	#endif
}

/* ��ȫ�� */
void led_safetybelt_ctl(void)
{
	/*if(ON == g_u8IgnSts)
	{
		if(can_id_351_lost_timecnt<CAN_LOST_TIME)//����ȫ�� CAN����
		{
			switch(IPconfig.protocol)
			{
				case HAVAL_2013_2P4L_AT_2WD:
					mid_led_ctl(LED_SAFETYBELTSIDE,(LED_STS_e)(can0.id_351.ABM_PsngrSeatBeltSts));
					break;
				case HAVAL_2013_1P5T_MT_2WD:
					mid_led_ctl(LED_SAFETYBELTSIDE,(LED_STS_e)(can1.id_351.ABM_PsngrSeatBeltSts));
					break;
				case HAVAL_2012_1P5T_MT_2WD:
					mid_led_ctl(LED_SAFETYBELTSIDE,(LED_STS_e)(can2.id_351.ABM_PsngrSeatBeltSts));
					break;
				case HAVAL_2015_1P5T_MT_2WD:
				case HAVAL_2015_1P5T_6AT_2WD:
					mid_led_ctl(LED_SAFETYBELTSIDE,(LED_STS_e)(can3.id_351.ABM_PsngrSeatBeltSts));
					break;
				case HAVAL_2011_2P0T_MT_2WD:
					mid_led_ctl(LED_SAFETYBELTSIDE,(LED_STS_e)(can4.id_351.ABM_PsngrSeatBeltSts));
					break;
				case HAVAL_2013_1P5T_MT_4WD:
					mid_led_ctl(LED_SAFETYBELTSIDE,(LED_STS_e)(can5.id_351.ABM_PsngrSeatBeltSts));
					break;
			}
		}
		else
		{
			mid_led_ctl(LED_SAFETYBELTSIDE,LED_ON);
		}

		if(OFF==mid_get_io_sts(&pin_io_in[PIN_IN_MAIN_BELT]))//����ȫ�� PIN��4����,����Ч
		{
			uart_special_data.specialInfo.LedWarning.first.bit.SeatbeltsNotFastened=1;
		}
		else
		{
			uart_special_data.specialInfo.LedWarning.first.bit.SeatbeltsNotFastened=0;
		}
	}
	else
	{
		mid_led_ctl(LED_SAFETYBELTSIDE,LED_OFF);
		uart_special_data.specialInfo.LedWarning.first.bit.SeatbeltsNotFastened=0;
	}*/
}

/* ��ɲ */
void led_braking_ctl(void)
{
	switch(IPconfig.protocol)                               
	{/*
		case HAVAL_2013_2P4L_AT_2WD:                            
			if(ON == g_u8IgnSts)
			{
				if(can_id_311_lost_timecnt<CAN_LOST_TIME)
				{
					uart_special_data.specialInfo.LedWarning.third.bit.BrakingFault=can0.id_311.BCM_ParkBrakeSts;
				}
				else
				{
					uart_special_data.specialInfo.LedWarning.third.bit.BrakingFault=0;
				}
			}
			else
			{
				if((lin_pid_0x42_lost_timecnt<LIN_LOST_TIME)&&(app_get_car_key_pos()==KEY_IN))
				{
					uart_special_data.specialInfo.LedWarning.third.bit.BrakingFault=lin0_msg.pid_0x42.ParkLampSts;
				}
				else
				{
					uart_special_data.specialInfo.LedWarning.third.bit.BrakingFault=0;
				}
			}
			
			if(OFF==mid_get_io_sts(&pin_io_in[PIN_IN_BRAKE_WARNING]))
			{
				uart_special_data.specialInfo.LedWarning.third.bit.BrakingFault=1;
			}                                            
			break;                                            
		case HAVAL_2013_1P5T_MT_2WD:                            
			if(ON == g_u8IgnSts)
			{
				if(can_id_311_lost_timecnt<CAN_LOST_TIME)
				{
					uart_special_data.specialInfo.LedWarning.third.bit.BrakingFault=can1.id_311.BCM_ParkBrakeSts;
				}
				else
				{
					uart_special_data.specialInfo.LedWarning.third.bit.BrakingFault=0;
				}
			}
			else
			{
				if((lin_pid_0x42_lost_timecnt<LIN_LOST_TIME)&&(app_get_car_key_pos()==KEY_IN))
				{
					uart_special_data.specialInfo.LedWarning.third.bit.BrakingFault=lin1_msg.pid_0x42.ParkLampSts;
				}
				else
				{
					uart_special_data.specialInfo.LedWarning.third.bit.BrakingFault=0;
				}
			}
			
			if(OFF==mid_get_io_sts(&pin_io_in[PIN_IN_BRAKE_WARNING]))
			{
				uart_special_data.specialInfo.LedWarning.third.bit.BrakingFault=1;
			}                                             
			break;                                            
		case HAVAL_2012_1P5T_MT_2WD:                            
			if(ON == g_u8IgnSts)
			{
				if(can_id_311_lost_timecnt<CAN_LOST_TIME)
				{
					uart_special_data.specialInfo.LedWarning.third.bit.BrakingFault=can2.id_311.BCM_ParkBrakeSts;
				}
				else
				{
					uart_special_data.specialInfo.LedWarning.third.bit.BrakingFault=0;
				}
			}
			else
			{
				if((lin_pid_0x42_lost_timecnt<LIN_LOST_TIME)&&(app_get_car_key_pos()==KEY_IN))
				{
					uart_special_data.specialInfo.LedWarning.third.bit.BrakingFault=lin2_msg.pid_0x42.ParkLampSts;
				}
				else
				{
					uart_special_data.specialInfo.LedWarning.third.bit.BrakingFault=0;
				}
			}
			
			if(OFF==mid_get_io_sts(&pin_io_in[PIN_IN_BRAKE_WARNING]))
			{
				uart_special_data.specialInfo.LedWarning.third.bit.BrakingFault=1;
			}                                             
			break;      
		case HAVAL_2015_1P5T_MT_2WD: 
		case HAVAL_2015_1P5T_6AT_2WD:                           
			if(ON == g_u8IgnSts)
			{
				if(can_id_311_lost_timecnt<CAN_LOST_TIME)
				{
					uart_special_data.specialInfo.LedWarning.third.bit.BrakingFault=can3.id_311.BCM_ParkBrakeSts;
				}
				else
				{
					uart_special_data.specialInfo.LedWarning.third.bit.BrakingFault=0;
				}
			}
			else
			{
				if((lin_pid_0x42_lost_timecnt<LIN_LOST_TIME)&&(app_get_car_key_pos()==KEY_IN))
				{
					uart_special_data.specialInfo.LedWarning.third.bit.BrakingFault=lin3_msg.pid_0x42.ParkLampSts;
				}
				else
				{
					uart_special_data.specialInfo.LedWarning.third.bit.BrakingFault=0;
				}
			}
			
			if(OFF==mid_get_io_sts(&pin_io_in[PIN_IN_BRAKE_WARNING]))
			{
				uart_special_data.specialInfo.LedWarning.third.bit.BrakingFault=1;
			}                                        
			break;   
		case HAVAL_2011_2P0T_MT_2WD:                            
			if(ON == g_u8IgnSts)
			{
				if(can_id_311_lost_timecnt<CAN_LOST_TIME)
				{
					uart_special_data.specialInfo.LedWarning.third.bit.BrakingFault=can4.id_311.BCM_ParkBrakeSts;
				}
				else
				{
					uart_special_data.specialInfo.LedWarning.third.bit.BrakingFault=0;
				}
			}
			else
			{
				if((lin_pid_0x42_lost_timecnt<LIN_LOST_TIME)&&(app_get_car_key_pos()==KEY_IN))
				{
					uart_special_data.specialInfo.LedWarning.third.bit.BrakingFault=lin4_msg.pid_0x42.ParkLampSts;
				}
				else
				{
					uart_special_data.specialInfo.LedWarning.third.bit.BrakingFault=0;
				}
			}
			
			if(OFF==mid_get_io_sts(&pin_io_in[PIN_IN_BRAKE_WARNING]))
			{
				uart_special_data.specialInfo.LedWarning.third.bit.BrakingFault=1;
			}                                             
			break;     
		case HAVAL_2013_1P5T_MT_4WD:                            
			if(ON == g_u8IgnSts)
			{
				if(can_id_311_lost_timecnt<CAN_LOST_TIME)
				{
					uart_special_data.specialInfo.LedWarning.third.bit.BrakingFault=can5.id_311.BCM_ParkBrakeSts;
				}
				else
				{
					uart_special_data.specialInfo.LedWarning.third.bit.BrakingFault=0;
				}
			}
			else
			{
				if((lin_pid_0x42_lost_timecnt<LIN_LOST_TIME)&&(app_get_car_key_pos()==KEY_IN))
				{
					uart_special_data.specialInfo.LedWarning.third.bit.BrakingFault=lin5_msg.pid_0x42.ParkLampSts;
				}
				else
				{
					uart_special_data.specialInfo.LedWarning.third.bit.BrakingFault=0;
				}
			}
			
			if(OFF==mid_get_io_sts(&pin_io_in[PIN_IN_BRAKE_WARNING]))
			{
				uart_special_data.specialInfo.LedWarning.third.bit.BrakingFault=1;
			}                                             
			break;                                    
	*/}
}

/* ���������� */
void led_engine_failure_ctl(void)
{
	switch(IPconfig.protocol)
	{/*
		case HAVAL_2013_2P4L_AT_2WD:
			if(ON == g_u8IgnSts)
			{
				if(can_id_371_lost_timecnt<CAN_LOST_TIME)
				{
					uart_special_data.specialInfo.LedWarning.third.bit.EngineError=can0.id_371.ECM_MILSts;
				}
				else
				{
					uart_special_data.specialInfo.LedWarning.third.bit.EngineError=1;
				}
			}
			else
			{
				uart_special_data.specialInfo.LedWarning.third.bit.EngineError=0;
			}
			break;
		case HAVAL_2013_1P5T_MT_2WD:
			if(ON == g_u8IgnSts)
			{
				if(can_id_271_lost_timecnt<CAN_LOST_TIME)
				{
					uart_special_data.specialInfo.LedWarning.third.bit.EngineError=can1.id_271.ECM_MILSts;
				}
				else
				{
					uart_special_data.specialInfo.LedWarning.third.bit.EngineError=1;
				}
			}
			else
			{
				uart_special_data.specialInfo.LedWarning.third.bit.EngineError=0;
			}
			break;
		case HAVAL_2012_1P5T_MT_2WD:
			if(ON == g_u8IgnSts)
			{
				if(can_id_271_lost_timecnt<CAN_LOST_TIME)
				{
					uart_special_data.specialInfo.LedWarning.third.bit.EngineError=can2.id_271.ECM_MILSts;
				}
				else
				{
					uart_special_data.specialInfo.LedWarning.third.bit.EngineError=1;
				}
			}
			else
			{
				uart_special_data.specialInfo.LedWarning.third.bit.EngineError=0;
			}
			break;
		case HAVAL_2015_1P5T_MT_2WD: 
		case HAVAL_2015_1P5T_6AT_2WD:
			if(ON == g_u8IgnSts)
			{
				if(can_id_271_lost_timecnt<CAN_LOST_TIME)
				{
					uart_special_data.specialInfo.LedWarning.third.bit.EngineError=can3.id_271.ECM_MILSts;
				}
				else
				{
					uart_special_data.specialInfo.LedWarning.third.bit.EngineError=1;
				}
			}
			else
			{
				uart_special_data.specialInfo.LedWarning.third.bit.EngineError=0;
			}
			break;
		case HAVAL_2011_2P0T_MT_2WD:
			if(ON == g_u8IgnSts)
			{
				if(can_id_271_lost_timecnt<CAN_LOST_TIME)
				{
					uart_special_data.specialInfo.LedWarning.third.bit.EngineError=can4.id_271.ECM_MILSts;
				}
				else
				{
					uart_special_data.specialInfo.LedWarning.third.bit.EngineError=1;
				}
			}
			else
			{
				uart_special_data.specialInfo.LedWarning.third.bit.EngineError=0;
			}
			break;
		case HAVAL_2013_1P5T_MT_4WD:
			if(ON == g_u8IgnSts)
			{
				if(can_id_271_lost_timecnt<CAN_LOST_TIME)
				{
					uart_special_data.specialInfo.LedWarning.third.bit.EngineError=can5.id_271.ECM_MILSts;
				}
				else
				{
					uart_special_data.specialInfo.LedWarning.third.bit.EngineError=1;
				}
			}
			else
			{
				uart_special_data.specialInfo.LedWarning.third.bit.EngineError=0;
			}
			break;
	*/}
}

/* ȼ�ͱ��� */
void led_fuel_warning_ctrl(void)//ȼ�ͱ������жϷ��ڽ���ȥ��
{
	if(ON == g_u8IgnSts)
	{
	}
	else
	{
		
	}
}

/* ˮ�±��� */
void led_temp_warning_ctrl(void)//ˮ�±������жϷ��ڽ���ȥ��
{
	if(ON == g_u8IgnSts)
	{
	}
	else
	{
		
	}
}

/* ���ٱ��� */
void led_120km_warning_ctrl(void)//���ٱ������жϷ��ڽ���ȥ��
{
	if(ON == g_u8IgnSts)
	{
	}
	else
	{
	}
}

/* ABS���� */
void led_abs_failure_ctrl(void)
{/*
	if(ON == g_u8IgnSts)
	{
		if(can_id_265_lost_timecnt<CAN_LOST_TIME)
		{
			switch(IPconfig.protocol)                               
			{                                                     
				case HAVAL_2013_2P4L_AT_2WD:                            
					uart_special_data.specialInfo.LedWarning.first.bit.ABSError=can0.id_265.ABS_ABSFault;                                            
					break;                                            
				case HAVAL_2013_1P5T_MT_2WD:                            
					uart_special_data.specialInfo.LedWarning.first.bit.ABSError=can1.id_265.ABS_ABSFault;                                             
					break;                                            
				case HAVAL_2012_1P5T_MT_2WD:                            
					uart_special_data.specialInfo.LedWarning.first.bit.ABSError=can2.id_265.ABS_ABSFault;                                             
					break;       
				case HAVAL_2015_1P5T_MT_2WD: 
				case HAVAL_2015_1P5T_6AT_2WD:                           
					uart_special_data.specialInfo.LedWarning.first.bit.ABSError=can3.id_265.ABS_ABSFault;                                        
					break;  
				case HAVAL_2011_2P0T_MT_2WD:                            
					uart_special_data.specialInfo.LedWarning.first.bit.ABSError=can4.id_265.ABS_ABSFault;                                             
					break;
				case HAVAL_2013_1P5T_MT_4WD:                            
					uart_special_data.specialInfo.LedWarning.first.bit.ABSError=can5.id_265.ABS_ABSFault;                                             
					break;                                          
			}     
		}
		else
		{
			uart_special_data.specialInfo.LedWarning.first.bit.ABSError=1;
		}
	}
	else
	{
		uart_special_data.specialInfo.LedWarning.first.bit.ABSError=0;
	}
*/}

/* ��ѹ���� */
void led_oil_press_ctrl(void)
{/*
	if(OFF == g_u8IgnSts)
	{
		uart_special_data.specialInfo.LedWarning.first.bit.OilPressWarning = 0;
		return;
	}
	else
	{
		if(ON == mid_get_io_sts(&pin_io_in[PIN_IN_OIL_PRESS]))
		{
			uart_special_data.specialInfo.LedWarning.first.bit.OilPressWarning = 0;
			return;
		}
		else
		{
			uart_special_data.specialInfo.LedWarning.first.bit.OilPressWarning = 1;
		}		
	}
*/
}

/* ���������� */
void led_engine_maintain_ctrl(void)
{/*
	switch(IPconfig.protocol)
	{
		case HAVAL_2013_2P4L_AT_2WD:
			if(ON == g_u8IgnSts)
			{
				if(can_id_371_lost_timecnt<CAN_LOST_TIME)
				{
					uart_special_data.specialInfo.LedWarning.first.bit.EngineMaintenancee=can0.id_371.ECM_EngineSVSSignal;
				}
				else
				{
					uart_special_data.specialInfo.LedWarning.first.bit.EngineMaintenancee=0;
				}
			}
			else
			{
				uart_special_data.specialInfo.LedWarning.first.bit.EngineMaintenancee=0;
			}
			break;
		case HAVAL_2013_1P5T_MT_2WD:
			if(ON == g_u8IgnSts)
			{
				if(can_id_271_lost_timecnt<CAN_LOST_TIME)
				{
					uart_special_data.specialInfo.LedWarning.first.bit.EngineMaintenancee=can1.id_271.ECM_EngineSVSSignal;
				}
				else
				{
					uart_special_data.specialInfo.LedWarning.first.bit.EngineMaintenancee=0;
				}
			}
			else
			{
				uart_special_data.specialInfo.LedWarning.first.bit.EngineMaintenancee=0;
			}
			break;
		case HAVAL_2012_1P5T_MT_2WD:
			if(ON == g_u8IgnSts)
			{
				if(can_id_271_lost_timecnt<CAN_LOST_TIME)
				{
					uart_special_data.specialInfo.LedWarning.first.bit.EngineMaintenancee=can2.id_271.ECM_EngineSVSSignal;
				}
				else
				{
					uart_special_data.specialInfo.LedWarning.first.bit.EngineMaintenancee=0;
				}
			}
			else
			{
				uart_special_data.specialInfo.LedWarning.first.bit.EngineMaintenancee=0;
			}
			break;
		case HAVAL_2015_1P5T_MT_2WD: 
		case HAVAL_2015_1P5T_6AT_2WD:
			if(ON == g_u8IgnSts)
			{
				if(can_id_271_lost_timecnt<CAN_LOST_TIME)
				{
					uart_special_data.specialInfo.LedWarning.first.bit.EngineMaintenancee=can3.id_271.ECM_EngineSVSSignal;
				}
				else
				{
					uart_special_data.specialInfo.LedWarning.first.bit.EngineMaintenancee=0;
				}
			}
			else
			{
				uart_special_data.specialInfo.LedWarning.first.bit.EngineMaintenancee=0;
			}
			break;
		case HAVAL_2011_2P0T_MT_2WD:
			if(ON == g_u8IgnSts)
			{
				if(can_id_271_lost_timecnt<CAN_LOST_TIME)
				{
					uart_special_data.specialInfo.LedWarning.first.bit.EngineMaintenancee=can4.id_271.ECM_EngineSVSSignal;
				}
				else
				{
					uart_special_data.specialInfo.LedWarning.first.bit.EngineMaintenancee=0;
				}
			}
			else
			{
				uart_special_data.specialInfo.LedWarning.first.bit.EngineMaintenancee=0;
			}
			break;
		case HAVAL_2013_1P5T_MT_4WD:
			if(ON == g_u8IgnSts)
			{
				if(can_id_271_lost_timecnt<CAN_LOST_TIME)
				{
					uart_special_data.specialInfo.LedWarning.first.bit.EngineMaintenancee=can5.id_271.ECM_EngineSVSSignal;
				}
				else
				{
					uart_special_data.specialInfo.LedWarning.first.bit.EngineMaintenancee=0;
				}
			}
			else
			{
				uart_special_data.specialInfo.LedWarning.first.bit.EngineMaintenancee=0;
			}
			break;
	}
*/}

/* �Զ���� */
void led_lamp_auto_light_ctrl(void)
{/*
	switch(IPconfig.protocol)
	{
		case HAVAL_2013_2P4L_AT_2WD:
			if(ON == g_u8IgnSts)
			{
				if(can_id_311_lost_timecnt<CAN_LOST_TIME)
				{
					uart_special_data.specialInfo.LedWarning.first.bit.HeadlampAutoLightError=0;
				}
				else
				{
					uart_special_data.specialInfo.LedWarning.first.bit.HeadlampAutoLightError=1;
				}
			}
			else
			{
				uart_special_data.specialInfo.LedWarning.first.bit.HeadlampAutoLightError=0;
			}
			break;
		case HAVAL_2013_1P5T_MT_2WD:
			if(ON == g_u8IgnSts)
			{
				if(can_id_311_lost_timecnt<CAN_LOST_TIME)
				{
					uart_special_data.specialInfo.LedWarning.first.bit.HeadlampAutoLightError=0;
				}
				else
				{
					uart_special_data.specialInfo.LedWarning.first.bit.HeadlampAutoLightError=1;
				}
			}
			else
			{
				uart_special_data.specialInfo.LedWarning.first.bit.HeadlampAutoLightError=0;
			}
			break;
		case HAVAL_2012_1P5T_MT_2WD:
			if(ON == g_u8IgnSts)
			{
				if(can_id_311_lost_timecnt<CAN_LOST_TIME)
				{
					uart_special_data.specialInfo.LedWarning.first.bit.HeadlampAutoLightError=0;
				}
				else
				{
					uart_special_data.specialInfo.LedWarning.first.bit.HeadlampAutoLightError=1;
				}
			}
			else
			{
				uart_special_data.specialInfo.LedWarning.first.bit.HeadlampAutoLightError=0;
			}
			break;
		case HAVAL_2015_1P5T_MT_2WD: 
		case HAVAL_2015_1P5T_6AT_2WD:
			if(ON == g_u8IgnSts)
			{
				if(can_id_311_lost_timecnt<CAN_LOST_TIME)
				{
					uart_special_data.specialInfo.LedWarning.first.bit.HeadlampAutoLightError=0;
				}
				else
				{
					uart_special_data.specialInfo.LedWarning.first.bit.HeadlampAutoLightError=1;
				}
			}
			else
			{
				uart_special_data.specialInfo.LedWarning.first.bit.HeadlampAutoLightError=0;
			}
			break;
		case HAVAL_2011_2P0T_MT_2WD:
			if(ON == g_u8IgnSts)
			{
				if(can_id_311_lost_timecnt<CAN_LOST_TIME)
				{
					uart_special_data.specialInfo.LedWarning.first.bit.HeadlampAutoLightError=0;
				}
				else
				{
					uart_special_data.specialInfo.LedWarning.first.bit.HeadlampAutoLightError=1;
				}
			}
			else
			{
				uart_special_data.specialInfo.LedWarning.first.bit.HeadlampAutoLightError=0;
			}
			break;
		case HAVAL_2013_1P5T_MT_4WD:
			if(ON == g_u8IgnSts)
			{
				if(can_id_311_lost_timecnt<CAN_LOST_TIME)
				{
					uart_special_data.specialInfo.LedWarning.first.bit.HeadlampAutoLightError=0;
				}
				else
				{
					uart_special_data.specialInfo.LedWarning.first.bit.HeadlampAutoLightError=1;
				}
			}
			else
			{
				uart_special_data.specialInfo.LedWarning.first.bit.HeadlampAutoLightError=0;
			}
			break;
	}
*/}

/* ����Ѳ�� */
void led_cruise_ctrl(void)
{/*
	switch(IPconfig.protocol)
	{
		case HAVAL_2013_2P4L_AT_2WD:
			if(ON == g_u8IgnSts)
			{
				if(can_id_371_lost_timecnt<CAN_LOST_TIME)
				{
					switch(can0.id_371.ECM_CruiseCtrlAct)
					{
						case 0:
							uart_special_data.specialInfo.LedWarning.first.bit.CruiceControlSystemMain=0;
							uart_special_data.specialInfo.LedWarning.third.bit.CruiceControlSystem=0;
							break;
						case 1:
							uart_special_data.specialInfo.LedWarning.first.bit.CruiceControlSystemMain=1;
							uart_special_data.specialInfo.LedWarning.third.bit.CruiceControlSystem=1;
							break;
						case 2:
							uart_special_data.specialInfo.LedWarning.first.bit.CruiceControlSystemMain=1;
							uart_special_data.specialInfo.LedWarning.third.bit.CruiceControlSystem=0;
							break;
						default:
							break;
					}
				}
				else
				{
					uart_special_data.specialInfo.LedWarning.first.bit.CruiceControlSystemMain=0;
					uart_special_data.specialInfo.LedWarning.third.bit.CruiceControlSystem=0;
				}
			}
			else
			{
				uart_special_data.specialInfo.LedWarning.first.bit.CruiceControlSystemMain=0;
				uart_special_data.specialInfo.LedWarning.third.bit.CruiceControlSystem=0;
			}
			break;
		case HAVAL_2013_1P5T_MT_2WD:
			if(ON == g_u8IgnSts)
			{
				if(can_id_271_lost_timecnt<CAN_LOST_TIME)
				{
					switch(can1.id_271.ECM_CruiseCtrlAct)
					{
						case 0:
							uart_special_data.specialInfo.LedWarning.first.bit.CruiceControlSystemMain=0;
							uart_special_data.specialInfo.LedWarning.third.bit.CruiceControlSystem=0;
							break;
						case 1:
							uart_special_data.specialInfo.LedWarning.first.bit.CruiceControlSystemMain=1;
							uart_special_data.specialInfo.LedWarning.third.bit.CruiceControlSystem=1;
							break;
						case 2:
							uart_special_data.specialInfo.LedWarning.first.bit.CruiceControlSystemMain=1;
							uart_special_data.specialInfo.LedWarning.third.bit.CruiceControlSystem=0;
							break;
						default:
							break;
					}
				}
				else
				{
					uart_special_data.specialInfo.LedWarning.first.bit.CruiceControlSystemMain=0;
					uart_special_data.specialInfo.LedWarning.third.bit.CruiceControlSystem=0;
				}
			}
			else
			{
				uart_special_data.specialInfo.LedWarning.first.bit.CruiceControlSystemMain=0;
				uart_special_data.specialInfo.LedWarning.third.bit.CruiceControlSystem=0;
			}
			break;
		case HAVAL_2012_1P5T_MT_2WD:
			if(ON == g_u8IgnSts)
			{
				if(can_id_271_lost_timecnt<CAN_LOST_TIME)
				{
					switch(can2.id_271.ECM_CruiseCtrlAct)
					{
						case 0:
							uart_special_data.specialInfo.LedWarning.first.bit.CruiceControlSystemMain=0;
							uart_special_data.specialInfo.LedWarning.third.bit.CruiceControlSystem=0;
							break;
						case 1:
							uart_special_data.specialInfo.LedWarning.first.bit.CruiceControlSystemMain=1;
							uart_special_data.specialInfo.LedWarning.third.bit.CruiceControlSystem=1;
							break;
						case 2:
							uart_special_data.specialInfo.LedWarning.first.bit.CruiceControlSystemMain=1;
							uart_special_data.specialInfo.LedWarning.third.bit.CruiceControlSystem=0;
							break;
						default:
							break;
					}
				}
				else
				{
					uart_special_data.specialInfo.LedWarning.first.bit.CruiceControlSystemMain=0;
					uart_special_data.specialInfo.LedWarning.third.bit.CruiceControlSystem=0;
				}
			}
			else
			{
				uart_special_data.specialInfo.LedWarning.first.bit.CruiceControlSystemMain=0;
				uart_special_data.specialInfo.LedWarning.third.bit.CruiceControlSystem=0;
			}
			break;
		case HAVAL_2015_1P5T_MT_2WD: 
		case HAVAL_2015_1P5T_6AT_2WD:
			if(ON == g_u8IgnSts)
			{
				if(can_id_271_lost_timecnt<CAN_LOST_TIME)
				{
					switch(can3.id_271.ECM_CruiseCtrlAct)
					{
						case 0:
							uart_special_data.specialInfo.LedWarning.first.bit.CruiceControlSystemMain=0;
							uart_special_data.specialInfo.LedWarning.third.bit.CruiceControlSystem=0;
							break;
						case 1:
							uart_special_data.specialInfo.LedWarning.first.bit.CruiceControlSystemMain=1;
							uart_special_data.specialInfo.LedWarning.third.bit.CruiceControlSystem=1;
							break;
						case 2:
							uart_special_data.specialInfo.LedWarning.first.bit.CruiceControlSystemMain=1;
							uart_special_data.specialInfo.LedWarning.third.bit.CruiceControlSystem=0;
							break;
						default:
							break;
					}
				}
				else
				{
					uart_special_data.specialInfo.LedWarning.first.bit.CruiceControlSystemMain=0;
					uart_special_data.specialInfo.LedWarning.third.bit.CruiceControlSystem=0;
				}
			}
			else
			{
				uart_special_data.specialInfo.LedWarning.first.bit.CruiceControlSystemMain=0;
				uart_special_data.specialInfo.LedWarning.third.bit.CruiceControlSystem=0;
			}
			break;
		case HAVAL_2011_2P0T_MT_2WD:
			if(ON == g_u8IgnSts)
			{
				if(can_id_271_lost_timecnt<CAN_LOST_TIME)
				{
					switch(can4.id_271.ECM_CruiseCtrlAct)
					{
						case 0:
							uart_special_data.specialInfo.LedWarning.first.bit.CruiceControlSystemMain=0;
							uart_special_data.specialInfo.LedWarning.third.bit.CruiceControlSystem=0;
							break;
						case 1:
							uart_special_data.specialInfo.LedWarning.first.bit.CruiceControlSystemMain=1;
							uart_special_data.specialInfo.LedWarning.third.bit.CruiceControlSystem=1;
							break;
						case 2:
							uart_special_data.specialInfo.LedWarning.first.bit.CruiceControlSystemMain=1;
							uart_special_data.specialInfo.LedWarning.third.bit.CruiceControlSystem=0;
							break;
						default:
							break;
					}
				}
				else
				{
					uart_special_data.specialInfo.LedWarning.first.bit.CruiceControlSystemMain=0;
					uart_special_data.specialInfo.LedWarning.third.bit.CruiceControlSystem=0;
				}
			}
			else
			{
				uart_special_data.specialInfo.LedWarning.first.bit.CruiceControlSystemMain=0;
				uart_special_data.specialInfo.LedWarning.third.bit.CruiceControlSystem=0;
			}
			break;
		case HAVAL_2013_1P5T_MT_4WD:
			if(ON == g_u8IgnSts)
			{
				if(can_id_271_lost_timecnt<CAN_LOST_TIME)
				{
					switch(can5.id_271.ECM_CruiseCtrlAct)
					{
						case 0:
							uart_special_data.specialInfo.LedWarning.first.bit.CruiceControlSystemMain=0;
							uart_special_data.specialInfo.LedWarning.third.bit.CruiceControlSystem=0;
							break;
						case 1:
							uart_special_data.specialInfo.LedWarning.first.bit.CruiceControlSystemMain=1;
							uart_special_data.specialInfo.LedWarning.third.bit.CruiceControlSystem=1;
							break;
						case 2:
							uart_special_data.specialInfo.LedWarning.first.bit.CruiceControlSystemMain=1;
							uart_special_data.specialInfo.LedWarning.third.bit.CruiceControlSystem=0;
							break;
						default:
							break;
					}
				}
				else
				{
					uart_special_data.specialInfo.LedWarning.first.bit.CruiceControlSystemMain=0;
					uart_special_data.specialInfo.LedWarning.third.bit.CruiceControlSystem=0;
				}
			}
			else
			{
				uart_special_data.specialInfo.LedWarning.first.bit.CruiceControlSystemMain=0;
				uart_special_data.specialInfo.LedWarning.third.bit.CruiceControlSystem=0;
			}
			break;
	}
*/}

void led_atm_warning_ctrl(void)
{/*
	switch(IPconfig.protocol)
	{
		case HAVAL_2013_2P4L_AT_2WD:
			if(ON == g_u8IgnSts)
			{
				if(can_id_131_lost_timecnt<CAN_LOST_TIME)
				{    
					switch(can0.id_131.TCU_TCUFailure)
					{
						case 0:
							uart_special_data.specialInfo.LedWarning.second.bit.ATMWarning=0;
							break;
						case 1:
							uart_special_data.specialInfo.LedWarning.second.bit.ATMWarning=1;
							break;
						case 2:
							uart_special_data.specialInfo.LedWarning.second.bit.ATMWarning=1;
							break;
						default:
							break;
					}
				}
				else
				{
					uart_special_data.specialInfo.LedWarning.second.bit.ATMWarning=1;
				}
			}
			else
			{
				uart_special_data.specialInfo.LedWarning.second.bit.ATMWarning=0;
			}
			break;
		case HAVAL_2013_1P5T_MT_2WD:
			if(ON == g_u8IgnSts)
			{
				uart_special_data.specialInfo.LedWarning.second.bit.ATMWarning=0;
			}
			else
			{
				uart_special_data.specialInfo.LedWarning.second.bit.ATMWarning=0;
			}
			break;
		case HAVAL_2012_1P5T_MT_2WD:
			if(ON == g_u8IgnSts)
			{
				uart_special_data.specialInfo.LedWarning.second.bit.ATMWarning=0;
			}
			else
			{
				uart_special_data.specialInfo.LedWarning.second.bit.ATMWarning=0;
			}
			break;
		case HAVAL_2015_1P5T_MT_2WD: 
			�ֶ����ĳ���û���Զ����������
			uart_special_data.specialInfo.LedWarning.second.bit.ATMWarning=0;
			break;
		case HAVAL_2015_1P5T_6AT_2WD:
			if(ON == g_u8IgnSts)
			{
				if(can_id_116_lost_timecnt<CAN_LOST_TIME)
				{
					uart_special_data.specialInfo.LedWarning.second.bit.ATMWarning=0;
				}
				else
				{
					uart_special_data.specialInfo.LedWarning.second.bit.ATMWarning=1;
				}
			}
			else
			{
				uart_special_data.specialInfo.LedWarning.second.bit.ATMWarning=0;
			}
			break;
		case HAVAL_2011_2P0T_MT_2WD:
			if(ON == g_u8IgnSts)
			{
				uart_special_data.specialInfo.LedWarning.second.bit.ATMWarning=0;
			}
			else
			{
				uart_special_data.specialInfo.LedWarning.second.bit.ATMWarning=0;
			}
			break;
		case HAVAL_2013_1P5T_MT_4WD:
			if(ON == g_u8IgnSts)
			{
				uart_special_data.specialInfo.LedWarning.second.bit.ATMWarning=0;
			}
			else
			{
				uart_special_data.specialInfo.LedWarning.second.bit.ATMWarning=0;
			}
			break;
	}
*/}

/* ̥ѹ���� */
void led_tpms_warning_ctrl(void)
{/*
	if(ON == g_u8IgnSts)
	{
		if(can_id_311_lost_timecnt<CAN_LOST_TIME)
		{
			switch(IPconfig.protocol)                               
			{                                                     
				case HAVAL_2013_2P4L_AT_2WD:                            
					switch(can0.id_311.BCM_TPWarningInd)                                  
					{                                                                    
						case 0:                                                            
							uart_special_data.specialInfo.LedWarning.first.bit.TpmsWarning=0;
							break;                                                           
						case 1:                                                            
							uart_special_data.specialInfo.LedWarning.first.bit.TpmsWarning=1;
							break;                                                           
						case 3:                                                            
							uart_special_data.specialInfo.LedWarning.first.bit.TpmsWarning=1;
							break;                                                           
						default:                                                           
							break;                                                           
					}                                                                    
					break;                                            
				case HAVAL_2013_1P5T_MT_2WD:                            
					switch(can1.id_311.BCM_TPWarningInd)
					{
						case 0:
							uart_special_data.specialInfo.LedWarning.first.bit.TpmsWarning=0;
							break;
						case 1:
							uart_special_data.specialInfo.LedWarning.first.bit.TpmsWarning=1;
							break;
						case 3:
							uart_special_data.specialInfo.LedWarning.first.bit.TpmsWarning=1;
							break;
						default:
							break;
					}                                             
					break;                                            
				case HAVAL_2012_1P5T_MT_2WD:                            
					switch(can2.id_311.BCM_TPWarningInd)
					{
						case 0:
							uart_special_data.specialInfo.LedWarning.first.bit.TpmsWarning=0;
							break;
						case 1:
							uart_special_data.specialInfo.LedWarning.first.bit.TpmsWarning=1;
							break;
						case 3:
							uart_special_data.specialInfo.LedWarning.first.bit.TpmsWarning=1;
							break;
						default:
							break;
					}                                             
					break;   
				case HAVAL_2015_1P5T_MT_2WD: 
				case HAVAL_2015_1P5T_6AT_2WD:                           
					switch(can3.id_311.BCM_TPWarningInd)
					{
						case 0:
							uart_special_data.specialInfo.LedWarning.first.bit.TpmsWarning=0;
							break;
						case 1:
							uart_special_data.specialInfo.LedWarning.first.bit.TpmsWarning=1;
							break;
						case 3:
							uart_special_data.specialInfo.LedWarning.first.bit.TpmsWarning=1;
							break;
						default:
							break;
					}                                        
					break;
				case HAVAL_2011_2P0T_MT_2WD:                            
					switch(can4.id_311.BCM_TPWarningInd)
					{
						case 0:
							uart_special_data.specialInfo.LedWarning.first.bit.TpmsWarning=0;
							break;
						case 1:
							uart_special_data.specialInfo.LedWarning.first.bit.TpmsWarning=1;
							break;
						case 3:
							uart_special_data.specialInfo.LedWarning.first.bit.TpmsWarning=1;
							break;
						default:
							break;
					}                                             
					break;  
				case HAVAL_2013_1P5T_MT_4WD:                            
					switch(can5.id_311.BCM_TPWarningInd)
					{
						case 0:
							uart_special_data.specialInfo.LedWarning.first.bit.TpmsWarning=0;
							break;
						case 1:
							uart_special_data.specialInfo.LedWarning.first.bit.TpmsWarning=1;
							break;
						case 3:
							uart_special_data.specialInfo.LedWarning.first.bit.TpmsWarning=1;
							break;
						default:
							break;
					}                                             
					break;                                          
			}
		}
		else
		{
			uart_special_data.specialInfo.LedWarning.first.bit.TpmsWarning=1;
		}
		
		if(uart_special_data.specialInfo.LedWarning.second.bit.TpmsError)//���TPMS��������ô�õ�ҲҪ��
		{
			uart_special_data.specialInfo.LedWarning.first.bit.TpmsWarning=1;
		}
	}
	else
	{
		uart_special_data.specialInfo.LedWarning.first.bit.TpmsWarning=0;
	}
*/}

/* ̥ѹ���� */
void led_tpms_failure_ctrl(void)
{/*
	if(ON == g_u8IgnSts)
	{
		if(can_id_311_lost_timecnt<CAN_LOST_TIME)
		{
			switch(IPconfig.protocol)                               
			{                                                     
				case HAVAL_2013_2P4L_AT_2WD:                            
					switch(can0.id_311.BCM_TPMSFailureInd)
					{
						case 0:
							uart_special_data.specialInfo.LedWarning.second.bit.TpmsError=0;
							break;
						case 1:
							uart_special_data.specialInfo.LedWarning.second.bit.TpmsError=1;
							break;
						case 3:
							uart_special_data.specialInfo.LedWarning.second.bit.TpmsError=1;
							break;
						default:
							break;
					}                                             
					break;                                            
				case HAVAL_2013_1P5T_MT_2WD:                            
					switch(can1.id_311.BCM_TPMSFailureInd)
					{
						case 0:
							uart_special_data.specialInfo.LedWarning.second.bit.TpmsError=0;
							break;
						case 1:
							uart_special_data.specialInfo.LedWarning.second.bit.TpmsError=1;
							break;
						case 3:
							uart_special_data.specialInfo.LedWarning.second.bit.TpmsError=1;
							break;
						default:
							break;
					}                                             
					break;                                            
				case HAVAL_2012_1P5T_MT_2WD:                            
					switch(can2.id_311.BCM_TPMSFailureInd)
					{
						case 0:
							uart_special_data.specialInfo.LedWarning.second.bit.TpmsError=0;
							break;
						case 1:
							uart_special_data.specialInfo.LedWarning.second.bit.TpmsError=1;
							break;
						case 3:
							uart_special_data.specialInfo.LedWarning.second.bit.TpmsError=1;
							break;
						default:
							break;
					}                                             
					break;
				case HAVAL_2015_1P5T_MT_2WD: 
				case HAVAL_2015_1P5T_6AT_2WD:                           
					switch(can3.id_311.BCM_TPMSFailureInd)
					{
						case 0:
							uart_special_data.specialInfo.LedWarning.second.bit.TpmsError=0;
							break;
						case 1:
							uart_special_data.specialInfo.LedWarning.second.bit.TpmsError=1;
							break;
						case 3:
							uart_special_data.specialInfo.LedWarning.second.bit.TpmsError=1;
							break;
						default:
							break;
					}                                        
					break;  
				case HAVAL_2011_2P0T_MT_2WD:                            
					switch(can4.id_311.BCM_TPMSFailureInd)
					{
						case 0:
							uart_special_data.specialInfo.LedWarning.second.bit.TpmsError=0;
							break;
						case 1:
							uart_special_data.specialInfo.LedWarning.second.bit.TpmsError=1;
							break;
						case 3:
							uart_special_data.specialInfo.LedWarning.second.bit.TpmsError=1;
							break;
						default:
							break;
					}                                             
					break;
				case HAVAL_2013_1P5T_MT_4WD:                            
					switch(can5.id_311.BCM_TPMSFailureInd)
					{
						case 0:
							uart_special_data.specialInfo.LedWarning.second.bit.TpmsError=0;
							break;
						case 1:
							uart_special_data.specialInfo.LedWarning.second.bit.TpmsError=1;
							break;
						case 3:
							uart_special_data.specialInfo.LedWarning.second.bit.TpmsError=1;
							break;
						default:
							break;
					}                                             
					break;                                          
			}        
		}
		else
		{
			uart_special_data.specialInfo.LedWarning.second.bit.TpmsError=1;
		}
	}
	else
	{
		uart_special_data.specialInfo.LedWarning.second.bit.TpmsError=0;
	}
*/}


void led_light_main_switch_and_position_ctl(void)
{/*
	switch(IPconfig.protocol)                               
	{                                                     
		case HAVAL_2013_2P4L_AT_2WD:                            
			if(ON == g_u8IgnSts)
			{
				if(can_id_311_lost_timecnt<CAN_LOST_TIME)
				{
					mid_led_ctl(LED_LIGHTMAINCTL,(LED_STS_e)(can0.id_311.BCM_PosLmpSts));
					mid_led_ctl(LED_POSITION,(LED_STS_e)(can0.id_311.BCM_PosLmpSts));		
				}
				else
				{
					mid_led_ctl(LED_LIGHTMAINCTL,LED_OFF);
					mid_led_ctl(LED_POSITION,LED_OFF);			
				}
			}
			else
			{
				if(lin_pid_0x42_lost_timecnt<LIN_LOST_TIME)
				{
					mid_led_ctl(LED_LIGHTMAINCTL,(LED_STS_e)(lin0_msg.pid_0x42.PositionLampSts));
					mid_led_ctl(LED_POSITION,(LED_STS_e)(lin0_msg.pid_0x42.PositionLampSts));			
				}
				else
				{
					mid_led_ctl(LED_LIGHTMAINCTL,LED_OFF);
					mid_led_ctl(LED_POSITION,LED_OFF);		
				}
			}                                            
			break;                                            
		case HAVAL_2013_1P5T_MT_2WD:
			if(ON == g_u8IgnSts)
			{
				if(can_id_311_lost_timecnt<CAN_LOST_TIME)
				{
					mid_led_ctl(LED_LIGHTMAINCTL,(LED_STS_e)(can1.id_311.BCM_PosLmpSts));
					mid_led_ctl(LED_POSITION,(LED_STS_e)(can1.id_311.BCM_PosLmpSts));		
				}
				else
				{
					mid_led_ctl(LED_LIGHTMAINCTL,LED_OFF);
					mid_led_ctl(LED_POSITION,LED_OFF);			
				}
			}
			else
			{
				if(lin_pid_0x42_lost_timecnt<LIN_LOST_TIME)
				{
					mid_led_ctl(LED_LIGHTMAINCTL,(LED_STS_e)(lin1_msg.pid_0x42.PositionLampSts));
					mid_led_ctl(LED_POSITION,(LED_STS_e)(lin1_msg.pid_0x42.PositionLampSts));			
				}
				else
				{
					mid_led_ctl(LED_LIGHTMAINCTL,LED_OFF);
					mid_led_ctl(LED_POSITION,LED_OFF);		
				}
			}                            
			break;                                            
		case HAVAL_2012_1P5T_MT_2WD:                            
			if(ON == g_u8IgnSts)
			{
				if(can_id_311_lost_timecnt<CAN_LOST_TIME)
				{
					mid_led_ctl(LED_LIGHTMAINCTL,(LED_STS_e)(can2.id_311.BCM_PosLmpSts));
					mid_led_ctl(LED_POSITION,(LED_STS_e)(can2.id_311.BCM_PosLmpSts));		
				}
				else
				{
					mid_led_ctl(LED_LIGHTMAINCTL,LED_OFF);
					mid_led_ctl(LED_POSITION,LED_OFF);			
				}
			}
			else
			{
				if(lin_pid_0x42_lost_timecnt<LIN_LOST_TIME)
				{
					mid_led_ctl(LED_LIGHTMAINCTL,(LED_STS_e)(lin2_msg.pid_0x42.PositionLampSts));
					mid_led_ctl(LED_POSITION,(LED_STS_e)(lin2_msg.pid_0x42.PositionLampSts));			
				}
				else
				{
					mid_led_ctl(LED_LIGHTMAINCTL,LED_OFF);
					mid_led_ctl(LED_POSITION,LED_OFF);		
				}
			}                                             
			break;   
		case HAVAL_2015_1P5T_MT_2WD: 
		case HAVAL_2015_1P5T_6AT_2WD:                           
			if(ON == g_u8IgnSts)
			{
				if(can_id_311_lost_timecnt<CAN_LOST_TIME)
				{
					mid_led_ctl(LED_LIGHTMAINCTL,(LED_STS_e)(can3.id_311.BCM_PosLmpSts));
					mid_led_ctl(LED_POSITION,(LED_STS_e)(can3.id_311.BCM_PosLmpSts));		
				}
				else
				{
					mid_led_ctl(LED_LIGHTMAINCTL,LED_OFF);
					mid_led_ctl(LED_POSITION,LED_OFF);			
				}
			}
			else
			{
				if(lin_pid_0x42_lost_timecnt<LIN_LOST_TIME)
				{
					mid_led_ctl(LED_LIGHTMAINCTL,(LED_STS_e)(lin3_msg.pid_0x42.PositionLampSts));
					mid_led_ctl(LED_POSITION,(LED_STS_e)(lin3_msg.pid_0x42.PositionLampSts));			
				}
				else
				{
					mid_led_ctl(LED_LIGHTMAINCTL,LED_OFF);
					mid_led_ctl(LED_POSITION,LED_OFF);		
				}
			}                                        
			break; 
		case HAVAL_2011_2P0T_MT_2WD:                            
			if(ON == g_u8IgnSts)
			{
				if(can_id_311_lost_timecnt<CAN_LOST_TIME)
				{
					mid_led_ctl(LED_LIGHTMAINCTL,(LED_STS_e)(can4.id_311.BCM_PosLmpSts));
					mid_led_ctl(LED_POSITION,(LED_STS_e)(can4.id_311.BCM_PosLmpSts));		
				}
				else
				{
					mid_led_ctl(LED_LIGHTMAINCTL,LED_OFF);
					mid_led_ctl(LED_POSITION,LED_OFF);			
				}
			}
			else
			{
				if(lin_pid_0x42_lost_timecnt<LIN_LOST_TIME)
				{
					mid_led_ctl(LED_LIGHTMAINCTL,(LED_STS_e)(lin4_msg.pid_0x42.PositionLampSts));
					mid_led_ctl(LED_POSITION,(LED_STS_e)(lin4_msg.pid_0x42.PositionLampSts));			
				}
				else
				{
					mid_led_ctl(LED_LIGHTMAINCTL,LED_OFF);
					mid_led_ctl(LED_POSITION,LED_OFF);		
				}
			}                                             
			break;  
		case HAVAL_2013_1P5T_MT_4WD:
			if(ON == g_u8IgnSts)
			{
				if(can_id_311_lost_timecnt<CAN_LOST_TIME)
				{
					mid_led_ctl(LED_LIGHTMAINCTL,(LED_STS_e)(can5.id_311.BCM_PosLmpSts));
					mid_led_ctl(LED_POSITION,(LED_STS_e)(can5.id_311.BCM_PosLmpSts));		
				}
				else
				{
					mid_led_ctl(LED_LIGHTMAINCTL,LED_OFF);
					mid_led_ctl(LED_POSITION,LED_OFF);			
				}
			}
			else
			{
				if(lin_pid_0x42_lost_timecnt<LIN_LOST_TIME)
				{
					mid_led_ctl(LED_LIGHTMAINCTL,(LED_STS_e)(lin5_msg.pid_0x42.PositionLampSts));
					mid_led_ctl(LED_POSITION,(LED_STS_e)(lin5_msg.pid_0x42.PositionLampSts));			
				}
				else
				{
					mid_led_ctl(LED_LIGHTMAINCTL,LED_OFF);
					mid_led_ctl(LED_POSITION,LED_OFF);		
				}
			}                            
			break;                                         
	}                                                     
*/}

void led_engine_guard_ctl(void)
{/*
	switch(IPconfig.protocol)                               
	{                                                     
		case HAVAL_2013_2P4L_AT_2WD:                            
			if(OFF==mid_get_io_sts(&pin_io_in[PIN_IN_ENGINE_GUARD]))//����֤��û��CAN�����ź�
			{
				uart_special_data.specialInfo.LedWarning.second.bit.EngineSecurity=1;
			}
			else
			{
				uart_special_data.specialInfo.LedWarning.second.bit.EngineSecurity=0;
			}                                            
			break;                                            
		case HAVAL_2013_1P5T_MT_2WD: //����֤����CAN�ź� 0:ON
		  if((OFF==mid_get_io_sts(&pin_io_in[PIN_IN_ENGINE_GUARD]))||(can1.id_271.ECM_ECMImmoAuthRes==0))
			{
				uart_special_data.specialInfo.LedWarning.second.bit.EngineSecurity=1;
			}
			else
			{
				uart_special_data.specialInfo.LedWarning.second.bit.EngineSecurity=0;
			}                    
			break;                                            
		case HAVAL_2012_1P5T_MT_2WD://û��CAN�ź�                            
			if((OFF==mid_get_io_sts(&pin_io_in[PIN_IN_ENGINE_GUARD])))
			{
				uart_special_data.specialInfo.LedWarning.second.bit.EngineSecurity=1;
			}
			else
			{
				uart_special_data.specialInfo.LedWarning.second.bit.EngineSecurity=0;
			}                                     
			break;
		case HAVAL_2015_1P5T_MT_2WD:����֤:CAN���б����ź� 0:ON
		case HAVAL_2015_1P5T_6AT_2WD:����֤:CAN���б����ź� 0:ON
			if((OFF==mid_get_io_sts(&pin_io_in[PIN_IN_ENGINE_GUARD]))||(can3.id_271.ECM_ECMImmoAuthRes==0))
			{
				uart_special_data.specialInfo.LedWarning.second.bit.EngineSecurity=1;
			}
			else
			{
				uart_special_data.specialInfo.LedWarning.second.bit.EngineSecurity=0;
			}                               
			break; 
		case HAVAL_2011_2P0T_MT_2WD://û��ʵ�����ԣ���Ĭ��ΪCAN 0:ON                           
			if((OFF==mid_get_io_sts(&pin_io_in[PIN_IN_ENGINE_GUARD]))||(can4.id_271.ECM_ECMImmoAuthRes==0))
			{
				uart_special_data.specialInfo.LedWarning.second.bit.EngineSecurity=1;
			}
			else
			{
				uart_special_data.specialInfo.LedWarning.second.bit.EngineSecurity=0;
			}                                
			break;  
		case HAVAL_2013_1P5T_MT_4WD:�Ǳ��ʼ��У�����
			if((OFF==mid_get_io_sts(&pin_io_in[PIN_IN_ENGINE_GUARD]))||(can5.id_271.ECM_ECMImmoAuthRes==0))
			{
				uart_special_data.specialInfo.LedWarning.second.bit.EngineSecurity=1;
			}
			else
			{
				uart_special_data.specialInfo.LedWarning.second.bit.EngineSecurity=0;
			}                 
			break;                                         
	}
*/}

void led_guard_ctl(void)
{/*
	if(OFF == g_u8IgnSts)
	{
		uart_special_data.specialInfo.LedWarning.second.bit.BurglarAlarm=0;
		return;
	}
	else
	{
		if(ON == mid_get_io_sts(&pin_io_in[PIN_IN_GUARD]))
		{
			uart_special_data.specialInfo.LedWarning.second.bit.BurglarAlarm = 0;
			return;
		}
		else
		{
			uart_special_data.specialInfo.LedWarning.second.bit.BurglarAlarm = 1;
		}
	}
*/
}

/* פ���� */
void led_parking_ctl(void)
{
	/*switch(IPconfig.protocol)
	{                                                     
		case HAVAL_2013_2P4L_AT_2WD:                            
			if(ON == g_u8IgnSts)
			{
				mid_led_ctl(LED_PARK,LED_OFF);
			}
			else
			{
				if((lin_pid_0x42_lost_timecnt<LIN_LOST_TIME)&&(app_get_car_key_pos()==KEY_OUT))
				{
					mid_led_ctl(LED_PARK,(LED_STS_e)lin0_msg.pid_0x42.ParkLampSts);
				}
				else
				{
					mid_led_ctl(LED_PARK,LED_OFF);
				}
			}
			break;                                            
		case HAVAL_2013_1P5T_MT_2WD:                            
			if(ON == g_u8IgnSts)
			{
				mid_led_ctl(LED_PARK,LED_OFF);
			}
			else
			{
				if((lin_pid_0x42_lost_timecnt<LIN_LOST_TIME)&&(app_get_car_key_pos()==KEY_OUT))
				{
					mid_led_ctl(LED_PARK,(LED_STS_e)lin1_msg.pid_0x42.ParkLampSts);
				}
				else
				{
					mid_led_ctl(LED_PARK,LED_OFF);
				}
			}                                             
			break;                                            
		case HAVAL_2012_1P5T_MT_2WD:                            
			if(ON == g_u8IgnSts)
			{
				mid_led_ctl(LED_PARK,LED_OFF);
			}
			else
			{
				if((lin_pid_0x42_lost_timecnt<LIN_LOST_TIME)&&(app_get_car_key_pos()==KEY_OUT))
				{
					mid_led_ctl(LED_PARK,(LED_STS_e)lin2_msg.pid_0x42.ParkLampSts);
				}
				else
				{
					mid_led_ctl(LED_PARK,LED_OFF);
				}
			}                                             
			break;   
		case HAVAL_2015_1P5T_MT_2WD: 
		case HAVAL_2015_1P5T_6AT_2WD:                           
			if(ON == g_u8IgnSts)
			{
				mid_led_ctl(LED_PARK,LED_OFF);
			}
			else
			{
				if((lin_pid_0x42_lost_timecnt<LIN_LOST_TIME)&&(app_get_car_key_pos()==KEY_OUT))
				{
					mid_led_ctl(LED_PARK,(LED_STS_e)lin3_msg.pid_0x42.ParkLampSts);
				}
				else
				{
					mid_led_ctl(LED_PARK,LED_OFF);
				}
			}                                        
			break;    
		case HAVAL_2011_2P0T_MT_2WD:                            
			if(ON == g_u8IgnSts)
			{
				mid_led_ctl(LED_PARK,LED_OFF);
			}
			else
			{
				if((lin_pid_0x42_lost_timecnt<LIN_LOST_TIME)&&(app_get_car_key_pos()==KEY_OUT))
				{
					mid_led_ctl(LED_PARK,(LED_STS_e)lin4_msg.pid_0x42.ParkLampSts);
				}
				else
				{
					mid_led_ctl(LED_PARK,LED_OFF);
				}
			}                                             
			break;  
		case HAVAL_2013_1P5T_MT_4WD:                            
			if(ON == g_u8IgnSts)
			{
				mid_led_ctl(LED_PARK,LED_OFF);
			}
			else
			{
				if((lin_pid_0x42_lost_timecnt<LIN_LOST_TIME)&&(app_get_car_key_pos()==KEY_OUT))
				{
					mid_led_ctl(LED_PARK,(LED_STS_e)lin5_msg.pid_0x42.ParkLampSts);
				}
				else
				{
					mid_led_ctl(LED_PARK,LED_OFF);
				}
			}                                             
			break;                                       
	}                      */
}

/* ��ص����� */
void led_charging_ctl(void)
{/*
	static U32 t1 = 0;
	static U8 t1_flag = 0;
	static U32 t_pass = 0;

	if(t1_flag == 0) 
	{
		t1_flag = 1;
		t1 = hal_timer_get_tick();
		return;
	}
	
	if(OFF == g_u8IgnSts)
	{
		uart_special_data.specialInfo.LedWarning.second.bit.ChargeLamp = 0;
		t1_flag = 0;
		return;
	}*/
//	if(ON == g_u8IgnSts) /* IGNON�ҹ���3��󣬿�ʼ�ж��Ƿ����� */
//	{
//		t_pass = hal_timer_passed(t1);
//		if((mid_get_io_sts(&pin_io_in[PIN_IN_CHARGE]) == ON) || (t_pass <= 300))
//		{
//			uart_special_data.specialInfo.LedWarning.second.bit.ChargeLamp = 0;
//			return;
//		}
//		if((t_pass > 300) && (mid_get_io_sts(&pin_io_in[PIN_IN_CHARGE]) == OFF))
//		{
//			uart_special_data.specialInfo.LedWarning.second.bit.ChargeLamp = 1;
//		}
//	}
	
}

/* ������ */
void led_4WD_ctl(void)
{/*
	static U8 timecnt=0;
	switch(IPconfig.protocol)                               
	{                                                     
		case HAVAL_2013_2P4L_AT_2WD:                            
			uart_special_data.specialInfo.LedWarning.second.bit.WD_4=0;
			break;                                            
		case HAVAL_2013_1P5T_MT_2WD:                            
			uart_special_data.specialInfo.LedWarning.second.bit.WD_4=0;                                    
			break;                                            
		case HAVAL_2012_1P5T_MT_2WD:                            
			uart_special_data.specialInfo.LedWarning.second.bit.WD_4=0;                                            
			break;      
		case HAVAL_2015_1P5T_MT_2WD: 
		case HAVAL_2015_1P5T_6AT_2WD:                           
			uart_special_data.specialInfo.LedWarning.second.bit.WD_4=0;                                      
			break;    
		case HAVAL_2011_2P0T_MT_2WD:                            
			uart_special_data.specialInfo.LedWarning.second.bit.WD_4=0;                                            
			break;  
		case HAVAL_2013_1P5T_MT_4WD:                            
			if(ON == g_u8IgnSts)
			{
				if(can_id_251_lost_timecnt<CAN_LOST_TIME)
				{
					uart_special_data.specialInfo.LedWarning.second.bit.WD_4=0;
					timecnt=0;
				}
				else
				{
					timecnt++;
					if(timecnt<=10)
					{
						uart_special_data.specialInfo.LedWarning.second.bit.WD_4=1;
					}
					else if(timecnt<=20)
					{
						uart_special_data.specialInfo.LedWarning.second.bit.WD_4=0;
						if(timecnt==20)
						{
							timecnt=0;
						}
					}
					
				}
			}
			else
			{
				uart_special_data.specialInfo.LedWarning.second.bit.WD_4=0;
				timecnt=0;
			}                                            
			break;                                       
	}   
*/}



/* ESP,�����ȶ�ϵͳ */
void led_ESP_ctl(void)
{/*
	switch(IPconfig.protocol)                               
	{                                                     
		case HAVAL_2013_2P4L_AT_2WD: 
			 ʵ��ץ�İ����� ID211,�˵Ƶ�ID����

			if(ON == g_u8IgnSts)
			{ 
				if(can_id_211_lost_timecnt<50)
				{
					uart_special_data.specialInfo.LedWarning.second.bit.ESP=can0.id_211.ESP;      
				}
				else
				{
					uart_special_data.specialInfo.LedWarning.second.bit.ESP=0;
				}  
			}
			else
			{
				uart_special_data.specialInfo.LedWarning.second.bit.ESP=0;
			}    

			break; 
		case HAVAL_2013_1P5T_MT_2WD:    
			if(ON == g_u8IgnSts)
			{ 
				if(can_id_211_lost_timecnt<CAN_LOST_TIME)
				{
					if(can1.id_211.ESP_ERR)
					{
						uart_special_data.specialInfo.LedWarning.second.bit.ESP=1;
					} 
					else if(can1.id_211.ESP_NML)
					{
						uart_special_data.specialInfo.LedWarning.second.bit.ESP=2;
					}
					else if(can1.id_211.ESP_OFF)
					{
						uart_special_data.specialInfo.LedWarning.second.bit.ESP=3;
					}
					else
					{
						uart_special_data.specialInfo.LedWarning.second.bit.ESP=0;
					}

				}
				else
				{
					uart_special_data.specialInfo.LedWarning.second.bit.ESP=0;
				}  
			}
			else
			{
				uart_special_data.specialInfo.LedWarning.second.bit.ESP=0;
			}    
			break; 
		case HAVAL_2012_1P5T_MT_2WD:  
			if(ON == g_u8IgnSts)
			{ 
				if(can_id_211_lost_timecnt<CAN_LOST_TIME)
				{
					if(can2.id_211.ESP_ERR)
					{
						uart_special_data.specialInfo.LedWarning.second.bit.ESP=1;
					} 
					else if(can2.id_211.ESP_NML)
					{
						uart_special_data.specialInfo.LedWarning.second.bit.ESP=2;
					}  
					else if(can2.id_211.ESP_OFF)
					{
						uart_special_data.specialInfo.LedWarning.second.bit.ESP=3;
					}
					else
					{
						uart_special_data.specialInfo.LedWarning.second.bit.ESP=0;
					}
				}
				else
				{
					uart_special_data.specialInfo.LedWarning.second.bit.ESP=0;
				}  
			}
			else
			{
				uart_special_data.specialInfo.LedWarning.second.bit.ESP=0;
			}   
			break; 
		case HAVAL_2011_2P0T_MT_2WD:							
			if(ON == g_u8IgnSts)
			{ 
				if(can_id_211_lost_timecnt<CAN_LOST_TIME)
				{
					if(can4.id_211.ESP_ERR)
					{
						uart_special_data.specialInfo.LedWarning.second.bit.ESP=1;
					} 
					else if(can4.id_211.ESP_NML)
					{
						uart_special_data.specialInfo.LedWarning.second.bit.ESP=2;
					} 
					else if(can4.id_211.ESP_OFF)
					{
						uart_special_data.specialInfo.LedWarning.second.bit.ESP=3;
					}
					else
					{
						uart_special_data.specialInfo.LedWarning.second.bit.ESP=0;
					}
				}
				else
				{
					uart_special_data.specialInfo.LedWarning.second.bit.ESP=0;
				}  
			}
			else
			{
				uart_special_data.specialInfo.LedWarning.second.bit.ESP=0;
			}   
			break;  
		2015_1P5T_MT��6ATֻ���ڵ�λ�������������ź�һ�������Թ���can3
		case HAVAL_2015_1P5T_MT_2WD: 
		case HAVAL_2015_1P5T_6AT_2WD:                 							
			if(ON == g_u8IgnSts)
			{ 
				if(can_id_211_lost_timecnt<CAN_LOST_TIME)
				{
					if(can3.id_211.ESP_ERR)
					{
						uart_special_data.specialInfo.LedWarning.second.bit.ESP=1;
					} 
					else if(can3.id_211.ESP_NML)
					{
						uart_special_data.specialInfo.LedWarning.second.bit.ESP=2;
					}
					else if(can3.id_211.ESP_OFF)
					{
						uart_special_data.specialInfo.LedWarning.second.bit.ESP=3;
					}
					else
					{
						uart_special_data.specialInfo.LedWarning.second.bit.ESP=0;
					}
				}
				else
				{
					uart_special_data.specialInfo.LedWarning.second.bit.ESP=0;
				}  
			}
			else
			{
				uart_special_data.specialInfo.LedWarning.second.bit.ESP=0;
			}    
			break;    
		case HAVAL_2013_1P5T_MT_4WD:
			if(ON == g_u8IgnSts)
			{ 
				if(can_id_211_lost_timecnt<CAN_LOST_TIME)
				{
					if(can5.id_211.ESP_ERR)
					{
						uart_special_data.specialInfo.LedWarning.second.bit.ESP=1;
					} 
					else if(can5.id_211.ESP_NML)
					{
						uart_special_data.specialInfo.LedWarning.second.bit.ESP=2;
					}  
					else if(can5.id_211.ESP_OFF)
					{
						uart_special_data.specialInfo.LedWarning.second.bit.ESP=3;
					}
					else
					{
						uart_special_data.specialInfo.LedWarning.second.bit.ESP=0;
					}
				}
				else
				{
					uart_special_data.specialInfo.LedWarning.second.bit.ESP=0;
				}  
			}
			else
			{
				uart_special_data.specialInfo.LedWarning.second.bit.ESP=0;
			}    
			break;  
	}   
*/}

/*���»���ָʾ��*/
void led_HDC_ctl(void)
{/*
	switch(IPconfig.protocol)                               
	{                                                     
		case HAVAL_2013_2P4L_AT_2WD:                            
			                                       
			break;                                            
		case HAVAL_2013_1P5T_MT_2WD:
		               
			break;                                            
		case HAVAL_2012_1P5T_MT_2WD:                       
			                              
			break;
		case HAVAL_2015_1P5T_MT_2WD:
		case HAVAL_2015_1P5T_6AT_2WD:                     
			if(ON == g_u8IgnSts)
			{ 
				if(can_id_242_lost_timecnt<CAN_LOST_TIME)
				{
					���»���ϵͳ�޹���
					if(0 == can3.id_242.HDC_ERROR)
					{
						uart_special_data.specialInfo.Radar.fifth.bit.HDC_ERROR = 0;
						uart_special_data.specialInfo.Radar.fifth.bit.HDC = can3.id_242.HDC; 
					}  
					else
					{
						uart_special_data.specialInfo.Radar.fifth.bit.HDC_ERROR = 1;
						uart_special_data.specialInfo.Radar.fifth.bit.HDC = 0;
					}
				}
				else
				{
					uart_special_data.specialInfo.Radar.fifth.bit.HDC = 0;
					uart_special_data.specialInfo.Radar.fifth.bit.HDC_ERROR = 0;
				}  
			}
			else
			{
				uart_special_data.specialInfo.Radar.fifth.bit.HDC = 0;
				uart_special_data.specialInfo.Radar.fifth.bit.HDC_ERROR = 0;
			}                             
			break; 
		case HAVAL_2011_2P0T_MT_2WD:                          
			                           
			break;  
		case HAVAL_2013_1P5T_MT_4WD:
			             
			break;                                         
	}
*/}


/*  PESP ָʾ�� */
void led_PEPS_ctl(void)
{/*
	U8 PEPS_Err1 = 0;
	U8 PEPS_Err2 = 0;
	U8 PEPS_Err3 = 0;

	switch(IPconfig.protocol)                               
	{                                                     
		case HAVAL_2013_2P4L_AT_2WD:                            
			                                       
			break;                                            
		case HAVAL_2013_1P5T_MT_2WD:

			if(ON == g_u8IgnSts)
			{
				if(can_id_0A5_lost_timecnt<CAN_LOST_TIME)
				{
					if(can1.id_0A5.PEPS_ERROR || can1.id_0A5.PEPS_ERROR1)
					{
						uart_special_data.specialInfo.Peps.first.bit.PEPS_ERROR = 1;
					}
					else
					{
						uart_special_data.specialInfo.Peps.first.bit.PEPS_ERROR = 0;
					}

					if(can1.id_0A5.PEPS_TurnGearPN)
					{
						uart_special_data.specialInfo.Peps.first.bit.PEPS_TurnGearPN = 1; 
					}
					else
					{
						uart_special_data.specialInfo.Peps.first.bit.PEPS_TurnGearPN = 0;
					}

					if(can1.id_0A5.PEPS_CheckNoKey)
					{
						uart_special_data.specialInfo.Peps.first.bit.PEPS_CheckNoKey = 1; 
					}
					else
					{
						uart_special_data.specialInfo.Peps.first.bit.PEPS_CheckNoKey = 0;
					}					
				}
				else  CAN �źŶ�ʧ
				{
					uart_special_data.specialInfo.Peps.first.bit.PEPS_TurnGearPN = 0;
					uart_special_data.specialInfo.Peps.first.bit.PEPS_ERROR = 0;
					uart_special_data.specialInfo.Peps.first.bit.PEPS_CheckNoKey = 0;	
				}
			}
			else Ϩ��״̬
			{
				if(lin_pid_0x42_lost_timecnt<LIN_LOST_TIME)
				{
					PEPS_Err1 = lin1_msg.pid_0x42.PEPS_ERROR;
					PEPS_Err2 = lin1_msg.pid_0x42.PEPS_ERROR1;
					PEPS_Err3 = lin1_msg.pid_0x42.PEPS_ERROR2;
					if(PEPS_Err1 || PEPS_Err2 || PEPS_Err3)
					{
						uart_special_data.specialInfo.Peps.first.bit.PEPS_ERROR = 1;
					}
					else
					{
						uart_special_data.specialInfo.Peps.first.bit.PEPS_ERROR = 0;
					}

					if(lin1_msg.pid_0x42.PEPS_TurnGearPN)
					{
						uart_special_data.specialInfo.Peps.first.bit.PEPS_TurnGearPN = 1; 	
					}
					else
					{
						uart_special_data.specialInfo.Peps.first.bit.PEPS_TurnGearPN = 0;
					}

					if(lin1_msg.pid_0x42.PEPS_CheckNoKey)
					{
						uart_special_data.specialInfo.Peps.first.bit.PEPS_CheckNoKey = 1; 
					}
					else
					{
						uart_special_data.specialInfo.Peps.first.bit.PEPS_CheckNoKey = 0;
					}					
				}
				elseLIN �źŶ�ʧ
				{
					uart_special_data.specialInfo.Peps.first.bit.PEPS_TurnGearPN = 0;
					uart_special_data.specialInfo.Peps.first.bit.PEPS_ERROR = 0;
					uart_special_data.specialInfo.Peps.first.bit.PEPS_CheckNoKey = 0;
				}
			}
					   
			break;                                            
		case HAVAL_2012_1P5T_MT_2WD:                       
			                              
			break;
		case HAVAL_2015_1P5T_MT_2WD:
		case HAVAL_2015_1P5T_6AT_2WD:  
			if(ON == g_u8IgnSts)
			{
				if(can_id_0A5_lost_timecnt<CAN_LOST_TIME)
				{
					if(can3.id_0A5.PEPS_ERROR || can3.id_0A5.PEPS_ERROR1)
					{
						uart_special_data.specialInfo.Peps.first.bit.PEPS_ERROR = 1;
					}
					else
					{
						uart_special_data.specialInfo.Peps.first.bit.PEPS_ERROR = 0;
					}

					if(can3.id_0A5.PEPS_TurnGearPN)
					{
						uart_special_data.specialInfo.Peps.first.bit.PEPS_TurnGearPN = 1; 
					}
					else
					{
						uart_special_data.specialInfo.Peps.first.bit.PEPS_TurnGearPN = 0;
					}

					if(can3.id_0A5.PEPS_CheckNoKey)
					{
						uart_special_data.specialInfo.Peps.first.bit.PEPS_CheckNoKey = 1; 
					}
					else
					{
						uart_special_data.specialInfo.Peps.first.bit.PEPS_CheckNoKey = 0;
					}					
				}
				else  CAN �źŶ�ʧ
				{
					uart_special_data.specialInfo.Peps.first.bit.PEPS_TurnGearPN = 0;
					uart_special_data.specialInfo.Peps.first.bit.PEPS_ERROR = 0;
					uart_special_data.specialInfo.Peps.first.bit.PEPS_CheckNoKey = 0;	
				}
			}
			else Ϩ��״̬
			{
				if(lin_pid_0x42_lost_timecnt<LIN_LOST_TIME)
				{
					PEPS_Err1 = lin3_msg.pid_0x42.PEPS_ERROR;
					PEPS_Err2 = lin3_msg.pid_0x42.PEPS_ERROR1;
					PEPS_Err3 = lin3_msg.pid_0x42.PEPS_ERROR2;
					if(PEPS_Err1 || PEPS_Err2 || PEPS_Err3)
					{
						uart_special_data.specialInfo.Peps.first.bit.PEPS_ERROR = 1;
					}
					else
					{
						uart_special_data.specialInfo.Peps.first.bit.PEPS_ERROR = 0;
					}

					if(lin3_msg.pid_0x42.PEPS_TurnGearPN)
					{
						uart_special_data.specialInfo.Peps.first.bit.PEPS_TurnGearPN = 1; 	
					}
					else
					{
						uart_special_data.specialInfo.Peps.first.bit.PEPS_TurnGearPN = 0;
					}

					if(lin3_msg.pid_0x42.PEPS_CheckNoKey)
					{
						uart_special_data.specialInfo.Peps.first.bit.PEPS_CheckNoKey = 1; 
					}
					else
					{
						uart_special_data.specialInfo.Peps.first.bit.PEPS_CheckNoKey = 0;
					}					
				}
				elseLIN �źŶ�ʧ
				{
					uart_special_data.specialInfo.Peps.first.bit.PEPS_TurnGearPN = 0;
					uart_special_data.specialInfo.Peps.first.bit.PEPS_ERROR = 0;
					uart_special_data.specialInfo.Peps.first.bit.PEPS_CheckNoKey = 0;
				}
			}                            
			break; 
			
		case HAVAL_2011_2P0T_MT_2WD:                          
			                           
			break;  
		case HAVAL_2013_1P5T_MT_4WD:
			if(ON == g_u8IgnSts)
			{
				if(can_id_0A5_lost_timecnt<CAN_LOST_TIME)
				{
					if(can5.id_0A5.PEPS_ERROR || can5.id_0A5.PEPS_ERROR1)
					{
						uart_special_data.specialInfo.Peps.first.bit.PEPS_ERROR = 1;
					}
					else
					{
						uart_special_data.specialInfo.Peps.first.bit.PEPS_ERROR = 0;
					}

					if(can5.id_0A5.PEPS_TurnGearPN)
					{
						uart_special_data.specialInfo.Peps.first.bit.PEPS_TurnGearPN = 1; 
					}
					else
					{
						uart_special_data.specialInfo.Peps.first.bit.PEPS_TurnGearPN = 0;
					}

					if(can5.id_0A5.PEPS_CheckNoKey)
					{
						uart_special_data.specialInfo.Peps.first.bit.PEPS_CheckNoKey = 1; 
					}
					else
					{
						uart_special_data.specialInfo.Peps.first.bit.PEPS_CheckNoKey = 0;
					}					
				}
				else  CAN �źŶ�ʧ
				{
					uart_special_data.specialInfo.Peps.first.bit.PEPS_TurnGearPN = 0;
					uart_special_data.specialInfo.Peps.first.bit.PEPS_ERROR = 0;
					uart_special_data.specialInfo.Peps.first.bit.PEPS_CheckNoKey = 0;	
				}
			}
			else Ϩ��״̬
			{
				if(lin_pid_0x42_lost_timecnt<LIN_LOST_TIME)
				{
					PEPS_Err1 = lin5_msg.pid_0x42.PEPS_ERROR;
					PEPS_Err2 = lin5_msg.pid_0x42.PEPS_ERROR1;
					PEPS_Err3 = lin5_msg.pid_0x42.PEPS_ERROR2;
					if(PEPS_Err1 || PEPS_Err2 || PEPS_Err3)
					{
						uart_special_data.specialInfo.Peps.first.bit.PEPS_ERROR = 1;
					}
					else
					{
						uart_special_data.specialInfo.Peps.first.bit.PEPS_ERROR = 0;
					}

					if(lin5_msg.pid_0x42.PEPS_TurnGearPN)
					{
						uart_special_data.specialInfo.Peps.first.bit.PEPS_TurnGearPN = 1; 	
					}
					else
					{
						uart_special_data.specialInfo.Peps.first.bit.PEPS_TurnGearPN = 0;
					}

					if(lin5_msg.pid_0x42.PEPS_CheckNoKey)
					{
						uart_special_data.specialInfo.Peps.first.bit.PEPS_CheckNoKey = 1; 
					}
					else
					{
						uart_special_data.specialInfo.Peps.first.bit.PEPS_CheckNoKey = 0;
					}					
				}
				elseLIN �źŶ�ʧ
				{
					uart_special_data.specialInfo.Peps.first.bit.PEPS_TurnGearPN = 0;
					uart_special_data.specialInfo.Peps.first.bit.PEPS_ERROR = 0;
					uart_special_data.specialInfo.Peps.first.bit.PEPS_CheckNoKey = 0;
				}
			}              
			break;                                         
	}
*/}

/* SnowMode LED */
void led_snow_mode_ctl(void)
{/*
	switch(IPconfig.protocol)                               
	{                                                     
		case HAVAL_2013_2P4L_AT_2WD:                            
			
			break;                                            
		case HAVAL_2013_1P5T_MT_2WD:                            
			                                   
			break;                                            
		case HAVAL_2012_1P5T_MT_2WD:                            
			                                          
			break;      
		case HAVAL_2015_1P5T_MT_2WD: 
		case HAVAL_2015_1P5T_6AT_2WD:                           
			if(ON == g_u8IgnSts && can_id_221_lost_timecnt<CAN_LOST_TIME)
			{
				CAN������ b47,b46,b45λ������ֻ��b45λΪ1ʱ����
				if(1 == can3.id_221.SnowModeLED)
				{					
					uart_special_data.specialInfo.SpecialLED.first.bit.SnowModeLED = 1;
				}
				else
				{
					uart_special_data.specialInfo.SpecialLED.first.bit.SnowModeLED = 0;	
				}
			}
			else
			{
				uart_special_data.specialInfo.SpecialLED.first.bit.SnowModeLED = 0;
			}                                           
			break;    
		case HAVAL_2011_2P0T_MT_2WD:                            
			                                           
			break;  
		case HAVAL_2013_1P5T_MT_4WD:                            
			                                   
			break; 
		default: 
			break;
	}   
*/}


/* ALS LED */
void led_ALS_ctl(void)
{/*
	switch(IPconfig.protocol)                               
	{                                                     
		case HAVAL_2013_2P4L_AT_2WD:                            
			if(ON == g_u8IgnSts)
			{
				ֻҪ�յ�CAN�źž��иñ���,CAN�źŶ�ʧ�ñ���Ҳ����ȡ��
				if(1 == can0.id_365.ALS)
				{					
					uart_special_data.specialInfo.SpecialLED.first.bit.ALS = 1;
				}
				else
				{
					uart_special_data.specialInfo.SpecialLED.first.bit.ALS = 0;
				}
			}
			else
			{
				uart_special_data.specialInfo.SpecialLED.first.bit.ALS = 0;
			}        
			break;                                            
		case HAVAL_2013_1P5T_MT_2WD:                            
			if(ON == g_u8IgnSts)
			{
				ֻҪ�յ�CAN�źž��иñ���,CAN�źŶ�ʧ�ñ���Ҳ����ȡ��
				if(1 == can1.id_365.ALS)
				{					
					uart_special_data.specialInfo.SpecialLED.first.bit.ALS = 1;
				}
				else
				{
					uart_special_data.specialInfo.SpecialLED.first.bit.ALS = 0;
				}
			}
			else
			{
				uart_special_data.specialInfo.SpecialLED.first.bit.ALS = 0;
			}                                         
			break;                                            
		case HAVAL_2012_1P5T_MT_2WD:                            
			                                          
			break;      
		case HAVAL_2015_1P5T_MT_2WD: 
		case HAVAL_2015_1P5T_6AT_2WD:                           
			if(ON == g_u8IgnSts)
			{
				ֻҪ�յ�CAN�źž��иñ���,CAN�źŶ�ʧ�ñ���Ҳ����ȡ��
				if(1 == can3.id_365.ALS)
				{					
					uart_special_data.specialInfo.SpecialLED.first.bit.ALS = 1;
				}
				else
				{
					uart_special_data.specialInfo.SpecialLED.first.bit.ALS = 0;
				}
			}
			else
			{
				uart_special_data.specialInfo.SpecialLED.first.bit.ALS = 0;
			}                                           
			break;    
		case HAVAL_2011_2P0T_MT_2WD:                            
			                                           
			break;  
		case HAVAL_2013_1P5T_MT_4WD:                            
			if(ON == g_u8IgnSts)
			{
				ֻҪ�յ�CAN�źž��иñ���,CAN�źŶ�ʧ�ñ���Ҳ����ȡ��
				if(1 == can5.id_365.ALS)
				{					
					uart_special_data.specialInfo.SpecialLED.first.bit.ALS = 1;
				}
				else
				{
					uart_special_data.specialInfo.SpecialLED.first.bit.ALS = 0;
				}
			}
			else
			{
				uart_special_data.specialInfo.SpecialLED.first.bit.ALS = 0;
			}                                           
			break; 
		default: 
			break;
	}   
*/}




void app_led_selfcheck(void)
{
	if(led_selfcheck_enable)//�Լ�ģʽ�£�����ҪMCU���Ƶ��Լ�ƣ�
													//�����Լ�ƣ��ɽ�������Լ����
	{
		
	}
}

void app_pro_led(void)
{
	if(ON == g_u8IgnSts)
	{
		led_selfcheck_enable=mcu_rec_special.settingsInfo.first.bits.led_selfcheck;//��core������Լ����
	}
	else
	{
		led_selfcheck_enable=0;
		mcu_rec_special.settingsInfo.first.bits.led_selfcheck=0;
	}
	
	//check led
	led_braking_ctl();
	led_engine_failure_ctl();
	led_tpms_failure_ctrl();
	led_tpms_warning_ctrl();
	led_lamp_auto_light_ctrl();
	led_frontfoglamp_ctl();
	led_rearfoglamp_ctl();
	led_highbeam_ctl();
	led_light_main_switch_and_position_ctl();
	led_turn_ctl();
	led_parking_ctl();
	led_engine_guard_ctl();
	led_guard_ctl();
	led_abs_failure_ctrl();
	led_airbag_ctl();
	led_atm_warning_ctrl();
	led_charging_ctl();
	led_cruise_ctrl();
	led_engine_maintain_ctrl();
	led_oil_press_ctrl();
	led_safetybelt_ctl();
	led_4WD_ctl();
	led_ESP_ctl();
	led_HDC_ctl();
	led_PEPS_ctl();
	led_snow_mode_ctl();
	led_ALS_ctl();
	
	//����жϱ������������ڽ���������
	//led_fuel_warning_ctrl();
	//led_temp_warning_ctrl();
	//led_120km_warning_ctrl();
	
	//LED�Լ�ƿ���
	app_led_selfcheck();
	
	//for test 2015/8/5 16:28:33
	//app_leds_all_on_test();
}

U8  app_get_led_onoff_sts(LED_NAME_e led_name)
{
	return ( mid_get_led_onoff_sts(led_name));
}


void app_leds_all_on_test(void)
{
	mid_led_ctl(LED_LEFTTURNLIGHT,LED_ON);
	mid_led_ctl(LED_REARFOGLAMP,LED_ON);
	mid_led_ctl(LED_FRONTFOGLAMP,LED_ON);
	mid_led_ctl(LED_HIGHBEAM,LED_ON);
	mid_led_ctl(LED_PARK,LED_ON);
	mid_led_ctl(LED_POSITION,LED_ON);
	mid_led_ctl(LED_SAFETYBELTSIDE,LED_ON);
	mid_led_ctl(LED_LIGHTMAINCTL,LED_ON);
	mid_led_ctl(LED_RIGHTTURNLIGHT,LED_ON);
	mid_led_ctl(LED_RESERVELED1,LED_ON);
	mid_led_ctl(LED_RESERVELED2,LED_ON);
}

void app_leds_all_off_test(void)
{
	mid_led_ctl(LED_LEFTTURNLIGHT,LED_OFF);
	mid_led_ctl(LED_REARFOGLAMP,LED_OFF);
	mid_led_ctl(LED_FRONTFOGLAMP,LED_OFF);
	mid_led_ctl(LED_HIGHBEAM,LED_OFF);
	mid_led_ctl(LED_PARK,LED_OFF);
	mid_led_ctl(LED_POSITION,LED_OFF);
	mid_led_ctl(LED_SAFETYBELTSIDE,LED_OFF);
	mid_led_ctl(LED_LIGHTMAINCTL,LED_OFF);
	mid_led_ctl(LED_RIGHTTURNLIGHT,LED_OFF);
	mid_led_ctl(LED_RESERVELED1,LED_OFF);
	mid_led_ctl(LED_RESERVELED2,LED_OFF);
}
