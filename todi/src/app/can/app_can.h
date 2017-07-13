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
        U8 bit1 : 1;
        U8 bit2 : 1;
        U8 bit3 : 1;
        U8 bit4 : 1;
        U8 bit5 : 1;
        U8 bit6 : 1;
        U8 bit7 : 1;
        U8 bit8 : 1;
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

extern DATA_BIT VCU_Status_Flag1; //状态1
#define  CHARGE      VCU_Status_Flag1.bits.bit1
#define  DCDC_EN     VCU_Status_Flag1.bits.bit2
#define  BMS_OFFLINE VCU_Status_Flag1.bits.bit3
#define  TM_OFFLINE  VCU_Status_Flag1.bits.bit4
#define  DCAC_EN     VCU_Status_Flag1.bits.bit5
#define  SPEED_EN    VCU_Status_Flag1.bits.bit6
#define  BRAJE_EN    VCU_Status_Flag1.bits.bit7
#define  ELE_BRAKE   VCU_Status_Flag1.bits.bit8

extern U8 VCU_Gear; //档位

extern DATA_BIT VCU_Mode; //整车模式

extern U8 VCU_Life; //整车控制器LIFE

extern DATA_BIT VCU_Status_Flag2;
#define  READY       VCU_Status_Flag2.bits.bit6
#define  CHARGE_OVERTIME VCU_Status_Flag2.bits.bit7  
#define  VCU_FAULT   VCU_Status_Flag2.bits.bit8 

extern U8 VCU_Code; //整车故障码

extern U8 VCU_TM_Brake_percent; //制动踏板开度
extern U8 VCU_TM_Speed_percent; //加速踏板开度

// VCU2TERMINAL_MOTOR01
extern U8 TM_Number; //电机个数
extern U8 TM_Serial; //电机序号
extern U8 TM_WorkStatus; //TM电机工作状态
extern U8 TM_Control_Temp; //TM电机控制器温度
extern U16 TM_Feedback_RPM; //TM电机反馈转速
extern U16 TM_Feedback_NM; //TM电机反馈扭矩
// VCU2TERMINAL_MOTOR02
extern U8 TM_Temp; //TM电机温度
extern U16 TM_Voltage_DC; //TM电机直流电压
extern U16 TM_Current_DC; //TM电机直流电流


//IRM
extern U32 IRM_Ohm_Positive; //总正绝缘阻值
extern U32 IRM_Ohm_Negative; //总负绝缘阻值
extern U8 IRM_Fault_Level; //设备故障等级
extern U8 IRM_Insulation_Level; //绝缘等级
extern U8 IRM_Life; //绝缘LIFE



extern U8 BMS_Mode; //电池充放电模式
extern U8 BMS_Status; //电池状态
extern U8 BAT_Temp_Average; //电池平均温度
extern U8 BMS_SOC; //SOC
extern U16 BMS_Current; //电池系统电流
extern U16 BMS_Voltage; //电池系统内总电压
extern U8 BMS_Kt; //高压继电器状态

extern U8 BAT_Temp_H_Limit; //最高允许电池单体温度
extern U8 BAT_Temp_L_Limit; //最低允许电池单体温度
extern U8 BMS_SOC_L_Limit; //最低允许SOC值
extern U16 BAT_Current_Discharge_Limit; //最高允许放电电流
extern U16 BAT_Current_Charge_Limit; //最高允许充电电流

extern U8 BAT_Temp_L; //电池单体最低温度
extern U8 BAT_Temp_L_Number; //电池单体低温度序号
extern U8 BAT_Temp_L_Case; //电池单体低温度箱号
extern U8 BAT_Temp_H; //电池单体最高温度
extern U8 BAT_Temp_H_Number; //电池单体高温度序号
extern U8 BAT_Temp_H_Case; //电池单体高温度箱号

extern U8 BAT_Voltage_Fault; //总压故障
extern U8 BAT_Single_Fault; //单压故障
extern U8 BAT_Temp_Fault; //温度故障
extern U8 BAT_Insulation_Fault; //绝缘故障
extern U8 BAT_Consistency_Fault; //电池一致性故障
extern U8 BAT_SOC_Fault; //SOC故障
extern U8 BAT_Current_Fault; //电流故障
extern U8 BAT_Lock_Fault; //高压互锁故障
extern U8 BMS_Comm_Fault; //BMS通讯故障
extern U8 BMS_System_Fault; //BMS系统故障

extern U16 BMS_Ohm; //绝缘阻值
extern U8 BMS_Number; //单体电压数量
extern U8 BMS_Temp_Number; //单体温度数量
extern U8 BMS_System_Unit; //系统数量
extern U8 BMS_System_Number; //系统号

extern U16 BAT_Single_L; //电池单体低电压
extern U8 BAT_Single_L_Number; //电池单体低电压位置
extern U8 BAT_Single_L_Case; //电池单体低电压箱号

extern U16 BAT_Single_H; //电池单体高电压
extern U8 BAT_Single_H_Number; //电池单体高电压位置
extern U8 BAT_Single_H_Case; //电池单体高电压箱号

extern U16 BAT_Cell_Voltage[400];
extern U8 BAT_Cell_Temperature[128];

extern U16 DCDC_Voltage; //DCDC输出电压
extern U16 DCDC_Current; //DCDC输出电流
extern U8 DCDC_Status; //DCDC工作状态
extern U8 DCDC_Output_Cut; //DCDC 输出切断
extern U8 DCDC_intput_Cut; //DCDC 输入切断
extern U8 DCDC_Fault_Code; //DCDC故障码
extern U8 DCDC_Level_Fault; //DCDC故障等级
extern U8 DCDC_Temp_Warn; //DCDC温度报警
extern U8 DCDC_Fault_Warn; //DCDC故障报警
extern U8 DCDC_Temp; //DCDC模块温度

extern U16 OILDCAC_U_Voltage; //DCAC输出U电压
extern U16 OILDCAC_U_Current; //DCAC输出U电流
extern U8 OILDCAC_Status; //DCAC工作状态
extern U8 OILDCAC_Output_Cut; //DCAC 输出切断
extern U8 OILDCAC_intput_Cut; //DCAC 输入切断
extern U8 OILDCAC_Fault_Code; //DCAC故障代码
extern U8 OILDCAC_Level_Fault; //DCAC故障等级
extern U8 OILDCAC_Temp_Warn; //DCDC温度报警
extern U8 OILDCAC_Fault_Warn; //DCDC故障报警
extern U8 OILDCAC_Temp; //DCAC模块温度

extern U16 OILDCAC_V_Voltage; //DCAC输出V电压
extern U16 OILDCAC_V_Current; //DCAC输出V电流
extern U16 OILDCAC_W_Voltage; //DCAC输出W电压
extern U16 OILDCAC_W_Current; //DCAC输出W电流

extern U16 AIRDCAC_U_Voltage; //DCAC输出U电压
extern U16 AIRDCAC_U_Current; //DCAC输出U电流
extern U8 AIRDCAC_Status; //DCAC工作状态
extern U8 AIRDCAC_Output_Cut; //DCAC 输出切断
extern U8 AIRDCAC_intput_Cut; //DCAC 输入切断
extern U8 AIRDCAC_Fault_Code; //DCAC故障代码
extern U8 AIRDCAC_Level_Fault; //DCAC故障等级
extern U8 AIRDCAC_Temp_Warn; //DCDC温度报警
extern U8 AIRDCAC_Fault_Warn; //DCDC故障报警
extern U8 AIRDCAC_Temp; //DCAC模块温度

extern U16 AIRDCAC_V_Voltage; //DCAC输出V电压
extern U16 AIRDCAC_V_Current; //DCAC输出V电流
extern U16 AIRDCAC_W_Voltage; //DCAC输出W电压
extern U16 AIRDCAC_W_Current; //DCAC输出W电流

extern DATA_DOUBLE ICS01_Status1; //高压采集状态1
extern DATA_DOUBLE ICS01_Status2; //高压采集状态2
extern DATA_DOUBLE ICS01_Status3; //高压采集状态3
extern DATA_DOUBLE ICS01_Status4; //高压采集状态4

extern U16 ICS01_Voltage_Front; //前端电压
extern U16 ICS01_Voltage_Rear; //后端电压
extern U8 ICS01_LIFE; //LIFE




/*******************************************************************************/
typedef void (*pCanAnalyse)(can_msg_t *msg, can_pro_way_e way);

void app_init_can(void);

void app_can_sent_task(void);

void app_can_lost_time_cnt_100ms(void);

extern can_signal_t can0;






#endif
