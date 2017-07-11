#ifndef APP_WARNING_H
#define APP_WARNING_H
#include "comm_typedef.h"

typedef union
{
	U8 bms_status_flag;
	struct
	{
		U8 bit1to2 :2,						//
				bit3to4 :2,						//
				bit5to6 :2,						//
				bit7to8 :2;					//
	} bits;
} Bms_Status_Flag;

typedef union
{
	U8 bms_status_flag;
	struct
	{
		U8 bit1 :1,						//
				bit2 :1,						//
				bit3 :1,						//
				bit4 :1,					//
				bit5 :1,					//
				bit6 :1,					//
				bit7 :1,					//
				bit8 :1;					//
	} bits;
} Bms_Temp_Flag;

typedef union
{
	U8 bms_comm_status;
	struct
	{
		U8 bit1 :1,						//
				bit2 :1,						//
				bit3 :1,						//
				bit4 :1,					//
				bit5 :1,					//
				bit6 :1,					//
				bit7 :1,					//
				bit8 :1;					//
	} bits;
} Bms_Comm_Status;

typedef union
{
	U8 bms_balance_status;
	struct
	{
		U8 bit1 :1,						//
				bit2 :1,						//
				bit3 :1,						//
				bit4 :1,					//
				bit5 :1,					//
				bit6 :1,					//
				bit7 :1,					//
				bit8 :1;					//
	} bits;
} Bms_Balance_Status;

typedef union
{
	U8 moudle_status;
	struct
	{
		U8 bit8 :1,						// 
				bit7 :1,						//
				bit6 :1,						//
				bit5 :1,					//
				bit4 :1,					//
				bit3 :1,					//��ģ������״̬  0:��ģ������ 1: ��ģ��δ����
				bit2 :1,					//��ģ������״̬  0:��ģ������ 1: ��ģ��δ����
				bit1 :1;					//ǰģ������״̬  0:ǰģ������ 1: ǰģ��δ����
	} bits;
} Moudle_Status;


typedef union
{
	U8 pump_status;
	struct
	{
		U8 bit1to4 :4,						//
				bit5to6 :2,						//
				bit7 :1,						//
				bit8 :1;				//
	} bits;
} Pump_Status;

typedef union
{
	U8 dcdc_status;
	struct
	{
		U8 bit1 :1,						//
				bit2to3 :2,						//
				bit4to5 :2,						//
				bit6to8 :3;				//
	} bits;
} DCDC_Status_1;

typedef struct
{
	U8 BAT_Voltage_Fault ; //��ѹ����
	U8 BAT_Single_Fault ; //��ѹ����
	U8 BAT_Temp_Fault ; //�¶ȹ���
	U8 BAT_Insulation_Fault ; //��Ե����
	
	U8 BAT_Consistency_Fault ; //���һ���Թ���
	U8 BAT_SOC_Fault ; //SOC����
	U8 BAT_Current_Fault ; //��������
	U8 BAT_Lock_Fault ; //��ѹ��������
	
	U8 BMS_Comm_Fault ; //BMSͨѶ����
	U8 BMS_System_Fault ; //BMSϵͳ����
	U8 DCDC_Temp_Warn ; //DCDC�¶ȱ���
	U8 DCDC_Fault_Warn ; //DCDC���ϱ���
	
	U8 OILDCAC_Temp_Warn ; //DCDC�¶ȱ���
	U8 OILDCAC_Fault_Warn ; //DCDC���ϱ���
	U8 AIRDCAC_Temp_Warn ; //DCDC�¶ȱ���
	U8 AIRDCAC_Fault_Warn ; //DCDC���ϱ���


} WarningDataStruct;

typedef union
{

	struct
	{
		unsigned bit0 :1;
		unsigned bit1 :1;
		unsigned bit2 :1;
		unsigned bit3 :1;
		unsigned bit4 :1;
		unsigned bit5 :1;
		unsigned bit6 :1;
		unsigned bit7 :1;
	} bits;
	unsigned char byte;
} Data_Struct;

WarningDataStruct warning_data;
void warning_get_data(void);
void warning_send_data(void);
#endif
