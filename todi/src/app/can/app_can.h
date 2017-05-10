#ifndef    __APP_CAN_H__
#define    __APP_CAN_H__

#include  "mid_can.h"

#define    MSG_BOX_ALl				31

#define		CAN_LOST_TIME			25

typedef  struct {//HAVAL_2013_2P4L_AT_2WD
	 struct  
	{
		/* b63 -b0 */
		U32		ABS_WssRLEdgesSum:16;
		U32		ABS_WssRREdgesSum:16;	
			
		U32		ABS_ABSFault:1;
		U32		ABS_EBDFault:1;	
		U32		ABS_VehicleSpeedValidSignal:1;
		U32		ABS_VehicleSpeed:13;
		U32   ABS_ABSControlActive:1;
		U32   Reserved:15;
	}id_265;
	struct  
	{	
		/* b63 -b0*/
		U32   ECM_EngTrqFailure:1;
		U32		ECM_ACCompRelaySts:1;
		U32		ECM_TCUTrqRedFail:2;
		U32		Reserved1:2;	
		U32		ECM_EngSpdFailure:1;
		U32		Reserved2:1;
		U32		ECM_TQI_ACOR:8;
		U32		ECM_EngSpd:16;
		U32		ECM_TQI:8;
		U32		ECM_CAN_TQFR:8;
		U32		Reserved3:2;		
		U32		ECM_CAN_TQ_STND:6;
		U32		ECM_IntakeAirTemp:8;	
	}id_111;
	 struct 
	{
		/* b63 -b0*/
		U32   ECM_MAFAltiFactor:8;
		U32		ECM_EngCoolantTemp:8;
		U32		ECM_CAN_SOAKTM:12;
		U32		ECM_EngCoolantTempWrn:1;
		U32		Reserved1:3;
		U32		ECM_BaroPressure:8;
		U32		ECM_ThrtlCmdPos:8;
		U32		Reserved2:3;
		U32		ECM_CAN_BRK_SW:1;
		U32		ECM_ThrtlCmdPosFail:1;		
		U32		ECM_BaroPressureFail:1;
		U32		ECM_CAN_BRK_LAMP:1;	
		U32		ECM_ECMImmoAuthRes:1;	
		U32		ECM_OBDDrivingCycle:1;	
		U32		ECM_OBDWarmUp:1;	
		U32		Reserved3:1;
		U32		ECM_EngStatus:2;
		U32		Reserved4:3;	
	}id_271;
	 struct 
	{
		/* b63 -b0*/
		U32		TCU_ConvertClhLckSts:2;	
		U32		TCU_TCURedTrqType:1;
		U32		Reserved1:1;	
		U32		TCU_GearChgActive:1;	
		U32		TCU_GearSts:3;
		U32		Reserved2:2;		
		U32		TCU_TCUFailure:2;	
		U32		TCU_GearLeverPos:4;	
		U32		Reserved3:5;	
		U32		TCU_TCUOBDMILOnReq:1;	
		U32		Reserved4:2;
		U32		TCU_TrqRedReq:8;	
		U32		TCU_TransOilTemp:8;	
		U32		Reserved5:4;
		U32		TCU_TCUTrqCtrlActiveFlag:2;	
		U32		Reserved6:10;
		U32		TCU_TCUVehicleSpd:8;	
	}id_131;
	 struct 
	{
		/* b63 -b0*/
		U32		BCM_LFTirePressure:8;	
		U32		BCM_RFTirePressure:8;
		U32		BCM_LeftFrontTireTemp:8;	
		U32		BCM_RightFrontTireTemp:8;	
		U32		BCM_LRTirePressure:8;
		U32		BCM_RRTirePressure:8;		
		U32		BCM_LeftRearTireTemp:8;	
		U32		BCM_RightRearTireTemp:8;	
	}id_095;
	struct 
	{
		/* b63 -b0*/
		U32		BCM_PosLmpSts:1;	
		U32		BCM_LHPosLmpFailSts:1;
		U32		BCM_RHPosLmpFailSts:1;	
		U32		BCM_LicPlateLmpFailSts:1;	
		U32		BCM_DRLSts:2;
		U32		BCM_DRLFailSts:2;	
		
		U32		BCM_FrontFogLmpSts:2;	
		U32		BCM_RearFogLmpSts:1;
		U32		BCM_HighBeamSts:1;
		U32		BCM_LowBeamSts:1;
		U32		BCM_LeftTurnSignalCmd:1;	
		U32		BCM_RightTurnSignalCmd:1;	
		U32		BCM_BrakeLmpFailSts:1;
		
		U32		BCM_RearLidSts:1;		
		U32		BCM_DriverDoorSts:1;	
		U32		BCM_LRDoorSts:1;	
		U32		BCM_PassengerDoorSts:1;	
		U32		BCM_RRDoorSts:1;
		U32		BCM_KeyInsertionSts:1;	
		U32		BCM_WindShieldWiperAct:1;	
		U32		BCM_BrakePedSts:1;
		
		U32		BCM_BrakePedFailSts:1;		
		U32		BCM_BattVolt:7;	
		
		U32		BCM_RearFogLmpFailSts:1;	
		U32		BCM_BacklightDutyCycle:7;	
		
		U32		BCM_RLSFailSts:2;
		U32		BCM_ParkBrakeSts:1;	
		U32		BCM_ParkBrakeFailSts:2;	
		U32		BCM_ReversGearSts:2;
		U32		BCM_HoodSts:1;		
		
		U32		BCM_ReversGearFailSts:2;	
		U32		BCM_LFTireIndSts:2;	
		U32		BCM_RFTireIndSts:2;	
		U32		BCM_LRTireIndSts:2;
		
		U32		BCM_RRTireIndSts:2;	
		U32		BCM_TPMSFailureInd:2;	
		U32		BCM_TPWarningInd:2;
		U32		BCM_TPFastLeakageInd:2;		
	}id_311;
	struct 
	{
		/* b63 -b0*/
		U32		ECM_FuelCutOffActive:1;	
		U32		ECM_AccEffPosSts:1;
		U32		ECM_AccActualPosSts:1;	
		U32		Reserved1:1;
		U32		ECM_MILSts:1;	
		U32		ECM_EngineSVSSignal:1;
		U32		ECM_CruiseCtrlAct:2;		
		U32		ECM_CAN_N_IDL:16;	
		U32		ECM_AccEffPos:8;	
		U32		ECM_AccActualPos:8;	
		U32		ECM_TgtCruiseVehSpd:8;
		U32		ECM_Fuelconsumption:16;	
	}id_371;
	struct 
	{
		/* b63 -b0*/
		U32		ABM_PsngrSeatBeltSts:1;	
		U32		ABM_DrSeatBeltSts:1;
		U32		ABM_AirBagFault:1;	
		U32   Reserved1:5;
		U32   Reserved2:24;
		U32   Reserved3:32;
	}id_351;
	
	struct{
		/*63-0*/
		U32 RearRadarLeftSensorSegNum:	3;
		U32 RearRadarLeftSensorSegFlash:	1;
		U32 RearRadarLeftMidSensorSegNum:	3;
		U32 RearRadarLeftMidSensorSegFlash:	1;
		U32 RearRadarRightMidSensorSegNum:	3;
		U32 RearRadarRightMidSensorSegFlash:	1;
		U32 RearRadarRightSensorSegNum:	3;
		U32 RearRadarRightSensorSegFlash:	1;
		U32 RearRadarDiag:	1;
		U32 RearRadarWarning:	1;
		U32 RearRadarsoundlevel:	3;
		U32 RearRadarReserved1:2;
		U32 RadarFailure:	1;
		U32 FrontRadarLeftSensorSegNum:	3;
		U32 FrontRadarLeftSensorSegFlash:	1;
		U32 FrontRadarLeftMidSensorSegNum:	3;
		U32 FrontRadarLeftMidSensorSegFlash:	1;
		U32 FrontRadarRightMidSensorSegNum:	3;
		U32 FrontRadarRightMidSensorSegFlash:	1;
		U32 FrontRadarRightSensorSegNum:	3;
		U32 FrontRadarRightSensorSegFlash:	1;
		U32 FrontRadarDiag:	1;
		U32 FrontRadarWarning:	1;
		U32 FrontRadarsoundlevel:	3;
		U32 FrontRadarReserved:	3;
		U32 RearRadarReserved2:16;
	}id_3df;
	struct
	{
		U32 Reserved1						: 7;
		U32 VehTotalMileageValid:	1;
		U32 VehTotalMileage			: 24;
		U32 Reserved2						: 32;
	}id_345;
	struct
	{
		U8 config[8];
	}id_7a6;
	struct
	{
		U32 ALS			: 1;
		U32 Reserved1	: 7;
		U32 Reserved2	: 24;
		U32 Reserved3	: 32;
	}id_365;
	struct
	{
		U32 CarACstate	: 8; /* 空调开关 0x10-on; 0x90-吹前档玻璃 */
		U32 WindSpeed	: 8; /* 风量级别 1-8级:0x10-0x80 */
		U32 outCarTemp	: 8;
		U32 inCarTemp	: 8;
		U32 Reserved1   : 32;
	}id_08B;

	struct /*另一块中控屏*/
	{
		U32 CarACstate	: 8; /* 前除霜 0x90-on; 0x40-后除霜 */
		U32 WindSpeed	: 8; /* 风量级别 1-8级:0x10-0x80；空调开关 0x40-on */
		U32 outCarTemp	: 8;
		U32 inCarTemp	: 8;
		U32 Reserved1   : 32;
	}id_485;
} car0_can_signal_t;
typedef  struct {//HAVAL_2013_1P5T_MT_2WD
	 struct  
	{
		/* b63 -b0 */
		U32		ABS_WssRLEdgesSum:16;
		U32		ABS_WssRREdgesSum:16;	
			
		U32		ABS_ABSFault:1;
		U32		ABS_EBDFault:1;	
		U32		ABS_VehicleSpeedValidSignal:1;
		U32		ABS_VehicleSpeed:13;
		U32   ABS_ABSControlActive:1;
		U32   Reserved:15;
	}id_265;
	struct  
	{	
		/* b63 -b0*/
		U32   Reserved1: 32;
		U32		Reserved2:	8;
		U32   ECM_EngSpd:16;
		U32		Reserved3:8;	
	}id_111;
	 struct 
	{
		/* b63 -b0*/
		U32 	ECM_Fuelconsumption:16;
		U32		Reserved1:8;
		U32		ECM_Fuelconsumption_H:8;
		U32		ECM_Fuelconsumption_L:8;
		U32		ECM_ThrtlCmdPos:8;
		U32   ECM_CruiseCtrlAct:2;
		U32		Reserved2:1;
		U32		ECM_CAN_BRK_SW:1;
		U32		ECM_ThrtlCmdPosFail:1;		
		U32		ECM_BaroPressureFail:1;
		U32		ECM_CAN_BRK_LAMP:1;	
		U32		ECM_ECMImmoAuthRes:1;
		U32		ECM_MILSts:1;	
		U32		ECM_OBDWarmUp:1;	
		U32		Reserved3:1;
		U32		ECM_EngStatus:2;	
		U32		Reserved4:2;
		U32		ECM_EngineSVSSignal:1;
	}id_271;
	 struct 
	{
		/* b63 -b0*/
		U32		BCM_LFTirePressure:8;	
		U32		BCM_RFTirePressure:8;
		U32		BCM_LeftFrontTireTemp:8;	
		U32		BCM_RightFrontTireTemp:8;	
		U32		BCM_LRTirePressure:8;
		U32		BCM_RRTirePressure:8;		
		U32		BCM_LeftRearTireTemp:8;	
		U32		BCM_RightRearTireTemp:8;	
	}id_095;
	struct 
	{
		/* b63 -b0*/
		U32		BCM_PosLmpSts:1;	
		U32		BCM_LHPosLmpFailSts:1;
		U32		BCM_RHPosLmpFailSts:1;	
		U32		BCM_LicPlateLmpFailSts:1;	
		U32		BCM_DRLSts:2;
		U32		BCM_DRLFailSts:2;		
		U32		BCM_FrontFogLmpSts:2;	
		U32		BCM_RearFogLmpSts:1;
		U32		BCM_HighBeamSts:1;
		U32		BCM_LowBeamSts:1;
		U32		BCM_LeftTurnSignalCmd:1;	
		U32		BCM_RightTurnSignalCmd:1;	
		U32		BCM_BrakeLmpFailSts:1;
		U32		BCM_RearLidSts:1;		
		U32		BCM_DriverDoorSts:1;	
		U32		BCM_LRDoorSts:1;	
		U32		BCM_PassengerDoorSts:1;	
		U32		BCM_RRDoorSts:1;
		U32		BCM_KeyInsertionSts:1;	
		U32		BCM_WindShieldWiperAct:1;	
		U32		BCM_BrakePedSts:1;
		U32		BCM_BrakePedFailSts:1;		
		U32		BCM_BattVolt:7;	
		U32		BCM_RearFogLmpFailSts:1;	
		U32		BCM_BacklightDutyCycle:7;	
		U32		BCM_RLSFailSts:2;
		U32		BCM_ParkBrakeSts:1;	
		U32		BCM_ParkBrakeFailSts:2;	
		U32		BCM_ReversGearSts:2;
		U32		BCM_HoodSts:1;		
		U32		BCM_ReversGearFailSts:2;	
		U32		BCM_LFTireIndSts:2;	
		U32		BCM_RFTireIndSts:2;	
		U32		BCM_LRTireIndSts:2;
		U32		BCM_RRTireIndSts:2;	
		U32		BCM_TPMSFailureInd:2;	
		U32		BCM_TPWarningInd:2;
		U32		BCM_TPFastLeakageInd:2;		
	}id_311;
	struct 
	{
		/* b63 -b0*/
		U32		ECM_FuelCutOffActive:1;	
		U32		ECM_AccEffPosSts:1;
		U32		ECM_AccActualPosSts:1;	
		U32		Reserved1:1;
		U32		Reserved0:1;	
		U32		ECM_EngineSVSSignal:1;
		U32		Reserverd2:2;	
		U32		ECM_CAN_N_IDL:16;	
		U32		ECM_EngCoolantTemp:8;	
		U32		ECM_AccActualPos:8;	
		U32		ECM_TgtCruiseVehSpd:8;
		U32		Reserverd3:16;	
	}id_371;
	struct 
	{
		/* b63 -b0*/
		U32		ABM_PsngrSeatBeltSts:1;	
		U32		ABM_DrSeatBeltSts:1;
		U32		ABM_AirBagFault:1;	
		U32   Reserved1:5;
		U32   Reserved2:24;
		U32   Reserved3:32;
	}id_351;
	
	struct{
		/*63-0*/
		U32 RearRadarLeftSensorSegNum:	3;
		U32 RearRadarLeftSensorSegFlash:	1;
		U32 RearRadarLeftMidSensorSegNum:	3;
		U32 RearRadarLeftMidSensorSegFlash:	1;
		U32 RearRadarRightMidSensorSegNum:	3;
		U32 RearRadarRightMidSensorSegFlash:	1;
		U32 RearRadarRightSensorSegNum:	3;
		U32 RearRadarRightSensorSegFlash:	1;
		U32 RearRadarDiag:	1;
		U32 RearRadarWarning:	1;
		U32 RearRadarsoundlevel:	3;
		U32 RearRadarReserved1:2;
		U32 RadarFailure:	1;
		U32 FrontRadarLeftSensorSegNum:	3;
		U32 FrontRadarLeftSensorSegFlash:	1;
		U32 FrontRadarLeftMidSensorSegNum:	3;
		U32 FrontRadarLeftMidSensorSegFlash:	1;
		U32 FrontRadarRightMidSensorSegNum:	3;
		U32 FrontRadarRightMidSensorSegFlash:	1;
		U32 FrontRadarRightSensorSegNum:	3;
		U32 FrontRadarRightSensorSegFlash:	1;
		U32 FrontRadarDiag:	1;
		U32 FrontRadarWarning:	1;
		U32 FrontRadarsoundlevel:	3;
		U32 FrontRadarReserved:	3;
		U32 RearRadarReserved2:16;
	}id_3df;
	struct
	{
		U32 Reserved1						: 7;
		U32 VehTotalMileageValid:	1;
		U32 VehTotalMileage			: 24;
		U32 Reserved2						: 32;
	}id_345;
	struct
	{
		U8 config[8];
	}id_7a6;
	struct
	{
		U32 reserved1:24;
		U32 reserved2:8;
		U32 reserved3:8;
		U32 reserved4:3;
		U32 ESP_ERR:1;
		U32 reserved5:1;
		U32 reserved6:1;
		U32 reserved7:1;
		U32 reserved8:1;
		U32 reserved9:8;	
		U32 reserved10:1;																
		U32 ESP_OFF		:	1;
		U32 ESP_NML	:	1;
		U32 reserved11				:	1;
		U32 reserved12	:	4;

	}id_211;

	struct
	{
		U32 ALS			: 1;
		U32 Reserved1	: 7;
		U32 Reserved2	: 24;
		U32 Reserved3	: 32;
	}id_365;

	struct  
	{	
		/* b63 -b0*/
		U32   Reserved1 		: 18;
		U32   PEPS_TurnOFFpwr	: 1;
		U32   PEPS_TakeAwayKey	: 1;
		U32   PEPS_CheckNoKey	: 1;
		U32   reserved2			: 11;
		U32	  PEPS_TurnGearPN	: 1;
		U32	  Reserved3 		: 3;
		U32	  PEPS_ERROR 		: 1;	
		U32	  Reserved4 		: 7;	
		U32	  PEPS_ERROR1 		: 1;	
		U32	  Reserved5 		: 19;	
	}id_0A5;

	struct
	{
		U32 CarACstate	: 8; /* 空调开关 0x10-on; 0x90-吹前档玻璃 */
		U32 WindSpeed	: 8; /* 风量级别 1-8级:0x10-0x80 */
		U32 outCarTemp	: 8;
		U32 inCarTemp	: 8;
		U32 Reserved1   : 32;
	}id_08B;

	struct /*另一块中控屏*/
	{
		U32 CarACstate	: 8; /* 前除霜 0x90-on; 0x40-后除霜 */
		U32 WindSpeed	: 8; /* 风量级别 1-8级:0x10-0x80；空调开关 0x40-on */
		U32 outCarTemp	: 8;
		U32 inCarTemp	: 8;
		U32 Reserved1	: 32;
	}id_485;

} car1_can_signal_t;
typedef  struct {//HAVAL_2012_1P5T_MT_2WD
	 struct  
	{
		/* b63 -b0 */
		U32		ABS_WssRLEdgesSum:16;
		U32		ABS_WssRREdgesSum:16;	
			
		U32		ABS_ABSFault:1;
		U32		ABS_EBDFault:1;	
		U32		ABS_VehicleSpeedValidSignal:1;
		U32		ABS_VehicleSpeed:13;
		U32   ABS_ABSControlActive:1;
		U32   Reserved:15;
	}id_265;
	struct  
	{	
		/* b63 -b0*/
		U32   Reserved1: 32;
		U32		Reserved2:	8;
		U32   ECM_EngSpd:16;
		U32		Reserved3:8;	
	}id_111;
	 struct 
	{
		/* b63 -b0*/
		U32 	ECM_Fuelconsumption:16;
		U32		Reserved1:8;
		U32		ECM_Fuelconsumption_H:8;
		U32		ECM_Fuelconsumption_L:8;
		U32		ECM_ThrtlCmdPos:8;
		U32   ECM_CruiseCtrlAct:2;
		U32		Reserved2:1;
		U32		ECM_CAN_BRK_SW:1;
		U32		ECM_ThrtlCmdPosFail:1;		
		U32		ECM_BaroPressureFail:1;
		U32		ECM_CAN_BRK_LAMP:1;	
		U32		ECM_ECMImmoAuthRes:1;
		U32		ECM_MILSts:1;	
		U32		ECM_OBDWarmUp:1;	
		U32		Reserved3:1;
		U32		ECM_EngStatus:2;
		U32		Reserved4:2;
		U32		ECM_EngineSVSSignal:1;
	}id_271;
	 struct 
	{
		/* b63 -b0*/
		U32		BCM_LFTirePressure:8;	
		U32		BCM_RFTirePressure:8;
		U32		BCM_LeftFrontTireTemp:8;	
		U32		BCM_RightFrontTireTemp:8;	
		U32		BCM_LRTirePressure:8;
		U32		BCM_RRTirePressure:8;		
		U32		BCM_LeftRearTireTemp:8;	
		U32		BCM_RightRearTireTemp:8;	
	}id_095;
	struct 
	{
		/* b63 -b0*/
		U32		BCM_PosLmpSts:1;	
		U32		BCM_LHPosLmpFailSts:1;
		U32		BCM_RHPosLmpFailSts:1;	
		U32		BCM_LicPlateLmpFailSts:1;	
		U32		BCM_DRLSts:2;
		U32		BCM_DRLFailSts:2;		
		U32		BCM_FrontFogLmpSts:2;	
		U32		BCM_RearFogLmpSts:1;
		U32		BCM_HighBeamSts:1;
		U32		BCM_LowBeamSts:1;
		U32		BCM_LeftTurnSignalCmd:1;	
		U32		BCM_RightTurnSignalCmd:1;	
		U32		BCM_BrakeLmpFailSts:1;
		U32		BCM_RearLidSts:1;		
		U32		BCM_DriverDoorSts:1;	
		U32		BCM_LRDoorSts:1;	
		U32		BCM_PassengerDoorSts:1;	
		U32		BCM_RRDoorSts:1;
		U32		BCM_KeyInsertionSts:1;	
		U32		BCM_WindShieldWiperAct:1;	
		U32		BCM_BrakePedSts:1;
		U32		BCM_BrakePedFailSts:1;		
		U32		BCM_BattVolt:7;	
		U32		BCM_RearFogLmpFailSts:1;	
		U32		BCM_BacklightDutyCycle:7;	
		U32		BCM_RLSFailSts:2;
		U32		BCM_ParkBrakeSts:1;	
		U32		BCM_ParkBrakeFailSts:2;	
		U32		BCM_ReversGearSts:2;
		U32		BCM_HoodSts:1;		
		U32		BCM_ReversGearFailSts:2;	
		U32		BCM_LFTireIndSts:2;	
		U32		BCM_RFTireIndSts:2;	
		U32		BCM_LRTireIndSts:2;
		U32		BCM_RRTireIndSts:2;	
		U32		BCM_TPMSFailureInd:2;	
		U32		BCM_TPWarningInd:2;
		U32		BCM_TPFastLeakageInd:2;		
	}id_311;
	struct 
	{
		/* b63 -b0*/
		U32		ECM_FuelCutOffActive:1;	
		U32		ECM_AccEffPosSts:1;
		U32		ECM_AccActualPosSts:1;	
		U32		Reserved1:1;
		U32		Reserved0:1;	
		U32		ECM_EngineSVSSignal:1;
		U32		Reserverd2:2;		
		U32		ECM_CAN_N_IDL:16;	
		U32		ECM_EngCoolantTemp:8;	
		U32		ECM_AccActualPos:8;	
		U32		ECM_TgtCruiseVehSpd:8;
		U32		Reserverd3:16;	
	}id_371;
	struct 
	{
		/* b63 -b0*/
		U32		ABM_PsngrSeatBeltSts:1;	
		U32		ABM_DrSeatBeltSts:1;
		U32		ABM_AirBagFault:1;	
		U32   Reserved1:5;
		U32   Reserved2:24;
		U32   Reserved3:32;
	}id_351;
	struct
	{
		U32 Reserved1						: 7;
		U32 VehTotalMileageValid:	1;
		U32 VehTotalMileage			: 24;
		U32 Reserved2						: 32;
	}id_345;
	struct
	{
		U8 config[8];
	}id_7a6;

	struct
	{
		U32 reserved1:24;
		U32 reserved2:8;
		U32 reserved3:8;
		U32 reserved4:3;
		U32 ESP_ERR:1;
		U32 reserved5:1;
		U32 reserved6:1;
		U32 reserved7:1;
		U32 reserved8:1;
		U32 reserved9:8;	
		U32 reserved10:1;																
		U32 ESP_OFF		:	1;
		U32 ESP_NML	:	1;
		U32 reserved11				:	1;
		U32 reserved12	:	4;

	}id_211;
	
	struct
	{
		U32 CarACstate	: 8; /* 空调开关 0x10-on; 0x90-吹前档玻璃 */
		U32 WindSpeed	: 8; /* 风量级别 1-8级:0x10-0x80 */
		U32 outCarTemp	: 8;
		U32 inCarTemp	: 8;
		U32 Reserved1   : 32;
	}id_08B;

	struct /*另一块中控屏*/
	{
		U32 CarACstate	: 8; /* 前除霜 0x90-on; 0x40-后除霜 */
		U32 WindSpeed	: 8; /* 风量级别 1-8级:0x10-0x80；空调开关 0x40-on */
		U32 outCarTemp	: 8;
		U32 inCarTemp	: 8;
		U32 Reserved1   : 32;
	}id_485;
} car2_can_signal_t;
typedef  struct {//HAVAL_2015_1P5T_6AT_2WD
	 struct  
	{
		/* b63 -b0 */
		U32		ABS_WssRLEdgesSum:16;
		U32		ABS_WssRREdgesSum:16;	
			
		U32		ABS_ABSFault:1;
		U32		ABS_EBDFault:1;	
		U32		ABS_VehicleSpeedValidSignal:1;
		U32		ABS_VehicleSpeed:13;
		U32   ABS_ABSControlActive:1;
		U32   Reserved:15;
	}id_265;
	struct  
	{	
		/* b63 -b0*/
		U32   Reserved1: 32;
		U32		Reserved2:	8;
		U32   ECM_EngSpd:16;
		U32		Reserved3:1;
		U32		ECM_EngSpdValid:1;
		U32		Reserved4:6;	
	}id_111;
	 struct 
	{
		/* b63 -b0*/
		U32 	ECM_Fuelconsumption:16;
		U32		Reserved1:8;
		U32		ECM_Fuelconsumption_H:8;
		U32		ECM_Fuelconsumption_L:8;
		U32		ECM_ThrtlCmdPos:8;
		U32   ECM_CruiseCtrlAct:2;
		U32		Reserved2:1;
		
		U32		ECM_CAN_BRK_SW:1;
		U32		ECM_ThrtlCmdPosFail:1;		
		U32		ECM_BaroPressureFail:1;
		U32		ECM_CAN_BRK_LAMP:1;	
		U32		ECM_ECMImmoAuthRes:1;
		U32		ECM_MILSts:1;	
		U32		ECM_OBDWarmUp:1;	
		U32		Reserved3:1;
		U32		ECM_EngStatus:2;
		U32		Reserved4:2;
		U32		ECM_EngineSVSSignal:1;
	}id_271;
	 struct 
	{
		/* b63 -b0*/
		U32		BCM_LFTirePressure:8;	
		U32		BCM_RFTirePressure:8;
		U32		BCM_LeftFrontTireTemp:8;	
		U32		BCM_RightFrontTireTemp:8;	
		U32		BCM_LRTirePressure:8;
		U32		BCM_RRTirePressure:8;		
		U32		BCM_LeftRearTireTemp:8;	
		U32		BCM_RightRearTireTemp:8;	
	}id_095;
	struct 
	{
		/* b63 -b0*/
		U32		BCM_PosLmpSts:1;	
		U32		BCM_LHPosLmpFailSts:1;
		U32		BCM_RHPosLmpFailSts:1;	
		U32		BCM_LicPlateLmpFailSts:1;	
		U32		BCM_DRLSts:2;
		U32		BCM_DRLFailSts:2;		
		U32		BCM_FrontFogLmpSts:2;	
		U32		BCM_RearFogLmpSts:1;
		U32		BCM_HighBeamSts:1;
		U32		BCM_LowBeamSts:1;
		U32		BCM_LeftTurnSignalCmd:1;	
		U32		BCM_RightTurnSignalCmd:1;	
		U32		BCM_BrakeLmpFailSts:1;
		U32		BCM_RearLidSts:1;		
		U32		BCM_DriverDoorSts:1;	
		U32		BCM_LRDoorSts:1;	
		U32		BCM_PassengerDoorSts:1;	
		U32		BCM_RRDoorSts:1;
		U32		BCM_KeyInsertionSts:1;	
		U32		BCM_WindShieldWiperAct:1;	
		U32		BCM_BrakePedSts:1;
		U32		BCM_BrakePedFailSts:1;		
		U32		BCM_BattVolt:7;	
		U32		BCM_RearFogLmpFailSts:1;	
		U32		BCM_BacklightDutyCycle:7;	
		U32		BCM_RLSFailSts:2;
		U32		BCM_ParkBrakeSts:1;	
		U32		BCM_ParkBrakeFailSts:2;	
		U32		BCM_ReversGearSts:2;
		U32		BCM_HoodSts:1;		
		U32		BCM_ReversGearFailSts:2;	
		U32		BCM_LFTireIndSts:2;	
		U32		BCM_RFTireIndSts:2;	
		U32		BCM_LRTireIndSts:2;
		U32		BCM_RRTireIndSts:2;	
		U32		BCM_TPMSFailureInd:2;	
		U32		BCM_TPWarningInd:2;
		U32		BCM_TPFastLeakageInd:2;		
	}id_311;
	struct 
	{
		/* b63 -b0*/
		U32		ECM_FuelCutOffActive:1;	
		U32		ECM_AccEffPosSts:1;
		U32		ECM_AccActualPosSts:1;	
		U32		Reserved1:1;
		U32		Reserved0:1;	
		U32		ECM_EngineSVSSignal:1;
		U32		Reserverd2:2;	
		U32		ECM_CAN_N_IDL:16;	
		U32		ECM_EngCoolantTemp:8;	
		U32		ECM_AccActualPos:8;	
		U32		ECM_TgtCruiseVehSpd:8;
		U32		Reserverd3:1;
		U32		ECM_EngCoolantTempValid:1;	
		U32		Reserverd4:14;	
	}id_371;
	struct 
	{
		/* b63 -b0*/
		U32		ABM_PsngrSeatBeltSts:1;	
		U32		ABM_DrSeatBeltSts:1;
		U32		ABM_AirBagFault:1;	
		U32   Reserved1:5;
		U32   Reserved2:24;
		U32   Reserved3:32;
	}id_351;
	struct{
		/*63-0*/
		U32 RearRadarLeftSensorSegNum:	3;
		U32 RearRadarLeftSensorSegFlash:	1;
		U32 RearRadarLeftMidSensorSegNum:	3;
		U32 RearRadarLeftMidSensorSegFlash:	1;
		U32 RearRadarRightMidSensorSegNum:	3;
		U32 RearRadarRightMidSensorSegFlash:	1;
		U32 RearRadarRightSensorSegNum:	3;
		U32 RearRadarRightSensorSegFlash:	1;
		U32 RearRadarDiag:	1;
		U32 RearRadarWarning:	1;
		U32 RearRadarsoundlevel:	3;
		U32 RearRadarReserved1:2;
		U32 RadarFailure:	1;
		U32 FrontRadarLeftSensorSegNum:	3;
		U32 FrontRadarLeftSensorSegFlash:	1;
		U32 FrontRadarLeftMidSensorSegNum:	3;
		U32 FrontRadarLeftMidSensorSegFlash:	1;
		U32 FrontRadarRightMidSensorSegNum:	3;
		U32 FrontRadarRightMidSensorSegFlash:	1;
		U32 FrontRadarRightSensorSegNum:	3;
		U32 FrontRadarRightSensorSegFlash:	1;
		U32 FrontRadarDiag:	1;
		U32 FrontRadarWarning:	1;
		U32 FrontRadarsoundlevel:	3;
		U32 FrontRadarReserved:	3;
		U32 RearRadarReserved2:16;
	}id_3df;
	struct
	{
		U32 Reserved1						: 7;
		U32 VehTotalMileageValid:	1;
		U32 VehTotalMileage			: 24;
		U32 Reserved2						: 32;
	}id_345;
	struct
	{
		//b63-b0
		U32 gear			:8;/* first byte */
		U32 reserved	    :8; 
		U32 SnowModeLED		:3; 
		U32 reserved2	    :13; 
		U32 reserved3	    :32;
	}id_221;
	struct
	{
		U8 config[8];
	}id_7a6;
	struct
	{
		U32 reserved1;
		U32 reserved2;
	}id_116;
	
	struct
	{
		U32 reserved1:24;
		U32 reserved2:8;
		U32 reserved3:8;
		U32 reserved4:3;
		U32 ESP_ERR:1;
		U32 reserved5:1;
		U32 reserved6:1;
		U32 reserved7:1;
		U32 reserved8:1;
		U32 reserved9:8;	
		U32 reserved10:1;																
		U32 ESP_OFF		:	1;
		U32 ESP_NML	:	1;
		U32 reserved11				:	1;
		U32 reserved12	:	4;

	}id_211;

	struct  
	{	
		/* b63 -b0*/
		U32   Reserved1 : 24;
		U32   HDC       : 2;
		U32   reserved2	: 2;
		U32   HDC_ERROR	: 1;
		U32	  Reserved3 : 3;
		U32	  Reserved4 : 32;	
	}id_242;

	struct  
	{	
		/* b63 -b0*/
		U32   Reserved1 		: 18;
		U32   PEPS_TurnOFFpwr	: 1;
		U32   PEPS_TakeAwayKey	: 1;
		U32   PEPS_CheckNoKey	: 1;
		U32   reserved2			: 11;                              
		
		U32	  PEPS_TurnGearPN	: 1;
		U32	  Reserved3 		: 3;
		U32	  PEPS_ERROR 		: 1;	
		U32	  Reserved4 		: 7;	
		U32	  PEPS_ERROR1 		: 1;	
		U32	  Reserved5 		: 19;	
	}id_0A5;

	struct
	{
		U32 ALS			: 1;
		U32 Reserved1	: 7;
		U32 Reserved2	: 24;
		U32 Reserved3	: 32;
	}id_365;

	struct
	{
		U32 CarACstate	: 8; /* 空调开关 0x10-on; 0x90-吹前档玻璃 */
		U32 WindSpeed	: 8; /* 风量级别 1-8级:0x10-0x80 */
		U32 outCarTemp	: 8;
		U32 inCarTemp	: 8;
		U32 Reserved1   : 32;
	}id_08B;

	struct /*另一块中控屏*/
	{
		U32 CarACstate	: 8; /* 前除霜 0x90-on; 0x40-后除霜 */
		U32 WindSpeed	: 8; /* 风量级别 1-8级:0x10-0x80；空调开关 0x40-on */
		U32 outCarTemp	: 8;
		U32 inCarTemp	: 8;
		U32 Reserved1   : 32;
	}id_485;
} car3_can_signal_t;
typedef  struct {//HAVAL_2011_2P0T_MT_2WD
	 struct  
	{
		/* b63 -b0 */
		U32		ABS_WssRLEdgesSum:16;
		U32		ABS_WssRREdgesSum:16;	
			
		U32		ABS_ABSFault:1;
		U32		ABS_EBDFault:1;	
		U32		ABS_VehicleSpeedValidSignal:1;
		U32		ABS_VehicleSpeed:13;
		U32   ABS_ABSControlActive:1;
		U32   Reserved:15;
	}id_265;
	 struct 
	{
		/* b63 -b0*/
		U32 	ECM_Fuelconsumption:16;
		U32		Reserved1:8;
		U32		ECM_Fuelconsumption_H:8;
		U32		ECM_Fuelconsumption_L:8;
		U32		ECM_ThrtlCmdPos:8;
		U32   ECM_CruiseCtrlAct:2;
		U32		Reserved2:1;
		U32		ECM_CAN_BRK_SW:1;
		U32		ECM_ThrtlCmdPosFail:1;		
		U32		ECM_BaroPressureFail:1;
		U32		ECM_CAN_BRK_LAMP:1;	
		U32		ECM_ECMImmoAuthRes:1;
		U32		ECM_MILSts:1;	
		U32		ECM_OBDWarmUp:1;	
		U32		Reserved3:1;
		U32		ECM_EngStatus:2;
		U32		Reserved4:2;
		U32		ECM_EngineSVSSignal:1;
	}id_271;
	 struct 
	{
		/* b63 -b0*/
		U32		BCM_LFTirePressure:8;	
		U32		BCM_RFTirePressure:8;
		U32		BCM_LeftFrontTireTemp:8;	
		U32		BCM_RightFrontTireTemp:8;	
		U32		BCM_LRTirePressure:8;
		U32		BCM_RRTirePressure:8;		
		U32		BCM_LeftRearTireTemp:8;	
		U32		BCM_RightRearTireTemp:8;	
	}id_095;
	struct 
	{
		/* b63 -b0*/
		U32		BCM_PosLmpSts:1;	
		U32		BCM_LHPosLmpFailSts:1;
		U32		BCM_RHPosLmpFailSts:1;	
		U32		BCM_LicPlateLmpFailSts:1;	
		U32		BCM_DRLSts:2;
		U32		BCM_DRLFailSts:2;		
		U32		BCM_FrontFogLmpSts:2;	
		U32		BCM_RearFogLmpSts:1;
		U32		BCM_HighBeamSts:1;
		U32		BCM_LowBeamSts:1;
		U32		BCM_LeftTurnSignalCmd:1;	
		U32		BCM_RightTurnSignalCmd:1;	
		U32		BCM_BrakeLmpFailSts:1;
		U32		BCM_RearLidSts:1;		
		U32		BCM_DriverDoorSts:1;	
		U32		BCM_LRDoorSts:1;	
		U32		BCM_PassengerDoorSts:1;	
		U32		BCM_RRDoorSts:1;
		U32		BCM_KeyInsertionSts:1;	
		U32		BCM_WindShieldWiperAct:1;	
		U32		BCM_BrakePedSts:1;
		U32		BCM_BrakePedFailSts:1;		
		U32		BCM_BattVolt:7;	
		U32		BCM_RearFogLmpFailSts:1;	
		U32		BCM_BacklightDutyCycle:7;	
		U32		BCM_RLSFailSts:2;
		U32		BCM_ParkBrakeSts:1;	
		U32		BCM_ParkBrakeFailSts:2;	
		U32		BCM_ReversGearSts:2;
		U32		BCM_HoodSts:1;		
		U32		BCM_ReversGearFailSts:2;	
		U32		BCM_LFTireIndSts:2;	
		U32		BCM_RFTireIndSts:2;	
		U32		BCM_LRTireIndSts:2;
		U32		BCM_RRTireIndSts:2;	
		U32		BCM_TPMSFailureInd:2;	
		U32		BCM_TPWarningInd:2;
		U32		BCM_TPFastLeakageInd:2;		
	}id_311;
	struct 
	{
		/* b63 -b0*/
		U32		ABM_PsngrSeatBeltSts:1;	
		U32		ABM_DrSeatBeltSts:1;
		U32		ABM_AirBagFault:1;	
		U32   Reserved1:5;
		U32   Reserved2:24;
		U32   Reserved3:32;
	}id_351;
	struct
	{
		U32 Reserved1						: 7;
		U32 VehTotalMileageValid:	1;
		U32 VehTotalMileage			: 24;
		U32 Reserved2						: 32;
	}id_345;
	struct
	{
		U8 config[8];
	}id_7a6;
	
	struct
	{
		U32 reserved1:24;
		U32 reserved2:8;
		U32 reserved3:8;
		U32 reserved4:3;
		U32 ESP_ERR:1;
		U32 reserved5:1;
		U32 reserved6:1;
		U32 reserved7:1;
		U32 reserved8:1;
		U32 reserved9:8;	
		U32 reserved10:1;																
		U32 ESP_OFF		:	1;
		U32 ESP_NML	:	1;
		U32 reserved11				:	1;
		U32 reserved12	:	4;
	}id_211;

	struct
	{
		U32 CarACstate	: 8; /* 空调开关 0x10-on; 0x90-吹前档玻璃 */
		U32 WindSpeed	: 8; /* 风量级别 1-8级:0x10-0x80 */
		U32 outCarTemp	: 8;
		U32 inCarTemp	: 8;
		U32 Reserved1   : 32;
	}id_08B;

	struct /*另一块中控屏*/
	{
		U32 CarACstate	: 8; /* 前除霜 0x90-on; 0x40-后除霜 */
		U32 WindSpeed	: 8; /* 风量级别 1-8级:0x10-0x80；空调开关 0x40-on */
		U32 outCarTemp	: 8;
		U32 inCarTemp	: 8;
		U32 Reserved1   : 32;
	}id_485;
} car4_can_signal_t;
typedef  struct {//HAVAL_2013_1P5T_MT_4WD
	 struct  
	{
		/* b63 -b0 */
		U32		ABS_WssRLEdgesSum:16;
		U32		ABS_WssRREdgesSum:16;	
			
		U32		ABS_ABSFault:1;
		U32		ABS_EBDFault:1;	
		U32		ABS_VehicleSpeedValidSignal:1;
		U32		ABS_VehicleSpeed:13;
		U32   ABS_ABSControlActive:1;
		U32   Reserved:15;
	}id_265;
	struct  
	{	
		/* b63 -b0*/
		U32   Reserved1: 32;
		U32		Reserved2:	8;
		U32   ECM_EngSpd:16;
		U32		Reserved3:8;	
	}id_111;
	 struct 
	{
		/* b63 -b0*/
		U32 	ECM_Fuelconsumption:16;
		U32		Reserved1:8;
		U32		ECM_Fuelconsumption_H:8;
		U32		ECM_Fuelconsumption_L:8;
		U32		ECM_ThrtlCmdPos:8;
		U32   ECM_CruiseCtrlAct:2;
		U32		Reserved2:1;
		U32		ECM_CAN_BRK_SW:1;
		U32		ECM_ThrtlCmdPosFail:1;		
		U32		ECM_BaroPressureFail:1;
		U32		ECM_CAN_BRK_LAMP:1;	
		U32		ECM_ECMImmoAuthRes:1;
		U32		ECM_MILSts:1;	
		U32		ECM_OBDWarmUp:1;	
		U32		Reserved3:1;
		U32		ECM_EngStatus:2;	
		U32		Reserved4:2;
		U32		ECM_EngineSVSSignal:1;
	}id_271;
	 struct 
	{
		/* b63 -b0*/
		U32		BCM_LFTirePressure:8;	
		U32		BCM_RFTirePressure:8;
		U32		BCM_LeftFrontTireTemp:8;	
		U32		BCM_RightFrontTireTemp:8;	
		U32		BCM_LRTirePressure:8;
		U32		BCM_RRTirePressure:8;		
		U32		BCM_LeftRearTireTemp:8;	
		U32		BCM_RightRearTireTemp:8;	
	}id_095;
	struct 
	{
		/* b63 -b0*/
		U32		BCM_PosLmpSts:1;	
		U32		BCM_LHPosLmpFailSts:1;
		U32		BCM_RHPosLmpFailSts:1;	
		U32		BCM_LicPlateLmpFailSts:1;	
		U32		BCM_DRLSts:2;
		U32		BCM_DRLFailSts:2;		
		U32		BCM_FrontFogLmpSts:2;	
		U32		BCM_RearFogLmpSts:1;
		U32		BCM_HighBeamSts:1;
		U32		BCM_LowBeamSts:1;
		U32		BCM_LeftTurnSignalCmd:1;	
		U32		BCM_RightTurnSignalCmd:1;	
		U32		BCM_BrakeLmpFailSts:1;
		U32		BCM_RearLidSts:1;		
		U32		BCM_DriverDoorSts:1;	
		U32		BCM_LRDoorSts:1;	
		U32		BCM_PassengerDoorSts:1;	
		U32		BCM_RRDoorSts:1;
		U32		BCM_KeyInsertionSts:1;	
		U32		BCM_WindShieldWiperAct:1;	
		U32		BCM_BrakePedSts:1;
		U32		BCM_BrakePedFailSts:1;		
		U32		BCM_BattVolt:7;	
		U32		BCM_RearFogLmpFailSts:1;	
		U32		BCM_BacklightDutyCycle:7;	
		U32		BCM_RLSFailSts:2;
		U32		BCM_ParkBrakeSts:1;	
		U32		BCM_ParkBrakeFailSts:2;	
		U32		BCM_ReversGearSts:2;
		U32		BCM_HoodSts:1;		
		U32		BCM_ReversGearFailSts:2;	
		U32		BCM_LFTireIndSts:2;	
		U32		BCM_RFTireIndSts:2;	
		U32		BCM_LRTireIndSts:2;
		U32		BCM_RRTireIndSts:2;	
		U32		BCM_TPMSFailureInd:2;	
		U32		BCM_TPWarningInd:2;
		U32		BCM_TPFastLeakageInd:2;		
	}id_311;
	struct 
	{
		/* b63 -b0*/
		U32		ECM_FuelCutOffActive:1;	
		U32		ECM_AccEffPosSts:1;
		U32		ECM_AccActualPosSts:1;	
		U32		Reserved1:1;
		U32		Reserved0:1;	
		U32		ECM_EngineSVSSignal:1;
		U32		Reserverd2:2;	
		U32		ECM_CAN_N_IDL:16;	
		U32		ECM_EngCoolantTemp:8;	
		U32		ECM_AccActualPos:8;	
		U32		ECM_TgtCruiseVehSpd:8;
		U32		Reserverd3:16;	
	}id_371;
	struct 
	{
		/* b63 -b0*/
		U32		ABM_PsngrSeatBeltSts:1;	
		U32		ABM_DrSeatBeltSts:1;
		U32		ABM_AirBagFault:1;	
		U32   Reserved1:5;
		U32   Reserved2:24;
		U32   Reserved3:32;
	}id_351;
	
	struct{
		/*63-0*/
		U32 RearRadarLeftSensorSegNum:	3;
		U32 RearRadarLeftSensorSegFlash:	1;
		U32 RearRadarLeftMidSensorSegNum:	3;
		U32 RearRadarLeftMidSensorSegFlash:	1;
		U32 RearRadarRightMidSensorSegNum:	3;
		U32 RearRadarRightMidSensorSegFlash:	1;
		U32 RearRadarRightSensorSegNum:	3;
		U32 RearRadarRightSensorSegFlash:	1;
		U32 RearRadarDiag:	1;
		U32 RearRadarWarning:	1;
		U32 RearRadarsoundlevel:	3;
		U32 RearRadarReserved1:2;
		U32 RadarFailure:	1;
		U32 FrontRadarLeftSensorSegNum:	3;
		U32 FrontRadarLeftSensorSegFlash:	1;
		U32 FrontRadarLeftMidSensorSegNum:	3;
		U32 FrontRadarLeftMidSensorSegFlash:	1;
		U32 FrontRadarRightMidSensorSegNum:	3;
		U32 FrontRadarRightMidSensorSegFlash:	1;
		U32 FrontRadarRightSensorSegNum:	3;
		U32 FrontRadarRightSensorSegFlash:	1;
		U32 FrontRadarDiag:	1;
		U32 FrontRadarWarning:	1;
		U32 FrontRadarsoundlevel:	3;
		U32 FrontRadarReserved:	3;
		U32 RearRadarReserved2:16;
	}id_3df;
	struct
	{
		U32 Reserved1						: 7;
		U32 VehTotalMileageValid:	1;
		U32 VehTotalMileage			: 24;
		U32 Reserved2						: 32;
	}id_345;
	struct
	{
		U8 config[8];
	}id_7a6;
	struct
	{
		U32 reserved1;
		U32 reserved2;
	}id_251;
	struct
	{
		U32 reserved1:24;
		U32 reserved2:8;
		U32 reserved3:8;
		U32 reserved4:3;
		U32 ESP_ERR:1;
		U32 reserved5:1;
		U32 reserved6:1;
		U32 reserved7:1;
		U32 reserved8:1;
		U32 reserved9:8;	
		U32 reserved10:1;																
		U32 ESP_OFF		:	1;
		U32 ESP_NML	:	1;
		U32 reserved11				:	1;
		U32 reserved12	:	4;
	}id_211;
	struct
	{
		U32 ALS			: 1;
		U32 Reserved1	: 7;
		U32 Reserved2	: 24;
		U32 Reserved3	: 32;
	}id_365;

	struct  
	{	
		/* b63 -b0*/
		U32   Reserved1 		: 18;
		U32   PEPS_TurnOFFpwr	: 1;
		U32   PEPS_TakeAwayKey	: 1;
		U32   PEPS_CheckNoKey	: 1;
		U32   reserved2			: 11;
		U32	  PEPS_TurnGearPN	: 1;
		U32	  Reserved3 		: 3;
		U32	  PEPS_ERROR 		: 1;	
		U32	  Reserved4 		: 7;	
		U32	  PEPS_ERROR1 		: 1;	
		U32	  Reserved5 		: 19;	
	}id_0A5;

	struct
	{
		U32 CarACstate	: 8; /* 空调开关 0x10-on; 0x90-吹前档玻璃 */
		U32 WindSpeed	: 8; /* 风量级别 1-8级:0x10-0x80 */
		U32 outCarTemp	: 8;
		U32 inCarTemp	: 8;
		U32 Reserved1   : 32;
	}id_08B;

	struct /*另一块中控屏*/
	{
		U32 CarACstate	: 8; /* 前除霜 0x90-on; 0x40-后除霜 */
		U32 WindSpeed	: 8; /* 风量级别 1-8级:0x10-0x80；空调开关 0x40-on */
		U32 outCarTemp	: 8;
		U32 inCarTemp	: 8;
		U32 Reserved1   : 32;
	}id_485;
}car5_can_signal_t;
/*
 typedef struct 
{
	U8 USD_data[8];	
}UDS_id_766;
*/


/*******************************************************************************/
typedef void(* pCanAnalyse)(can_msg_t *msg,can_pro_way_e way);


void app_init_can(void);

void app_can_sent_task(void);

void app_can_lost_time_cnt_100ms(void);

extern car0_can_signal_t can0;
extern car1_can_signal_t can1;
extern car2_can_signal_t can2;
extern car3_can_signal_t can3;
extern car4_can_signal_t can4;
extern car5_can_signal_t can5;

extern U16 can_id_095_lost_timecnt;
extern U16 can_id_111_lost_timecnt;
extern U16 can_id_131_lost_timecnt;
extern U16 can_id_265_lost_timecnt;
extern U16 can_id_271_lost_timecnt;
extern U16 can_id_311_lost_timecnt;
extern U16 can_id_351_lost_timecnt;
extern U16 can_id_371_lost_timecnt;
extern U16 can_id_7E7_lost_timecnt;
extern U16 can_id_3df_lost_timecnt;
extern U16 can_id_251_lost_timecnt;
extern U16 can_id_116_lost_timecnt;
extern U16 can_id_211_lost_timecnt;
extern U16 can_id_242_lost_timecnt;
extern U16 can_id_0A5_lost_timecnt;
extern U16 can_id_365_lost_timecnt;
extern U16 can_id_08B_lost_timecnt;
extern U16 can_id_485_lost_timecnt;



#endif
