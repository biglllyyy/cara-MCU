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
	/*���*/
	CAN_ID_1882BBAB,  //send
	CAN_ID_1002FF1E,
	CAN_ID_1003FF1E,
	CAN_ID_1004FF1E,
	/*����*/
	//CAN_ID_18FF1103,
	CAN_ID_1429289B,
	CAN_ID_142A289B,
	CAN_ID_18AB9B28, //send
	/*�Ǳ���Զ��ͨѶ*/
	CAN_ID_18FFA017,  //send
	/*DC-DC��Դ*/
	CAN_ID_1828272B,
	/*�ͱÿ�����*/
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
		U8 u8ParamType; /* 1--�������ͣ�1�� */
		U8 u8BattBoxCnt; /* ��������������λ:1���� */
		U8 u8BattSubControlCnt; /* ��ع���ϵͳ�ӿص�Ԫ��������λ:1���� */
		U8 u8BSTotalStringHigh; /* ���ϵͳ�ܴ��������ֽڣ� */
		U8 u8BSTotalStringLow; /* ���ϵͳ�ܴ��������ֽڣ� */
		U8 u8BMSNumberHigh; /* ��ع���ϵͳ��Ÿ��ֽ� */
		U8 u8BMSNumberLow; /* ��ع���ϵͳ��ŵ��ֽ� */
		U8 u8RelayStatus; /* �ŵ�̵���״̬  */
	} ID_18AA28F3; /* BMS -LCD01 */
#if 0
	struct  //�����������ƣ�BMS -LCD01
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
		U8 u8BattSysLife; /* ��ع���ϵͳ�� LIFE(0~255)  */
		U8 u8BattStatusFlag1; /* ��� Status_Flag1  */
		U8 u8BattStatusFlag2; /* ��� Status_Flag2  */
	} ID_1818D0F3; /* BMS -CAN2_B1 */
	struct {
		U8 u8HighestVolHigh; /* ��ߵ����ص�ѹ���ֽ�   */
		U8 u8HighestVolLow; /* ��ߵ����ص�ѹ���ֽ�   */
		U8 u8LowestVolHigh; /* ��͵����ص�ѹ���ֽ�   */
		U8 u8LowestVolLow; /* ��͵����ص�ѹ���ֽ�   */
		U8 u8HighestTemp; /* �������¶�   */
		U8 u8LowestTemp; /* �������¶�   */
		U8 u8BattStatusFlag3; /* ��� Status_Flag3  */
		U8 u8RelayStatusFlag3; /* �̵���Status_Flag4  */
	} ID_1819D0F3; /* BMS -CAN2_B2 */
	struct {
		U8 u8HighestVolBoxNum; /* ��ߵ����ѹ������ţ�1~N�� */
		U8 u8HighestVolPosition; /* ��ߵ����ѹ��������λ�� */
		U8 u8LowestVolBoxNum; /* ��͵����ѹ������ţ�1~N�� */
		U8 u8LowestVolPosition; /* ��͵����ѹ��������λ�� */
		U8 u8HighestTempBoxNum; /* ����¶�������ţ�1~N�� */
		U8 u8HighestTempPosition; /* ����¶���������λ�� */
		U8 u8LowestTempBoxNum; /* ����¶ȵ�ѹ������ţ�1~N�� */
		U8 u8LowestTempPosition; /* ����¶���������λ�� */
	} ID_181AD0F3; /* BMS -CAN2_B3 */
	struct  //�������ƣ�BMSCAN2_B4
	{
		U8 u8BMUCommStatus1to8; /* 1~8��BMU ͨѶ״̬ */
		U8 u8BMUCommStatus9to16; /* 9~16��BMU ͨѶ״̬ */
		U8 u8BMUCommStatus17to24; /* 17~24��BMU ͨѶ״̬ */
		U8 u8BMUCommStatus25to32;/* 25~32��BMU ͨѶ״̬ */
		U8 Reserved1;
		U8 Reserved2;
		U8 Reserved3;
		U8 Reserved4;
	} ID_181BD0F3;
	struct  //�������ƣ�BMSCAN2_B5
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
	struct  //�������ƣ�BMSCAN2_B6
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
	struct  //�������ƣ�BMSCAN2_B7
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
	struct  //�������ƣ�BMSCAN2_B8
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

	struct	//�������ƣ�LCD_BMS  �Ǳ�����Ϣ��������  ID��0x1800F328
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

	struct	//�������ƣ�BMS -LCD02  ���������ѹ��Ϣ����֡  ID��0x180028F3
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
	struct	//�������ƣ�BCTDT1  �����¶���Ϣ����֡  ID��0x180028F4
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

	/*---------------- ��� -----------------------*/
	struct  //������Ϣ���Ǳ��ͣ�
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
		U8 u8MotorContStatus; /* �������������״̬ */
		U8 u8TickSignal; /* �������������ź�  */
		U16 u16MotorTorque; /* ���ʵ��ת��   */
		U16 u16MotorSpeed; /* ���ʵ��ת��   */
		U8 u8VehicleSpeed; /* ����    */
		U8 u8Gear; /* ��λ   */
	} ID_1002FF1E; /* �����������һ */

	struct {
		U16 u16DMCBusVol; /* �����������DMC��ĸ�ߵ�ѹ */
		U16 u16DMCBusCurr; /* �����������DMC��ĸ�ߵ���  */
		U8 u8DMCTemp; /* ����������¶�   */
		U8 u8MotorTemp; /* ����¶�   */
		U8 u8DMCWarnCode; /* ������������ϴ���    */
		U8 u8DMCVerNum; /* ����������汾��   */
	} ID_1003FF1E; /* ����������ݶ� */

	struct {
		U8 u8ThrottleOpening; /* ���ſ��� */
		U8 u8BrakePedalOpening; /*  �ƶ�̤�忪��   */
		U8 u8ChargeDischargeState; /* ��ŵ�״̬    */
		U8 RemoteMonitoringNationalStandardStalls; /* Զ�̼�ع��굵λ  */
		U8 Reserved1;
		U8 Reserved2;
		U8 Reserved3;
		U8 Reserved4;
	} ID_1004FF1E; /* ������������� */

	/*---------------- ���ÿ����� -----------------------*/
#if 0 //�������Ҫ�Ǳ����
	struct {
		U8 u8APMStartStopInstruction; /*������ָͣ��*/
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
		U8 u8APMotorSpeedLow; /* ���õ��ת�ٵ�λ */
		U8 u8APMotorSpeedHigh; /* ���õ��ת�ٸ�λ  */
		U8 u8ControllerBusVotLow; /* Ԥ��   */
		U8 u8ControllerBusVotHigh; /* Ԥ��   */
		U8 u8ControllerCurrent; /* Ԥ��   */
		U8 u8APControllerTemp; /*  �������¶ȣ��˴��������Ǳ����¶ȣ�*/
		U8 u8APTickSignal; /*  �����ź� */
		U8 u8APControllerStatus; /* ����������״̬   */
	} ID_1429289B; /* ���õ����������һ */

	struct {
		U8 u8APMControlerTemp; /*���ÿ������¶�*/
		U8 Reserved1;
		U8 Reserved2;
		U8 Reserved3;
		U8 Reserved4;
		U8 Reserved5;
		U8 Reserved6;
		U8 Reserved7;
	} ID_142A289B; /* ���õ���������ݶ�*/

	struct {
		U8 Reserved1; /*���ÿ������¶�*/
		U8 FrontGasStorePreValue;  //ǰ����Ͳ��ѹֵ 0.01MPa/Bit,���100
		U8 BehindGasStorePreValue; //Byte3 ����Ͳ��ѹֵ
		U8 Reserved2;
		U8 Reserved3;
		U8 Reserved4;
		U8 Reserved5;
		U8 Reserved6;
	} ID_18AB9B28; /* �Ǳ�ר��ͨѶָ��*/

	/*---------------- �Ǳ���Զ�̼���ն� -----------------------*/
	struct {
		U8 TotalMileageL; /*���ÿ������¶�*/
		U8 TotalMileageM;  //ǰ����Ͳ��ѹֵ 0.01MPa/Bit,���100
		U8 TotalMileageH; //Byte3 ����Ͳ��ѹֵ
		U8 Reserved1;
		U8 Reserved2;
		U8 Reserved3;
		U8 Reserved4;
		U8 Reserved5;
	} ID_18FFA017; /* LCD_CANBUS1*/

	/*---------------- DC-DC��Դ -----------------------*/
	struct {
		U8 u8DCDCSysStatus; /* DC-DCϵͳ״̬ */
		U8 u8DCDCTemp; /* DCDCģ���¶�  */
		U16 u16DCDCVolOutput; /* DCDC�����ѹ   */
		U16 u16DCDCCurrOutput; /* DCDC�������   */
		U8 u8DCDCWarnCode; /* DCDC���ϴ���   */
		U8 u8DCDCCANVerCode; /* DCDC CANͨѶЭ��汾��   */
	} ID_1828272B; /* DC-DC_msg1 */

	/*---------------- �ͱÿ����� -----------------------*/
	struct {
		U8 u8DMYTDMotorSpeedLow; /* �ͱõ��ת�ٵ�λ */
		U8 u8DMYTDMotorSpeedHigh; /* �ͱõ��ת�ٸ�λ */
		U8 u8DMYTDBusVolLow; /* ������ĸ�ߵ�ѹ��λ */
		U8 u8DMYTDBusVolHigh; /* ������ĸ�ߵ�ѹ��λ */
		U8 u8DMYTDCurrent; /* ���������� */
		U8 u8DMYTDTemp; /*  �������¶ȣ��˴��������Ǳ����¶ȣ�*/
		U8 u8DMYTDTickSignal; /*  �����ź� */
		U8 u8DMYTDStatus; /* ����������״̬   */
	} ID_142A2895; /* �ͱ÷�����Ϣ */

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
