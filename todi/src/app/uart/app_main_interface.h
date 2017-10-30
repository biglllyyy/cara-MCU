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
           right_key :1,					//key3
           cancel_key :1,					//key4
           keyRemain :4;					//key0
	} bits;
} Key_Status;
//!<С������
#define WORD_WRITE(dest,data)	do{dest[0] = data&0xFF;(dest[1]=data>>8)&0xFF;}while(0)
#define DWORD_WRITE(dest,data)	do{dest[0] = data&0xFF;(dest[1]=data>>8)&0xFF;(dest[2]=data>>16)&0xFF;(dest[3]=data>>24)&0xFF;}while(0)

typedef struct
{
	union {
		struct {
			U8
			out_status8 :1,
			out_status7 :1,
			out_status6 :1,
			out_status5 :1,
			out_status4 :1,
			out_status3 :1,
			out_status2 :1,
			out_status1 :1;
		}bits;
		U8 byte;
	}out1_8;

	union {
		struct {
			U8
			out_resever :1,
			out_status15 :1,
			out_status14 :1,
			out_status13 :1,
			out_status12 :1,
			out_status11 :1,
			out_status10 :1,
			out_status09 :1;
		}bits;
		U8 byte;
	}out9_16;

U8
	switch23: 1,
	switch22: 1,
	switch21: 1,
	switch20: 1,
	switch19: 1,
	switch18: 1,
	switch17: 1,
	switch16: 1;

U8
	switch31: 1,
	switch30: 1,
	switch29: 1,
	switch28: 1,
	switch27: 1,
	switch26: 1,
	switch25: 1,
	switch24: 1;

U8
	switch39: 1,
	switch38: 1,
	switch37: 1,
	switch36: 1,
	switch35: 1,
	switch34: 1,
	switch33: 1,
	switch32: 1;

U8
	switch_reserve: 5,
	switch42: 1,
	switch41: 1,
	switch40: 1;

U8  version;
}moudle_struct;

//!<С��
typedef struct
{
	Key_Status key;					//key1
//key0
	U8 speed;							//����	1km/h
	U8 rpm[2];							//ת��	1r/min
	U8 totalVoltage[2];                 //�ܵ�ѹ 0.1V/bit��-10000
	U8 totalCurrent[2];				    //�ܵ��� 0.1A/bit��-10000
	U8 gear;							/*0000:�յ� 0001: 1 �� 0010: 2 �� 0011: 3 �� 0100: 4 �� 0101: 5 �� 0110: 6 �� 0111: 7����*/
	U8 odo[4];							
	U8 trip[2];							//���С��	0-9999 <0.1>
	U8 soc;							//ϵͳSOC  0.4%/bit
	U8 airPressure1[2];					//��ѹ1	10 kpa/bit��0
	U8 airPressure2[2];					//��ѹ2	10 kpa/bit��0
	U8	moter_control_temp[2];        //����������¶�, 1��/bit��-40 
	U8	moter_temp[2];                //����¶�, 1��/bit��-40
    U8
        control_IN08:1, //����ƿ���
		control_IN07:1, //Զ��ƿ���
        control_IN06:1, //С�ƿ���
		control_IN05:1, //��ת�򿪹�
        control_IN04:1, //Կ��ON����
        control_IN03:1, //����
        control_IN02:1, //����
		control_IN01:1; //��ת�򿪹�
    U8
		control_IN16:1, //��θߵ�����
        control_IN15:1, //��μ�Ъ������
		control_IN14:1, //��ε͵�����
        control_IN13:1, //����
		control_IN12:1, //����
		control_IN11:1, //����
		control_IN10:1, //����ƿ���
        control_IN09:1; //ǰ��ƿ���
	U8
        control_IN24:1, //����
		control_IN23:1, //����
        control_IN22:1, //����
		control_IN21:1, //�����ˮ����
        control_IN20:1, //ǰ�ſ�����
        control_IN19:1, //����
        control_IN18:1, //����
		control_IN17:1; //����
	U8
		control_IN32:1, //����
        control_IN31:1, //����
        control_IN30:1, //����
        control_IN29:1, //����
        control_IN28:1, //ǰ�Źؿ���
		control_IN27:1, //���ſ�����
		control_IN26:1, //���Źؿ���
		control_IN25:1; //����
    U8
		N1: 5,			//����
		control_IN35:1, //����
        control_IN34:1, //����
		control_IN33:1; //����
	U8 systemCode[2];	//ϵͳ����
	U8 charge_status;	//���״̬
    U8 dateTime[4];		//ʱ��
    U8 battery[4];      //���ص�ѹ
	U8 version;			//�汾��

	moudle_struct front_moudle;
	moudle_struct middle_moudle;
	moudle_struct back_moudle;
} Main_Interface_Data_Struct;

extern Main_Interface_Data_Struct main_interface_data;

void main_interface_get_data(void);
void main_interface_send_data(void);
#endif
