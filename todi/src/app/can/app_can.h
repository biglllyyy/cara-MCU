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
	ID_104C1000, //��ص�ѹ 100֡����0x104C19A4-0x104C1A07 16֡����¶�0x104C1A18-0x104C1A24
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


/**********����Э�����************/


extern unsigned int moto_voltage; //�����ѹ
extern unsigned int moto_current; //�������
extern unsigned int moto_speed; //�綯ת��
extern unsigned char Motor_Temperature; //����¶�
extern unsigned char Inverter_t; //������¶�

extern unsigned char Accelerator; //���ſ���
extern unsigned int Engine_Speed;//������ת��

extern unsigned int Fule_rate;//�ͺ�
extern unsigned char water_temp;//ˮ��
extern unsigned char Engine_Fuhe;//����������
extern unsigned char Niaosu;//����;
extern unsigned char Accelerator_Shiji; //����̤��ʵ��
extern unsigned char Engine_oil_press; //����ѹ��
extern unsigned char ambient_air_temperature; //��Χ�����¶�

extern unsigned char Current_Gear; //��λ
extern unsigned char TCU_Position;//���λ��
extern unsigned char TCU_warn;//���������
extern unsigned char STOP;//ͣ������
extern unsigned int Speed_rmp; //�����ת��
extern unsigned char Car_mode;//����ģʽ
extern unsigned char TCU_code;//��������ϴ���
extern unsigned char TCU_level;//��������ϵȼ�

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
extern unsigned char Brake_percent; //�ƶ�̤�忪��
extern unsigned char Speed_percent; //����̤�忪��
extern unsigned char Digit_input3;//����������3

extern unsigned long Program;//����汾
extern unsigned long tcu_spn; //TCU������
extern unsigned char tcu_fmi;
extern unsigned long tcu_cm; 
extern unsigned char tcu_oc;
extern unsigned char Car_LIFE;//����������

extern unsigned int BMS_V; //��ص�ѹ
extern unsigned char BMS_SOC; //���SOC
extern unsigned int BMS_A; //��ص���
extern unsigned char Status_Flag1;
extern unsigned char Status_Flag2;
extern unsigned char Status_Flag3;

extern unsigned int BMS_A_charge; //��������
extern unsigned int BMS_A_discharge; //������
extern unsigned char Warn_level;//���ϵȼ�
extern unsigned int BMS_V_average; //���ƽ����ѹ
extern unsigned char BMS_T_H;//�������¶�

extern unsigned int Oum_zheng; //��Ե����ֵ
extern unsigned int Oum_fu; //��Ե����ֵ
extern unsigned int Battery_single_H; //��ص���ߵ�ѹ
extern unsigned int Battery_single_L; //��ص���͵�ѹ

extern unsigned char Battery_number[10];//��ر��
extern unsigned int Battery_voltage[10];//��ص����ѹ

extern unsigned char Battery_number_t[10];//����¶ȱ��
extern unsigned int Battery_temp[10];//��ص����¶�

extern unsigned int DCAC_W;
extern unsigned int DCAC_V;
extern unsigned int DCAC_U;
extern unsigned char Sanreqi_t;//ɢ�����¶�
extern unsigned char DCAC_Warn_code;//���ϴ���

extern unsigned char AC_Warn_code;//AC���ϴ���
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
extern unsigned char Indoor_t;//�����¶�
extern unsigned char Outdoor_t;//�����¶�
extern unsigned char Set_t;//�趨
extern unsigned char AC_req;//��������
extern unsigned char AC_LIFE;//AC����
/*******************************************/

typedef void (*pCanAnalyse)(can_msg_t *msg, can_pro_way_e way);

void app_init_can(void);

void app_can_sent_task(void);

void app_can_lost_time_cnt_100ms(void);

extern void mid_can1_prepare(can_msg_t*msg_tx,can_msg_t*msg_rx);
#endif
