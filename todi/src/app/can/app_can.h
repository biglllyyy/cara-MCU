#ifndef    __APP_CAN_H__
#define    __APP_CAN_H__

#include  "mid_can.h"

#define    MSG_BOX_ALl				23

#define		CAN_LOST_TIME			25


typedef union {

    struct {
        unsigned bit1 : 2;
        unsigned bit2 : 2;
        unsigned bit3 : 2;
        unsigned bit4 : 2;
    };
    unsigned char byte;
} DATA_DOUBLE;

typedef union {

    struct {
        unsigned bit1 : 1;
        unsigned bit2 : 1;
        unsigned bit3 : 1;
        unsigned bit4 : 1;
        unsigned bit5 : 1;
        unsigned bit6 : 1;
        unsigned bit7 : 1;
        unsigned bit8 : 1;
    };
    unsigned char byte;
} DATA_BIT;


typedef enum
{
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
	CAN_ID_ALL
} MSG_ID_TYPE;

typedef enum
{
	//received frame
	ID_100017EF_period = 100, //VCU2ICU01
	ID_1801FBEF_period = 500, //
	ID_1811FBEF_period = 500, //
	ID_18FF08F2_period = 500, //
	ID_10F8159E_Period = 100, //BMS STATUS 1
	ID_10F8169E_Period = 100, //BMS STATUS 2
	ID_18F8179E_Period = 1000, //BMS STATUS 3
	ID_18F8189E_Period = 1000, //BMS STATUS 4
	ID_18F8199E_Period = 1000, //BMS STATUS 5
	ID_18F81A9E_Period = 1000, //BMS STATUS 6
	ID_18FF12F7_Period = 500, //DCDC21
	ID_18FF0AF8_Period = 500, //OILDCAC21
	ID_18FF0BF8_Period = 500, //OILDCAC22
	ID_18FF0CF9_Period = 500, //AIRDCAC21
	ID_18FF0DF9_Period = 500, //AIRDCAC22
	ID_1801EFA9_Period = 100, //ICS01
	ID_104C1000_Period = 1000,
	//send frame
	ID_1801EF17_Period = 100,
	ID_1802EF17_Period = 500,
	
} CAN_ID_PERIOD;

typedef struct
{
	/*can msg receive*/

	U8 ID_100017EF[8]; //VCU2ICU01
	U8 ID_1801FBEF[8]; //
	U8 ID_1811FBEF[8]; //
	U8 ID_18FF08F2[8]; //
	U8 ID_10F8159E[8]; //BMS STATUS 1
	U8 ID_10F8169E[8]; //BMS STATUS 2
	U8 ID_18F8179E[8]; //BMS STATUS 3
	U8 ID_18F8189E[8]; //BMS STATUS 4
	U8 ID_18F8199E[8]; //BMS STATUS 5
	U8 ID_18F81A9E[8]; //BMS STATUS 6
	U8 ID_18FF12F7[8]; //DCDC21
	U8 ID_18FF0AF8[8]; //OILDCAC21
	U8 ID_18FF0BF8[8]; //OILDCAC22
	U8 ID_18FF0CF9[8]; //AIRDCAC21
	U8 ID_18FF0DF9[8]; //AIRDCAC22
	U8 ID_1801EFA9[8]; //ICS01
	//U8 ID_104C1000[8]; //BATTERY V and temp

} can_signal_t;



/*******************分包处理*****************/
//VCU
unsigned int  VCU_TM_NM = 5000; //TM目标扭矩
unsigned int  VCU_TM_RPM = 15000; //TM目标转速

unsigned char VCU_TM_Brake_percent; //制动踏板开度
unsigned char VCU_TM_Speed_percent; //加速踏板开度
DATA_BIT VCU_Status_Flag1; //状态1
unsigned char VCU_Gear; //档位
DATA_BIT VCU_Mode; //整车模式
unsigned char VCU_Life; //整车控制器LIFE
DATA_BIT VCU_Status_Flag2;
unsigned char VCU_Code; //整车故障码
// VCU2TERMINAL_MOTOR01
unsigned char TM_Number; //电机个数
unsigned char TM_Serial; //电机序号
unsigned char TM_WorkStatus; //TM电机工作状态
unsigned char TM_Control_Temp = 40; //TM电机控制器温度
unsigned int  TM_Feedback_RPM = 20000; //TM电机反馈转速
unsigned int  TM_Feedback_NM = 20000; //TM电机反馈扭矩
// VCU2TERMINAL_MOTOR02
unsigned char TM_Temp = 40; //TM电机温度
unsigned int  TM_Voltage_DC; //TM电机直流电压
unsigned int  TM_Current_DC = 10000; //TM电机直流电流
/*
//MCU01
unsigned int  TM_Feedback_RPM=20000;//TM电机反馈转速
unsigned int  TM_Feedback_NM=10000;//TM电机反馈扭矩
unsigned int  TM_Current_AC;//TM电机交流电流
unsigned char TM_StatusFeedback;//TM电机状态反馈
unsigned char TM_WorkStatus;//TM电机工作状态
unsigned char TM_LIFE;//TM电机LIFE
//MCU02
unsigned char TM_Temp=40;//TM电机温度
unsigned char TM_Control_Temp=40;//TM电机控制器温度
unsigned int  TM_Torque_Limit_Up;//电机扭矩上限
unsigned int  TM_Torque_Limit_Down;//电机扭矩下限
DATA_BIT      TM_Fault;//TM电机故障等级
unsigned char TM_Status;//TM电机状态
DATA_BIT      TM_Status_Flag1;//电机状态1
//MCU03
unsigned int  TM_Code;//TM电机故障码
unsigned int  TM_Current_DC=20000;//TM电机直流电流
unsigned int  TM_Voltage_DC;//TM电机直流电压
 */
//IRM01
unsigned long IRM_Ohm_Positive; //总正绝缘阻值 
unsigned long IRM_Ohm_Negative; //总负绝缘阻值 
unsigned char IRM_Fault_Level; //设备故障等级
unsigned char IRM_Insulation_Level; //绝缘等级
unsigned char IRM_Life; //绝缘LIFE

/*unsigned char VCU_Kt; //主接触器控制命令
unsigned char VCU_ON; //钥匙下电状态
unsigned char VCU_Order; //故障处理命令
unsigned int  VCU_Voltage; //电机控制器电压
 */

unsigned char BMS_Mode; //电池充放电模式
unsigned char BMS_Status; //电池状态
unsigned char BAT_Temp_Average = 40; //电池平均温度
unsigned char BMS_SOC; //SOC
unsigned int  BMS_Current = 32000; //电池系统电流
unsigned int  BMS_Voltage; //电池系统内总电压
unsigned char BMS_Kt; //高压继电器状态

unsigned char BAT_Temp_H_Limit = 40; //最高允许电池单体温度
unsigned char BAT_Temp_L_Limit = 40; //最低允许电池单体温度
unsigned char BMS_SOC_L_Limit; //最低允许SOC值
unsigned int  BAT_Current_Discharge_Limit = 32000; //最高允许放电电流
unsigned int  BAT_Current_Charge_Limit = 32000; //最高允许充电电流

unsigned char BAT_Temp_L = 40; //电池单体最低温度
unsigned char BAT_Temp_L_Number; //电池单体低温度序号
unsigned char BAT_Temp_L_Case; //电池单体低温度箱号
unsigned char BAT_Temp_H = 40; //电池单体最高温度
unsigned char BAT_Temp_H_Number; //电池单体高温度序号
unsigned char BAT_Temp_H_Case; //电池单体高温度箱号

unsigned char BAT_Voltage_Fault; //总压故障
unsigned char BAT_Single_Fault; //单压故障
unsigned char BAT_Temp_Fault; //温度故障
unsigned char BAT_Insulation_Fault; //绝缘故障
unsigned char BAT_Consistency_Fault; //电池一致性故障
unsigned char BAT_SOC_Fault; //SOC故障
unsigned char BAT_Current_Fault; //电流故障
unsigned char BAT_Lock_Fault; //高压互锁故障
unsigned char BMS_Comm_Fault; //BMS通讯故障
unsigned char BMS_System_Fault; //BMS系统故障

unsigned int  BMS_Ohm; //绝缘阻值 
unsigned char BMS_Number; //单体电压数量
unsigned char BMS_Temp_Number; //单体温度数量
unsigned char BMS_System_Unit; //系统数量
unsigned char BMS_System_Number; //系统号

unsigned int  BAT_Single_L; //电池单体低电压
unsigned char BAT_Single_L_Number; //电池单体低电压位置
unsigned char BAT_Single_L_Case; //电池单体低电压箱号

unsigned int  BAT_Single_H; //电池单体低电压
unsigned char BAT_Single_H_Number; //电池单体低电压位置
unsigned char BAT_Single_H_Case; //电池单体低电压箱号



unsigned int  DCDC_Voltage; //DCDC输出电压
unsigned int  DCDC_Current; //DCDC输出电流
unsigned char DCDC_Status; //DCDC工作状态
unsigned char DCDC_Output_Cut; //DCDC 输出切断
unsigned char DCDC_intput_Cut; //DCDC 输入切断
unsigned char DCDC_Fault_Code; //DCDC故障码
unsigned char DCDC_Level_Fault; //DCDC故障等级
unsigned char DCDC_Temp_Warn; //DCDC温度报警
unsigned char DCDC_Fault_Warn; //DCDC故障报警
unsigned char DCDC_Temp = 40; //DCDC模块温度

unsigned int  OILDCAC_U_Voltage; //DCAC输出U电压
unsigned int  OILDCAC_U_Current; //DCAC输出U电流
unsigned char OILDCAC_Status; //DCAC工作状态
unsigned char OILDCAC_Output_Cut; //DCAC 输出切断
unsigned char OILDCAC_intput_Cut; //DCAC 输入切断
unsigned char OILDCAC_Fault_Code; //DCAC故障代码
unsigned char OILDCAC_Level_Fault; //DCAC故障等级
unsigned char OILDCAC_Temp_Warn; //DCDC温度报警
unsigned char OILDCAC_Fault_Warn; //DCDC故障报警
unsigned char OILDCAC_Temp = 40; //DCAC模块温度

unsigned int  OILDCAC_V_Voltage; //DCAC输出V电压
unsigned int  OILDCAC_V_Current; //DCAC输出V电流
unsigned int  OILDCAC_W_Voltage; //DCAC输出W电压
unsigned int  OILDCAC_W_Current; //DCAC输出W电流

unsigned int  AIRDCAC_U_Voltage; //DCAC输出U电压
unsigned int  AIRDCAC_U_Current; //DCAC输出U电流
unsigned char AIRDCAC_Status; //DCAC工作状态
unsigned char AIRDCAC_Output_Cut; //DCAC 输出切断
unsigned char AIRDCAC_intput_Cut; //DCAC 输入切断
unsigned char AIRDCAC_Fault_Code; //DCAC故障代码
unsigned char AIRDCAC_Level_Fault; //DCAC故障等级
unsigned char AIRDCAC_Temp_Warn; //DCDC温度报警
unsigned char AIRDCAC_Fault_Warn; //DCDC故障报警
unsigned char AIRDCAC_Temp = 40; //DCAC模块温度

unsigned int  AIRDCAC_V_Voltage; //DCAC输出V电压
unsigned int  AIRDCAC_V_Current; //DCAC输出V电流
unsigned int  AIRDCAC_W_Voltage; //DCAC输出W电压
unsigned int  AIRDCAC_W_Current; //DCAC输出W电流

DATA_DOUBLE ICS01_Status1; //高压采集状态1
DATA_DOUBLE ICS01_Status2; //高压采集状态2
DATA_DOUBLE ICS01_Status3; //高压采集状态3
DATA_DOUBLE ICS01_Status4; //高压采集状态4

unsigned int  ICS01_Voltage_Front; //前端电压
unsigned int  ICS01_Voltage_Rear; //后端电压
unsigned char ICS01_LIFE; //LIFE




/*******************************************************************************/
typedef void (*pCanAnalyse)(can_msg_t *msg, can_pro_way_e way);

void app_init_can(void);

void app_can_sent_task(void);

void app_can_lost_time_cnt_100ms(void);

extern can_signal_t can0;

extern U16 can_id_100017EF_lost_timecnt;
extern U16 can_id_1801FBEF_lost_timecnt;
extern U16 can_id_1811FBEF_lost_timecnt;
extern U16 can_id_18FF08F2_lost_timecnt;
extern U16 can_id_10F8159E_lost_timecnt;
extern U16 can_id_10F8169E_lost_timecnt;
extern U16 can_id_18F8179E_lost_timecnt;
extern U16 can_id_18F8189E_lost_timecnt;
extern U16 can_id_18F8199E_lost_timecnt;
extern U16 can_id_18F81A9E_lost_timecnt;
extern U16 can_id_18FF12F7_lost_timecnt;
extern U16 can_id_18FF0AF8_lost_timecnt;
extern U16 can_id_18FF0BF8_lost_timecnt;
extern U16 can_id_18FF0CF9_lost_timecnt;
extern U16 can_id_18FF0DF9_lost_timecnt;
extern U16 can_id_1801EFA9_lost_timecnt;
extern U16 can_id_104C1000_lost_timecnt;

//bat vol and temp
extern U16  BAT_Cell_Voltage[400];
extern U8   BAT_Cell_Temperature[128];


#endif
