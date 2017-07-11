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
	ID_104C1000, //��ص�ѹ 100֡����0x104C19A4-0x104C1A07 16֡����¶�0x104C1A18-0x104C1A24
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

extern DATA_BIT VCU_Status_Flag1; //״̬1
#define  CHARGE      VCU_Status_Flag1.bits.bit1
#define  DCDC_EN     VCU_Status_Flag1.bits.bit2
#define  BMS_OFFLINE VCU_Status_Flag1.bits.bit3
#define  TM_OFFLINE  VCU_Status_Flag1.bits.bit4
#define  DCAC_EN     VCU_Status_Flag1.bits.bit5
#define  SPEED_EN    VCU_Status_Flag1.bits.bit6
#define  BRAJE_EN    VCU_Status_Flag1.bits.bit7
#define  ELE_BRAKE   VCU_Status_Flag1.bits.bit8

extern U8 VCU_Gear; //��λ

extern DATA_BIT VCU_Mode; //����ģʽ

extern U8 VCU_Life; //����������LIFE

extern DATA_BIT VCU_Status_Flag2;
#define  READY       VCU_Status_Flag2.bits.bit6
#define  CHARGE_OVERTIME VCU_Status_Flag2.bits.bit7  
#define  VCU_FAULT   VCU_Status_Flag2.bits.bit8 

extern U8 VCU_Code; //����������

extern U8 VCU_TM_Brake_percent; //�ƶ�̤�忪��
extern U8 VCU_TM_Speed_percent; //����̤�忪��

// VCU2TERMINAL_MOTOR01
extern U8 TM_Number; //�������
extern U8 TM_Serial; //������
extern U8 TM_WorkStatus; //TM�������״̬
extern U8 TM_Control_Temp; //TM����������¶�
extern U16 TM_Feedback_RPM; //TM�������ת��
extern U16 TM_Feedback_NM; //TM�������Ť��
// VCU2TERMINAL_MOTOR02
extern U8 TM_Temp; //TM����¶�
extern U16 TM_Voltage_DC; //TM���ֱ����ѹ
extern U16 TM_Current_DC; //TM���ֱ������


//IRM
extern U32 IRM_Ohm_Positive; //������Ե��ֵ
extern U32 IRM_Ohm_Negative; //�ܸ���Ե��ֵ
extern U8 IRM_Fault_Level; //�豸���ϵȼ�
extern U8 IRM_Insulation_Level; //��Ե�ȼ�
extern U8 IRM_Life; //��ԵLIFE



extern U8 BMS_Mode; //��س�ŵ�ģʽ
extern U8 BMS_Status; //���״̬
extern U8 BAT_Temp_Average; //���ƽ���¶�
extern U8 BMS_SOC; //SOC
extern U16 BMS_Current; //���ϵͳ����
extern U16 BMS_Voltage; //���ϵͳ���ܵ�ѹ
extern U8 BMS_Kt; //��ѹ�̵���״̬

extern U8 BAT_Temp_H_Limit; //��������ص����¶�
extern U8 BAT_Temp_L_Limit; //��������ص����¶�
extern U8 BMS_SOC_L_Limit; //�������SOCֵ
extern U16 BAT_Current_Discharge_Limit; //�������ŵ����
extern U16 BAT_Current_Charge_Limit; //������������

extern U8 BAT_Temp_L; //��ص�������¶�
extern U8 BAT_Temp_L_Number; //��ص�����¶����
extern U8 BAT_Temp_L_Case; //��ص�����¶����
extern U8 BAT_Temp_H; //��ص�������¶�
extern U8 BAT_Temp_H_Number; //��ص�����¶����
extern U8 BAT_Temp_H_Case; //��ص�����¶����

extern U8 BAT_Voltage_Fault; //��ѹ����
extern U8 BAT_Single_Fault; //��ѹ����
extern U8 BAT_Temp_Fault; //�¶ȹ���
extern U8 BAT_Insulation_Fault; //��Ե����
extern U8 BAT_Consistency_Fault; //���һ���Թ���
extern U8 BAT_SOC_Fault; //SOC����
extern U8 BAT_Current_Fault; //��������
extern U8 BAT_Lock_Fault; //��ѹ��������
extern U8 BMS_Comm_Fault; //BMSͨѶ����
extern U8 BMS_System_Fault; //BMSϵͳ����

extern U16 BMS_Ohm; //��Ե��ֵ
extern U8 BMS_Number; //�����ѹ����
extern U8 BMS_Temp_Number; //�����¶�����
extern U8 BMS_System_Unit; //ϵͳ����
extern U8 BMS_System_Number; //ϵͳ��

extern U16 BAT_Single_L; //��ص���͵�ѹ
extern U8 BAT_Single_L_Number; //��ص���͵�ѹλ��
extern U8 BAT_Single_L_Case; //��ص���͵�ѹ���

extern U16 BAT_Single_H; //��ص���ߵ�ѹ
extern U8 BAT_Single_H_Number; //��ص���ߵ�ѹλ��
extern U8 BAT_Single_H_Case; //��ص���ߵ�ѹ���

extern U16 BAT_Cell_Voltage[400];
extern U8 BAT_Cell_Temperature[128];

extern U16 DCDC_Voltage; //DCDC�����ѹ
extern U16 DCDC_Current; //DCDC�������
extern U8 DCDC_Status; //DCDC����״̬
extern U8 DCDC_Output_Cut; //DCDC ����ж�
extern U8 DCDC_intput_Cut; //DCDC �����ж�
extern U8 DCDC_Fault_Code; //DCDC������
extern U8 DCDC_Level_Fault; //DCDC���ϵȼ�
extern U8 DCDC_Temp_Warn; //DCDC�¶ȱ���
extern U8 DCDC_Fault_Warn; //DCDC���ϱ���
extern U8 DCDC_Temp; //DCDCģ���¶�

extern U16 OILDCAC_U_Voltage; //DCAC���U��ѹ
extern U16 OILDCAC_U_Current; //DCAC���U����
extern U8 OILDCAC_Status; //DCAC����״̬
extern U8 OILDCAC_Output_Cut; //DCAC ����ж�
extern U8 OILDCAC_intput_Cut; //DCAC �����ж�
extern U8 OILDCAC_Fault_Code; //DCAC���ϴ���
extern U8 OILDCAC_Level_Fault; //DCAC���ϵȼ�
extern U8 OILDCAC_Temp_Warn; //DCDC�¶ȱ���
extern U8 OILDCAC_Fault_Warn; //DCDC���ϱ���
extern U8 OILDCAC_Temp; //DCACģ���¶�

extern U16 OILDCAC_V_Voltage; //DCAC���V��ѹ
extern U16 OILDCAC_V_Current; //DCAC���V����
extern U16 OILDCAC_W_Voltage; //DCAC���W��ѹ
extern U16 OILDCAC_W_Current; //DCAC���W����

extern U16 AIRDCAC_U_Voltage; //DCAC���U��ѹ
extern U16 AIRDCAC_U_Current; //DCAC���U����
extern U8 AIRDCAC_Status; //DCAC����״̬
extern U8 AIRDCAC_Output_Cut; //DCAC ����ж�
extern U8 AIRDCAC_intput_Cut; //DCAC �����ж�
extern U8 AIRDCAC_Fault_Code; //DCAC���ϴ���
extern U8 AIRDCAC_Level_Fault; //DCAC���ϵȼ�
extern U8 AIRDCAC_Temp_Warn; //DCDC�¶ȱ���
extern U8 AIRDCAC_Fault_Warn; //DCDC���ϱ���
extern U8 AIRDCAC_Temp; //DCACģ���¶�

extern U16 AIRDCAC_V_Voltage; //DCAC���V��ѹ
extern U16 AIRDCAC_V_Current; //DCAC���V����
extern U16 AIRDCAC_W_Voltage; //DCAC���W��ѹ
extern U16 AIRDCAC_W_Current; //DCAC���W����

extern DATA_DOUBLE ICS01_Status1; //��ѹ�ɼ�״̬1
extern DATA_DOUBLE ICS01_Status2; //��ѹ�ɼ�״̬2
extern DATA_DOUBLE ICS01_Status3; //��ѹ�ɼ�״̬3
extern DATA_DOUBLE ICS01_Status4; //��ѹ�ɼ�״̬4

extern U16 ICS01_Voltage_Front; //ǰ�˵�ѹ
extern U16 ICS01_Voltage_Rear; //��˵�ѹ
extern U8 ICS01_LIFE; //LIFE




/*******************************************************************************/
typedef void (*pCanAnalyse)(can_msg_t *msg, can_pro_way_e way);

void app_init_can(void);

void app_can_sent_task(void);

void app_can_lost_time_cnt_100ms(void);

extern can_signal_t can0;






#endif
