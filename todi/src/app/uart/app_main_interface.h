#ifndef APP_MAIN_INTERFACE_H_
#define APP_MAIN_INTERFACE_H_

#include "comm_typedef.h"
typedef union
{
	U8 u8KeyState;
	struct
	{
		U8 ok_key :1,						//key1
				left_key :1,						//key2
				right_key :1,						//key3
				cancel_key :1,						//key4
				keyRemain :4;						//key0
	} bits;
} Key_Status;

typedef union
{
	U32 data;
	struct
	{
		U32 odo :20,						//�����	0-999999
				gear :2,							//��λ	0��P����1��R����2��N����3��D��
				totalVoltage :10;					//�ܵ�ѹ 0-1000

	} bits;
} Odo_Gear_TotalVol;

typedef union
{
	U32 can_valid;
		struct
		{
			U32 CAN_ID_REVERSED1 :1,    //0��Ч 1��Ч
				CAN_ID_REVERSED2 :1,
				CAN_ID_18AA28F3 :1,
				CAN_ID_1818D0F3 :1,
				CAN_ID_1819D0F3 :1,
				CAN_ID_181AD0F3 :1,
				CAN_ID_181BD0F3 :1,
				CAN_ID_181CD0F3 :1,
				CAN_ID_181DD0F3 :1,
				CAN_ID_181ED0F3 :1,
				CAN_ID_181FD0F3 :1,
				/*BMS Interaction*/
				//CAN_ID_1800F328,  send remove else will crash
				CAN_ID_180028F3 :1,
				CAN_ID_180028F4 :1,
				/*���*/
				//CAN_ID_1882BBAB,  send remove else will crash
				CAN_ID_1002FF1E :1,
				CAN_ID_1003FF1E :1,
				CAN_ID_1004FF1E :1,
				/*����*/
				//CAN_ID_18FF1103,
				CAN_ID_1429289B :1,
				CAN_ID_142A289B :1,
				//CAN_ID_18AB9B28,   send remove else will crash
				/*�Ǳ���Զ��ͨѶ*/
				//CAN_ID_18FFA017,    send remove else will crash
				/*DC-DC��Դ*/
				CAN_ID_1828272B :1,
				/*�ͱÿ�����*/
				CAN_ID_142A2895 :1,
				CAN_ID_reversed:12;

		} bits;



} Can_Valid_Bits;

typedef union
{
	U32 u32_switch_capture;
		struct
		{
		   U32 MCU_IN1:1,     //��ת�򿪹� 4
			   MCU_IN2:1,     //��������   3
			   MCU_IN3:1,     //���ſ�״̬ 2
			   MCU_IN4 :1,   //ǰ�ſ�״̬ (���԰��ϵ�1)
			   MCU_IN5 :1,   //��ת�򿪹�
			   MCU_IN6 :1,   //С�ƿ���
			   MCU_IN7 :1,   //Զ��ƿ���
			   MCU_IN8 :1,   //����ƿ���
			   MCU_IN9 :1,   //ǰ��ƿ���
			   MCU_IN10:1,    //����ƿ���
			   MCU_IN11:1,    //ǰ��ѹ�������أ��Ǳ�������Ѳ�ͼ����ʾ��
			   MCU_IN12:1,    //����ѹ�������أ��Ǳ�������Ѳ�ͼ����ʾ��
			   MCU_IN13:1,   //����
			   MCU_IN14:1,    //��ε͵�����
			   MCU_IN15:1,    //��μ�Ъ������
			   MCU_IN16:1,    //��θߵ�����
			   MCU_IN17:1,   //�����ˮ������
			   MCU_IN18:1,    //
			   MCU_IN19:1,    //���1����
			   MCU_IN20:1,    //���2����
			   MCU_IN21:1,    //
			   MCU_IN22:1,    //˾�����ȿ���
			   MCU_IN23:1,   //·�ƿ���
			   MCU_IN24:1,    //
			   MCU_IN25:1,    //������۷�����
			   MCU_IN26:1,    //ǿ���г�����
			   MCU_IN27:1,    //���ƶ��ź�
			   MCU_IN28:1,   //˾���ƿ���
			   MCU_IN29:1,   //
			   MCU_IN30:1,    //
			   MCU_IN31:1,    //��ȫ������
			   MCU_IN32:1;     //
	} bits;

}Switch_Capture;

typedef union
{
	U8 u8MotorContStatus;
	struct
	{
		U8 bit1to3 :3,						//key1
				bit4to5 :2,						//key2
				bit6to8 :3;					//key3
	} bits;
}MotorContStatus;


typedef struct
{
	Key_Status key;					//key1
//key0
	U8 speed;							//����	0-140
	U16 rpm;							//ת��	-20000 ~ 45531


	U32 odo;	
	
	U16 totalVoltage;
	U8  gear;
	U8  reversed1;
	

	//Odo_Gear_TotalVol Odo_gear_totalVol;

	U16 totalCurrent;                  //�ܵ���	0-1000 <-500>
	U16 trip;							//���С��	0-9999 <0.1>

	U8 soc;							//ϵͳSOC  0-100
	U8 ready_signal;						//ready
	U8 airPressure1;					//��ѹ1	0-100
	U8 airPressure2;					//��ѹ2	0-100

	U8 batteryTH;						//����������¶�	0-240 <-40>
	U8 batteryTL;						//����������¶�	0-240 <-40>
	U16 batteryVH;						//��������ߵ�ѹ	0-50 <0.1>

	U16 batteryVL;						//��������͵�ѹ	0-50 <0.1>
	U8  moter_control_temp;
	U8  moter_temp;
	
	Switch_Capture switch_capture;
	
	U16  tm_zhuanju_nm;
	U16  reversed2;

	

} Main_Interface_Data_Struct;

extern Main_Interface_Data_Struct main_interface_data;

void main_interface_get_data(void);
void main_interface_send_data(void);
#endif
