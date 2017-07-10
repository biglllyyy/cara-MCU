#include "app_Carkey.h"
#include "mid_switch.h"
#include "io_cfg.h"
#include "g_variable.h"
#include "app_lin.h"
#include "app_IPconfig.h"
#include "app_io.h"

/*
Ign_sts_e app_get_ign_sts(void)
{
	Ign_sts_e temp;
	temp=(Ign_sts_e)mid_get_io_filtered(pin_filter_in, PIN_IN_IGN);
	
	return temp;
}
*/

Key_sts_e  app_get_car_key_sts(void)
{
	Key_sts_e key_sts;
	switch(IPconfig.protocol)                               
	{                                                     
		case HAVAL_2013_2P4L_AT_2WD:                            
			//PIN脚点火信号和LIN报文的KEY状态进行合并，以PIN脚点火信号为准
			if(g_u8IgnSts)
			{
				switch(lin0_msg.pid_0x03.IGNSts)
				{
					case 0:
						key_sts = KEY_ON;
						break;
					case 1:
						key_sts = KEY_ON;
						break;
					case 2:
						key_sts = KEY_ON;
						break;
					case 4:
						key_sts = KEY_ON;
						break;
					case 8:
						key_sts = KEY_START;
						break;
					default:
						key_sts = KEY_ON;
						break;
				}
			}
			else
			{
				switch(lin0_msg.pid_0x03.IGNSts)
				{
					case 0:
						key_sts = KEY_OFF;
						break;
					case 1:
						key_sts = KEY_OFF;
						break;
					case 2:
						key_sts = KEY_ACC;
						break;
					case 4:
						key_sts = KEY_OFF;
						break;
					case 8:
						key_sts = KEY_OFF;
						break;
					default:
						key_sts = KEY_OFF;
						break;
				}
			}
			                                     
			break;                                            
		case HAVAL_2013_1P5T_MT_2WD:                            
			//PIN脚点火信号和LIN报文的KEY状态进行合并，以PIN脚点火信号为准
			if(g_u8IgnSts)
			{
				switch(lin1_msg.pid_0x03.IGNSts)
				{
					case 0:
						key_sts = KEY_ON;
						break;
					case 1:
						key_sts = KEY_ON;
						break;
					case 2:
						key_sts = KEY_ON;
						break;
					case 4:
						key_sts = KEY_ON;
						break;
					case 8:
						key_sts = KEY_START;
						break;
					default:
						key_sts = KEY_ON;
						break;
				}
			}
			else
			{
				switch(lin1_msg.pid_0x03.IGNSts)
				{
					case 0:
						key_sts = KEY_OFF;
						break;
					case 1:
						key_sts = KEY_OFF;
						break;
					case 2:
						key_sts = KEY_ACC;
						break;
					case 4:
						key_sts = KEY_OFF;
						break;
					case 8:
						key_sts = KEY_OFF;
						break;
					default:
						key_sts = KEY_OFF;
						break;
				}
			}
			                                      
			break;                                            
		case HAVAL_2012_1P5T_MT_2WD:                           
			//PIN脚点火信号和LIN报文的KEY状态进行合并，以PIN脚点火信号为准
			if(g_u8IgnSts)
			{
				switch(lin2_msg.pid_0x03.IGNSts)
				{
					case 0:
						key_sts = KEY_ON;
						break;
					case 1:
						key_sts = KEY_ON;
						break;
					case 2:
						key_sts = KEY_ON;
						break;
					case 4:
						key_sts = KEY_ON;
						break;
					case 8:
						key_sts = KEY_START;
						break;
					default:
						key_sts = KEY_ON;
						break;
				}
			}
			else
			{
				switch(lin2_msg.pid_0x03.IGNSts)
				{
					case 0:
						key_sts = KEY_OFF;
						break;
					case 1:
						key_sts = KEY_OFF;
						break;
					case 2:
						key_sts = KEY_ACC;
						break;
					case 4:
						key_sts = KEY_OFF;
						break;
					case 8:
						key_sts = KEY_OFF;
						break;
					default:
						key_sts = KEY_OFF;
						break;
				}
			}
			                                   
			break;  
		case HAVAL_2015_1P5T_MT_2WD: 
		case HAVAL_2015_1P5T_6AT_2WD:                           
			//PIN脚点火信号和LIN报文的KEY状态进行合并，以PIN脚点火信号为准
			if(g_u8IgnSts)
			{
				switch(lin3_msg.pid_0x03.IGNSts)
				{
					case 0:
						key_sts = KEY_ON;
						break;
					case 1:
						key_sts = KEY_ON;
						break;
					case 2:
						key_sts = KEY_ON;
						break;
					case 4:
						key_sts = KEY_ON;
						break;
					case 8:
						key_sts = KEY_START;
						break;
					default:
						key_sts = KEY_ON;
						break;
				}
			}
			else
			{
				switch(lin3_msg.pid_0x03.IGNSts)
				{
					case 0:
						key_sts = KEY_OFF;
						break;
					case 1:
						key_sts = KEY_OFF;
						break;
					case 2:
						key_sts = KEY_ACC;
						break;
					case 4:
						key_sts = KEY_OFF;
						break;
					case 8:
						key_sts = KEY_OFF;
						break;
					default:
						key_sts = KEY_OFF;
						break;
				}
			}
			                          
			break; 
			
		case HAVAL_2011_2P0T_MT_2WD:
			//PIN脚点火信号和LIN报文的KEY状态进行合并，以PIN脚点火信号为准
			if(g_u8IgnSts)
			{
				switch(lin4_msg.pid_0x03.IGNSts)
				{
					case 0:
						key_sts = KEY_ON;
						break;
					case 1:
						key_sts = KEY_ON;
						break;
					case 2:
						key_sts = KEY_ON;
						break;
					case 4:
						key_sts = KEY_ON;
						break;
					case 8:
						key_sts = KEY_START;
						break;
					default:
						key_sts = KEY_ON;
						break;
				}
			}
			else
			{
				switch(lin4_msg.pid_0x03.IGNSts)
				{
					case 0:
						key_sts = KEY_OFF;
						break;
					case 1:
						key_sts = KEY_OFF;
						break;
					case 2:
						key_sts = KEY_ACC;
						break;
					case 4:
						key_sts = KEY_OFF;
						break;
					case 8:
						key_sts = KEY_OFF;
						break;
					default:
						key_sts = KEY_OFF;
						break;
				}
			}
			break;  
		case HAVAL_2013_1P5T_MT_4WD:                            
			//PIN脚点火信号和LIN报文的KEY状态进行合并，以PIN脚点火信号为准
			if(g_u8IgnSts)
			{
				switch(lin5_msg.pid_0x03.IGNSts)
				{
					case 0:
						key_sts = KEY_ON;
						break;
					case 1:
						key_sts = KEY_ON;
						break;
					case 2:
						key_sts = KEY_ON;
						break;
					case 4:
						key_sts = KEY_ON;
						break;
					case 8:
						key_sts = KEY_START;
						break;
					default:
						key_sts = KEY_ON;
						break;
				}
			}
			else
			{
				switch(lin5_msg.pid_0x03.IGNSts)
				{
					case 0:
						key_sts = KEY_OFF;
						break;
					case 1:
						key_sts = KEY_OFF;
						break;
					case 2:
						key_sts = KEY_ACC;
						break;
					case 4:
						key_sts = KEY_OFF;
						break;
					case 8:
						key_sts = KEY_OFF;
						break;
					default:
						key_sts = KEY_OFF;
						break;
				}
			}
			                                      
			break;                                         
	}
	
	return key_sts;                                                            
}
  
Key_pos_e app_get_car_key_pos(void)
{
	Key_pos_e Key_pos;

	if(lin_pid_0x03_lost_timecnt<50)
	{
		switch(IPconfig.protocol)                               
		{                                                     
			case HAVAL_2013_2P4L_AT_2WD:                            
				Key_pos=(Key_pos_e)lin0_msg.pid_0x03.KeyInSts;                                            
				break;                                            
			case HAVAL_2013_1P5T_MT_2WD:                            
				Key_pos=(Key_pos_e)lin1_msg.pid_0x03.KeyInSts;                                             
				break;                                            
			case HAVAL_2012_1P5T_MT_2WD:                            
				Key_pos=(Key_pos_e)lin2_msg.pid_0x03.KeyInSts;                                             
				break;   
			case HAVAL_2015_1P5T_MT_2WD: 
			case HAVAL_2015_1P5T_6AT_2WD:                           
				Key_pos=(Key_pos_e)lin3_msg.pid_0x03.KeyInSts;                                        
				break;    
			case HAVAL_2011_2P0T_MT_2WD:
				Key_pos=(Key_pos_e)lin4_msg.pid_0x03.KeyInSts;
				break;  
			case HAVAL_2013_1P5T_MT_4WD:                            
				Key_pos=(Key_pos_e)lin5_msg.pid_0x03.KeyInSts;                                             
				break;                                              
		}                                                     
	}
	else
	{
		Key_pos=KEY_OUT;
	}
	/*IGNON时，认为钥匙在钥匙孔内*/
	if(g_u8IgnSts == ON)
		Key_pos=KEY_IN;
	
	return Key_pos;
}
