#ifndef BUZZER_H_INCLUDE
#define BUZZER_H_INCLUDE
#include "comm_typedef.h"

/* ��������BEEP���� */
#define BEEPCOUNT	32	

#define BUZZER_CHORD_FREQUENCY 40000
#define BUZZER_CHORD_MIDDLE 32768

#define  BUZZER_CHN					0

extern U8 g_turn_beep_flg;

#define buzzerdef(size)                           	\
struct {                                          	\
	S16 priority;		/* ���ȼ��ͱ�� 1,2,3... */\
	U16 frequency;		/* ������Ƶ�� */           \
	S16 cycle_index;	/* ��ѭ������ 0,1,2... */  \
	S16 cycle_total;	/* ��ѭ������ 0,1,2... */	\
	S16 onoff_index;	/* �Ѳ���λ�� 0,1,2... */  \
	S16 onoff_total;	/* �����ݳ��� 2,4,6... */  \
	S16 onoff_data[(size)]; /* ���� on(),off()... */\
	const U8 *tone;		/*����*/						\
}
typedef buzzerdef(2) buzzer;
typedef void(*buz_handle)(U16 action);
#define  on(ms)   (ms)
#define  off(ms)  (ms)
#define  Hz(freq) (freq)

#define  ms2tck(ms) ((ms)/10) /* 10ms a tick*/

static U8 beep_count;
static struct
{
	/******************************************************************************/
	S16 startup;       /* buzzer need to startup?*/
	S16 shutdown;      /* buzzer need to shutdown?*/
	S32 current;       /* current buzzer that beepping*/
	S32 alarm_tck;     /* tck remains to switch*/
	/******************************************************************************/
	U32 priority_bits; /*priority bits vector, msb-low, lsb-high*/
	buzzer *list[BEEPCOUNT];   /*list of all buzzers*/
}g_buzzer;


extern U8 waring1_sts;
extern U8 waring2_sts;

extern void init_buzzer(void);
extern void start_buzzer(U32 priority);
extern void restart_buzzer(U32 priority);
extern void stop_buzzer(U32 priority);
extern void select_buzzer(U32 priority, U16 index);
extern void process_buzzer(void); /* 10ms interval*/
extern void select_buzzer(U32 priority, U16 index);
extern void buz_init_beep_val(short buz_name,buzzer* buz);
extern void buzzer_start_test(U16 frequency,U8 duty);
extern void buzzer_start_fine_test(U16 frequency,U16 duty);
extern void buzzer_start_pwm(void);
extern void buzzer_change_pwm_duty(void);
extern void buzzer_stop_test(void);
extern U8 IsAnyBeepWarnigOn(void);

//�������
//����
#define CHORD_A2    0		//220Hz
#define CHORD_B2    1
#define CHORD_H2    2
#define CHORD_C2    3
#define CHORD_Cis2  4
#define CHORD_D2    5
#define CHORD_Dis2  6
#define CHORD_E2    7
#define CHORD_F2    8
#define CHORD_Fis2  9
#define CHORD_G2    10
#define CHORD_Gis2  11
#define CHORD_A3    12		//440Hz     ��׼C���dou
#define CHORD_B3    13
#define CHORD_H3    14
#define CHORD_C3    15
#define CHORD_Cis3  16
#define CHORD_D3    17
#define CHORD_Dis3  18
#define CHORD_E3    19
#define CHORD_F3    20
#define CHORD_Fis3  21
#define CHORD_G3    22
#define CHORD_Gis3  23
#define CHORD_A4    24		//880Hz
#define CHORD_B4    25
#define CHORD_H4    26
#define CHORD_C4    27
#define CHORD_Cis4  28
#define CHORD_D4    29
#define CHORD_Dis4  30
#define CHORD_E4    31
#define CHORD_F4    32
#define CHORD_Fis4  33
#define CHORD_G4    34
#define CHORD_Gis4  35
#define CHORD_A5    36		//1760Hz
#define CHORD_B5    37
#define CHORD_H5    38
#define CHORD_C5    39
#define CHORD_Cis5  40
#define CHORD_D5    41
#define CHORD_Dis5  42
#define CHORD_E5    43
#define CHORD_F5    44
#define CHORD_Fis5  45
#define CHORD_G5    46
#define CHORD_Gis5  47
#define CHORD_A6    48		//3520Hz
#define CHORD_B6    49
#define CHORD_H6    50
#define CHORD_C6    51
#define CHORD_Cis6  52
#define CHORD_D6    53
#define CHORD_Dis6  54
#define CHORD_E6    55

#define CHORD_EoS   255	//���׽���
#define CHORD_EN    128	//������Ч��־

//chord player initializer
extern void buzzer_chord_player_init(void);

//chord player play
//tone: music which need to be played
//start time: from when to play
//if chord player is already running or there is no tone in the , FALSE will be return
Bool buzzer_chord_player_play(const U8 *tone,U32 start_time);

//chord player stop
extern void buzzer_chord_player_stop(void);

//check if chord player is running
//if chord player is running,TRUE will be return
extern Bool buzzer_chord_player_is_running(void);

//chord player step play
//result:PWM ratio transfer to PPG
//
extern void buzzer_chord_player_play_one_step(void);

#endif /* BUZZER_H_INCLUDE*/
