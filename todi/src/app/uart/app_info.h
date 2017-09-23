#ifndef APP_INFO_H
#define APP_INFO_H

#include "comm_typedef.h"

#define MCU_IN1      PDDR02_P0	// Կ��ST����
#define MCU_IN2      PDDR02_P1	// 
#define MCU_IN3      PDDR02_P2	// 
#define MCU_IN4      PDDR02_P3	// ��ת�򿪹�
#define MCU_IN5      PDDR02_P5	// ��ת�򿪹�
#define MCU_IN6      PDDR02_P6	// С�ƿ���
#define MCU_IN7      PDDR02_P7	// Զ��ƿ���
#define MCU_IN8      PDDR03_P0	// ����ƿ���
#define MCU_IN9      PDDR03_P1	// ǰ��ƿ���
#define MCU_IN10     PDDR03_P2	// ����ƿ���
#define MCU_IN11     PDDR03_P3	// 
#define MCU_IN12     PDDR03_P4	// �����俪��
#define MCU_IN13     PDDR03_P5	// 
#define MCU_IN14     PDDR03_P6	// ��ε͵�����
#define MCU_IN15     PDDR03_P7	// ��μ�Ъ������
#define MCU_IN16     PDDR15_P0	// ��θߵ�����
#define MCU_IN17     PDDR15_P1	// �����ˮ������
#define MCU_IN18     PDDR04_P0	// 
#define MCU_IN19     PDDR04_P1	// 
#define MCU_IN20     PDDR04_P3	// 
#define MCU_IN21     PDDR04_P4	// 
#define MCU_IN22     PDDR04_P5	// 
#define MCU_IN23     PDDR04_P6	// ·�ƿ���
#define MCU_IN24     PDDR04_P7	// ǰ�ſ�����
#define MCU_IN25     PDDR05_P0	// ǰ�Źؿ���
#define MCU_IN26     PDDR05_P1	// ���ſ�����
#define MCU_IN27     PDDR05_P2	// ���Źؿ���
#define MCU_IN28     PDDR05_P3	// 
#define MCU_IN29     PDDR05_P4	// 
#define MCU_IN30     PDDR05_P5	// 
#define MCU_IN31     PDDR05_P6	//  


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
	//U8 :6;                  /* Ԥ�� */
	U8  u8MenuNum;        /* �˵���Ϣ���� */
	U8  u8BattBoxNum;      /* ������ */
	U32 u32UTCTime;        /* UTCʱ������ */
}FRAMEA20TOMCU_t;


typedef enum
{
	//MENU_L2_NULL = 1,   /* NULL */
	MENU_CONTROLSYSTEM = 1,   /* ����ϵͳ��Ϣ */
	MENU_MOTORSYSTEM,   /* ������ϵͳ��Ϣ���� */
	MENU_AUXILIARYSYSTEM, /* ����ϵͳ��Ϣ���� */
	MENU_TCUSYSTEM,   /* tcuϵͳ��Ϣ���� */
	MENU_BATTERYMANAGESYSTEM,   /* ��ع���ϵͳ��Ϣ���� */
	MENU_BATTERYSTATE,   /* ���״̬��Ϣ */
	MENU_AIRCONTROLSYSTEM,   /* �յ�ϵͳ��Ϣ���� */
	MENU_FRONT_MODULE,		/* ǰģ�� */
	MENU_MIDDLE_MODULE,/* ��ģ�� */
	MENU_BACK_MODULE,/* ��ģ�� */
	MENU_MODULE_MAX,

	OnlineCheckCMD = 252,  /* ����������� */
	ExcuteBootCMD = 250,    /* ����������� */
	
}SUBINFO_NUM;   /* ��ǰ������� */

extern FRAMEA20TOMCU_t A20_mcu;
extern FRAMEA20TOMCU_t  g_tUart1Rec;


extern void cal_info_task_100ms(void);
extern void app_sub_trip_clear(void);

#endif
