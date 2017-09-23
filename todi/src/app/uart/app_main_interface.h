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

//!<С��
typedef struct
{
	Key_Status key;					//key1
//key0
	U8 speed;							//����	1km/h
	U8 rpm[2];							//ת��	1r/min
	U8 totalVoltage[2];                 // 0.1V/bit��-10000
	U8 totalCurrent[2];				    //0.1A/bit��-10000
	U8	gear;							/*0000:�յ� 0001: 1 �� 0010: 2 �� 0011: 3 �� 0100: 4 �� 0101: 5 �� 0110: 6 �� 0111: 7����*/
	U8 odo[4];							//0.4%/bit
	U8 trip[2];							//���С��	0-9999 <0.1>
	U8 soc;							//ϵͳSOC  0-100
	U8 airPressure1;					//��ѹ1	10 kpa/bit��0
	U8 airPressure2;					//��ѹ2	10 kpa/bit��0
	U8	moter_control_temp;        //����������¶�, 1��/bit��-40 
	U8	moter_temp;                //����¶�, 1��/bit��-40
	U8
		control_IN08:1, //����ƿ���
		control_IN07:1, //Զ��ƿ���
		control_IN06:1, //С�ƿ���
		control_IN05:1, //��ת�򿪹�
		control_IN04:1, //Կ��ST����
		control_IN03:1, //����
		control_IN02:1, //����
		control_IN01:1; //��ת�򿪹�
    U8
		control_IN16:1, //��θߵ�����
		control_IN15:1, //��μ�Ъ������
		control_IN14:1, //��ε͵�����
		control_IN13:1, //����
		control_IN12:1, //���ƿ���
		control_IN11:1, //����
		control_IN10:1, //����ƿ���		
		control_IN09:1; //ǰ��ƿ���
				
	U8
		control_IN24:1, //����
		control_IN23:1, //����
        control_IN22:1, //����
        control_IN21:1, //�����ˮ����
        control_IN20:1, //ǰ�ſ�����
        control_IN19:1, //·�ƿ���
        control_IN18:1, //����		
		control_IN17:1; //����
	U8
        control_IN32:1, //����
        control_IN31:1, //����
        control_IN30:1, //����
        control_IN29:1, //ȼ�������ͱ���
        control_IN28:1, //ǰ�Źؿ���
        control_IN27:1, //���ſ�����
        control_IN26:1, //���Źؿ���
		control_IN25:1; //����
    U8  		
		N1: 5,			//����
        control_IN35:1, //Կ��ON����
        control_IN34:1, //��Դ���ѿ���
		control_IN33:1; //Σ�վ�������
	U8
		diagnostic_mode:1,	//���ģʽ����Ƭ
        climbing_mode:1,	//����ģʽ����Ƭ
        HV_overhaul:1,		//��ѹ����ָʾ����Ƭ
        main_power:1,		//�ܵ�Դָʾ����Ƭ
        defroster:1,		//��˪������Ƭ   δ����
        back_door:1,		//����ſ�����Ƭ
        park_brake:1,		//פ���ƶ�����Ƭ
		brake_light:1;		//ɲ���Ʒ���Ƭ
	U8  
		N2: 2,		        //����
        ABS:1,				//ABS����Ƭ
        dryer:1,			//����������Ƭ
        Kt:1,				//������Ӵ���
        power_mode:1,		//Powerģʽ����Ƭ(���綯ģʽ or �춯ģʽ or ���ģʽ)
        charge_confirm:1,	//���ȷ�Ϸ���Ƭ
		AC:1;				//AC�յ�����Ƭ
	U8 fault_level[4];	//���ϵȼ�	
	U8 fault_code[4];		//���ϴ��루�����е�ϵͳ����,��10������ʾ,������ɫ������ϵȼ��仯,�������Э���ĵ���
	U8 SPN[4];			//SPN��16������ʾ��
	U8 urea_level[4];		//����Һλ, 0.4 %
    U8 dateTime[4];		//ʱ��
    U8 battery[4];           //���ص�ѹ
} Main_Interface_Data_Struct;

extern Main_Interface_Data_Struct main_interface_data;

void main_interface_get_data(void);
void main_interface_send_data(void);
#endif
