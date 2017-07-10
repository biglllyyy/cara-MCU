#ifndef    __APP_CAN_H__
#define    __APP_CAN_H__

#include  "mid_can.h"

#define    MSG_BOX_ALl				23

#define		CAN_LOST_TIME			25

typedef enum {
	/*BMS CAN ID*/
	CAN_ID_18AA28F3,
	CAN_ID_1818D0F3,
	CAN_ID_1819D0F3,
	CAN_ID_181AD0F3,
	CAN_ID_181BD0F3,
	CAN_ID_181CD0F3,
	CAN_ID_181DD0F3,
	CAN_ID_181ED0F3,
	CAN_ID_181FD0F3,
	/*BMS Interaction*/
	CAN_ID_1800F328,  //send
	CAN_ID_180028F3,
	CAN_ID_180028F4,
	/*电机*/
	CAN_ID_1882BBAB,  //send
	CAN_ID_1002FF1E,
	CAN_ID_1003FF1E,
	CAN_ID_1004FF1E,
	/*气泵*/
	//CAN_ID_18FF1103,
	CAN_ID_1429289B,
	CAN_ID_142A289B,
	CAN_ID_18AB9B28, //send
	/*仪表与远程通讯*/
	CAN_ID_18FFA017,  //send
	/*DC-DC电源*/
	CAN_ID_1828272B,
	/*油泵控制器*/
	CAN_ID_142A2895,
	CAN_ID_ALL
} MSG_ID_TYPE;

typedef enum {
	CAN_ID_18AA28F3_PERIOD = 2000, /*Period:2000ms*/
	CAN_ID_1818D0F3_PERIOD = 1000, /*Period:1000ms*/
	CAN_ID_1819D0F3_PERIOD = 1000, /*Period:1000ms*/
	CAN_ID_181AD0F3_PERIOD = 1000, /*Period:1000ms*/
	CAN_ID_181BD0F3_PERIOD = 1000, /*Period:1000ms*/
	CAN_ID_181CD0F3_PERIOD = 1000, /*Period:1000ms*/
	CAN_ID_181DD0F3_PERIOD = 1000, /*Period:1000ms*/
	CAN_ID_181ED0F3_PERIOD = 1000, /*Period:1000ms*/
	CAN_ID_181FD0F3_PERIOD = 1000, /*Period:1000ms*/
	CAN_ID_1800F328_PERIOD = 100, /*Period:100ms*/
	CAN_ID_180028F3_PERIOD = 100, /*Period:100ms*/
	CAN_ID_180028F4_PERIOD = 100, /*Period:100ms*/
	CAN_ID_1882BBAB_PERIOD = 100, /*Period:100ms*/
	CAN_ID_1002FF1E_PERIOD = 100, /*Period:100ms*/
	CAN_ID_1003FF1E_PERIOD = 100, /*Period:100ms*/
	CAN_ID_1004FF1E_PERIOD = 100, /*Period:100ms*/
	CAN_ID_1429289B_PERIOD = 100, /*Period:100ms*/
	CAN_ID_142A289B_PERIOD = 100, /*Period:100ms*/
	CAN_ID_18AB9B28_PERIOD = 100, /*Period:100ms*/
	CAN_ID_18FFA017_PERIOD = 100, /*Period:100ms*/
	CAN_ID_1828272B_PERIOD = 100, /*Period:100ms*/
	CAN_ID_142A2895_PERIOD = 100, /*Period:100ms*/
} CAN_ID_PERIOD;

typedef struct {
	/*can msg receive*/
	struct {
		U8 u8ParamType; /* 1--参数类型（1） */
		U8 u8BattBoxCnt; /* 电池箱体个数（单位:1个） */
		U8 u8BattSubControlCnt; /* 电池管理系统从控单元个数（单位:1个） */
		U8 u8BSTotalStringHigh; /* 电池系统总串数（高字节） */
		U8 u8BSTotalStringLow; /* 电池系统总串数（低字节） */
		U8 u8BMSNumberHigh; /* 电池管理系统编号高字节 */
		U8 u8BMSNumberLow; /* 电池管理系统编号低字节 */
		U8 u8RelayStatus; /* 放电继电器状态  */
	} ID_18AA28F3; /* BMS -LCD01 */
#if 0
	struct  //参数报文名称：BMS -LCD01
	{
		U8 DataType2;
		U8 NumOfBUM;
		U8 NumOfBatterySingle;
		U8 NumOfBatteryTemperatureDetect;
		U8 Reserved1;
		U8 Reserved2;
		U8 Reserved3;
		U8 Reserved4;
	}ID_18AA28F3_datatype2;
#endif
	struct {
		U8 u8TotalVolHigh;
		U8 u8TotalVolLow;
		U8 u8TotalCurrHigh;
		U8 u8TotalCurrLow;
		U8 u8SOC;
		U8 u8BattSysLife; /* 电池管理系统的 LIFE(0~255)  */
		U8 u8BattStatusFlag1; /* 电池 Status_Flag1  */
		U8 u8BattStatusFlag2; /* 电池 Status_Flag2  */
	} ID_1818D0F3; /* BMS -CAN2_B1 */
	struct {
		U8 u8HighestVolHigh; /* 最高单体电池电压高字节   */
		U8 u8HighestVolLow; /* 最高单体电池电压高字节   */
		U8 u8LowestVolHigh; /* 最低单体电池电压高字节   */
		U8 u8LowestVolLow; /* 最低单体电池电压低字节   */
		U8 u8HighestTemp; /* 电池最高温度   */
		U8 u8LowestTemp; /* 电池最低温度   */
		U8 u8BattStatusFlag3; /* 电池 Status_Flag3  */
		U8 u8RelayStatusFlag3; /* 继电器Status_Flag4  */
	} ID_1819D0F3; /* BMS -CAN2_B2 */
	struct {
		U8 u8HighestVolBoxNum; /* 最高单体电压所在箱号（1~N） */
		U8 u8HighestVolPosition; /* 最高单体电压所在箱内位置 */
		U8 u8LowestVolBoxNum; /* 最低单体电压所在箱号（1~N） */
		U8 u8LowestVolPosition; /* 最低单体电压所在箱内位置 */
		U8 u8HighestTempBoxNum; /* 最高温度所在箱号（1~N） */
		U8 u8HighestTempPosition; /* 最高温度所在箱内位置 */
		U8 u8LowestTempBoxNum; /* 最低温度电压所在箱号（1~N） */
		U8 u8LowestTempPosition; /* 最低温度所在箱内位置 */
	} ID_181AD0F3; /* BMS -CAN2_B3 */
	struct  //报文名称：BMSCAN2_B4
	{
		U8 u8BMUCommStatus1to8; /* 1~8号BMU 通讯状态 */
		U8 u8BMUCommStatus9to16; /* 9~16号BMU 通讯状态 */
		U8 u8BMUCommStatus17to24; /* 17~24号BMU 通讯状态 */
		U8 u8BMUCommStatus25to32;/* 25~32号BMU 通讯状态 */
		U8 Reserved1;
		U8 Reserved2;
		U8 Reserved3;
		U8 Reserved4;
	} ID_181BD0F3;
	struct  //报文名称：BMSCAN2_B5
	{
		U8 BalancedStatusOf1to8BMU;
		U8 BalancedStatusOf9to16BMU;
		U8 BalancedStatusOf17to24BMU;
		U8 BalancedStatusOf25to32BMU;
		U8 Reserved1;
		U8 Reserved2;
		U8 Reserved3;
		U8 Reserved4;
	} ID_181CD0F3;
	struct  //报文名称：BMSCAN2_B6
	{
		U8 ChargingPlug1DCPTemperature;
		U8 ChargingPlug1DCNTemperature;
		U8 ChargingPlug2DCPTemperature;
		U8 ChargingPlug2DCNTemperature;
		U8 PositiveInsulationResistanceH;
		U8 PositiveInsulationResistanceL;
		U8 NegativeInsulationResistanceH;
		U8 NegativeInsulationResistanceL;
	} ID_181DD0F3;
	struct  //报文名称：BMSCAN2_B7
	{
		U8 ResidualEnergyH;
		U8 ResidualEnergyL;
		U8 Status_Flag5;
		U8 Status_Flag6;
		U8 Reserved1;
		U8 Reserved2;
		U8 Reserved3;
		U8 Reserved4;
	} ID_181ED0F3;
	struct  //报文名称：BMSCAN2_B8
	{
		U8 HighSingleVoltPos;
		U8 LowSingleVoltPos;
		U8 MaxTemperPos;
		U8 MinTemperPos;
		U8 HighSingleVoltPackNum;
		U8 LowSingleVoltPackNum;
		U8 MaxTemperPackNum;
		U8 MinTemperPackNum;
	} ID_181FD0F3;

	/*can interactive msg send  */

	struct	//报文名称：LCD_BMS  仪表单体信息请求命令  ID：0x1800F328
	{
		U8 NumOfBMU;
		U8 Reserved1;
		U8 Reserved2;
		U8 Reserved3;
		U8 Reserved4;
		U8 Reserved5;
		U8 Reserved6;
		U8 Reserved7;
	} ID_1800F328;

	struct	//报文名称：BMS -LCD02  反馈单体电压信息数据帧  ID：0x180028F3
	{
		U8 NumOfBMU;
		U8 PackgeN;
		U8 NBMUVolt1H;
		U8 NBMUVolt1L;
		U8 NBMUVolt2H;
		U8 NBMUVolt2L;
		U8 NBMUVolt3H;
		U8 NBMUVolt3L;
	} ID_180028F3;
	struct	//报文名称：BCTDT1  反馈温度信息数据帧  ID：0x180028F4
	{
		U8 NumOfBMU;
		U8 PackgeN;
		U8 NBMUTempe1;
		U8 NBMUTempe2;
		U8 NBMUTempe3;
		U8 NBMUTempe4;
		U8 NBMUTempe5;
		U8 NBMUTempe6;
	} ID_180028F4;

	/*---------------- 电机 -----------------------*/
	struct  //整车信息（仪表发送）
	{
		U8 VelStatusInfo;
		U8 MaxTempeOfBat;
		U8 SingleLowestVolt;
		U8 SingleHighestVolt;
		U8 Reserved1;
		U8 Reserved2;
		U8 Reserved3;
		U8 Reserved4;
	} ID_1882BBAB;

	struct {
		U8 u8MotorContStatus; /* 电机控制器基本状态 */
		U8 u8TickSignal; /* 生命（心跳）信号  */
		U16 u16MotorTorque; /* 电机实际转矩   */
		U16 u16MotorSpeed; /* 电机实际转速   */
		U8 u8VehicleSpeed; /* 车速    */
		U8 u8Gear; /* 档位   */
	} ID_1002FF1E; /* 电机运行数据一 */

	struct {
		U16 u16DMCBusVol; /* 电机控制器（DMC）母线电压 */
		U16 u16DMCBusCurr; /* 电机控制器（DMC）母线电流  */
		U8 u8DMCTemp; /* 电机控制器温度   */
		U8 u8MotorTemp; /* 电机温度   */
		U8 u8DMCWarnCode; /* 电机控制器故障代码    */
		U8 u8DMCVerNum; /* 电机控制器版本号   */
	} ID_1003FF1E; /* 电机运行数据二 */

	struct {
		U8 u8ThrottleOpening; /* 油门开度 */
		U8 u8BrakePedalOpening; /*  制动踏板开度   */
		U8 u8ChargeDischargeState; /* 充放电状态    */
		U8 RemoteMonitoringNationalStandardStalls; /* 远程监控国标档位  */
		U8 Reserved1;
		U8 Reserved2;
		U8 Reserved3;
		U8 Reserved4;
	} ID_1004FF1E; /* 电机运行数据三 */

	/*---------------- 气泵控制器 -----------------------*/
#if 0 //这个不需要仪表接收
	struct {
		U8 u8APMStartStopInstruction; /*气泵启停指令*/
		U8 Reserved1;
		U8 Reserved2;
		U8 Reserved3;
		U8 Reserved4;
		U8 Reserved5;
		U8 Reserved6;
		U8 Reserved7;
	}CANID_0x18FF1103;
#endif
	struct {
		U8 u8APMotorSpeedLow; /* 气泵电机转速低位 */
		U8 u8APMotorSpeedHigh; /* 气泵电机转速高位  */
		U8 u8ControllerBusVotLow; /* 预留   */
		U8 u8ControllerBusVotHigh; /* 预留   */
		U8 u8ControllerCurrent; /* 预留   */
		U8 u8APControllerTemp; /*  控制器温度（此处给出的是泵体温度）*/
		U8 u8APTickSignal; /*  心跳信号 */
		U8 u8APControllerStatus; /* 控制器基本状态   */
	} ID_1429289B; /* 气泵电机运行数据一 */

	struct {
		U8 u8APMControlerTemp; /*气泵控制器温度*/
		U8 Reserved1;
		U8 Reserved2;
		U8 Reserved3;
		U8 Reserved4;
		U8 Reserved5;
		U8 Reserved6;
		U8 Reserved7;
	} ID_142A289B; /* 气泵电机运行数据二*/

	struct {
		U8 Reserved1; /*气泵控制器温度*/
		U8 FrontGasStorePreValue;  //前储气筒气压值 0.01MPa/Bit,最大100
		U8 BehindGasStorePreValue; //Byte3 后储气筒气压值
		U8 Reserved2;
		U8 Reserved3;
		U8 Reserved4;
		U8 Reserved5;
		U8 Reserved6;
	} ID_18AB9B28; /* 仪表专用通讯指令*/

	/*---------------- 仪表与远程监控终端 -----------------------*/
	struct {
		U8 TotalMileageL; /*气泵控制器温度*/
		U8 TotalMileageM;  //前储气筒气压值 0.01MPa/Bit,最大100
		U8 TotalMileageH; //Byte3 后储气筒气压值
		U8 Reserved1;
		U8 Reserved2;
		U8 Reserved3;
		U8 Reserved4;
		U8 Reserved5;
	} ID_18FFA017; /* LCD_CANBUS1*/

	/*---------------- DC-DC电源 -----------------------*/
	struct {
		U8 u8DCDCSysStatus; /* DC-DC系统状态 */
		U8 u8DCDCTemp; /* DCDC模块温度  */
		U16 u16DCDCVolOutput; /* DCDC输出电压   */
		U16 u16DCDCCurrOutput; /* DCDC输出电流   */
		U8 u8DCDCWarnCode; /* DCDC故障代码   */
		U8 u8DCDCCANVerCode; /* DCDC CAN通讯协议版本号   */
	} ID_1828272B; /* DC-DC_msg1 */

	/*---------------- 油泵控制器 -----------------------*/
	struct {
		U8 u8DMYTDMotorSpeedLow; /* 油泵电机转速低位 */
		U8 u8DMYTDMotorSpeedHigh; /* 油泵电机转速高位 */
		U8 u8DMYTDBusVolLow; /* 控制器母线电压低位 */
		U8 u8DMYTDBusVolHigh; /* 控制器母线电压高位 */
		U8 u8DMYTDCurrent; /* 控制器电流 */
		U8 u8DMYTDTemp; /*  控制器温度（此处给出的是泵体温度）*/
		U8 u8DMYTDTickSignal; /*  心跳信号 */
		U8 u8DMYTDStatus; /* 控制器基本状态   */
	} ID_142A2895; /* 油泵发送信息 */

} can_signal_t;

/*******************************************************************************/
typedef void (*pCanAnalyse)(can_msg_t *msg, can_pro_way_e way);

void app_init_can(void);

void app_can_sent_task(void);

void app_can_lost_time_cnt_100ms(void);

extern can_signal_t can0;

extern U16 can_id_18AA28F3_lost_timecnt;
extern U16 can_id_1818D0F3_lost_timecnt;
extern U16 can_id_1819D0F3_lost_timecnt;
extern U16 can_id_181AD0F3_lost_timecnt;
extern U16 can_id_181BD0F3_lost_timecnt;
extern U16 can_id_181CD0F3_lost_timecnt;
extern U16 can_id_181DD0F3_lost_timecnt;
extern U16 can_id_181ED0F3_lost_timecnt;
extern U16 can_id_181FD0F3_lost_timecnt;
extern U16 can_id_1800F328_lost_timecnt;
extern U16 can_id_180028F3_lost_timecnt;
extern U16 can_id_180028F4_lost_timecnt;
extern U16 can_id_1882BBAB_lost_timecnt;
extern U16 can_id_1002FF1E_lost_timecnt;
extern U16 can_id_1003FF1E_lost_timecnt;
extern U16 can_id_1004FF1E_lost_timecnt;
extern U16 can_id_1429289B_lost_timecnt;
extern U16 can_id_142A289B_lost_timecnt;
extern U16 can_id_18AB9B28_lost_timecnt;
extern U16 can_id_18FFA017_lost_timecnt;
extern U16 can_id_1828272B_lost_timecnt;
extern U16 can_id_142A2895_lost_timecnt;

#endif
