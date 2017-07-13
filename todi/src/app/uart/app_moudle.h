#ifndef APP_MOUDLE_H
#define APP_MOUDLE_H

#include "comm_typedef.h"
#include "app_can.h"

extern U8 pSpeed; //外部CAN这算车速

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
#define  F500ms_LCD       gKEY[1].bits.bit6 //lcd显示更新时间
#define  alarm_update_flag  gKEY[1].bits.bit7//报警显示更新时间
//#define  CLR_single_miles gKEY[1].bit7
#define  save_flag        gKEY[1].bits.bit8

#define  key_long_set     gKEY[2].bits.bit1
#define  M_ON             gKEY[2].bits.bit2  
#define  kFLASH           gKEY[2].bits.bit3
#define  FLASH            gKEY[2].bits.bit4 //转向灯闪烁标志
#define  FLASH_1s         gKEY[2].bits.bit5 
#define  DM1_CLEAR        gKEY[2].bits.bit6 
#define  R_flag           gKEY[2].bits.bit7
#define  bLCD_RESET       gKEY[2].bits.bit8

#define  F300ms           gKEY[3].bits.bit1 // 2016/10/22新增更新时间
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



DATA_BIT gCTL[8]; //仪表输出控制信息

extern U8 pSpeed; //外部CAN这算车速


unsigned char Fversion;
unsigned char Mversion;
unsigned char Rversion;


extern U8 front_moudle_count;
extern U8 middle_moudle_count;
extern U8 back_moudle_count;


DBCM_KEY fKEY; //前模块功率开关状态
DBCM_KEY mKEY; //中（顶）模块功率开关状态
DBCM_KEY rKEY; //后模块功率开关状态

unsigned int fFreq; //前模块频率
unsigned int mFreq; //中（顶）模块频率
unsigned int rFreq; //后模块频率

unsigned int fSpeed; //前模块折算车速
unsigned int mSpeed; //中（顶）模块折算车速
unsigned int rSpeed; //后模块折算车速

unsigned int fADR[2]; //前模块电阻值
unsigned int mADR[2]; //中（顶）模块电阻值
unsigned int rADR[2]; //后模块电阻值

DBCM_POUT fPOUT; //前模块功率输出状态
DBCM_POUT mPOUT; //中（顶）模块功率输出状态
DBCM_POUT rPOUT; //后模块功率输出状态

unsigned int fSingle_miles;
unsigned int mSingle_miles;
unsigned int rSingle_miles;

/*
 报警灯相关
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

extern LED_DATA alarm_led[4]; //报警灯全局变量
void LED_Out(void);

#define BUZZ  alarm_led[0].bits.bit0 //蜂鸣器 
#define CS_A  alarm_led[0].bits.bit4 //4051
#define CS_B  alarm_led[0].bits.bit3 
#define CS_C  alarm_led[0].bits.bit2 
#define CS_E  alarm_led[0].bits.bit5 

#define PO1   alarm_led[0].bits.bit6 //功率输出    
#define PO2   alarm_led[0].bits.bit7 //功率输出  

/*
#define LED1  BLED1 //前门开指示        
#define LED2  BLED2 //中门开指示                         
#define LED3  BLED3 //空挡指示  
#define LED4  BLED4 //左转向报警      
#define LED5  BLED5 //前进挡指示    
#define LED6  BLED6 //电量不足报警 
#define LED7  BLED7 //充电指示  
#define LED8  BLED8 //右转向报警  
#define LED9  BLED9 //气压低报警
#define LED10 BLED10 //总电源指示              
#define LED11 BLED11 //气压低报警
#define LED12 BLED12 //SOC报警   
#define LED13 BLED13 //电机故障报警            
#define LED14 BLED14 //动力电池故障报警                 
#define LED15 BLED15 //小灯 */

#define LED16 alarm_led[2].bits.bit7 //蓄电池充电指示                
#define LED17 alarm_led[3].bits.bit0 //远光灯报警  
#define LED18 alarm_led[3].bits.bit1 //驻车制动报警
#define LED19 alarm_led[3].bits.bit2 //前雾灯报警
#define LED20 alarm_led[3].bits.bit3 //后雾灯报警
#define LED21 alarm_led[3].bits.bit4 //倒车灯
#define LED22 alarm_led[3].bits.bit5 //近光灯报警 
#define LED23 alarm_led[3].bits.bit6 //缓速器报警               
#define LED24 alarm_led[3].bits.bit7 //刹车指示

/*
 开关采集驱动相关
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

extern SW_DATA sw_input[5]; //开关采集全局变量
void SW_Input(void);


#define IN1   (!MCU_IN1)     //左转向开关
#define IN2   (!MCU_IN2)     //保留开关
#define IN3   (MCU_IN3)     //中门开状态 控火有效 不用取反
#define IN4   (MCU_IN4)     //前门开状态 控火有效 不用取反
#define IN5   (!MCU_IN5)     //右转向开关
#define IN6   (!MCU_IN6)     //小灯开关
#define IN7   (!MCU_IN7)     //远光灯开关
#define IN8   (!MCU_IN8)     //近光灯开关
#define IN9   (!MCU_IN9)     //前雾灯开关
#define IN10  (!MCU_IN10)     //后雾灯开关
#define IN11  (!MCU_IN11)     //前气压报警开关（仪表蜂鸣提醒并图标显示）
#define IN12  (!MCU_IN12)     //后气压报警开关（仪表蜂鸣提醒并图标显示）
#define IN13  (!MCU_IN13)    //保留
#define IN14  (!MCU_IN14)     //雨刮低档开关
#define IN15  (!MCU_IN15)     //雨刮间歇档开关
#define IN16  (!MCU_IN16)     //雨刮高档开关
#define IN17  (!MCU_IN17)    //雨刮喷水档开关
#define IN18  (!MCU_IN18)     //
#define IN19  (!MCU_IN19)     //厢灯1开关
#define IN20  (!MCU_IN20)     //厢灯2开关
#define IN21  (!MCU_IN21)     //
#define IN22  (!MCU_IN22)     //司机风扇开关
#define IN23  (!MCU_IN23)    //路牌开关
#define IN24  (!MCU_IN24)     //
#define IN25  (!MCU_IN25)     //电控排污阀开关
#define IN26  (!MCU_IN26)     //强制行车开关
#define IN27  (!MCU_IN27)     //手制动信号
#define IN28  (!MCU_IN28)    //司机灯开关
#define IN29  (!MCU_IN29)    //
#define IN30  (!MCU_IN30)     //
#define IN31  (!MCU_IN31)     //安全带开关
#define IN32  (!MCU_IN32)     //

#define IN33  0      //
#define IN34  0      //
#define IN35  0  //
#define IN36  0     //ASR故障灯（低电平有效）

#define IN37  0      //ABS故障灯（低电平有效）

#define wake_up1  (!MCU_IN29)      // 唤醒（危险报警）危急报警开关（控火） 需要唤醒  暂时定义为高有效
#define wake_up2  MCU_IN30      //	唤醒  钥匙ACC档   暂时定义为高有效

#define wake_up3  MCU_IN32     // ON档  钥匙ON开关（唤醒） 暂时定义为高有效



extern DATA_BIT gCTL[8]; //仪表输出控制信息
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

extern DBCM_KEY fKEY; //前模块功率开关状态
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

extern DBCM_KEY mKEY; //中（顶）模块功率开关状态
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

extern DBCM_KEY rKEY; //后模块功率开关状态
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

#define CAN_TIME 30//CAN节点故障报警
#define BAT24_TIME 100//蓄电池电压过低报警10秒

extern unsigned int fFreq; //前模块频率
extern unsigned int mFreq; //中（顶）模块频率
extern unsigned int rFreq; //后模块频率
extern unsigned int fSpeed; //前模块折算车速
extern unsigned int mSpeed; //中（顶）模块折算车速
extern unsigned int rSpeed; //后模块折算车速
extern unsigned int fADR[2]; //前模块电阻值
extern unsigned int mADR[2]; //中（顶）模块电阻值
extern unsigned int rADR[2]; //后模块电阻值
extern DBCM_POUT fPOUT; //前模块功率输出状态
extern DBCM_POUT mPOUT; //中（顶）模块功率输出状态
extern DBCM_POUT rPOUT; //后模块功率输出状态
extern unsigned char fPF[16]; //前模块功率输出故障
extern unsigned char mPF[16]; //中（顶）模块功率故障
extern unsigned char rPF[16]; //后模块功率输出故障
extern unsigned int fpcur[15];//前模块功率输出电流
extern unsigned int mpcur[15];//中模块功率输出电流
extern unsigned int rpcur[15];//后模块功率输出电流

extern unsigned int ADR[8]; //模拟量采集   10/15
extern unsigned int ADV[8]; //模拟量采集 


//extern const unsigned char BMP_LOGO[];

void BCAN_Lost_handle(void);
void Moudle_Logic_handle(void); 

void SW_Output_Init(void);
void SW_Input_Init(void);

void moudle_task_10ms(void);
#endif

#endif
