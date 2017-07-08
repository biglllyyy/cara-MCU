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



/*******************�ְ�����*****************/
//VCU
unsigned int  VCU_TM_NM = 5000; //TMĿ��Ť��
unsigned int  VCU_TM_RPM = 15000; //TMĿ��ת��

unsigned char VCU_TM_Brake_percent; //�ƶ�̤�忪��
unsigned char VCU_TM_Speed_percent; //����̤�忪��
DATA_BIT VCU_Status_Flag1; //״̬1
unsigned char VCU_Gear; //��λ
DATA_BIT VCU_Mode; //����ģʽ
unsigned char VCU_Life; //����������LIFE
DATA_BIT VCU_Status_Flag2;
unsigned char VCU_Code; //����������
// VCU2TERMINAL_MOTOR01
unsigned char TM_Number; //�������
unsigned char TM_Serial; //������
unsigned char TM_WorkStatus; //TM�������״̬
unsigned char TM_Control_Temp = 40; //TM����������¶�
unsigned int  TM_Feedback_RPM = 20000; //TM�������ת��
unsigned int  TM_Feedback_NM = 20000; //TM�������Ť��
// VCU2TERMINAL_MOTOR02
unsigned char TM_Temp = 40; //TM����¶�
unsigned int  TM_Voltage_DC; //TM���ֱ����ѹ
unsigned int  TM_Current_DC = 10000; //TM���ֱ������
/*
//MCU01
unsigned int  TM_Feedback_RPM=20000;//TM�������ת��
unsigned int  TM_Feedback_NM=10000;//TM�������Ť��
unsigned int  TM_Current_AC;//TM�����������
unsigned char TM_StatusFeedback;//TM���״̬����
unsigned char TM_WorkStatus;//TM�������״̬
unsigned char TM_LIFE;//TM���LIFE
//MCU02
unsigned char TM_Temp=40;//TM����¶�
unsigned char TM_Control_Temp=40;//TM����������¶�
unsigned int  TM_Torque_Limit_Up;//���Ť������
unsigned int  TM_Torque_Limit_Down;//���Ť������
DATA_BIT      TM_Fault;//TM������ϵȼ�
unsigned char TM_Status;//TM���״̬
DATA_BIT      TM_Status_Flag1;//���״̬1
//MCU03
unsigned int  TM_Code;//TM���������
unsigned int  TM_Current_DC=20000;//TM���ֱ������
unsigned int  TM_Voltage_DC;//TM���ֱ����ѹ
 */
//IRM01
unsigned long IRM_Ohm_Positive; //������Ե��ֵ 
unsigned long IRM_Ohm_Negative; //�ܸ���Ե��ֵ 
unsigned char IRM_Fault_Level; //�豸���ϵȼ�
unsigned char IRM_Insulation_Level; //��Ե�ȼ�
unsigned char IRM_Life; //��ԵLIFE

/*unsigned char VCU_Kt; //���Ӵ�����������
unsigned char VCU_ON; //Կ���µ�״̬
unsigned char VCU_Order; //���ϴ�������
unsigned int  VCU_Voltage; //�����������ѹ
 */

unsigned char BMS_Mode; //��س�ŵ�ģʽ
unsigned char BMS_Status; //���״̬
unsigned char BAT_Temp_Average = 40; //���ƽ���¶�
unsigned char BMS_SOC; //SOC
unsigned int  BMS_Current = 32000; //���ϵͳ����
unsigned int  BMS_Voltage; //���ϵͳ���ܵ�ѹ
unsigned char BMS_Kt; //��ѹ�̵���״̬

unsigned char BAT_Temp_H_Limit = 40; //��������ص����¶�
unsigned char BAT_Temp_L_Limit = 40; //��������ص����¶�
unsigned char BMS_SOC_L_Limit; //�������SOCֵ
unsigned int  BAT_Current_Discharge_Limit = 32000; //�������ŵ����
unsigned int  BAT_Current_Charge_Limit = 32000; //������������

unsigned char BAT_Temp_L = 40; //��ص�������¶�
unsigned char BAT_Temp_L_Number; //��ص�����¶����
unsigned char BAT_Temp_L_Case; //��ص�����¶����
unsigned char BAT_Temp_H = 40; //��ص�������¶�
unsigned char BAT_Temp_H_Number; //��ص�����¶����
unsigned char BAT_Temp_H_Case; //��ص�����¶����

unsigned char BAT_Voltage_Fault; //��ѹ����
unsigned char BAT_Single_Fault; //��ѹ����
unsigned char BAT_Temp_Fault; //�¶ȹ���
unsigned char BAT_Insulation_Fault; //��Ե����
unsigned char BAT_Consistency_Fault; //���һ���Թ���
unsigned char BAT_SOC_Fault; //SOC����
unsigned char BAT_Current_Fault; //��������
unsigned char BAT_Lock_Fault; //��ѹ��������
unsigned char BMS_Comm_Fault; //BMSͨѶ����
unsigned char BMS_System_Fault; //BMSϵͳ����

unsigned int  BMS_Ohm; //��Ե��ֵ 
unsigned char BMS_Number; //�����ѹ����
unsigned char BMS_Temp_Number; //�����¶�����
unsigned char BMS_System_Unit; //ϵͳ����
unsigned char BMS_System_Number; //ϵͳ��

unsigned int  BAT_Single_L; //��ص���͵�ѹ
unsigned char BAT_Single_L_Number; //��ص���͵�ѹλ��
unsigned char BAT_Single_L_Case; //��ص���͵�ѹ���

unsigned int  BAT_Single_H; //��ص���͵�ѹ
unsigned char BAT_Single_H_Number; //��ص���͵�ѹλ��
unsigned char BAT_Single_H_Case; //��ص���͵�ѹ���



unsigned int  DCDC_Voltage; //DCDC�����ѹ
unsigned int  DCDC_Current; //DCDC�������
unsigned char DCDC_Status; //DCDC����״̬
unsigned char DCDC_Output_Cut; //DCDC ����ж�
unsigned char DCDC_intput_Cut; //DCDC �����ж�
unsigned char DCDC_Fault_Code; //DCDC������
unsigned char DCDC_Level_Fault; //DCDC���ϵȼ�
unsigned char DCDC_Temp_Warn; //DCDC�¶ȱ���
unsigned char DCDC_Fault_Warn; //DCDC���ϱ���
unsigned char DCDC_Temp = 40; //DCDCģ���¶�

unsigned int  OILDCAC_U_Voltage; //DCAC���U��ѹ
unsigned int  OILDCAC_U_Current; //DCAC���U����
unsigned char OILDCAC_Status; //DCAC����״̬
unsigned char OILDCAC_Output_Cut; //DCAC ����ж�
unsigned char OILDCAC_intput_Cut; //DCAC �����ж�
unsigned char OILDCAC_Fault_Code; //DCAC���ϴ���
unsigned char OILDCAC_Level_Fault; //DCAC���ϵȼ�
unsigned char OILDCAC_Temp_Warn; //DCDC�¶ȱ���
unsigned char OILDCAC_Fault_Warn; //DCDC���ϱ���
unsigned char OILDCAC_Temp = 40; //DCACģ���¶�

unsigned int  OILDCAC_V_Voltage; //DCAC���V��ѹ
unsigned int  OILDCAC_V_Current; //DCAC���V����
unsigned int  OILDCAC_W_Voltage; //DCAC���W��ѹ
unsigned int  OILDCAC_W_Current; //DCAC���W����

unsigned int  AIRDCAC_U_Voltage; //DCAC���U��ѹ
unsigned int  AIRDCAC_U_Current; //DCAC���U����
unsigned char AIRDCAC_Status; //DCAC����״̬
unsigned char AIRDCAC_Output_Cut; //DCAC ����ж�
unsigned char AIRDCAC_intput_Cut; //DCAC �����ж�
unsigned char AIRDCAC_Fault_Code; //DCAC���ϴ���
unsigned char AIRDCAC_Level_Fault; //DCAC���ϵȼ�
unsigned char AIRDCAC_Temp_Warn; //DCDC�¶ȱ���
unsigned char AIRDCAC_Fault_Warn; //DCDC���ϱ���
unsigned char AIRDCAC_Temp = 40; //DCACģ���¶�

unsigned int  AIRDCAC_V_Voltage; //DCAC���V��ѹ
unsigned int  AIRDCAC_V_Current; //DCAC���V����
unsigned int  AIRDCAC_W_Voltage; //DCAC���W��ѹ
unsigned int  AIRDCAC_W_Current; //DCAC���W����

DATA_DOUBLE ICS01_Status1; //��ѹ�ɼ�״̬1
DATA_DOUBLE ICS01_Status2; //��ѹ�ɼ�״̬2
DATA_DOUBLE ICS01_Status3; //��ѹ�ɼ�״̬3
DATA_DOUBLE ICS01_Status4; //��ѹ�ɼ�״̬4

unsigned int  ICS01_Voltage_Front; //ǰ�˵�ѹ
unsigned int  ICS01_Voltage_Rear; //��˵�ѹ
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
