#ifndef APP_INFO_H
#define APP_INFO_H

#include "comm_typedef.h"

#define MCU_IN1     PDDR02_P0
#define MCU_IN2     PDDR02_P1
#define MCU_IN3     PDDR02_P2
#define MCU_IN4     PDDR02_P3
#define MCU_IN5     PDDR02_P5
#define MCU_IN6     PDDR02_P6
#define MCU_IN7     PDDR02_P7
#define MCU_IN8     PDDR03_P0

#define MCU_IN9      PDDR03_P1
#define MCU_IN10     PDDR03_P2
#define MCU_IN11     PDDR03_P3
#define MCU_IN12     PDDR03_P4
#define MCU_IN13     PDDR03_P5
#define MCU_IN14     PDDR03_P6
#define MCU_IN15     PDDR03_P7
#define MCU_IN16     PDDR15_P0

#define MCU_IN17     PDDR15_P1
#define MCU_IN18     PDDR04_P0
#define MCU_IN19     PDDR04_P1
#define MCU_IN20     PDDR04_P3
#define MCU_IN21     PDDR04_P4
#define MCU_IN22     PDDR04_P5

#define MCU_IN23     PDDR04_P6

#define MCU_IN24     PDDR04_P7

#define MCU_IN25     PDDR05_P0
#define MCU_IN26     PDDR05_P1
#define MCU_IN27     PDDR05_P2
#define MCU_IN28     PDDR05_P3
#define MCU_IN29     PDDR05_P4
#define MCU_IN30     PDDR05_P5
#define MCU_IN31     PDDR05_P6

#define MCU_IN32     PDDR05_P7

//#define MCU_IN1     (pin_filter_in[PIN_IN_01].result)
//#define MCU_IN2     (pin_filter_in[PIN_IN_02].result)
//#define MCU_IN3     (pin_filter_in[PIN_IN_03].result)
//#define MCU_IN4     (pin_filter_in[PIN_IN_04].result)
//#define MCU_IN5     (pin_filter_in[PIN_IN_05].result)
//#define MCU_IN6     (pin_filter_in[PIN_IN_06].result)
//#define MCU_IN7     (pin_filter_in[PIN_IN_07].result)
//#define MCU_IN8     (pin_filter_in[PIN_IN_08].result)

//#define MCU_IN9      (pin_filter_in[PIN_IN_09].result)
//#define MCU_IN10     (pin_filter_in[PIN_IN_10].result)
//#define MCU_IN11     (pin_filter_in[PIN_IN_11].result)
//#define MCU_IN12     (pin_filter_in[PIN_IN_12].result)
//#define MCU_IN13     (pin_filter_in[PIN_IN_13].result)
//#define MCU_IN14     (pin_filter_in[PIN_IN_14].result)
//#define MCU_IN15     (pin_filter_in[PIN_IN_15].result)
//#define MCU_IN16     (pin_filter_in[PIN_IN_16].result)

//#define MCU_IN17     (pin_filter_in[PIN_IN_17].result)
//#define MCU_IN18     (pin_filter_in[PIN_IN_18].result)
//#define MCU_IN19     (pin_filter_in[PIN_IN_19].result)
//#define MCU_IN20     (pin_filter_in[PIN_IN_20].result)
//#define MCU_IN21     (pin_filter_in[PIN_IN_21].result)
//#define MCU_IN22     (pin_filter_in[PIN_IN_22].result)

//#define MCU_IN23     (pin_filter_in[PIN_IN_23].result)

//#define MCU_IN24     (pin_filter_in[PIN_IN_24].result)

//#define MCU_IN25     (pin_filter_in[PIN_IN_25].result)
//#define MCU_IN26     (pin_filter_in[PIN_IN_26].result)
//#define MCU_IN27     (pin_filter_in[PIN_IN_27].result)
//#define MCU_IN28     (pin_filter_in[PIN_IN_28].result)
//#define MCU_IN29     (pin_filter_in[PIN_IN_29].result)
//#define MCU_IN30     (pin_filter_in[PIN_IN_30].result)
//#define MCU_IN31     (pin_filter_in[PIN_IN_31].result)

//#define MCU_IN32     (pin_filter_in[PIN_IN_32].result)

extern U16 single_miles;
extern U32 total_miles;
extern U8  key_info_value;
extern U8  CurrentMenu;




typedef struct
{
	U8 :6;
	U8 time_set_enable:1;		/* clear trip */
	U8 u8TripClear:1;		/* clear trip */
	//U8 time_set_enable:1;		/* clear trip */
	//U8 :6;                  /* 预留 */
	U8  u8MenuNum;        /* 菜单信息请求 */
	U8  u8BattBoxNum;      /* 电池箱号 */
	U32 u32UTCTime;        /* UTC时间设置 */
}FRAMEA20TOMCU_t;


typedef enum
{
	//MENU_L2_NULL = 1,   /* NULL */
	MENU_CONTROLSYSTEM = 1,   /* 控制系统信息 */
	MENU_MOTORSYSTEM,   /* 发动机系统信息界面 */
	MENU_AUXILIARYSYSTEM, /* 辅助系统信息界面 */
	MENU_TCUSYSTEM,   /* tcu系统信息界面 */
	MENU_BATTERYMANAGESYSTEM,   /* 电池管理系统信息界面 */
	MENU_BATTERYSTATE,   /* 电池状态信息 */
	MENU_AIRCONTROLSYSTEM,   /* 空调系统信息界面 */
	MENU_FRONT_MODULE,		/* 前模块 */
	MENU_MIDDLE_MODULE,/* 中模块 */
	MENU_BACK_MODULE,/* 后模块 */
	MENU_MODULE_MAX,

	OnlineCheckCMD = 252,  /* 升级程序相关 */
	ExcuteBootCMD = 250,    /* 升级程序相关 */
	
}SUBINFO_NUM;   /* 当前界面序号 */

extern FRAMEA20TOMCU_t A20_mcu;
extern FRAMEA20TOMCU_t  g_tUart1Rec;


extern void cal_info_task_100ms(void);
extern void app_sub_trip_clear(void);

#endif
