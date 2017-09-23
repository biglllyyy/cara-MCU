#ifndef APP_MOUDLE_H
#define APP_MOUDLE_H

#include "comm_typedef.h"
#include "app_can.h"
#include "mb91520.h"

//extern U8 pSpeed; //�ⲿCAN���㳵��

#if 1
typedef struct {
    U32 id;
    U8 data[8];
    U8 len;
    U8 type;
} CAN_MESSAGE;



typedef union
{ 
    U8 BYTES[2];
    U16  WORDS;
    struct 
    { 
        U8 O1:1; 
        U8 O2:1; 
        U8 O3:1; 
        U8 O4:1; 
        U8 O5:1; 
        U8 O6:1; 
        U8 O7:1; 
        U8 O8:1; 
        U8 O9:1; 
        U8 O10:1; 
        U8 O11:1; 
        U8 O12:1; 
        U8 O13:1; 
        U8 O14:1; 
        U8 O15:1; 
        U8  OD:1;
    } BITS; 
} DBCM_POUT; 

typedef union {
    struct {
        U8 kl8 : 1;
        U8 kl7 : 1;
        U8 kl6 : 1;
        U8 kl5 : 1;
        U8 kl4 : 1;
        U8 kl3 : 1;
        U8 kl2 : 1;
        U8 kl1 : 1;

        U8     : 1;
        U8 kl15 : 1;
        U8 kl14 : 1;
        U8 kl13 : 1;
        U8 kl12 : 1;
        U8 kl11 : 1;
        U8 kl10 : 1;
		U8 kl9 : 1;

		
		U8 :6; 
		U8 add :1;
        U8 kh1 :1;
        
             
    } BITS;
    U8 BYTES[3];
} DBCM_KEY;

extern U8 delay_cnt;
extern DATA_BIT gKEY[5];

#define  key_set          gKEY[0].bits.bit1 
#define  pkey_set         gKEY[0].bits.bit2 
#define  bkey_set         gKEY[0].bits.bit3
#define  key_up           gKEY[0].bits.bit4
#define  pkey_up          gKEY[0].bits.bit5  
#define  bkey_up          gKEY[0].bits.bit6
#define  key_AV           gKEY[0].bits.bit7
#define  pkey_AV          gKEY[0].bits.bit8

#define  bkey_AV          gKEY[1].bits.bit1
#define  key_duble        gKEY[1].bits.bit2
#define  F50ms            gKEY[1].bits.bit3
#define  F100ms           gKEY[1].bits.bit4
#define  F500ms           gKEY[1].bits.bit5  
#define  F500ms_LCD       gKEY[1].bits.bit6 //lcd��ʾ����ʱ��
#define  alarm_update_flag  gKEY[1].bits.bit7//������ʾ����ʱ��
//#define  CLR_single_miles gKEY[1].bit7
//#define  save_flag        gKEY[1].bits.bit8

#define  key_long_set     gKEY[2].bits.bit1
#define  M_ON             gKEY[2].bits.bit2  
#define  kFLASH           gKEY[2].bits.bit3
#define  FLASH            gKEY[2].bits.bit4 //ת�����˸��־
#define  FLASH_1s         gKEY[2].bits.bit5 
#define  DM1_CLEAR        gKEY[2].bits.bit6 
#define  R_flag           gKEY[2].bits.bit7
#define  bLCD_RESET       gKEY[2].bits.bit8

#define  F300ms           gKEY[3].bits.bit1 // 2016/10/22��������ʱ��
#define  F400ms           gKEY[3].bits.bit2
#define  F1000ms          gKEY[3].bits.bit3
#define  FLASH_3s         gKEY[3].bits.bit4
#define  time_flag        gKEY[3].bits.bit5
#define  M_ON_flag        gKEY[3].bits.bit6
#define  rLED_flag        gKEY[3].bits.bit7
#define  F5s              gKEY[3].bits.bit8

#define  F10min           gKEY[4].bits.bit1
#define  F_DISP           gKEY[4].bits.bit2
#define  F_20s            gKEY[4].bits.bit3
#define  F_20s_flag       gKEY[4].bits.bit4
#define  F_3              gKEY[4].bits.bit4



DATA_BIT gCTL[8]; //�Ǳ����������Ϣ

extern U8 pSpeed; //�ⲿCAN���㳵��


extern U8 Fversion;
extern U8 Mversion;
extern U8 Rversion;


extern U8 front_moudle_count;
extern U8 middle_moudle_count;
extern U8 back_moudle_count;


extern DBCM_KEY fKEY; //ǰģ�鹦�ʿ���״̬
extern DBCM_KEY mKEY; //�У�����ģ�鹦�ʿ���״̬
extern DBCM_KEY rKEY; //��ģ�鹦�ʿ���״̬

extern U16 fFreq; //ǰģ��Ƶ��
extern U16 mFreq; //�У�����ģ��Ƶ��
extern U16 rFreq; //��ģ��Ƶ��

extern U16 fSpeed; //ǰģ�����㳵��
extern U16 mSpeed; //�У�����ģ�����㳵��
extern U16 rSpeed; //��ģ�����㳵��

extern U16 fADR[2]; //ǰģ�����ֵ
extern U16 mADR[2]; //�У�����ģ�����ֵ
extern U16 rADR[2]; //��ģ�����ֵ

extern DBCM_POUT fPOUT; //ǰģ�鹦�����״̬
extern DBCM_POUT mPOUT; //�У�����ģ�鹦�����״̬
extern DBCM_POUT rPOUT; //��ģ�鹦�����״̬

extern U16 fSingle_miles;
extern U16 mSingle_miles;
extern U16 rSingle_miles;

/*
 ���������
 */
typedef union {

    struct {
        U8 bit0 : 1;
        U8 bit1 : 1;
        U8 bit2 : 1;
        U8 bit3 : 1;
        U8 bit4 : 1;
        U8 bit5 : 1;
        U8 bit6 : 1;
        U8 bit7 : 1;
    }bits;
    U8 byte;
} LED_DATA;

extern LED_DATA alarm_led[4]; //������ȫ�ֱ���
void LED_Out(void);

#define BUZZ  alarm_led[0].bits.bit0 //������ 
#define CS_A  alarm_led[0].bits.bit4 //4051
#define CS_B  alarm_led[0].bits.bit3 
#define CS_C  alarm_led[0].bits.bit2 
#define CS_E  alarm_led[0].bits.bit5 

#define PO1   alarm_led[0].bits.bit6 //�������    
#define PO2   alarm_led[0].bits.bit7 //�������  


#define LED1  0 //ǰ�ſ�ָʾ        
#define LED2  0 //���ſ�ָʾ                         
#define LED3  0 //�յ�ָʾ  
#define LED4  0 //��ת�򱨾�      
#define LED5  0 //ǰ����ָʾ    
#define LED6  0 //�������㱨�� 
#define LED7  0 //���ָʾ  
#define LED8  0 //��ת�򱨾�  
#define LED9  0 //��ѹ�ͱ���
#define LED10 0 //�ܵ�Դָʾ              
#define LED11 0 //��ѹ�ͱ���
#define LED12 0 //SOC����   
#define LED13 0 //������ϱ���            
#define LED14 0 //������ع��ϱ���                 
#define LED15 0 //С�� 

#define LED16 alarm_led[2].bits.bit7 //���س��ָʾ                
#define LED17 alarm_led[3].bits.bit0 //Զ��Ʊ���  
#define LED18 alarm_led[3].bits.bit1 //פ���ƶ�����
#define LED19 alarm_led[3].bits.bit2 //ǰ��Ʊ���
#define LED20 alarm_led[3].bits.bit3 //����Ʊ���
#define LED21 alarm_led[3].bits.bit4 //������
#define LED22 alarm_led[3].bits.bit5 //����Ʊ��� 
#define LED23 alarm_led[3].bits.bit6 //����������               
#define LED24 alarm_led[3].bits.bit7 //ɲ��ָʾ

/*
 ���زɼ��������
 */
typedef union {

    struct {
        U8 bit0 : 1;
        U8 bit1 : 1;
        U8 bit2 : 1;
        U8 bit3 : 1;
        U8 bit4 : 1;
        U8 bit5 : 1;
        U8 bit6 : 1;
        U8 bit7 : 1;
    }bits;
    U8 byte;
} SW_DATA;

extern SW_DATA sw_input[5]; //���زɼ�ȫ�ֱ���
void SW_Input(void);
#define e_total_miles      0
#define DMC_Gear           0
#define	DMC_Status         0
#define DMC_Level 		   0
#define DMC_Rpm            0   //���ת��


#define IN1   (!MCU_IN1)     //��ת�򿪹�
#define IN2   (!MCU_IN2)     //��������
#define IN3   (MCU_IN3)     //���ſ�״̬ �ػ���Ч ����ȡ��
#define IN4   (MCU_IN4)     //ǰ�ſ�״̬ �ػ���Ч ����ȡ��
#define IN5   (!MCU_IN5)     //��ת�򿪹�
#define IN6   (!MCU_IN6)     //С�ƿ���
#define IN7   (!MCU_IN7)     //Զ��ƿ���
#define IN8   (!MCU_IN8)     //����ƿ���
#define IN9   (!MCU_IN9)     //ǰ��ƿ���
#define IN10  (!MCU_IN10)     //����ƿ���
#define IN11  (!MCU_IN11)     //ǰ��ѹ�������أ��Ǳ�������Ѳ�ͼ����ʾ��
#define IN12  (!MCU_IN12)     //����ѹ�������أ��Ǳ�������Ѳ�ͼ����ʾ��
#define IN13  (!MCU_IN13)    //����
#define IN14  (!MCU_IN14)     //��ε͵�����
#define IN15  (!MCU_IN15)     //��μ�Ъ������
#define IN16  (!MCU_IN16)     //��θߵ�����
#define IN17  (!MCU_IN17)    //�����ˮ������
#define IN18  (!MCU_IN18)     //
#define IN19  (!MCU_IN19)     //���1����
#define IN20  (!MCU_IN20)     //���2����
#define IN21  (!MCU_IN21)     //
#define IN22  (!MCU_IN22)     //˾�����ȿ���
#define IN23  (!MCU_IN23)    //·�ƿ���
#define IN24  (!MCU_IN24)     //
#define IN25  (!MCU_IN25)     //������۷�����
#define IN26  (!MCU_IN26)     //ǿ���г�����
#define IN27  (!MCU_IN27)     //���ƶ��ź�
#define IN28  (!MCU_IN28)    //˾���ƿ���
#define IN29  (!MCU_IN29)    //
#define IN30  (!MCU_IN30)     //
#define IN31  (!MCU_IN31)     //��ȫ������
#define IN32  (!MCU_IN32)     //

#define IN33  0      //
#define IN34  0      //
#define IN35  0  //
#define IN36  0     //ASR���ϵƣ��͵�ƽ��Ч��

#define IN37  0      //ABS���ϵƣ��͵�ƽ��Ч��

#define wake_up1  (PDDR07_P5)      // ���ѣ�Σ�ձ�����Σ���������أ��ػ� ��Ҫ����  ��ʱ����Ϊ����Ч
#define wake_up2  PDDR07_P6      //	����  Կ��ACC��   ��ʱ����Ϊ����Ч

#define wake_up3  PDDR07_P7     // ON��  Կ��ON���أ����ѣ� ��ʱ����Ϊ����Ч



extern DATA_BIT gCTL[8]; //�Ǳ����������Ϣ
#define F_PO1   gCTL[0].bits.bit1
#define F_PO2   gCTL[0].bits.bit2
#define F_PO3   gCTL[0].bits.bit3
#define F_PO4   gCTL[0].bits.bit4
#define F_PO5   gCTL[0].bits.bit5
#define F_PO6   gCTL[0].bits.bit6
#define F_PO7   gCTL[0].bits.bit7
#define F_PO8   gCTL[0].bits.bit8 
#define F_PO9   gCTL[1].bits.bit1
#define F_PO10  gCTL[1].bits.bit2
#define F_PO11  gCTL[1].bits.bit3
#define F_PO12  gCTL[1].bits.bit4
#define F_PO13  gCTL[1].bits.bit5
#define F_PO14  gCTL[1].bits.bit6
#define F_PO15  gCTL[1].bits.bit7
#define F_PO16  gCTL[1].bits.bit8  /////

#define M_PO1  gCTL[2].bits.bit1
#define M_PO2  gCTL[2].bits.bit2
#define M_PO3  gCTL[2].bits.bit3
#define M_PO4  gCTL[2].bits.bit4
#define M_PO5  gCTL[2].bits.bit5
#define M_PO6  gCTL[2].bits.bit6
#define M_PO7  gCTL[2].bits.bit7
#define M_PO8  gCTL[2].bits.bit8 
#define M_PO9   gCTL[3].bits.bit1
#define M_PO10  gCTL[3].bits.bit2
#define M_PO11  gCTL[3].bits.bit3
#define M_PO12  gCTL[3].bits.bit4
#define M_PO13  gCTL[3].bits.bit5
#define M_PO14  gCTL[3].bits.bit6
#define M_PO15  gCTL[3].bits.bit7
#define M_PO16  gCTL[3].bits.bit8  /////

#define R_PO1  gCTL[4].bits.bit1
#define R_PO2  gCTL[4].bits.bit2
#define R_PO3  gCTL[4].bits.bit3
#define R_PO4  gCTL[4].bits.bit4
#define R_PO5  gCTL[4].bits.bit5
#define R_PO6  gCTL[4].bits.bit6
#define R_PO7  gCTL[4].bits.bit7
#define R_PO8  gCTL[4].bits.bit8 
#define R_PO9   gCTL[5].bits.bit1
#define R_PO10  gCTL[5].bits.bit2
#define R_PO11  gCTL[5].bits.bit3
#define R_PO12  gCTL[5].bits.bit4
#define R_PO13  gCTL[5].bits.bit5
#define R_PO14  gCTL[5].bits.bit6
#define R_PO15  gCTL[5].bits.bit7
#define R_PO16  gCTL[5].bits.bit8  /////

extern DBCM_KEY fKEY; //ǰģ�鹦�ʿ���״̬
#define fKL1    fKEY.BITS.kl1
#define fKL2    fKEY.BITS.kl2
#define fKL3    fKEY.BITS.kl3
#define fKL4    fKEY.BITS.kl4
#define fKL5    fKEY.BITS.kl5
#define fKL6    fKEY.BITS.kl6
#define fKL7    fKEY.BITS.kl7
#define fKL8    fKEY.BITS.kl8
#define fKL9    fKEY.BITS.kl9
#define fKL10   fKEY.BITS.kl10
#define fKL11   fKEY.BITS.kl11
#define fKL12   fKEY.BITS.kl12
#define fKL13   fKEY.BITS.kl13
#define fKL14   fKEY.BITS.kl14
#define fKL15   fKEY.BITS.kl15
#define fKH1    fKEY.BITS.kh1
#define fADD    fKEY.BITS.add

extern DBCM_KEY mKEY; //�У�����ģ�鹦�ʿ���״̬
#define mKL1    mKEY.BITS.kl1
#define mKL2    mKEY.BITS.kl2
#define mKL3    mKEY.BITS.kl3
#define mKL4    mKEY.BITS.kl4
#define mKL5    mKEY.BITS.kl5
#define mKL6    mKEY.BITS.kl6
#define mKL7    mKEY.BITS.kl7
#define mKL8    mKEY.BITS.kl8
#define mKL9    mKEY.BITS.kl9
#define mKL10   mKEY.BITS.kl10
#define mKL11   mKEY.BITS.kl11
#define mKL12   mKEY.BITS.kl12
#define mKL13   mKEY.BITS.kl13
#define mKL14   mKEY.BITS.kl14
#define mKL15   mKEY.BITS.kl15
#define mKH1    mKEY.BITS.kh1
#define mADD    mKEY.BITS.add

extern DBCM_KEY rKEY; //��ģ�鹦�ʿ���״̬
#define rKL1    rKEY.BITS.kl1
#define rKL2    rKEY.BITS.kl2
#define rKL3    rKEY.BITS.kl3
#define rKL4    rKEY.BITS.kl4
#define rKL5    rKEY.BITS.kl5
#define rKL6    rKEY.BITS.kl6
#define rKL7    rKEY.BITS.kl7
#define rKL8    rKEY.BITS.kl8
#define rKL9    rKEY.BITS.kl9
#define rKL10   rKEY.BITS.kl10
#define rKL11   rKEY.BITS.kl11
#define rKL12   rKEY.BITS.kl12
#define rKL13   rKEY.BITS.kl13
#define rKL14   rKEY.BITS.kl14
#define rKL15   rKEY.BITS.kl15
#define rKH1    rKEY.BITS.kh1
#define rADD    rKEY.BITS.add

#define CAN_TIME 30//CAN�ڵ���ϱ���
#define BAT24_TIME 100//���ص�ѹ���ͱ���10��

extern U16 fFreq; //ǰģ��Ƶ��
extern U16 mFreq; //�У�����ģ��Ƶ��
extern U16 rFreq; //��ģ��Ƶ��
extern U16 fSpeed; //ǰģ�����㳵��
extern U16 mSpeed; //�У�����ģ�����㳵��
extern U16 rSpeed; //��ģ�����㳵��
extern U16 fADR[2]; //ǰģ�����ֵ
extern U16 mADR[2]; //�У�����ģ�����ֵ
extern U16 rADR[2]; //��ģ�����ֵ
extern DBCM_POUT fPOUT; //ǰģ�鹦�����״̬
extern DBCM_POUT mPOUT; //�У�����ģ�鹦�����״̬
extern DBCM_POUT rPOUT; //��ģ�鹦�����״̬
extern U8 fPF[16]; //ǰģ�鹦���������
extern U8 mPF[16]; //�У�����ģ�鹦�ʹ���
extern U8 rPF[16]; //��ģ�鹦���������
extern U16 fpcur[15];//ǰģ�鹦���������
extern U16 mpcur[15];//��ģ�鹦���������
extern U16 rpcur[15];//��ģ�鹦���������

extern U16 ADR[8]; //ģ�����ɼ�   10/15
extern U16 ADV[8]; //ģ�����ɼ� 


//extern const U8 BMP_LOGO[];

void BCAN_Lost_handle(void);
void Moudle_Logic_handle(void); 

void SW_Output_Init(void);
void SW_Input_Init(void);

void moudle_task_10ms(void);
#endif

#endif
