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
	ID_180689F4_period = 100,
	ID_180789F4_period = 100,
	ID_181B80f4_period = 100,
	ID_0C008980_period = 100,
	ID_0C018980_Period = 100,
	ID_0C028980_Period = 100,
	ID_0C068980_Period = 100,
	ID_0C058980_Period = 100,
	ID_0C048980_Period = 100,
	ID_18xx89F4_Period = 100,

	//send frame
} CAN_ID_PERIOD;


typedef struct
{
//0X180689F4
	U8 ID_180689F4_byte12[2];		//总线电压 0.1V/bit	
	U8 ID_180689F4_byte34[2];		//充放电电流 0.1A/bit， -32000
	U8 ID_180689F4_byte5;		//SOC（电池模块 SOC） 0.4％/bit
	U8 ID_180689F4_byte6;		//电池箱内环境最高温度 1℃/bit， -40
	struct
	{
		U8 bit7 :1;         //温度过低
		U8 bit6 :1;         //SOC太低停车
		U8 bit5 :1;         //温度过高
		U8 bit4 :1;         //过电流
		U8 bit3 :1;         //SOC过低
		U8 bit2 :1;         //SOC过高
		U8 bit1 :1;         //模块电压过低
		U8 bit0 :1;			//模块电压过高		
	}ID_180689F4_byte7;			//电池 Status_Flag1
	U8 ID_180689F4_byte8;		//预留
	
//ID_180789F4
	U8 ID_180789F4_byte12[2];	//电池模块最低电压   高4位为电池箱号（0-15），低12位为电压：0.01V/bit
	U8 ID_180789F4_byte34[2];	//电池模块最高电压   高4位为电池箱号（0-15），低12位为电压：0.01V/bit
	U8 ID_180789F4_byte5;		//电池模块最高温度	 1℃/bit， -40
	U8 ID_180789F4_byte6;		//电池模块最低温度	 1℃/bit， -40
	struct {
		U8 bit7 :1;         //充电指示
		U8 bit6 :1;         //充电口连接器指示
		U8 bit5 :1;         //充电接触器控制
		U8 bit4 :1;         //充电完成
		U8 bit3 :1;         //预留
		U8 bit2 :1;         //预留
		U8 bit1 :1;         //预留
		U8 bit0 :1;			//预留
	}ID_180789F4_byte7;		//电池 Status_Flag2
	U8 ID_180789F4_byte8;		//预留

//0X181B80F4
	U8 ID_181B80F4_byte12[2];	//电池当前最大允许放电电流 0.1A/bit， -32000
	U8 ID_181B80F4_byte34[2];	//电池当前最大允许充电电流 0.1A/bit， -32000
	struct {
		U8 bit67 :2;        //整组欠压报警 （总线电压欠压报警） 00：无报警； 01：一级报警； 10：二级报警
		U8 bit45 :2;        //单体欠压报警  					00：无报警； 01：一级报警； 10：二级报警
		U8 bit23 :2;        //整组过压报警 （总线电压过压报警） 00：无报警； 01：一级报警； 10：二级报警
		U8 bit01 :2;		//单体过压报警 						00：无报警； 01：一级报警； 10：二级报警
	}ID_181B80F4_byte5;		//Status_Flags3
	struct {
		U8 bit67 :2;        //模块温差报警                      00：无报警； 01：一级报警； 10：二级报警
		U8 bit45 :2;        //单体压差报警  					00：无报警； 01：一级报警； 10：二级报警
		U8 bit23 :2;        //电池高温报警                      00：无报警； 01：一级报警； 10：二级报警
		U8 bit01 :2;		//电池低温报警 						00：无报警； 01：一级报警； 10：二级报警		
	}ID_181B80F4_byte6;		//Status_Flags4
	struct {
		U8 bit67 :2;        //电池 SOC 低报警                   00：无报警； 01：一级报警； 10：二级报警
		U8 bit45 :2;        //绝缘检测报警  					00：无报警； 01：一级报警； 10：二级报警
		U8 bit23 :2;        //电池充电电流过大报警              00：无报警； 01：一级报警； 10：二级报警
		U8 bit01 :2;		//电池放电电流过大报警      		00：无报警； 01：一级报警； 10：二级报警		
	}ID_181B80F4_byte7;		//Status_Flags5
	struct {
		U8 bit67 :2;        //预留
		U8 bit45 :2;        //预留
		U8 bit23 :2;        //外网通讯故障 （总线电压过压报警） 00：无报警； 01：一级报警； 10：二级报警
		U8 bit01 :2;		//内网通讯故障 						00：无报警； 01：一级报警； 10：二级报警
	}ID_181B80F4_byte8;		//Status_Flags6
}BatteryManagement;

typedef struct {
	U16 bit0 :4;			//电池箱号
	U16 bit4 :12;			//电池电压
}Voltage;						//高 4 位为电池箱号（0-15），低 12 位为电压： 0.01V/ bit

extern Voltage vol_temp_msg[4];

extern U16 vol[112*4];				//高 4 位为电池箱号（0-15），低 12 位为电压： 0.01V/ bit	356个
extern U8 temperater[26*8];			//温度点1-96		1℃/bit， -40							96个

typedef struct{
	U16 pager;       	//显示菜单页
	U8 voltage[32][2];		//电压： 0.01V/位
}BatteryVoltage;

typedef struct{
	U8 pager;        		//显示菜单页
	U8 Temperater[32];		//分辨率 1℃/bit 偏移量-40℃
}BatteryTemperater;

typedef struct
{
//0X0C008980
	U8 ID_0C008980_byte12[2];		//电机控制器前端电压 0.1V/bit, -10000
	U8 ID_0C008980_byte3;		//电机温度 1℃/bit， -40
	U8 ID_0C008980_byte4;		//电机控制器温度 1℃/bit， -40
	U8 ID_0C008980_byte56[2]; 	//电机控制器电流 0.1A/bit， -10000
	U8 ID_0C008980_byte78[2];		//电机转速 0.5rpm/bit
	
//0X0C018980
	U8 ID_0C018980_byte1;		//司机加速踏板 0.4%/bit
	U8 ID_0C018980_byte2;		//司机制动踏板 0.4%/bit
	struct {
		U8 bit7 :1;         //ready				电机控制器工作正常，可以发送转矩命令
		U8 bit6 :1;         //故障				电机控制器当前处于故障状态，禁止对电机控制器发送工作命令。
		U8 bit5 :1;         //翻转				电机控制器当前状态为倒车。
		U8 bit4 :1;         //制动				1：仪表控制制动灯亮 0：仪表控制制动灯灭
		U8 bit3 :1;         //驱动
		U8 bit2 :1;         //预留
		U8 bit1 :1;         //散热风扇开		1 为开启， 0 为关闭。
		U8 bit0 :1;			//水泵开			1 为开启， 0 为关闭。
	}ID_0C018980_byte3;		//驱动系统状态
	struct {
		U8 bit7 :1;         //预留
		U8 bit6 :1;         //预留
		U8 bit5 :1;         //预留
		U8 bit4 :1;         //预留
		U8 bit3 :1;         //预留
		U8 bit02 :3;		//01前进 02空挡 （04倒车 1：仪表控制倒车灯亮 0：仪表控制倒车灯灭）		
	}ID_0C018980_byte4;			//司机操作状态
	U8 ID_0C018980_byte5;		//保留
	U8 ID_0C018980_byte67[2];		//系统代码
	U8 ID_0C018980_byte8;		//整车控制器LIFE

//0X0C028980
	U8 ID_0C028980_byte12[2];		//电机目标转矩 1NM/bit， -32000
	struct {
		U8 bit7 :1;         //预留
		U8 bit6 :1;         //K1	电池主接触器
		U8 bit5 :1;         //预留
		U8 bit4 :1;         //K3	充电接触器
		U8 bit3 :1;         //K4	电机控制器主接触器
		U8 bit2 :1;         //K5	电机控制器预充接触器
		U8 bit1 :1;         //预留
		U8 bit0 :1;			//预留
	}ID_0C028980_byte3;			//接触器开启 Status_Flag
	U8 ID_0C028980_byte4;		//电机工作模式 显示十进制数字
	U8 ID_0C028980_byte5;		//预留
	struct {
		U8 bit7 :1;         //预留
		U8 bit6 :1;         //预留
		U8 bit5 :1;         //预留
		U8 bit4 :1;         //预留
		U8 bit3 :1;         //预留
		U8 bit2 :1;         //地面停车充电			1： 电池和充电接头间的线为实线  0： 电池和充电接头间的线为虚线
		U8 bit1 :1;         //电池行车				1： 电池和电机间的线为实线 		0： 电池和电机间的线连接成虚线
		U8 bit0 :1;			//预留
		}ID_0C028980_byte6;			//能量条状态
	U8 ID_0C028980_byte7;		//预留
	U8 ID_0C028980_byte8;		//预留
}MenuVcuControl;

//0X0C058980
typedef struct
{
	U8 ID_0C058980_byte12[2];		//油泵DC/DC 输出电压 0.1V/bit， -10000
	U8 ID_0C058980_byte34[2];		//油泵DC/DC 输出电流 0.1V/bit， -10000
	U8 ID_0C058980_byte56[2];		//油泵DC/DC 输入电流 0.1V/bit， -10000
	U8  ID_0C058980_byte7;		//油泵DC/DC 温度     1℃/bit，  -40
	struct {
		U8 bit7 :1;         //1 运行 0 停机
		U8 bit6 :1;         //预留		
		U8 bit5 :1;         //缺相
		U8 bit4 :1;         //短相
		U8 bit3 :1;         //过压
		U8 bit2 :1;         //欠压
		U8 bit1 :1;         //过热
		U8 bit0 :1; 		//过流
	}ID_0C058980_byte8;			//油泵DC/DC 状态及故障
}OilPump;

//0X0C068980
typedef struct{
	U8 ID_0C068980_byte12[2];		//气泵DC/DC 输出电压 0.1V/bit， -10000
	U8 ID_0C068980_byte34[2];		//气泵DC/DC 输出电流 0.1V/bit， -10000
	U8 ID_0C068980_byte56[2];		//气泵DC/DC 输入电流 0.1V/bit， -10000
	U8 ID_0C068980_byte7;		//气泵DC/DC 温度     1℃/bit，  -40
	struct {
		U8 bit7 :1;         //1 运行 0 停机
		U8 bit6 :1;         //气压信号
		U8 bit5 :1; 		//缺相
		U8 bit4 :1;         //短相
		U8 bit3 :1;         //过压		
		U8 bit2 :1;         //欠压
		U8 bit1 :1;         //过热
		U8 bit0 :1;			//过流
	}ID_0C068980_byte8;			//气泵DC/DC 状态及故障
}AirPump;

typedef struct
{
//0X0C048980
	U8 ID_0C048980_byte12[2];		//DC/DC 输出电压 0.1V/bit， -10000
	U8 ID_0C048980_byte34[2];		//DC/DC 输出电流 0.1V/bit， -10000
	U8 ID_0C048980_byte56[2];		//DC/DC 输入电流 0.1V/bit， -10000
	U8 ID_0C048980_byte7;		//DC/DC 温度     1℃/bit，  -40
	struct {
		U8 bit7 :1;         //1 运行 0 停机
		U8 bit6 :1;         //预留
		U8 bit5 :1;         //预留
		U8 bit4 :1;         //预留		
		U8 bit3 :1;         //过压
		U8 bit2 :1; 		//欠压
		U8 bit1 :1;         //过热
		U8 bit0 :1;			//过流
	}ID_0C048980_byte8;			//DC/DC 状态及故障
}DcdcControl;

extern BatteryManagement bms_msg;
extern BatteryVoltage bat_vol_msg;
extern BatteryTemperater bat_temp_msg;
extern MenuVcuControl vcu_msg;
extern AirPump air_pump_msg;
extern OilPump oil_pump_msg;
extern DcdcControl dcdc_msg;

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
extern unsigned int  PRESS[2]; //气压值
extern U32 Vcan_timeout_cfg;
extern U32 Bcan_timeout_cfg;
extern U32 Dcan_timeout_cfg;
extern U32 Acan_timeout_cfg;

extern BatteryManagement bms_msg;
extern BatteryVoltage bat_vol_msg;
extern BatteryTemperater bat_temp_msg;
extern MenuVcuControl vcu_msg;
extern AirPump air_pump_msg;
extern OilPump oil_pump_msg;
extern DcdcControl dcdc_msg;

typedef void (*pCanAnalyse)(can_msg_t *msg, can_pro_way_e way);

void app_init_can(void);

void app_can_sent_task(void);

void app_can_lost_time_cnt_100ms(void);

extern void mid_can1_prepare(can_msg_t*msg_tx,can_msg_t*msg_rx);
#endif
