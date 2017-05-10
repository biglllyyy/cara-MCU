#ifndef APP_UART_CMD_H
#define APP_UART_CMD_H

#include "comm_typedef.h"
#define default_value 0

#define GOLDEN_RATIO_PRIME_32  (0x9e370001UL)
#define STR4_HASH(a,b,c,d)	    (((a)<<24)|((b)<<16)|((c)<<8)|(d))*GOLDEN_RATIO_PRIME_32


#define    MAGIC_UPDATE_UP          STR4_HASH('U','P','U','P')					
#define    MAGIC_UPDATE_DOWN        STR4_HASH('D','O','U','P')					
#define    MAGIC_GENERAL_UP         STR4_HASH('U','P','G','E')					
#define    MAGIC_GENERAL_DOWN       STR4_HASH('D','O','G','E') 					
#define    MAGIC_SPECIAL_UP         STR4_HASH('U','P','S','P')					
#define    MAGIC_SPECIAL_DOWN       STR4_HASH('D','O','S','P')					
#define    MAGIC_OTHER_UP           STR4_HASH('U','P','O','T')					
#define    MAGIC_OTHER_DOWN         STR4_HASH('D','O','O','T')					

#if      ((MAGIC_UPDATE_UP == MAGIC_UPDATE_DOWN)  || \
         (MAGIC_UPDATE_UP== MAGIC_GENERAL_UP)          || \
         (MAGIC_UPDATE_UP == MAGIC_GENERAL_DOWN)    || \
         (MAGIC_UPDATE_UP == MAGIC_SPECIAL_UP)            ||		\
         (MAGIC_UPDATE_UP == MAGIC_SPECIAL_DOWN)    ||			\
         (MAGIC_UPDATE_UP== MAGIC_OTHER_UP)             ||				\
         (MAGIC_UPDATE_UP == MAGIC_OTHER_DOWN)      ||		\
         (MAGIC_UPDATE_DOWN == MAGIC_GENERAL_UP)    ||			\
         (MAGIC_UPDATE_DOWN == MAGIC_GENERAL_DOWN)  ||			\
         (MAGIC_UPDATE_DOWN == MAGIC_SPECIAL_UP)    ||			\
         (MAGIC_UPDATE_DOWN == MAGIC_SPECIAL_DOWN)  ||			\
         (MAGIC_UPDATE_DOWN == MAGIC_OTHER_UP)      ||			\
         (MAGIC_UPDATE_DOWN == MAGIC_OTHER_DOWN)    ||			\
         (MAGIC_GENERAL_UP == MAGIC_GENERAL_DOWN)   ||			\
         (MAGIC_GENERAL_UP == MAGIC_SPECIAL_UP)     ||			\
         (MAGIC_GENERAL_UP == MAGIC_SPECIAL_DOWN)   ||			\
         (MAGIC_GENERAL_UP == MAGIC_OTHER_UP)       ||			\
         (MAGIC_GENERAL_UP == MAGIC_OTHER_DOWN)     ||			\
         (MAGIC_GENERAL_DOWN == MAGIC_SPECIAL_UP)  ||			\
         (MAGIC_GENERAL_DOWN == MAGIC_SPECIAL_DOWN) ||			\
        (MAGIC_GENERAL_DOWN == MAGIC_OTHER_UP)     ||			\
        (MAGIC_GENERAL_DOWN == MAGIC_OTHER_DOWN)   ||			\
        (MAGIC_SPECIAL_UP == MAGIC_SPECIAL_DOWN)   ||		\
        (MAGIC_SPECIAL_UP == MAGIC_OTHER_UP)     ||			\
       (MAGIC_SPECIAL_UP == MAGIC_OTHER_DOWN)     ||		\
       (MAGIC_SPECIAL_DOWN == MAGIC_OTHER_UP)     ||		\
       (MAGIC_SPECIAL_DOWN == MAGIC_OTHER_DOWN)   ||		\
       (MAGIC_OTHER_UP     == MAGIC_OTHER_DOWN))
       	
#error "Magic hash conflicts"		
		
#endif				


/*
 * frame sent GeneralInfo by MCU
 */
typedef struct {
	U32		head:8;
	U32		length:8;
	U32		crc16:16;
	U32		magic;
	struct  
	{	
		U32    	hwVer: 3;
		U32	   	releaseVer: 5;
		U32	   	betaVer: 8;
		U32    	rpm:16;

		U32    	time;

		U32    	speed:8;
		U32    	temprature:8;
		U32   	ign_sts: 1;
		U32    	key2_long: 1;
		U32			key1_long: 1;
		U32			key0_long: 1;
		U32			key3: 1;
		U32			key2: 1;
		U32			key1: 1;
		U32			key0: 1;
		/* b7 	-b0 */
		U32			key_status: 2;	
		U32			gear_mode: 2;	
		U32  		gear: 4;
          	
		/* b7 -b0 */
		U32		:2;
		U32		trunkDoor: 1;
		U32		hoodDoor: 1;
		U32		rightRearDoor: 1;
		U32		leftRearDoor: 1;
		U32		rightFrontDoor: 1;
		U32		leftFrontDoor: 1;			
		U32		odo:24;			/* need to changed order */
		/* 32 bit */
		U32		averageSpeed:8;
		U32		reserve:8;
		
		U32		remainMileage:16;

		U32		trip1:24;
		U32	    fuel:8;
		/* 32 bit */
		U32		trip2:24;
		U32		soc:8;

		U32		maintenanceMileage:16;
		U32     outTemprature:16;

		//msb-lsb in bit field
		struct averageFuelInfo
		{
			U16 unit:1;
			U16 val:15;
		}averageFuel;
		
		struct InstantaneousFuelInfo
		{
			U16 unit:1;
			U16 val:15;
		}InstantaneousFuel;

		U32  	batteryCurrent:16;
		U32  	batteryVoltage:16;	
	}generalInfo;
	U8 tail;
}GeneralFrame;

/*
 * frame sent SpecialInfo by MCU
 */
typedef struct{
	union{
		U8 val;
		struct{
			U8 FrontRadaReversed:2;
			U8 FrontRadarLeftMidSensorWarningLevel:3;
			U8 FrontRadarLeftSensorWarningLevel:3;
		}bit;
	}first;
	union{
		U8 val;
		struct{
			U8 FrontRadaReversed:2;
			U8 FrontRadarRightMidSensorWarningLevel:3;
			U8 FrontRadarRightSensorWarningLevel:3;
		}bit;
	}second;
	union{
		U8 val;
		struct{
			U8 RearRadaReversed:2;
			U8 RearRadarLeftMidSensorWarningLevel:3;
			U8 RearRadarLeftSensorWarningLevel:3;
		}bit;
	}third;
	union{
		U8 val;
		struct{
			U8 RearRadaReversed:2;
			U8 RearRadarRightMidSensorWarningLevel:3;
			U8 RearRadarRightSensorWarningLevel:3;
		}bit;
	}fourth;
	union
	{
		U8 val;
		struct{
			U8 Reserved:3;
			U8 RearRadarWarning:1;
			U8 FrontRadarWarning:1;
			U8 RearRadarDiag:1;
			U8 FrontRadarDiag:1;
			U8 RadarFailure:1;  
		}bit;
	}fifth;
}RadarInfo;

typedef struct{
	union{
		uint8_t val;
		struct{
			U8 TpmsWarning:1;
			U8 HeadlampAutoLightError:1;
			U8 CruiceControlSystemMain:1;
			U8 EngineMaintenancee:1;	
			U8 OilPressWarning:1;			
			U8 ABSError:1;		
			U8 SeatbeltsNotFastened:1;
			U8 SRSError:1;		
		}bit;	
	}first;
	union{
		uint8_t val;
		struct{
			U8 ESP_OFF:1;
			U8 ESP:1;
			U8 ChargeLamp:1;
			U8 BurglarAlarm:1;	
			U8 EngineSecurity:1;			
			U8 WD_4:1;		
			U8 ATMWarning:1;
			U8 TpmsError:1;		
		}bit;	
	}second;
	union{
		uint8_t val;
		struct{
			U8 TestMode:1;
			U8 ClearTrip1:1;
			U8 ClearTrip2:1;
			U8 IPconfig:2;	
			U8 BrakingFault:1;
			U8 CruiceControlSystem:1;
			U8 EngineError:1;
		}bit;
	}third;
}LedWarningInfo;

typedef struct
{
	U8 Hour;
	U8 Minute;
}DrivingTimeInfo;

typedef struct{
	U8 FrontLeftTyre;
	U8 FrontRightTyre;
	U8 RearLeftTyre;
	U8 RearRightTyre;
}TpmsInfo;

typedef struct{
	U8 FrontLeftTyre;
	U8 FrontRightTyre;
	U8 RearLeftTyre;
	U8 RearRightTyre;
}TtmsInfo;

typedef struct
{
	union
	{
		U8 val;
		struct
		{
			U8 reserve	:4;
			U8 NotDisp	:2;
			U8 DispMode	:2;
		}bit;
	}first;
}MainDispInfo;

typedef struct
{
	union
	{
		U8 val;
		struct
		{
			U8 B7:1;
			U8 B6:1;
			U8 B5:1;
			U8 B4:1;
			U8 B3:1;
			U8 B2:1;
			U8 B1:1;
			U8 B0:1;
		}bit;
	}first;
}CarNumInfo;

typedef struct
{
	RadarInfo	Radar;	//倒车雷达
	LedWarningInfo			LedWarning;			//LED报警灯
	DrivingTimeInfo   DrivingTime;//行驶时间
	TpmsInfo						Tpms;			//TPMS胎压监测
	TtmsInfo						Ttms;//胎温监测
	MainDispInfo				MainDisp;//主界面显示内容配置
	CarNumInfo					CarNum;//车型编号
}SpecialInfo;

typedef struct {
	uint8_t		head;
	uint8_t		length;
	uint16_t	crc;
	uint32_t	magic;
	SpecialInfo specialInfo;
	uint8_t		tail;
}SpecialFrame;


/*
 * frame sent UpdateInfo by MCU
 */
typedef struct
{
	uint8_t cmd;
	uint16_t  No;
	uint8_t sts;
}UpdateInfo;

typedef struct {
	uint8_t		head;
	uint8_t		length;
	uint16_t	crc;
	uint32_t	magic;
	UpdateInfo updateInfo;
	uint8_t		tail;
}UpdateFrame;


/*
 * setting information
 */
typedef struct {
	union{
		uint8_t  val; 
		struct
		{
			uint8_t	engineerMode: 1,
					: 1,
					: 1,
					averageFuelClean: 1,
					averageSpdClean: 1,
					trip1Clean: 1,
					closeReq: 1,
					commandReq: 1;						
		}bits;
	}first;
	uint32_t  warningSoundAndInterfaceSync:8;
	uint32_t  SoundAndInterfaceSync:8;	
	uint32_t	Reserved:8;
	union{
		uint32_t u32Time;
		uint8_t u8Time[4];
	}Time;
}SettingsGeneralInfo;

typedef struct {
	union{
		uint8_t  val; 
		struct
		{
			uint8_t	led_selfcheck:1,
											: 2,
					DriveTimeClean:	1,
					CarService  : 1,
					IPconfigInfo: 1,
				  DriveSystem : 1,	
				  trip2Clean: 1;
		}bits;
	}first;
	
	uint8_t  Backlight;
	
	union{
		uint32_t u32TotalTrip;
		uint8_t u8TotalTrip[4];
	}TotalTrip;
	
	union
	{
		U8 val;
		struct
		{
			U8	reserve:2;
			U8  WriteMainNotDisp:1;
			U8	WriteMainDispMode:1;
			U8	MainNotDisp:2;
			U8	MainDispMode:2;
		}bit;
	}MainDisp;
	
}SettingsSpecialInfo;

#define UPDATE_DATE_FIELD_LEN	1024

typedef struct
{
	uint8_t cmd;
	uint16_t No;
	uint8_t date[UPDATE_DATE_FIELD_LEN];
}SettingUpdateInfo;
/*
 * frame core to mcu
 */
typedef struct {
	uint8_t		 head;
	uint8_t		 length;
	uint16_t	 crc;
	uint32_t	 magic;
	SettingsGeneralInfo settingsInfo;
	uint8_t		 tail;
}SettingsGeneralFrame;

typedef struct {
	uint8_t		 head;
	uint8_t		 length;
	uint16_t	 crc;
	uint32_t	 magic;
	SettingsSpecialInfo settingsInfo;
	uint8_t		 tail;
}SettingsSpecialFrame;

typedef struct {
	uint8_t		 head;
	uint16_t		 length;
	uint16_t	 crc;
	uint32_t	 magic;
	SettingUpdateInfo settingsInfo;
	uint8_t		 tail;
}SettingsUpdateFrame;

/*
 * byte length for frame
 */
#define GENERAL_FRAME_LEN	(49)
#define SPECIAL_FRAME_LEN	(29)
#define UPDATE_FRAME_LEN	(13)

#define SETTINGS_GENERAL_FRAME_LEN	(17)
#define SETTINGS_SPECIAL_FRAME_LEN	(16)
#define SETTINGS_UPDATE_FRAME_LEN	(UPDATE_DATE_FIELD_LEN+3+10)

#define SETTINGS_FRAME_LEN_MID ((SETTINGS_GENERAL_FRAME_LEN>=SETTINGS_SPECIAL_FRAME_LEN)? (SETTINGS_GENERAL_FRAME_LEN+1):(SETTINGS_SPECIAL_FRAME_LEN+1))
#define SETTINGS_FRAME_LEN_MAX ((SETTINGS_UPDATE_FRAME_LEN>=SETTINGS_FRAME_LEN_MID)? (SETTINGS_UPDATE_FRAME_LEN+1):(SETTINGS_FRAME_LEN_MID))
//#define SETTINGS_FRAME_LEN_MAX SETTINGS_FRAME_LEN_MID	

extern uint8_t g_u8IgnSts;
extern uint8_t get_new_uart_frame;
extern GeneralFrame	 uart_general_data;
extern SpecialFrame	 uart_special_data;
extern UpdateFrame	 uart_update_data;
extern SettingsGeneralFrame mcu_rec_general;
extern SettingsSpecialFrame mcu_rec_special;
extern SettingsUpdateFrame mcu_rec_update;

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
extern U16 can_id_345_lost_timecnt;
extern U16 can_id_221_lost_timecnt;
extern U16 can_id_7a6_lost_timecnt;

void app_uart_frame_init(void);	 //init
void app_frame_sent_tasks(void);	//sent
void boot_app_frame_get_task(void);	//get
void app_add_uart_task(void);
void app_mcu_core_update(void);

#endif // APP_UART_CMD_H
