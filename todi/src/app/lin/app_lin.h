#ifndef _APP_LIN_H
#define _APP_LIN_H

#include "comm_typedef.h"

#define		LIN_LOST_TIME			25


//一种车型对应一种结构体
typedef struct//HAVAL_2013_2P4L_AT_2WD
{
	struct
	{
		//first byte
		unsigned int AutoLampSwSts:1;
		unsigned int ParkLampSts:1;
		unsigned int PositionLampSts:1;
		unsigned int RearFogLampSts:1;
		unsigned int FrontFogLampSts:1;
		unsigned int RightTurnLampSts:1;
		unsigned int LeftTurnLampSts:1;
		unsigned int HighBeamCmdSts:1;     
		//second byte	 
		unsigned int DriverDoorSts:1;
		unsigned int BackgroundLightDutyCycle:7;
		//third byte	
		unsigned int Reserved1:3;
		unsigned int RrunkDoorSts:1;
		unsigned int RightRearDoorSts:1;
		unsigned int LeftRearDoorSts:1;
		unsigned int RightFrontDoorSts:1;
		unsigned int Reserved2:1;
			
		unsigned int Reserved3:8;
			
		unsigned int Reserved4:32;
	}pid_0x42;
	
	struct
	{
		unsigned int SunroofAutoClose:2;
		unsigned int RLSSensitivityReq:3;
		unsigned int WiperSwitchSts:3;
		unsigned int VehicleSpeed:8;
		unsigned int AmbientTemperature:8;
		unsigned int Reserved1:3;
		unsigned int KeyInSts:1;
		unsigned int IGNSts:4;
		
		unsigned int Reserved2:32;
	}pid_0x03;
	
	struct
	{
		unsigned int Reserved1:7;
		unsigned int IPErrorSts:1;
		
		unsigned int Reserved2:24;
		
		unsigned int Reserved3:32;
	}pid_0x92;
	
}car0_lin_msg_t;
typedef struct//HAVAL_2013_1P5T_MT_2WD
{
	struct
	{
		//first byte
		unsigned int AutoLampSwSts:1;
		unsigned int ParkLampSts:1;
		unsigned int PositionLampSts:1;
		unsigned int RearFogLampSts:1;
		unsigned int FrontFogLampSts:1;
		unsigned int RightTurnLampSts:1;
		unsigned int LeftTurnLampSts:1;
		unsigned int HighBeamCmdSts:1;  
		//second byte	    
		unsigned int DriverDoorSts:1;
		unsigned int BackgroundLightDutyCycle:7;
		//third byte	
		unsigned int Reserved1:3;//b7-b5
		unsigned int RrunkDoorSts:1;//b4
		unsigned int RightRearDoorSts:1;//b3
		unsigned int LeftRearDoorSts:1;//b2
		unsigned int RightFrontDoorSts:1;//b1
		unsigned int Reserved2:1;//b0
		//forth byte	
		unsigned int Reserved3:7;
		unsigned int PEPS_CheckNoKey	: 1;
	
		unsigned int Reserved4 			: 1;
		unsigned int PEPS_ERROR2 		: 1;
		unsigned int PEPS_ERROR1 		: 1;
		unsigned int PEPS_ERROR 		: 1;
		unsigned int Reserved5   		: 1;
		unsigned int PEPS_TurnGearPN	: 1;
		unsigned int Reserved6			: 2;

		unsigned int Reserved7			: 24;
	}pid_0x42;
	
	struct
	{
		unsigned int SunroofAutoClose:2;
		unsigned int RLSSensitivityReq:3;
		unsigned int WiperSwitchSts:3;
		unsigned int VehicleSpeed:8;
		unsigned int AmbientTemperature:8;
		unsigned int Reserved1:3;
		unsigned int KeyInSts:1;
		unsigned int IGNSts:4;
		
		unsigned int Reserved2:32;
	}pid_0x03;
	
	struct
	{
		unsigned int Reserved1:7;
		unsigned int IPErrorSts:1;
		
		unsigned int Reserved2:24;
		
		unsigned int Reserved3:32;
	}pid_0x92;
	
}car1_lin_msg_t;
typedef struct//HAVAL_2012_1P5T_MT_2WD
{
	struct
	{
		unsigned int AutoLampSwSts:1;
		unsigned int ParkLampSts:1;
		unsigned int PositionLampSts:1;
		unsigned int RearFogLampSts:1;
		unsigned int FrontFogLampSts:1;
		unsigned int RightTurnLampSts:1;
		unsigned int LeftTurnLampSts:1;
		unsigned int HighBeamCmdSts:1;   
			   
		unsigned int DriverDoorSts:1;
		unsigned int BackgroundLightDutyCycle:7;
			
		unsigned int Reserved1:16;
			
		unsigned int Reserved2:32;
	}pid_0x42;
	
	struct
	{
		unsigned int SunroofAutoClose:2;
		unsigned int RLSSensitivityReq:3;
		unsigned int WiperSwitchSts:3;
		unsigned int VehicleSpeed:8;
		unsigned int AmbientTemperature:8;
		unsigned int Reserved1:3;
		unsigned int KeyInSts:1;
		unsigned int IGNSts:4;
		
		unsigned int Reserved2:32;
	}pid_0x03;
	
	struct
	{
		unsigned int Reserved1:7;
		unsigned int IPErrorSts:1;
		
		unsigned int Reserved2:24;
		
		unsigned int Reserved3:32;
	}pid_0x92;
	
}car2_lin_msg_t;
typedef struct//HAVAL_2015_1P5T_6AT_2WD
{
	struct
	{
		unsigned int AutoLampSwSts:1;
		unsigned int ParkLampSts:1;
		unsigned int PositionLampSts:1;
		unsigned int RearFogLampSts:1;
		unsigned int FrontFogLampSts:1;
		unsigned int RightTurnLampSts:1;
		unsigned int LeftTurnLampSts:1;
		unsigned int HighBeamCmdSts:1;

		unsigned int DriverDoorSts:1;
		unsigned int BackgroundLightDutyCycle:7;

		unsigned int Reserved1:3;
		unsigned int RrunkDoorSts:1;
		unsigned int RightRearDoorSts:1;
		unsigned int LeftRearDoorSts:1;
		unsigned int RightFrontDoorSts:1;
		unsigned int Reserved2:1;

		unsigned int Reserved3:7;
		unsigned int PEPS_CheckNoKey	: 1;
	
		unsigned int Reserved4 			: 1;
		unsigned int PEPS_ERROR2 		: 1;
		unsigned int PEPS_ERROR1 		: 1;
		unsigned int PEPS_ERROR 		: 1;
		unsigned int Reserved5   		: 1;
		unsigned int PEPS_TurnGearPN	: 1;
		unsigned int Reserved6			: 2;

		unsigned int Reserved7			: 24;
	}pid_0x42;
	
	struct
	{
		unsigned int SunroofAutoClose:2;
		unsigned int RLSSensitivityReq:3;
		unsigned int WiperSwitchSts:3;
		unsigned int VehicleSpeed:8;
		unsigned int AmbientTemperature:8;
		unsigned int Reserved1:3;
		unsigned int KeyInSts:1;
		unsigned int IGNSts:4;
		
		unsigned int Reserved2:32;
	}pid_0x03;
	
	struct
	{
		unsigned int Reserved1:7;
		unsigned int IPErrorSts:1;
		
		unsigned int Reserved2:24;
		
		unsigned int Reserved3:32;
	}pid_0x92;
	
}car3_lin_msg_t;
typedef struct//HAVAL_2011_2P0T_MT_2WD
{
	struct
	{
		unsigned int AutoLampSwSts:1;
		unsigned int ParkLampSts:1;
		unsigned int PositionLampSts:1;
		unsigned int RearFogLampSts:1;
		unsigned int FrontFogLampSts:1;
		unsigned int RightTurnLampSts:1;
		unsigned int LeftTurnLampSts:1;
		unsigned int HighBeamCmdSts:1;   
			   
		unsigned int DriverDoorSts:1;
		unsigned int BackgroundLightDutyCycle:7;
			
		unsigned int Reserved1:16;
			
		unsigned int Reserved2:32;
	}pid_0x42;
	
	struct
	{
		unsigned int SunroofAutoClose:2;
		unsigned int RLSSensitivityReq:3;
		unsigned int WiperSwitchSts:3;
		unsigned int VehicleSpeed:8;
		unsigned int AmbientTemperature:8;
		unsigned int Reserved1:3;
		unsigned int KeyInSts:1;
		unsigned int IGNSts:4;
		
		unsigned int Reserved2:32;
	}pid_0x03;
	
	struct
	{
		unsigned int Reserved1:7;
		unsigned int IPErrorSts:1;
		
		unsigned int Reserved2:24;
		
		unsigned int Reserved3:32;
	}pid_0x92;
	
}car4_lin_msg_t;
typedef struct//HAVAL_2013_1P5T_MT_4WD
{
	struct
	{
		//first byte
		unsigned int AutoLampSwSts:1;
		unsigned int ParkLampSts:1;
		unsigned int PositionLampSts:1;
		unsigned int RearFogLampSts:1;
		unsigned int FrontFogLampSts:1;
		unsigned int RightTurnLampSts:1;
		unsigned int LeftTurnLampSts:1;
		unsigned int HighBeamCmdSts:1;  
		//second byte	    
		unsigned int DriverDoorSts:1;
		unsigned int BackgroundLightDutyCycle:7;
		//third byte	
		unsigned int Reserved1:3;//b7-b5
		unsigned int RrunkDoorSts:1;//b4
		unsigned int RightRearDoorSts:1;//b3
		unsigned int LeftRearDoorSts:1;//b2
		unsigned int RightFrontDoorSts:1;//b1
		unsigned int Reserved2:1;//b0
		//forth byte	
		unsigned int Reserved3:7;
		unsigned int PEPS_CheckNoKey	: 1;
	
		unsigned int Reserved4 			: 1;
		unsigned int PEPS_ERROR2 		: 1;
		unsigned int PEPS_ERROR1 		: 1;
		unsigned int PEPS_ERROR 		: 1;
		unsigned int Reserved5   		: 1;
		unsigned int PEPS_TurnGearPN	: 1;
		unsigned int Reserved6			: 2;

		unsigned int Reserved7			: 24;
	}pid_0x42;
	
	struct
	{
		unsigned int SunroofAutoClose:2;
		unsigned int RLSSensitivityReq:3;
		unsigned int WiperSwitchSts:3;
		unsigned int VehicleSpeed:8;
		unsigned int AmbientTemperature:8;
		unsigned int Reserved1:3;
		unsigned int KeyInSts:1;
		unsigned int IGNSts:4;
		
		unsigned int Reserved2:32;
	}pid_0x03;
	
	struct
	{
		unsigned int Reserved1:7;
		unsigned int IPErrorSts:1;
		
		unsigned int Reserved2:24;
		
		unsigned int Reserved3:32;
	}pid_0x92;
	
}car5_lin_msg_t;

//对外的接口
extern car0_lin_msg_t lin0_msg;
extern car1_lin_msg_t lin1_msg;
extern car2_lin_msg_t lin2_msg;
extern car3_lin_msg_t lin3_msg;
extern car4_lin_msg_t lin4_msg;
extern car5_lin_msg_t lin5_msg;

extern U16 lin_pid_0x42_lost_timecnt;
extern U16 lin_pid_0x03_lost_timecnt;
extern U16 lin_pid_0x92_lost_timecnt;

extern U16 lin_lost_timecnt;
extern void app_lin_stop(void);
extern void app_lin_init(void);
extern U8 app_lin_msg(U8 nPortnum,U8 nLINID,U8 current_rec_status);
extern void app_lin_task10(void);
extern void app_lin_lost_time_cnt_100ms(void);

#endif
