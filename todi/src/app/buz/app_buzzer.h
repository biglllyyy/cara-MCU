#ifndef  APP_BUZ_H
#define  APP_BUZ_H

#include "comm_typedef.h"
#include "buzzer_cfg.h"
#include "mid_buzzer.h"

#define SPEED_20KMPH 20
#define REVERSE_GEAR 7

/* ��λ��10ms */
#define _1S					100
#define _2S					200
#define _3S					300
#define _8S					800
#define _23S				2300
#define _1MIN				6000
#define _20MIN			120000UL	

//�����������
//1������������ 
//2��������ά�� 
//3���Զ����������
//4��abs����
//5������ѹ���� 
//6����ȫ����
//7����ȫ�� 
//8������������ 
//9��̥ѹ 
//10������ 
//11��ˮ�� 
//12������ 
#define ENGINE_FAILURE 				1
#define	ENGINE_MAINTAIN 			2
#define ATM_WARNING					3
#define ABS_FAILURE					4
#define OIL_PRESS					5
#define AIRBAG						6		
#define SAFE_SELT					7
#define ENGINE_GUARD				8
#define TPMS						9
#define SPD_120KM					10
#define WATER_TEMP					11
#define FUEL						12	
#define LIDAR						13

#define BEEP_FRT_CHN			 	 0

//����ת�����������
#define CONST_TICK_TIME      			1000//us
#define CONST_TOCK_TIME      			1500//us		
#define CONST_TICKTOCK_FREQ  			800//Hz
#define CONST_TICKTOCK_DUTY  			80//%	
//����120km/h����������
#define CONST_120KMPH_FREQ	 			50000//Hz
#define CONST_CHANGE_DUTY_PERIOD 	10000//us

typedef struct
{
	U32 cycle_timecnt;
	U32 total_timecnt;
	U32 delay_timecnt;
}beep_on_time_T;

typedef enum
{
	NULL_STS,
	TICK_ON,
	TICK_OFF,
	TOCK_ON,
	TOCK_OFF
}beep_turn_sts_e;

typedef enum
{
	TURN_OFF_WARNING,
	TURN_ON_WARNING
}beep_turn_warning_e;

//���������ȼ����
typedef  enum{
	BEEP_OFF = 0,
	BEEP_STARTUP,
	BEEP_FAST_LEAKAGESTATES,
	BEEP_SPD_120KMPH,	/* 120km���ٱ��� 3*/
	BEEP_RADAR_ACTIVE,	/* �״Ｄ��      4*/
	BEEP_RADAR_FAILURE,	/* �״�ʧЧ 	 5*/
	BEEP_RADAR_LEVEL_1,	/* �״�ȼ�1 	 6*/
	BEEP_RADAR_LEVEL_2,	/* �״�ȼ�2 	 7*/
	BEEP_RADAR_LEVEL_3,	/* �״�ȼ�3 	 8*/
	BEEP_RADAR_LEVEL_4, /* �״�ȼ�4 	 9*/
	BEEP_SAFE_SELT_OFF_FIRST,	/* ��ȫ��δϵ1  	10*/
	BEEP_SAFE_SELT_OFF_SECOND,/* ��ȫ��δϵ2 		11*/
	BEEP_KEY_NOT_OUT,	/* Կ��δ�� 				12*/
	BEEP_LIGHT_ON,			/* �ƹ�δ��				13 */
	BEEP_ESP_CLOSED, /* ESP�ر� 					14*/
	BEEP_TURN,		/* ת���(ת������ȼ���Ϊ���)  15*/
	BEEP_LOAD_OK,
	BEEP_BRAKE,	/* ��ɲδ�� 						17*/
	BEEP_DOOR,  /*��ʻ�У�����δ��					18*/
	BEEP_FUEL_EMPTY, /* ȼ�͹��ͱ��� 				19*/
}buz_name_t;

extern void app_buz_init(void);
extern void app_buz_ctl(void);
extern void app_set_buz_sts(buz_name_t buzzer_name,U8 value);
extern void app_beep_turn_ctrl(void);
//extern void app_beep_120kmph_ctrl(void);
extern void app_buz_load_success( void );
extern U16	buz_sts[BEEPCOUNT];

extern void app_buz_play_WAV_Init(void);
extern void app_buz_play_WAV_task(void);
void Set_play_WAV_flag(void);
void Clean_play_WAV_flag(void);



#endif
