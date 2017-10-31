#include "app_moudle.h"
#include "hal_GPIO.h"
#include "uart_cfg.h"
#include "app_info.h"
#include "mb91520.h"

//U8 pSpeed = 0; //�ⲿCAN���㳵��
DATA_BIT gCTL[8];

#if 1

#define BMS_Status_Flag1  0
#define BMS_Status_Flag2  0
#define BMS_Status_Flag3  0
#define BMS_Status_Flag5  0



U8 Fversion;
U8 Mversion;
U8 Rversion;

LED_DATA alarm_led[4] =
{ 0 };

U16 single_miles = 0;
U32 total_miles = 0;

U8 Fversion = 0;
U8 Mversion = 0;
U8 Rversion = 0;

U16 Ncan_count = 0; //������ECUģ��
U16 BAT24V_count = 0; //���ص�ѹ������ʱ

U8 front_moudle_count = 0;
U8 middle_moudle_count = 0;
U8 back_moudle_count = 0;


U16 M_PO11_count = 1000; //���10���ʱ

DBCM_KEY fKEY; //ǰģ�鹦�ʿ���״̬
DBCM_KEY mKEY; //�У�����ģ�鹦�ʿ���״̬
DBCM_KEY rKEY; //��ģ�鹦�ʿ���״̬

U16 fFreq; //ǰģ��Ƶ��
U16 mFreq; //�У�����ģ��Ƶ��
U16 rFreq; //��ģ��Ƶ��

U16 fSpeed; //ǰģ�����㳵��
U16 mSpeed; //�У�����ģ�����㳵��
U16 rSpeed; //��ģ�����㳵��
U8 pSpeed = 0; //�ⲿCAN���㳵�� KM/H

U16 fADR[2]; //ǰģ�����ֵ
U16 mADR[2]; //�У�����ģ�����ֵ
U16 rADR[2]; //��ģ�����ֵ

U16 ADR[8] =
{ 0 }; //ģ�����ɼ�   10/15
U16 ADV[8] =
{ 0 }; //ģ�����ɼ�

DBCM_POUT fPOUT =
{ 0 }; //ǰģ�鹦�����״̬
DBCM_POUT mPOUT =
{ 0 }; //�У�����ģ�鹦�����״̬
DBCM_POUT rPOUT =
{ 0 }; //��ģ�鹦�����״̬

U8 fPF[16] =
{ 0 }; //ǰģ�鹦���������
U8 mPF[16] =
{ 0 }; //�У�����ģ�鹦�ʹ���
U8 rPF[16] =
{ 0 }; //��ģ�鹦���������

U16 fpcur[15] =
{ 0 }; //ǰģ�鹦���������
U16 mpcur[15] =
{ 0 }; //��ģ�鹦���������
U16 rpcur[15] =
{ 0 }; //��ģ�鹦���������

U16 fSingle_miles = 0;
U16 mSingle_miles = 0;
U16 rSingle_miles = 0;

extern DATA_BIT gKEY[5]={0};


void SW_Output_Init(void)
{
	set_single_io_dir(7, 0, IO_GENERAL_OUTPUT);
	set_single_io_dir(7, 1, IO_GENERAL_OUTPUT);
	set_single_io_dir(7, 2, IO_GENERAL_OUTPUT);
	set_single_io_dir(7, 3, IO_GENERAL_OUTPUT);
	set_single_io_dir(7, 4, IO_GENERAL_OUTPUT);
	set_single_io_dir(7, 5, IO_GENERAL_OUTPUT);
	set_single_io_dir(7, 6, IO_GENERAL_OUTPUT);
	set_single_io_dir(7, 7, IO_GENERAL_OUTPUT);

	set_single_io_dir(8, 0, IO_GENERAL_OUTPUT);
	set_single_io_dir(8, 1, IO_GENERAL_OUTPUT);
	set_single_io_dir(8, 2, IO_GENERAL_OUTPUT);
	set_single_io_dir(8, 3, IO_GENERAL_OUTPUT);
	set_single_io_dir(8, 4, IO_GENERAL_OUTPUT);
	set_single_io_dir(8, 5, IO_GENERAL_OUTPUT);
	set_single_io_dir(8, 6, IO_GENERAL_OUTPUT);
	set_single_io_dir(8, 7, IO_GENERAL_OUTPUT);

}

void SW_Input_Init(void)
{
	set_single_io_dir(2, 0, IO_INPUT);
	set_single_io_dir(2, 1, IO_INPUT);
	set_single_io_dir(2, 2, IO_INPUT);
	set_single_io_dir(2, 3, IO_INPUT);
	set_single_io_dir(2, 5, IO_INPUT);
	set_single_io_dir(2, 6, IO_INPUT);
	set_single_io_dir(2, 7, IO_INPUT);
	set_single_io_dir(3, 0, IO_INPUT);

	set_single_io_dir(3, 1, IO_INPUT);
	set_single_io_dir(3, 2, IO_INPUT);
	set_single_io_dir(3, 3, IO_INPUT);
	set_single_io_dir(3, 4, IO_INPUT);
	set_single_io_dir(3, 5, IO_INPUT);
	set_single_io_dir(3, 6, IO_INPUT);
	set_single_io_dir(3, 7, IO_INPUT);
	set_single_io_dir(15, 0, IO_INPUT);

	set_single_io_dir(15, 1, IO_INPUT);
	set_single_io_dir(4, 0, IO_INPUT);
	set_single_io_dir(4, 1, IO_INPUT);
	set_single_io_dir(4, 3, IO_INPUT);
	set_single_io_dir(4, 4, IO_INPUT);
	set_single_io_dir(4, 5, IO_INPUT);

	set_single_io_dir(4, 6, IO_INPUT);
	set_single_io_dir(4, 7, IO_INPUT);

	set_single_io_dir(5, 0, IO_INPUT);
	set_single_io_dir(5, 1, IO_INPUT);
	set_single_io_dir(5, 2, IO_INPUT);
	set_single_io_dir(5, 3, IO_INPUT);
	set_single_io_dir(5, 4, IO_INPUT);
	set_single_io_dir(5, 5, IO_INPUT);
	set_single_io_dir(5, 6, IO_INPUT);

	set_single_io_dir(5, 7, IO_INPUT);
}

void SYSTME_Logic(void)
{
//	F_PO1 = (IN16 && M_ON); //��ο쵵
//	F_PO2 = ((IN14 || IN15 || IN21)&& M_ON); //�������
//	F_PO3 = (IN21 && M_ON); //��ˮ���
	F_PO4 = (IN6 && IN9); //��ǰ���
	F_PO5 = (IN6); //С�ƣ���������������С��)
	F_PO6 = (FLASH && (IN5 || wake_up1)); //��ǰת���
	F_PO7 = (FLASH && (IN1 || wake_up1)); //��ǰת���
	F_PO8 = fKL3; //������
	F_PO9 = (IN7); //Զ�ص�
	F_PO10 = (IN6 && mKL15); //ǰ��̤����
	F_PO11 = (IN7); //Զ���
	F_PO12 = rKL3; //ɲ����
	F_PO13 = (IN6 && IN8); //�����
	F_PO14 = (IN6); //С��(ǰλ�õƣ�������)
	F_PO15 = !(IN6 || IN7 || IN8);//�ռ��г���

	M_PO1 = (IN20 && (pSpeed < 5)); //ǰ�ſ��ŵ�ŷ�
	M_PO2 = (IN28); //ǰ�Źص�ŷ�
	M_PO3 = (IN27 && (pSpeed < 5)); //ǰ�ſ���ŷ�
	M_PO4 = (IN26); //���Źص�ŷ�
	M_PO5 = (mKL5); //���ٵ�3�������2����
	M_PO6 = (mKL7); //���ٵ�2�������2����
	M_PO7 = (mKL4); //���չ�ƣ������1����
	M_PO8 = (mKL6); //���չ�ƣ������1����
	M_PO9 = (M_ON);	//Կ��2�����յ���Դ��
	M_PO10 = (M_ON); //Կ��2�������ص��ӣ�
	M_PO11 = (M_ON); //Կ��2���������ȵ�Դ��
	M_PO12 = (mKL1 && mKL3); //˾����12
	M_PO13 = (mKL8); //·�Ƶ�
	M_PO14 = (mKL9); //������ʾ��
	M_PO15 = (IN6); //С�ƣ���λ�õƣ�

	R_PO1 = (mKH1 && IN6); //����̤����
	R_PO2 = (IN6);//С��(��λ�õ�)
	R_PO3 = (rKL3); //ɲ����
	R_PO4 = (mKL8);//��·��
	R_PO5 = (IN6 && IN9 && IN10); //�����
	R_PO6 = (M_ON); //�ܵ�Դ�̵���(��ʱ3s�ϵ�)
	R_PO7 = (M_ON && (vcu_msg.ID_0C018980_byte4.bit02 ==0x04)); //������
	R_PO8 = M_ON; //��������Դ
	R_PO9 = (rKL3); //��������Դ
	R_PO10 = 0; //������ŷ����
	R_PO11 = (rKL1); //��ֵ�
	R_PO12 = (IN6); //��ת��ƣ��ࣩ
	R_PO13 = (mKH1 && IN6); //��ת��ƣ�LED��
	R_PO14 = (IN19 && M_ON);//��ת��ƣ��ࣩ
	R_PO15 = (FLASH && IN1 && M_ON); //��ת��ƣ�LED��
}


void LED_Logic(void)
{}

//ǰģ��ķ��ͺ���
void BCAN_SendCtl_720(void)
{
	#if 0
	CAN_MESSAGE message_POUT;
	DATA_BIT temp;
	stcCANId_t tSendID;
	strCANData_t tSendData;
	temp.byte = 0;

	message_POUT.id = 0x720;
	message_POUT.data[0] = gCTL[0].byte;
	message_POUT.data[1] = gCTL[1].byte;
	message_POUT.data[2] = 0;
	message_POUT.data[3] = 0;
	message_POUT.data[4] = 0;
	//message_POUT.data[5] = 0;

	temp.bits.bit1 = ((M_ON&& wake_up3) || wake_up2);
	temp.bits.bit2 = IN14; //����
	temp.bits.bit3 = IN16; //�쵵
	temp.bits.bit4 = IN15; //��Ъ 
	temp.bits.bit5 = IN21; //��ˮ
	if (IN14 && IN16)
		temp.bits.bit2 = 0;
	if (IN21)
	{
		temp.bits.bit2 = 0; //����
		temp.bits.bit3 = 0; //��Ъ
		temp.bits.bit4 = 0; //�쵵
	}
	
	message_POUT.data[5] = temp.byte;//((temp.bits.bit5<<5)|(temp.bits.bit4<<4)|(temp.bits.bit3<<3)|(temp.bits.bit2<<2)|(temp.bits.bit1));//

	message_POUT.data[6] = 0; //pwm Ƶ�����
	message_POUT.data[7] = 0;
	memcpy(&tSendData.ui8byte[0], &message_POUT.data[0], 8);
	tSendID.ui32Id = message_POUT.id;
	tSendID.ui8IdType = MSGTYPE_STD;
	tSendData.ui8NumberOfBytes = 0x08;
	//message_POUT.len = 8;
	//message_POUT.type = 1;
	//CAN1_SendMessage(&message_POUT);
	CAN_UpdateAndSendMessage(CAN1, 0, tSendID, tSendData);
	#endif
}

//��ģ��ķ��ͺ���
void BCAN_SendCtl_721(void)
{
	#if 0
	CAN_MESSAGE message_POUT;
	DATA_BIT temp;
	stcCANId_t tSendID;
	strCANData_t tSendData;
	temp.byte = 0;

	message_POUT.id = 0x721;
	message_POUT.data[0] = gCTL[2].byte;
	message_POUT.data[1] = gCTL[3].byte;
	message_POUT.data[2] = 0;
	message_POUT.data[3] = 0;
	message_POUT.data[4] = 0;
	message_POUT.data[5] = 0;
	message_POUT.data[6] = 0;
	message_POUT.data[7] = 0;
	memcpy(&tSendData.ui8byte[0], &message_POUT.data[0], 8);
	tSendID.ui32Id = message_POUT.id;
	tSendID.ui8IdType = MSGTYPE_STD;
	tSendData.ui8NumberOfBytes = 0x08;
	//message_POUT.len = 8;
	//message_POUT.type = 1;
	//CAN1_SendMessage(&message_POUT);
	CAN_UpdateAndSendMessage(CAN1, 1, tSendID, tSendData);
	#endif
}

//��ģ��ķ��ͺ���
void BCAN_SendCtl_722(void)
{
	#if 0
	CAN_MESSAGE message_POUT;
	DATA_BIT temp;
	stcCANId_t tSendID;
	strCANData_t tSendData;
	temp.byte = 0;

	message_POUT.id = 0x722;
	message_POUT.data[0] = gCTL[4].byte;
	message_POUT.data[1] = gCTL[5].byte;
	message_POUT.data[2] = 0;
	message_POUT.data[3] = 0;
	message_POUT.data[4] = 0;
	message_POUT.data[5] = 0;

	message_POUT.data[6] = 0;

	message_POUT.data[7] = 0;
	memcpy(&tSendData.ui8byte[0], &message_POUT.data[0], 8);
	tSendID.ui32Id = message_POUT.id;
	tSendID.ui8IdType = MSGTYPE_STD;
	tSendData.ui8NumberOfBytes = 0x08;
	//message_POUT.len = 8;
	//message_POUT.type = 1;
	//CAN1_SendMessage(&message_POUT);
	CAN_UpdateAndSendMessage(CAN1, 2, tSendID, tSendData);
	#endif
}



void BCAN_Lost_handle(void)   //100ms moudle task
{
	static unsigned int cnt0 = 0;
	static unsigned int cnt1 = 3;
	static unsigned int cnt2 = 7;
	static unsigned int cnt3 = 9;
	static unsigned int cnt4 = 0;
	static unsigned int cnt5 = 0;

	//CAN�ڵ���ϼ�ʱ3s
    if (Fcan_count >= CAN_TIME)Fcan_count = CAN_TIME;
    else Fcan_count++;
    if (Rcan_count >= CAN_TIME)Rcan_count = CAN_TIME;
    else Rcan_count++;
    if (Mcan_count >= CAN_TIME)Mcan_count = CAN_TIME;
    else Mcan_count++;
	if (IN10 && IN6 && (IN7 || IN8) && IN9) { //����ƿ����ǳ������أ�����
        if (cnt5 > 10) rLED_flag = 1;
        else cnt5++;
    }else if((IN6 && (IN7 || IN8) && IN9)==0){
        rLED_flag = 0;
        cnt5=0;
    }
	if (Fcan_count >= CAN_TIME)
	{
//		rKEY.BITS.kl6 = 1;
	}
}


void Moudle_Logic_handle(void)  //50ms Logic handle
{
	//BCAN_send_mile(); 
	if (M_ON&& (wake_up3 || wake_up2))
	{ //������main������ᵼ����˸Ƶ���쳣wake_up3��0ʱ��M_ON����������0����Ϊ����������ʱ
	//	LED_Logic();
//		LED_Out();  //���ֻҪ�򿪾ͻᵼ��Ƶ������ ��Ϊ����ǿպ���
	//	SYSTME_Logic();
	}
	SYSTME_Logic();
	BCAN_send_mile();
	BCAN_SendCtl();

	//F50ms= 1;
}


void moudle_task_10ms(void)
{
	static U16 cnt3 = 9;
	static U16 Fcount = 0;
	if (wake_up3) { //ON�����ж�//����//M_ON�ڵ����һ˲���ʹ������Դ��Ч����M_ON�ڶϵ�ᱣ��300ms���Ӷ�Ӱ�쿪�����ɼ�
        if (cnt3 > 30) M_ON = 1;
        else cnt3++;
    } else {
        if (cnt3 < 1) M_ON = 0;
        else cnt3--;
    }

	if (Fcount >= 40) {//0.4s
        Fcount = 0;
        FLASH = !FLASH;
    } else Fcount++;
}
#endif
