#include <string.h>
#include "app_tpms.h"
#include "app_uart_cmd.h"
#include "app_can.h"
#include "mid_string.h"
#include "app_buzzer.h"
#include "buzzer_cfg.h"
#include "app_IPconfig.h"

void app_tpms_init(void)
{       
	memset(&uart_special_data.specialInfo.Tpms,0,sizeof(uart_special_data.specialInfo.Tpms));
	memset(&uart_special_data.specialInfo.Ttms,0,sizeof(uart_special_data.specialInfo.Ttms));
}

void app_tpms_task(void)
{/*
	if(ON == g_u8IgnSts)
	{
		if(can_id_095_lost_timecnt<CAN_LOST_TIME)
		{
			switch(IPconfig.protocol)                               
			{                                                     
				case HAVAL_2013_2P4L_AT_2WD:                            
					uart_special_data.specialInfo.Tpms.FrontLeftTyre=can0.id_095.BCM_LFTirePressure;
					uart_special_data.specialInfo.Tpms.FrontRightTyre=can0.id_095.BCM_RFTirePressure;
					uart_special_data.specialInfo.Tpms.RearLeftTyre=can0.id_095.BCM_LRTirePressure;
					uart_special_data.specialInfo.Tpms.RearRightTyre=can0.id_095.BCM_RRTirePressure;	
					uart_special_data.specialInfo.Ttms.FrontLeftTyre=can0.id_095.BCM_LeftFrontTireTemp;
					uart_special_data.specialInfo.Ttms.FrontRightTyre=can0.id_095.BCM_RightFrontTireTemp;
					uart_special_data.specialInfo.Ttms.RearLeftTyre=can0.id_095.BCM_LeftRearTireTemp;
					uart_special_data.specialInfo.Ttms.RearRightTyre=can0.id_095.BCM_RightRearTireTemp;                                            
					break;                                            
				case HAVAL_2013_1P5T_MT_2WD:                            
					uart_special_data.specialInfo.Tpms.FrontLeftTyre=can1.id_095.BCM_LFTirePressure;
					uart_special_data.specialInfo.Tpms.FrontRightTyre=can1.id_095.BCM_RFTirePressure;
					uart_special_data.specialInfo.Tpms.RearLeftTyre=can1.id_095.BCM_LRTirePressure;
					uart_special_data.specialInfo.Tpms.RearRightTyre=can1.id_095.BCM_RRTirePressure;	
					uart_special_data.specialInfo.Ttms.FrontLeftTyre=can1.id_095.BCM_LeftFrontTireTemp;
					uart_special_data.specialInfo.Ttms.FrontRightTyre=can1.id_095.BCM_RightFrontTireTemp;
					uart_special_data.specialInfo.Ttms.RearLeftTyre=can1.id_095.BCM_LeftRearTireTemp;
					uart_special_data.specialInfo.Ttms.RearRightTyre=can1.id_095.BCM_RightRearTireTemp;                                             
					break;                                            
				case HAVAL_2012_1P5T_MT_2WD:                            
					uart_special_data.specialInfo.Tpms.FrontLeftTyre=can2.id_095.BCM_LFTirePressure;
					uart_special_data.specialInfo.Tpms.FrontRightTyre=can2.id_095.BCM_RFTirePressure;
					uart_special_data.specialInfo.Tpms.RearLeftTyre=can2.id_095.BCM_LRTirePressure;
					uart_special_data.specialInfo.Tpms.RearRightTyre=can2.id_095.BCM_RRTirePressure;	
					uart_special_data.specialInfo.Ttms.FrontLeftTyre=can2.id_095.BCM_LeftFrontTireTemp;
					uart_special_data.specialInfo.Ttms.FrontRightTyre=can2.id_095.BCM_RightFrontTireTemp;
					uart_special_data.specialInfo.Ttms.RearLeftTyre=can2.id_095.BCM_LeftRearTireTemp;
					uart_special_data.specialInfo.Ttms.RearRightTyre=can2.id_095.BCM_RightRearTireTemp;                                             
					break; 
				case HAVAL_2015_1P5T_MT_2WD: 
				case HAVAL_2015_1P5T_6AT_2WD:                           
					uart_special_data.specialInfo.Tpms.FrontLeftTyre=can3.id_095.BCM_LFTirePressure;
					uart_special_data.specialInfo.Tpms.FrontRightTyre=can3.id_095.BCM_RFTirePressure;
					uart_special_data.specialInfo.Tpms.RearLeftTyre=can3.id_095.BCM_LRTirePressure;
					uart_special_data.specialInfo.Tpms.RearRightTyre=can3.id_095.BCM_RRTirePressure;	
					uart_special_data.specialInfo.Ttms.FrontLeftTyre=can3.id_095.BCM_LeftFrontTireTemp;
					uart_special_data.specialInfo.Ttms.FrontRightTyre=can3.id_095.BCM_RightFrontTireTemp;
					uart_special_data.specialInfo.Ttms.RearLeftTyre=can3.id_095.BCM_LeftRearTireTemp;
					uart_special_data.specialInfo.Ttms.RearRightTyre=can3.id_095.BCM_RightRearTireTemp;                                        
					break;  
				case HAVAL_2011_2P0T_MT_2WD:                            
					uart_special_data.specialInfo.Tpms.FrontLeftTyre=can4.id_095.BCM_LFTirePressure;
					uart_special_data.specialInfo.Tpms.FrontRightTyre=can4.id_095.BCM_RFTirePressure;
					uart_special_data.specialInfo.Tpms.RearLeftTyre=can4.id_095.BCM_LRTirePressure;
					uart_special_data.specialInfo.Tpms.RearRightTyre=can4.id_095.BCM_RRTirePressure;	
					uart_special_data.specialInfo.Ttms.FrontLeftTyre=can4.id_095.BCM_LeftFrontTireTemp;
					uart_special_data.specialInfo.Ttms.FrontRightTyre=can4.id_095.BCM_RightFrontTireTemp;
					uart_special_data.specialInfo.Ttms.RearLeftTyre=can4.id_095.BCM_LeftRearTireTemp;
					uart_special_data.specialInfo.Ttms.RearRightTyre=can4.id_095.BCM_RightRearTireTemp;                                             
					break;   
				case HAVAL_2013_1P5T_MT_4WD:                            
					uart_special_data.specialInfo.Tpms.FrontLeftTyre=can5.id_095.BCM_LFTirePressure;
					uart_special_data.specialInfo.Tpms.FrontRightTyre=can5.id_095.BCM_RFTirePressure;
					uart_special_data.specialInfo.Tpms.RearLeftTyre=can5.id_095.BCM_LRTirePressure;
					uart_special_data.specialInfo.Tpms.RearRightTyre=can5.id_095.BCM_RRTirePressure;	
					uart_special_data.specialInfo.Ttms.FrontLeftTyre=can5.id_095.BCM_LeftFrontTireTemp;
					uart_special_data.specialInfo.Ttms.FrontRightTyre=can5.id_095.BCM_RightFrontTireTemp;
					uart_special_data.specialInfo.Ttms.RearLeftTyre=can5.id_095.BCM_LeftRearTireTemp;
					uart_special_data.specialInfo.Ttms.RearRightTyre=can5.id_095.BCM_RightRearTireTemp;                                             
					break;                                         
			}    
		}
		else
		{
			memset(&uart_special_data.specialInfo.Tpms,0,sizeof(uart_special_data.specialInfo.Tpms));
			memset(&uart_special_data.specialInfo.Ttms,0,sizeof(uart_special_data.specialInfo.Ttms));
		}
	}
	else
	{
		memset(&uart_special_data.specialInfo.Tpms,0,sizeof(uart_special_data.specialInfo.Tpms));
		memset(&uart_special_data.specialInfo.Ttms,0,sizeof(uart_special_data.specialInfo.Ttms));
	}
*/}
