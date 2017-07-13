#ifndef APP_MOUDLE_H
#define APP_MOUDLE_H

#include "comm_typedef.h"
#include "app_can.h"

extern U8 pSpeed; //�ⲿCAN���㳵��

#if 0
typedef struct {
    unsigned long id;
    unsigned char data[8];
    unsigned char len;
    unsigned char type;
} CAN_MESSAGE;



typedef union
{ 
    unsigned char BYTES[2];
    unsigned int  WORDS;
    struct 
    { 
        unsigned char O1:1; 
        unsigned char O2:1; 
        unsigned char O3:1; 
        unsigned char O4:1; 
        unsigned char O5:1; 
        unsigned char O6:1; 
        unsigned char O7:1; 
        unsigned char O8:1; 
        unsigned char O9:1; 
        unsigned char O10:1; 
        unsigned char O11:1; 
        unsigned char O12:1; 
        unsigned char O13:1; 
        unsigned char O14:1; 
        unsigned char O15:1; 
        unsigned  OD:1;
    } BITS; 
} DBCM_POUT; 

typedef union {
    struct {
        unsigned kl8 : 1;
        unsigned kl7 : 1;
        unsigned kl6 : 1;
        unsigned kl5 : 1;
        unsigned kl4 : 1;
        unsigned kl3 : 1;
        unsigned kl2 : 1;
        unsigned kl1 : 1;

        unsigned     : 1;
        unsigned kl15 : 1;
        unsigned kl14 : 1;
        unsigned kl13 : 1;
        unsigned kl12 : 1;
        unsigned kl11 : 1;
        unsigned kl10 : 1;
		unsigned kl9 : 1;

		
		unsigned :6; 
		unsigned add :1;
        unsigned kh1 :1;
        
             
    } BITS;
    unsigned char BYTES[3];
} DBCM_KEY;

unsigned char delay_cnt;
DATA_BIT gKEY[5];

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
#define  save_flag        gKEY[1].bits.bit8

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


unsigned char Fversion;
unsigned char Mversion;
unsigned char Rversion;


extern U8 front_moudle_count;
extern U8 middle_moudle_count;
extern U8 back_moudle_count;


DBCM_KEY fKEY; //ǰģ�鹦�ʿ���״̬
DBCM_KEY mKEY; //�У�����ģ�鹦�ʿ���״̬
DBCM_KEY rKEY; //��ģ�鹦�ʿ���״̬

unsigned int fFreq; //ǰģ��Ƶ��
unsigned int mFreq; //�У�����ģ��Ƶ��
unsigned int rFreq; //��ģ��Ƶ��

unsigned int fSpeed; //ǰģ�����㳵��
unsigned int mSpeed; //�У�����ģ�����㳵��
unsigned int rSpeed; //��ģ�����㳵��

unsigned int fADR[2]; //ǰģ�����ֵ
unsigned int mADR[2]; //�У�����ģ�����ֵ
unsigned int rADR[2]; //��ģ�����ֵ

DBCM_POUT fPOUT; //ǰģ�鹦�����״̬
DBCM_POUT mPOUT; //�У�����ģ�鹦�����״̬
DBCM_POUT rPOUT; //��ģ�鹦�����״̬

unsigned int fSingle_miles;
unsigned int mSingle_miles;
unsigned int rSingle_miles;

/*
 ���������
 */
typedef union {

    struct {
        unsigned bit0 : 1;
        unsigned bit1 : 1;
        unsigned bit2 : 1;
        unsigned bit3 : 1;
        unsigned bit4 : 1;
        unsigned bit5 : 1;
        unsigned bit6 : 1;
        unsigned bit7 : 1;
    }bits;
    unsigned char byte;
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

/*
#define LED1  BLED1 //ǰ�ſ�ָʾ        
#define LED2  BLED2 //���ſ�ָʾ                         
#define LED3  BLED3 //�յ�ָʾ  
#define LED4  BLED4 //��ת�򱨾�      
#define LED5  BLED5 //ǰ����ָʾ    
#define LED6  BLED6 //�������㱨�� 
#define LED7  BLED7 //���ָʾ  
#define LED8  BLED8 //��ת�򱨾�  
#define LED9  BLED9 //��ѹ�ͱ���
#define LED10 BLED10 //�ܵ�Դָʾ              
#define LED11 BLED11 //��ѹ�ͱ���
#define LED12 BLED12 //SOC����   
#define LED13 BLED13 //������ϱ���            
#define LED14 BLED14 //������ع��ϱ���                 
#define LED15 BLED15 //С�� */

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
        unsigned bit0 : 1;
        unsigned bit1 : 1;
        unsigned bit2 : 1;
        unsigned bit3 : 1;
        unsigned bit4 : 1;
        unsigned bit5 : 1;
        unsigned bit6 : 1;
        unsigned bit7 : 1;
    }bits;
    unsigned char byte;
} SW_DATA;

extern SW_DATA sw_input[5]; //���زɼ�ȫ�ֱ���
void SW_Input(void);


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

#define wake_up1  (!MCU_IN29)      // ���ѣ�Σ�ձ�����Σ���������أ��ػ� ��Ҫ����  ��ʱ����Ϊ����Ч
#define wake_up2  MCU_IN30      //	����  Կ��ACC��   ��ʱ����Ϊ����Ч

#define wake_up3  MCU_IN32     // ON��  Կ��ON���أ����ѣ� ��ʱ����Ϊ����Ч



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

extern unsigned int fFreq; //ǰģ��Ƶ��
extern unsigned int mFreq; //�У�����ģ��Ƶ��
extern unsigned int rFreq; //��ģ��Ƶ��
extern unsigned int fSpeed; //ǰģ�����㳵��
extern unsigned int mSpeed; //�У�����ģ�����㳵��
extern unsigned int rSpeed; //��ģ�����㳵��
extern unsigned int fADR[2]; //ǰģ�����ֵ
extern unsigned int mADR[2]; //�У�����ģ�����ֵ
extern unsigned int rADR[2]; //��ģ�����ֵ
extern DBCM_POUT fPOUT; //ǰģ�鹦�����״̬
extern DBCM_POUT mPOUT; //�У�����ģ�鹦�����״̬
extern DBCM_POUT rPOUT; //��ģ�鹦�����״̬
extern unsigned char fPF[16]; //ǰģ�鹦���������
extern unsigned char mPF[16]; //�У�����ģ�鹦�ʹ���
extern unsigned char rPF[16]; //��ģ�鹦���������
extern unsigned int fpcur[15];//ǰģ�鹦���������
extern unsigned int mpcur[15];//��ģ�鹦���������
extern unsigned int rpcur[15];//��ģ�鹦���������

extern unsigned int ADR[8]; //ģ�����ɼ�   10/15
extern unsigned int ADV[8]; //ģ�����ɼ� 


//extern const unsigned char BMP_LOGO[];

void BCAN_Lost_handle(void);
void Moudle_Logic_handle(void); 

void SW_Output_Init(void);
void SW_Input_Init(void);

void moudle_task_10ms(void);
#endif

#endif
