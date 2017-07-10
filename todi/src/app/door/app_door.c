#include "app_door.h"
#include "app_uart_cmd.h"
#include "app_lin.h"
#include "app_IPconfig.h"
#include "app_can.h"

void app_door_init(void)
{
	uart_general_data.generalInfo.leftFrontDoor	=0;
	uart_general_data.generalInfo.trunkDoor			=0;
	uart_general_data.generalInfo.rightRearDoor	=0;
	uart_general_data.generalInfo.rightFrontDoor=0;
	uart_general_data.generalInfo.leftRearDoor	=0;
}

U8 app_get_door_open_sts(void)//获取车门的状态
{
	if((uart_general_data.generalInfo.leftFrontDoor==1)
		||(uart_general_data.generalInfo.rightFrontDoor==1)
		||(uart_general_data.generalInfo.leftRearDoor==1)
		||(uart_general_data.generalInfo.rightRearDoor==1)
		||(uart_general_data.generalInfo.trunkDoor==1))
		
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void app_get_door_sts_100ms(void)
{/*
	switch(IPconfig.protocol)
	{
		case HAVAL_2013_2P4L_AT_2WD:
			if(ON == g_u8IgnSts)
			{
				if(can_id_311_lost_timecnt<CAN_LOST_TIME)
				{
					uart_general_data.generalInfo.leftFrontDoor = can0.id_311.BCM_DriverDoorSts;
					uart_general_data.generalInfo.trunkDoor			=	can0.id_311.BCM_RearLidSts;
					uart_general_data.generalInfo.rightRearDoor	=	can0.id_311.BCM_RRDoorSts;
					uart_general_data.generalInfo.rightFrontDoor=	can0.id_311.BCM_PassengerDoorSts;
					uart_general_data.generalInfo.leftRearDoor	=	can0.id_311.BCM_LRDoorSts;    
				}
				else
				{
					app_door_init();
				}
			}
			else
			{
				if(lin_pid_0x42_lost_timecnt<LIN_LOST_TIME)
				{
					uart_general_data.generalInfo.leftFrontDoor	=	lin0_msg.pid_0x42.DriverDoorSts;
					uart_general_data.generalInfo.rightFrontDoor=	lin0_msg.pid_0x42.RightFrontDoorSts;
					uart_general_data.generalInfo.leftRearDoor	=	lin0_msg.pid_0x42.LeftRearDoorSts;
					uart_general_data.generalInfo.rightRearDoor	=	lin0_msg.pid_0x42.RightRearDoorSts;
					uart_general_data.generalInfo.trunkDoor			=	lin0_msg.pid_0x42.RrunkDoorSts;
				}
				else
				{
					app_door_init();
				}
			}
			break;
		case HAVAL_2013_1P5T_MT_2WD:
			if(ON == g_u8IgnSts)
			{
				if(can_id_311_lost_timecnt<CAN_LOST_TIME)
				{
					uart_general_data.generalInfo.leftFrontDoor = can1.id_311.BCM_DriverDoorSts;
					uart_general_data.generalInfo.trunkDoor			=	can1.id_311.BCM_RearLidSts;
					uart_general_data.generalInfo.rightRearDoor	=	can1.id_311.BCM_RRDoorSts;
					uart_general_data.generalInfo.rightFrontDoor=	can1.id_311.BCM_PassengerDoorSts;
					uart_general_data.generalInfo.leftRearDoor	=	can1.id_311.BCM_LRDoorSts;    
				}
				else
				{
					app_door_init();
				}
			}
			else
			{
				if(lin_pid_0x42_lost_timecnt<LIN_LOST_TIME)
				{
					uart_general_data.generalInfo.leftFrontDoor	=	lin1_msg.pid_0x42.DriverDoorSts;
					uart_general_data.generalInfo.rightFrontDoor=	lin1_msg.pid_0x42.RightFrontDoorSts;
					uart_general_data.generalInfo.leftRearDoor	=	lin1_msg.pid_0x42.LeftRearDoorSts;
					uart_general_data.generalInfo.rightRearDoor	=	lin1_msg.pid_0x42.RightRearDoorSts;
					uart_general_data.generalInfo.trunkDoor			=	lin1_msg.pid_0x42.RrunkDoorSts;
				}
				else
				{
					app_door_init();
				}
			}
			break;
		case HAVAL_2012_1P5T_MT_2WD:
			if(ON == g_u8IgnSts)
			{
				if(can_id_311_lost_timecnt<CAN_LOST_TIME)
				{
					uart_general_data.generalInfo.leftFrontDoor = can2.id_311.BCM_DriverDoorSts;
					uart_general_data.generalInfo.trunkDoor			=	can2.id_311.BCM_RearLidSts;
					uart_general_data.generalInfo.rightRearDoor	=	can2.id_311.BCM_RRDoorSts;
					uart_general_data.generalInfo.rightFrontDoor=	can2.id_311.BCM_PassengerDoorSts;
					uart_general_data.generalInfo.leftRearDoor	=	can2.id_311.BCM_LRDoorSts;    
				}
				else
				{
					app_door_init();
				}
			}
			else
			{
				if(lin_pid_0x42_lost_timecnt<LIN_LOST_TIME)
				{
					uart_general_data.generalInfo.leftFrontDoor	=	lin2_msg.pid_0x42.DriverDoorSts;
					uart_general_data.generalInfo.rightFrontDoor=	0;
					uart_general_data.generalInfo.leftRearDoor	=	0;
					uart_general_data.generalInfo.rightRearDoor	=	0;
					uart_general_data.generalInfo.trunkDoor			=	0;
				}                                                  
				else
				{
					app_door_init();
				}
			}
			break;
		case HAVAL_2015_1P5T_MT_2WD: 
		case HAVAL_2015_1P5T_6AT_2WD:
			if(ON == g_u8IgnSts)
			{
				if(can_id_311_lost_timecnt<CAN_LOST_TIME)
				{
					uart_general_data.generalInfo.leftFrontDoor = can3.id_311.BCM_DriverDoorSts;
					uart_general_data.generalInfo.trunkDoor			=	can3.id_311.BCM_RearLidSts;
					uart_general_data.generalInfo.rightRearDoor	=	can3.id_311.BCM_RRDoorSts;
					uart_general_data.generalInfo.rightFrontDoor=	can3.id_311.BCM_PassengerDoorSts;
					uart_general_data.generalInfo.leftRearDoor	=	can3.id_311.BCM_LRDoorSts;    
				}
				else
				{
					app_door_init();
				} 
			}
			else
			{
				if(lin_pid_0x42_lost_timecnt<LIN_LOST_TIME)
				{
					uart_general_data.generalInfo.leftFrontDoor	=	lin3_msg.pid_0x42.DriverDoorSts;
					uart_general_data.generalInfo.rightFrontDoor=	lin3_msg.pid_0x42.RightFrontDoorSts;
					uart_general_data.generalInfo.leftRearDoor	=	lin3_msg.pid_0x42.LeftRearDoorSts;
					uart_general_data.generalInfo.rightRearDoor	=	lin3_msg.pid_0x42.RightRearDoorSts;
					uart_general_data.generalInfo.trunkDoor			=	lin3_msg.pid_0x42.RrunkDoorSts;
				}
				else
				{
					app_door_init();
				}
			}
			break; 
		case HAVAL_2011_2P0T_MT_2WD:
			if(ON == g_u8IgnSts)
			{
				if(can_id_311_lost_timecnt<CAN_LOST_TIME)
				{
					uart_general_data.generalInfo.leftFrontDoor = can4.id_311.BCM_DriverDoorSts;
					uart_general_data.generalInfo.trunkDoor			=	can4.id_311.BCM_RearLidSts;
					uart_general_data.generalInfo.rightRearDoor	=	can4.id_311.BCM_RRDoorSts;
					uart_general_data.generalInfo.rightFrontDoor=	can4.id_311.BCM_PassengerDoorSts;
					uart_general_data.generalInfo.leftRearDoor	=	can4.id_311.BCM_LRDoorSts;    
				}
				else
				{
					app_door_init();
				} 
			}
			else
			{
				if(lin_pid_0x42_lost_timecnt<LIN_LOST_TIME)
				{
					uart_general_data.generalInfo.leftFrontDoor	=	lin4_msg.pid_0x42.DriverDoorSts;
					uart_general_data.generalInfo.rightFrontDoor=	0;
					uart_general_data.generalInfo.leftRearDoor	=	0;
					uart_general_data.generalInfo.rightRearDoor	=	0;
					uart_general_data.generalInfo.trunkDoor			=	0;
				}
				else
				{
					app_door_init();
				}
			}
			break; 
		case HAVAL_2013_1P5T_MT_4WD:
			if(ON == g_u8IgnSts)
			{
				if(can_id_311_lost_timecnt<CAN_LOST_TIME)
				{
					uart_general_data.generalInfo.leftFrontDoor = can5.id_311.BCM_DriverDoorSts;
					uart_general_data.generalInfo.trunkDoor			=	can5.id_311.BCM_RearLidSts;
					uart_general_data.generalInfo.rightRearDoor	=	can5.id_311.BCM_RRDoorSts;
					uart_general_data.generalInfo.rightFrontDoor=	can5.id_311.BCM_PassengerDoorSts;
					uart_general_data.generalInfo.leftRearDoor	=	can5.id_311.BCM_LRDoorSts;    
				}
				else
				{
					app_door_init();
				}
			}
			else
			{
				if(lin_pid_0x42_lost_timecnt<LIN_LOST_TIME)
				{
					uart_general_data.generalInfo.leftFrontDoor	=	lin5_msg.pid_0x42.DriverDoorSts;
					uart_general_data.generalInfo.rightFrontDoor=	lin5_msg.pid_0x42.RightFrontDoorSts;
					uart_general_data.generalInfo.leftRearDoor	=	lin5_msg.pid_0x42.LeftRearDoorSts;
					uart_general_data.generalInfo.rightRearDoor	=	lin5_msg.pid_0x42.RightRearDoorSts;
					uart_general_data.generalInfo.trunkDoor			=	lin5_msg.pid_0x42.RrunkDoorSts;
				}
				else
				{
					app_door_init();
				}
			}
			break; 
	}
*/}
