#ifndef    __APP_CAN_H__
#define    __APP_CAN_H__

#include  "mid_can.h"

#define    MSG_BOX_ALl				23

#define		CAN_LOST_TIME			25


typedef union {

    struct {
        U8 bit1 : 2;
        U8 bit2 : 2;
        U8 bit3 : 2;
        U8 bit4 : 2;
    }bits;
    U8 byte;
} DATA_DOUBLE;

typedef union {

    struct {
        U8 bit8 : 1;
        U8 bit7 : 1;
        U8 bit6 : 1;
        U8 bit5 : 1;
        U8 bit4 : 1;
        U8 bit3 : 1;
        U8 bit2 : 1;
        U8 bit1 : 1;
    }bits;
    U8 byte;
} DATA_BIT;


typedef enum
{
	ID_7e7,
	ID_100017EF, //VCU2ICU01
	ID_1801FBEF, //
	ID_1811FBEF, //
	ID_18FF08F2, //
	ID_10F8159E, //BMS STATUS 1
	ID_10F8169E, //BMS STATUS 2
	ID_18F8179E, //BMS STATUS 3
	ID_18F8189E, //BMS STATUS 4
	ID_18F8199E, //BMS STATUS 5
	ID_18F81A9E, //BMS STATUS 6
	ID_18FF12F7, //DCDC21
	ID_18FF0AF8, //OILDCAC21
	ID_18FF0BF8, //OILDCAC22
	ID_18FF0CF9, //AIRDCAC21
	ID_18FF0DF9, //AIRDCAC22
	ID_1801EFA9, //ICS01
	ID_104C1000, //电池电压 100帧报文0x104C19A4-0x104C1A07 16帧电池温度0x104C1A18-0x104C1A24
	ID_UPDATA,
	CAN_ID_ALL
} MSG_ID_TYPE;

typedef enum
{
	//received frame
	ID_0C03A1A7_period = 100,
	ID_0C04A1A7_period = 100,
	ID_0C05A1A7_period = 100,
	ID_0C06A1A7_period = 100,
	ID_0C07A1A7_Period = 100,
	ID_0C08A1A7_Period = 100,
	ID_1818D0F3_Period = 100,
	ID_1819D0F3_Period = 100,
	ID_181AD0F3_Period = 100,
	ID_180028F3_Period = 100,
	ID_180128F3_Period = 100,
	ID_180228F3_Period = 100,
	ID_180328F3_Period = 100,
	ID_180428F3_Period = 100,
	ID_180028F4_Period = 100,
	ID_180128F4_Period = 100,
	ID_180228F4_Period = 100,
	ID_0C09A79B_period = 100,
	ID_18FFC09E_period = 100,
	ID_18FECA00_period = 100,

	//send frame
	
} CAN_ID_PERIOD;


/**********随州协议变量************/


extern unsigned int moto_voltage; //电机电压
extern unsigned int moto_current; //电机电流
extern unsigned int moto_speed; //电动转速
extern unsigned char Motor_Temperature; //电机温度
extern unsigned char Inverter_t; //逆变器温度

extern unsigned char Accelerator; //油门开度
extern unsigned int Engine_Speed;//发动机转速

extern unsigned int Fule_rate;//油耗
extern unsigned char water_temp;//水温
extern unsigned char Engine_Fuhe;//发动机负荷
extern unsigned char Niaosu;//尿素;
extern unsigned char Accelerator_Shiji; //油门踏板实际
extern unsigned char Engine_oil_press; //机油压力
extern unsigned char ambient_air_temperature; //周围空气温度

extern unsigned char Current_Gear; //档位
extern unsigned char TCU_Position;//离合位置
extern unsigned char TCU_warn;//变速箱故障
extern unsigned char STOP;//停车故障
extern unsigned int Speed_rmp; //输出轴转速
extern unsigned char Car_mode;//整车模式
extern unsigned char TCU_code;//变速箱故障代码
extern unsigned char TCU_level;//变速箱故障等级

extern DATA_BIT CAN_KEY[4];
#define  CAN_ON           CAN_KEY[0].bits.bit1
#define  CAN_ACC          CAN_KEY[0].bits.bit2  
#define  Diagnosis        CAN_KEY[0].bits.bit3  
#define  Mode_S           CAN_KEY[0].bits.bit4  
#define  AC_SWITCH        CAN_KEY[0].bits.bit5  
#define  Hybrid           CAN_KEY[0].bits.bit6  
#define  Electric         CAN_KEY[0].bits.bit7  
#define  Tradition        CAN_KEY[0].bits.bit8  

#define  High_Voltage     CAN_KEY[1].bits.bit5
#define  Charge_Check     CAN_KEY[1].bits.bit6  

#define  Battery_Kt       CAN_KEY[2].bits.bit2  

#define  Brake_Pedal      CAN_KEY[3].bits.bit1 
extern unsigned char Brake_percent; //制动踏板开度
extern unsigned char Speed_percent; //加速踏板开度
extern unsigned char Digit_input3;//数字量输入3

extern unsigned long Program;//程序版本
extern unsigned long tcu_spn; //TCU故障码
extern unsigned char tcu_fmi;
extern unsigned long tcu_cm; 
extern unsigned char tcu_oc;
extern unsigned char Car_LIFE;//整车控制器

extern unsigned int BMS_V; //电池电压
extern unsigned char BMS_SOC; //电池SOC
extern unsigned int BMS_A; //电池电流
extern unsigned char Status_Flag1;
extern unsigned char Status_Flag2;
extern unsigned char Status_Flag3;

extern unsigned int BMS_A_charge; //最大充电电流
extern unsigned int BMS_A_discharge; //最大电流
extern unsigned char Warn_level;//故障等级
extern unsigned int BMS_V_average; //电池平均电压
extern unsigned char BMS_T_H;//电池最高温度

extern unsigned int Oum_zheng; //绝缘正阻值
extern unsigned int Oum_fu; //绝缘负阻值
extern unsigned int Battery_single_H; //电池单体高电压
extern unsigned int Battery_single_L; //电池单体低电压

extern unsigned char Battery_number[10];//电池编号
extern unsigned int Battery_voltage[10];//电池单体电压

extern unsigned char Battery_number_t[10];//电池温度编号
extern unsigned int Battery_temp[10];//电池单体温度

extern unsigned int DCAC_W;
extern unsigned int DCAC_V;
extern unsigned int DCAC_U;
extern unsigned char Sanreqi_t;//散热器温度
extern unsigned char DCAC_Warn_code;//故障代码

extern unsigned char AC_Warn_code;//AC故障代码
extern DATA_BIT AC_KEY[2];
#define  AC_opean         AC_KEY[0].bits.bit6
#define  AC_mind_speed    AC_KEY[0].bits.bit7
#define  AC_High_speed    AC_KEY[0].bits.bit8

#define  AC_warn1         AC_KEY[1].bits.bit1
#define  AC_cold1_shuang  AC_KEY[1].bits.bit2
#define  AC_wind          AC_KEY[1].bits.bit3
#define  AC_save          AC_KEY[1].bits.bit4
#define  AC_cold2_shuang  AC_KEY[1].bits.bit5
#define  AC_cold2         AC_KEY[1].bits.bit6
#define  AC_cold1         AC_KEY[1].bits.bit8
extern unsigned char Indoor_t;//室内温度
extern unsigned char Outdoor_t;//室外温度
extern unsigned char Set_t;//设定
extern unsigned char AC_req;//制冷请求
extern unsigned char AC_LIFE;//AC生命
/*******************************************/

typedef void (*pCanAnalyse)(can_msg_t *msg, can_pro_way_e way);

void app_init_can(void);

void app_can_sent_task(void);

void app_can_lost_time_cnt_100ms(void);

extern void mid_can1_prepare(can_msg_t*msg_tx,can_msg_t*msg_rx);
#endif
