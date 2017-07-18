#ifndef APP_UART_CMD_H
#define APP_UART_CMD_H

#include "comm_typedef.h"
#include "uart_queue.h"

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
		U32			reserved: 1;
		U32		mode_choice:2;
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
		U32		reserve:4;
		U32		themeID:4;
		
		U32		remainMileage:16;

		U32		trip1:24;
		U32	    fuel:8;
		/* 32 bit */
		U32		trip2:24;
		U32		soc:8;

		U32		maintenanceMileage:16;
		U32     outCarTemprature:16;

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

typedef struct
{
	/* 硬件版本号 */
	union{
		U16 val;
		struct{
			U16 	mainVer: 3;
			U16 	releaseVer: 5;
			U16 	betaVer: 8;
		}bit;
	}first;
} HW_version;

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
			U8 HDC_ERROR:1;
			U8 HDC:2;/*陡坡缓降*/
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
			U8 TpmsWarning:1;	/* 胎压报警 */
			U8 HeadlampAutoLightError:1;
			U8 CruiceControlSystemMain:1;
			U8 EngineMaintenancee:1; /* 发动机维修报警 */
			U8 OilPressWarning:1; /* 机油报警 */
			U8 ABSError:1;	/* ABS 报警*/
			U8 SeatbeltsNotFastened:1; /* 安全带未系报警*/
			U8 SRSError:1;	/* 气囊报警 */
		}bit;	
	}first;
	union{
		uint8_t val;
		struct{
//			U8 ESP_OFF:1;
			U8 ESP:2;
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
			U8 reserve		:1;
			U8 key2_long	:1;
			U8 key1_long	:1;
			U8 key0_long	:1;		
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
	union
	{
		U8 val;
		struct
		{
			U8 PEPSReversed 	: 2;
			U8 TurnOFFpwr		: 1;
			U8 TakeAwayKey		: 1;
			U8 KeyLowBattery	: 1;
			U8 PEPS_CheckNoKey	: 1;
			U8 PEPS_TurnGearPN	: 1;
			U8 PEPS_ERROR 		: 1;
		}bit;
	}first;
}PEPSInfo;

typedef struct
{
	union
	{
		U8 val;
		struct
		{
			U8 Reversed2 		: 3;
			U8 ALS 				: 1;
			U8 Reversed1 		: 3;
			U8 SnowModeLED		: 1;
		}bit;
	}first;
}SpicalLEDInfo;



typedef struct
{
	HW_version				hw_ver;
	RadarInfo				Radar;	//倒车雷达
	LedWarningInfo			LedWarning;			//LED报警灯
	DrivingTimeInfo   		DrivingTime;//行驶时间
	TpmsInfo				Tpms;			//TPMS胎压监测
	TtmsInfo				Ttms;//胎温监测
	MainDispInfo			MainDisp;//主界面显示内容配置
	CarNumInfo				CarNum;//车型编号
	PEPSInfo				Peps;
	SpicalLEDInfo			SpecialLED;

	U16 this_trip_distance;
	U16 this_trip_time;
	U8  oil_precent;
	U8  ad_saved;
	U8  temp;
	U8  water_temp;
	U32 total_used_fuel;
	U16 this_trip_used_fuel;
	U16 this_trip_AFE;
	
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
 * setting information
 */
typedef struct {
	union{
		uint8_t  val; 
		struct
		{
			uint8_t	engineerMode: 1,
					load_success: 1,
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
	union{
		uint8_t val;
		struct
		{
			uint8_t Reserved:4,
			themeID:4;
		}bits;
	}fourth;
	/* uint32_t	Reserved:8; */
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
			U8	serviceNum:2;  /*保养次数*/
			U8  WriteMainNotDisp:1;
			U8	WriteMainDispMode:1;
			U8	MainNotDisp:2;
			U8	MainDispMode:2;
		}bit;
	}MainDisp;
/*Qt界面来配置车型*/	
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
	}CarNumInfo;

	/*Qt界面来设置保养里程*/
	union{
		uint16_t u16srvMil;
		uint8_t  u8srvMil[2];
	}serviceMileage;

}SettingsSpecialInfo;

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


/*
 * byte length for frame
 */
#define GENERAL_FRAME_LEN	(49) /* data_len + 9 */
#define SPECIAL_FRAME_LEN	(49) /* data_len + 9 */
#define SETTINGS_GENERAL_FRAME_LEN	(17)
#define SETTINGS_SPECIAL_FRAME_LEN	(19)  //uart设置carnum、保养里程 前 16 后19 

#if SETTINGS_GENERAL_FRAME_LEN>=SETTINGS_SPECIAL_FRAME_LEN
#define SETTINGS_FRAME_LEN_MAX (SETTINGS_GENERAL_FRAME_LEN+1)
#else
//#define SETTINGS_FRAME_LEN_MAX (SETTINGS_SPECIAL_FRAME_LEN+1)
#define SETTINGS_FRAME_LEN_MAX (9+1)  //for 206

#endif

extern uint8_t g_u8IgnSts;

extern GeneralFrame	 uart_general_data;
extern SpecialFrame	 uart_special_data;
extern SettingsGeneralFrame mcu_rec_general;
extern SettingsSpecialFrame mcu_rec_special;
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
void app_frame_sent_task(void);	//sent
void app_frame_get_task(void);	//get

U8 app_uart_get_car_num(void);
U8 app_uart_get_car_srv_num(void);
U16 app_uart_get_car_srv_mil(void);
U8 app_uart_get_theme_id(void);
U8 app_uart_get_sound_sync(void);


void uart_set_heartbeat_tick(U32 tk);
/* 检查ARM是否活着 */
void app_uart_arm_alive(void);
/* 获取是否ARM发给MCU的第一帧 */
unsigned char get_uart_first_frame(void);
/* 获取是否收到的ARM发给MCU的关屏请求 */
unsigned char get_uart_close_lcd_req(void);
/* 获取是否超速报警 */
unsigned char get_uart_Overspeed(void);



//add for 206
#define    MAXMENUNUM  8
#define    A20_MCU_DATA_LENTH			7//这里是设置队列buffer缓存，必须设置大于所需字节

#define   A20_MCU_DATA_BUF_LENGTH      (A20_MCU_DATA_LENTH+7)

/*add for zhongkun 206*/
typedef enum
{
	NULL_TYPE,			//00, NULL
	CAR_INFO_TYPE,		//01，通用信息帧；
	WARNING_TYPE,               //02, 报警信息帧;
	MENU_BATMANAGE_TYPE,	//03，电池管理系统界面；
	MENU_BATSTATUS_TYPE,	//04，电池状态信息界面；
	MENU_BATTEMPERATURE_TYPE,   //05，电池状态信息界面；
	MENU_MOTOR_TYPE,		//06，电机控制器信息界面；
	MENU_AIRPUMP_TYPE,		//07，气泵控制器信息界面；
	MENU_OILPUMP_TYPE,		//08，油泵控制器信息界面；
	MENU_DCDC_TYPE,		//09，DC-DC电源信息界面；
	MENU_HIGH_PRESSURE, //10, 高压系统信息
	MENU_FRONT_MOUDLE,	//11，仪表诊断信息界面
	MENU_MIDDLE_MOUDLE,	//12，仪表诊断信息界面
	MENU_BACK_MOUDLE,	//13，仪表诊断信息界面
	
} FRAME_TYPES;

void app_farme_sent_task(void);
void app_frame_get_task20(void);
void app_uart_data_init(void);
void uart_data_parse(UartQueue *p);
U8   app_frame_sent_sub(void);
void app_main_farme_sent_task(void);


#endif // APP_UART_CMD_H
