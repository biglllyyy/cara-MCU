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
extern U16 single_miles;
extern U32 total_miles;
extern U8  key_info_value;
extern U8  CurrentMenu;




typedef struct
{
	
	U8 u8TripClear:1;		/* clear trip */
	U8 :7;                  /* 预留 */
	U8  u8MenuNum;        /* 菜单信息请求 */
	U8  u8BattBoxNum;      /* 电池箱号 */
	U32 u32UTCTime;        /* UTC时间设置 */
}FRAMEA20TOMCU_t;


typedef enum
{
	//SUBINFO_WARN = 1,   /* 报警界面 */
	SUBINFO_BMS = 1,   /* 电池管理系统界面信息 */
	SUBINFO_BS,   /* 电池状态界面信息 */
	SUBINFO_BSTEMP, /* 电池温度探头界面信息 */
	SUBINFO_MOTOR,   /* 电机控制器界面信息 */
	SUBINFO_AP,   /* 气泵控制器界面信息 */
	SUBINFO_DMY,   /* 油泵控制器界面信息 */
	SUBINFO_DCDC,   /* DCDC界面信息 */
	SUBINFO_HIGH_PRESSURE,   /* 高压系统界面信息 */
	SUBINFO_FRONT,   /* qian界面信息 */
	
	SUBINFO_MIDDLE,   /* zhong界面信息 */
	
	SUBINFO_BACK,   /* hou界面信息 */
	SUBINFO_ALL
}SUBINFO_NUM;   /* 当前界面序号 */

extern FRAMEA20TOMCU_t A20_mcu;
extern FRAMEA20TOMCU_t  g_tUart1Rec;


extern void cal_info_task_100ms(void);
extern void app_sub_trip_clear(void);

#endif
