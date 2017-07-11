#include "app_moudle.h"
#include "hal_GPIO.h"
#include "uart_cfg.h"

U8 pSpeed = 0; //�ⲿCAN���㳵��

#if 0

#define BMS_Status_Flag1  can.ID_1818D0F3.u8BattStatusFlag1
#define BMS_Status_Flag2  can.ID_1818D0F3.u8BattStatusFlag2
#define BMS_Status_Flag3  can.ID_1819D0F3.u8BattStatusFlag3
#define BMS_Status_Flag5  can.ID_181ED0F3.Status_Flag5

#define e_total_miles      g_tInfo.u32ODO
#define DMC_Gear           can.ID_1002FF1E.u8Gear
#define	DMC_Status         ((can.ID_1002FF1E.u8MotorContStatus >> 5)&0x07)
#define DMC_Level 		   ((can.ID_1002FF1E.u8MotorContStatus >> 3)&0x03)
#define DMC_Rpm            can.ID_1002FF1E.u16MotorSpeed   //���ת��

unsigned char Fversion;
unsigned char Mversion;
unsigned char Rversion;

LED_DATA alarm_led[4] =
{ 0 };

unsigned int single_miles = 0;
unsigned long total_miles = 0;

unsigned char Fversion = 0;
unsigned char Mversion = 0;
unsigned char Rversion = 0;

unsigned int Ncan_count = 0; //������ECUģ��
unsigned int BAT24V_count = 0; //���ص�ѹ������ʱ

U8 front_moudle_count = 0;
U8 middle_moudle_count = 0;
U8 back_moudle_count = 0;


unsigned int M_PO11_count = 1000; //���10���ʱ

DBCM_KEY fKEY; //ǰģ�鹦�ʿ���״̬
DBCM_KEY mKEY; //�У�����ģ�鹦�ʿ���״̬
DBCM_KEY rKEY; //��ģ�鹦�ʿ���״̬

unsigned int fFreq; //ǰģ��Ƶ��
unsigned int mFreq; //�У�����ģ��Ƶ��
unsigned int rFreq; //��ģ��Ƶ��

unsigned int fSpeed; //ǰģ�����㳵��
unsigned int mSpeed; //�У�����ģ�����㳵��
unsigned int rSpeed; //��ģ�����㳵��
U8 pSpeed = 0; //�ⲿCAN���㳵��

unsigned int fADR[2]; //ǰģ�����ֵ
unsigned int mADR[2]; //�У�����ģ�����ֵ
unsigned int rADR[2]; //��ģ�����ֵ

unsigned int ADR[8] =
{ 0 }; //ģ�����ɼ�   10/15
unsigned int ADV[8] =
{ 0 }; //ģ�����ɼ�

DBCM_POUT fPOUT =
{ 0 }; //ǰģ�鹦�����״̬
DBCM_POUT mPOUT =
{ 0 }; //�У�����ģ�鹦�����״̬
DBCM_POUT rPOUT =
{ 0 }; //��ģ�鹦�����״̬

unsigned char fPF[16] =
{ 0 }; //ǰģ�鹦���������
unsigned char mPF[16] =
{ 0 }; //�У�����ģ�鹦�ʹ���
unsigned char rPF[16] =
{ 0 }; //��ģ�鹦���������

unsigned int fpcur[15] =
{ 0 }; //ǰģ�鹦���������
unsigned int mpcur[15] =
{ 0 }; //��ģ�鹦���������
unsigned int rpcur[15] =
{ 0 }; //��ģ�鹦���������

unsigned int fSingle_miles = 0;
unsigned int mSingle_miles = 0;
unsigned int rSingle_miles = 0;

void SW_Output_Init(void)
{
	set_io_dir(7, 0, IO_GENERAL_OUTPUT);
	set_io_dir(7, 1, IO_GENERAL_OUTPUT);
	set_io_dir(7, 2, IO_GENERAL_OUTPUT);
	set_io_dir(7, 3, IO_GENERAL_OUTPUT);
	set_io_dir(7, 4, IO_GENERAL_OUTPUT);
	set_io_dir(7, 5, IO_GENERAL_OUTPUT);
	set_io_dir(7, 6, IO_GENERAL_OUTPUT);
	set_io_dir(7, 7, IO_GENERAL_OUTPUT);

	set_io_dir(8, 0, IO_GENERAL_OUTPUT);
	set_io_dir(8, 1, IO_GENERAL_OUTPUT);
	set_io_dir(8, 2, IO_GENERAL_OUTPUT);
	set_io_dir(8, 3, IO_GENERAL_OUTPUT);
	set_io_dir(8, 4, IO_GENERAL_OUTPUT);
	set_io_dir(8, 5, IO_GENERAL_OUTPUT);
	set_io_dir(8, 6, IO_GENERAL_OUTPUT);
	set_io_dir(8, 7, IO_GENERAL_OUTPUT);

}

void SW_Input_Init(void)
{
	set_io_dir(0, 3, IO_INPUT);
	set_io_dir(0, 2, IO_INPUT);
	set_io_dir(0, 1, IO_INPUT);
	set_io_dir(0, 0, IO_INPUT);
	set_io_dir(0, 4, IO_INPUT);
	set_io_dir(0, 5, IO_INPUT);
	set_io_dir(0, 6, IO_INPUT);
	set_io_dir(0, 7, IO_INPUT);

	set_io_dir(1, 0, IO_INPUT);
	set_io_dir(1, 1, IO_INPUT);
	set_io_dir(1, 2, IO_INPUT);
	set_io_dir(1, 3, IO_INPUT);
	set_io_dir(1, 4, IO_INPUT);
	set_io_dir(1, 5, IO_INPUT);
	set_io_dir(1, 6, IO_INPUT);
	set_io_dir(1, 7, IO_INPUT);

	set_io_dir(2, 2, IO_INPUT);
	set_io_dir(2, 3, IO_INPUT);
	set_io_dir(2, 4, IO_INPUT);
	set_io_dir(2, 5, IO_INPUT);
	set_io_dir(2, 6, IO_INPUT);
	set_io_dir(2, 7, IO_INPUT);

	set_io_dir(3, 0, IO_INPUT);
	set_io_dir(4, 7, IO_INPUT);

	set_io_dir(5, 0, IO_INPUT);
	set_io_dir(5, 1, IO_INPUT);
	set_io_dir(5, 2, IO_INPUT);
	set_io_dir(5, 3, IO_INPUT);
	set_io_dir(5, 4, IO_INPUT);
	set_io_dir(5, 5, IO_INPUT);
	set_io_dir(5, 6, IO_INPUT);

	set_io_dir(4, 4, IO_INPUT);
}


void SYSTME_Logic(void)
{
	static U16  tempcount = 0; //recoard 5s
	static U16  tempcount2 = 0; //recoard 10 min
	//F_PO1= (IN9); //��ο쵵
	//F_PO2 = (IN14 && M_ON);//�������
	//F_PO3 = (IN21 && M_ON);//��ˮ���
	F_PO2= (IN6 && IN9);//��ǰ���
	F_PO5 = (IN28 && wake_up2);//������۷�
	F_PO6 = (FLASH && (IN5 || wake_up1));//��ת��� 400ms��һ��
	F_PO7 = (FLASH && (IN1 || wake_up1));//��ת��� 400ms��һ��

	if(((g_tInfo.u16MotorSpeed>=16300*2)||(g_tInfo.u16MotorSpeed<=15700*2))&&((g_tInfo.u16MotorSpeed!=0xffff)))
	{
		if((tempcount<=9)&&(tempcount2<=(12000-1)))
		{
			F_PO8 = 1;
			tempcount++;
			tempcount2 = 0;
		}
		if(tempcount==10)
		{
			F_PO8 = 0;
			tempcount2++;
		}
		if(tempcount2==((12000-1)))
		{
			tempcount = 0;
		}
	}
	else
	{
		tempcount = 0;
		tempcount2 = 0;
	}

	//F_PO8 = (((DMC_Rpm * 0.5) > 16000) && F5s);//������
	F_PO9 = (IN7 && !IN8);//��Զ�ص�
	F_PO10 = (IN6 && IN9);//��ǰ���
	F_PO11 = (IN7 && !IN8);//��Զ���
	F_PO12 = (fKL8);//���е� //
	F_PO13 = (IN8 && IN6 && IN7);//�����
	F_PO14 = (IN6);//λ�õ�
	F_PO15 = (IN8 && IN6 && IN7);//�ҽ���
	F_PO16 = 0;//���ŵ���

	M_PO1 = (IN19);//ǰ·�Ƶ�Դ
	M_PO2 = (wake_up2 && IN24);//���2
	M_PO3 = (IN6 && IN4);//ǰ��̤����
	M_PO4 = (fKL6);//�ƶ���
	M_PO5 = (IN6 && IN3);//����̤����
	M_PO6 = (IN19);//��·��
	M_PO7 = (wake_up2 && fKL10);//���Ӿ�����
	M_PO8 = (IN9 && IN10 && IN6);//�����
	M_PO9 = (IN19);//��·�� //(wake_up2 && IN6 && fKL8); //���е�
	M_PO10 = (wake_up2 && IN18);//˾������
	M_PO11 = (wake_up2);//��������Դ
	M_PO12 = (wake_up2 && IN25);//˾����
	M_PO13 = (DMC_Gear == 2);//�����Ƶ�Դ
	if(g_tInfo.u8VehicleSpeed<3)
	{
		M_PO14 = 1;//�űõ�Դ
	}
	M_PO15 = (wake_up2 && IN23);//���1
	M_PO16 = 0;//���ŵ���

	R_PO1 = (rKL6);//�ƶ���
	R_PO2 = (IN19);//��·��
	R_PO3 = (rLED_flag);//�����
	R_PO4 = (M_ON);//������������Դ
	R_PO5 = (FLASH && (IN1 || wake_up1));//��ת���
	R_PO6 = (M_ON);//��ѹ��������Դ
	R_PO7 = (FLASH && (IN5 || wake_up1));//��ת��
	R_PO8 = IN22;//������
	R_PO9 = (DMC_Gear == 2);//������1
	R_PO10 = IN22;//������
	R_PO11 = (mKH1 && IN6);//ǰ��̤����
	R_PO12 = (IN6);//λ�õ�ʾ�ߵ�
	R_PO13 = (mKL15 && IN6);//����̤����
	R_PO14 = (DMC_Gear == 2);//����������
	R_PO15 = (DMC_Gear == 2);//������2
	R_PO16 = 0;
}

void LED_Logic(void)
{
	//LED1
	if (IN4)                                 //ǰ�ſ�ָʾ
	{
		LED1ON
	}
	else
	{
		LED1OFF
	}

	if (IN3) //���ſ�ָʾ
	//LED3 = (DMC_Gear==0 || DMC_Gear==3); //�յ�ָʾ
	{
		LED2ON
	}
	else
	{
		LED2OFF
	}
	//��ת��
	if (IN1 || wake_up1)
	{
		//LED4 = FLASH;
		LED4ON
	}
	else
	{
		LED4OFF
	}

	//LED5 = (DMC_Gear==1); //ǰ����ָʾ

	if (((BMS_Status_Flag5 & 0x01) == 1) && (((BMS_Status_Flag3 & 0x01) == 1)))
	{
		LED6ON
	} //���״̬
	else
	{
		LED6OFF
	}

	if (((BMS_Status_Flag3 & 0x01) == 1))
	{
		LED7ON
	} //���ǹ����
	else
	{
		LED7OFF
	}
	//��ת���
	if (IN5 || wake_up1)
	{
		//LED8 = FLASH;
		LED8ON
	}
	else
	{
		LED8OFF
	}

	if ( IN11)
	{
		LED9ON
	} //��ѹ1���ź�
	else
	{
		LED9OFF
	}

	LED10ON
	//�ܵ�Դָʾ

	if ( IN12)
	{
		LED11ON
	} //��ѹ2���ź�}
	else
	{
		LED11OFF
	}

	if (((BMS_Status_Flag2 & 0x03) == 1) || ((BMS_Status_Flag2 & 0x03) == 2))
	{
		LED12ON
	} //SOC�ͱ���
	else
	{
		LED12OFF
	}

	if ((DMC_Status == 3 && DMC_Level == 0)
			|| (DMC_Status == 3 && DMC_Level > 0 && FLASH_1s))
	{	LED13ON} //�������
	else
	{	LED13OFF}

	if ((((BMS_Status_Flag1 & 0x03) == 1) || ((BMS_Status_Flag1 & 0x03) == 2))
			|| (((BMS_Status_Flag1 >> 2) & 0x03) == 1)
			|| (((BMS_Status_Flag1 >> 2) & 0x03) == 2)
			|| (((BMS_Status_Flag1 >> 4) & 0x03) == 1)
			|| (((BMS_Status_Flag1 >> 4) & 0x03) == 2)
			|| (((BMS_Status_Flag1 >> 6) & 0x03) == 1)
			|| (((BMS_Status_Flag1 >> 6) & 0x03) == 2)
			|| ((BMS_Status_Flag2 & 0x03) == 1)
			|| ((BMS_Status_Flag2 & 0x03) == 2)
			|| (((BMS_Status_Flag2 >> 2) & 0x03) == 1)
			|| (((BMS_Status_Flag2 >> 2) & 0x03) == 2)
			|| (((BMS_Status_Flag2 >> 4) & 0x03) == 1)
			|| (((BMS_Status_Flag2 >> 4) & 0x03) == 2)
			|| (((BMS_Status_Flag2 >> 6) & 0x03) == 1)
			|| (((BMS_Status_Flag2 >> 6) & 0x03) == 2))
	{
		LED14ON
	} //�������ϵͳ����
	else
	{
		LED14OFF
	}

	if (IN6)
	{
		LED15ON
	} //С��
	else
	{
		LED15OFF
	}

	if (BAT24V_count == BAT24_TIME)
	{
		LED16ON
	} //���ص�ѹ��
	else
	{
		LED16OFF
	}

	LED17= (IN7 && !IN8); //Զ���

	LED18= (IN26); //��ɲ

	LED19= (IN9 && IN6); //ǰ���

	LED20= (IN9 && IN10 && IN6); //�����

	//LED21 = (DMC_Gear==2 /*&& M_ON*/);//������

	LED22= (IN8 && IN6 && IN7); //�����

	//LED23 = (IN14 && IN31 && M_ON);//������

	LED24= (fKL6); //ɲ��ָʾ

	//������ Ϊ0ʱ�죬Ϊ1ʱ����
	if ((pSpeed > 69) || (DMC_Status == 3 && DMC_Level == 0) || mKL2 || fKL15)
		BUZZ= 0;
		else if (!fKL4 || !fKL5 || mKL4 || mKL6 || IN11 ||IN12 //IN30
				|| (DMC_Status == 3 && DMC_Level == 1)
				|| (((BMS_Status_Flag3 >> 5) & 0x01) == 1)
				|| (((BMS_Status_Flag3 >> 6) & 0x01) == 1))
		BUZZ = !FLASH_1s;
		else if (IN1 || IN5 || wake_up1)
		BUZZ = !FLASH;
		else BUZZ = 1;

		//���� С�ƿ��رպ�
	if (IN6)
	{
		//OC1RS = e_backlight;
	}
	else
	{
		//	OC1RS = 0;
	}
	//

	/*if (M_ON) {
	 PO1 = 0;
	 PO2 = 0;
	 } else {
	 PO1 = 1;
	 PO2 = 1;
	 }
	 PO1 = (M_ON || wake_up1 || wake_up2);

	 PO2 = (DMC_Gear == 2);*/

	//����
	/*
	 if (((tcu_start_allow == 1)&&(hcu_starter_forbid != 1)) || (ENG_start_req == 21415))
	 PO1 = 0;
	 else if (hcu_starter_forbid == 1) PO1 = 1;
	 else PO1 = 1;
	 */
	//if (Brake_Pedal == 0) PO2 = 1;
	//else if (Brake_Pedal == 1) PO2 = 0;
	/**************�ְ��ṹ*******************/
	/*
	 mil_lamp_request = (CAN_18FECAEF.bData[0] >> 2)&0x03;
	 fail_mode = (CAN_18FECAEF.bData[0] >> 4)&0x03;
	 mil_buzz_request = (CAN_18FECAEF.bData[0] >> 6)&0x03;
	 Lamp_status_YELLOW = (CAN_18FECA03.bData[0] >> 2)&0x03;
	 Lamp_status_RED = (CAN_18FECA03.bData[0] >> 4)&0x03;
	 */
	//DM_ECU(1);
	/* Maintain_warn = (CAN_18FECA00.bData[0] >> 2)&0x03; //ά������warn
	 Stop_warn = (CAN_18FECA00.bData[0] >> 4)&0x03; //ͣ������stop
	 Engine_warn = (CAN_18FECA00.bData[0] >> 6)&0x03; //����������
	 */
}

//ǰģ��ķ��ͺ���
void BCAN_SendCtl_720(void)
{
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
}

//��ģ��ķ��ͺ���
void BCAN_SendCtl_721(void)
{
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
}

//��ģ��ķ��ͺ���
void BCAN_SendCtl_722(void)
{
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
}

void BCAN_send_mile(void)
{
	CAN_MESSAGE msg;
	stcCANId_t tSendID;
	strCANData_t tSendData;
	msg.data[0] = (unsigned char) (e_total_miles); //�Ͱ�λ
	msg.data[1] = (unsigned char) (e_total_miles >> 8); //������λ
	msg.data[2] = (unsigned char) (e_total_miles >> 16); //������λ
	msg.data[3] = (unsigned char) (e_total_miles >> 24); //�߰�λ
	msg.data[4] = 0;
	msg.data[5] = 0;
	msg.data[6] = 0;
	msg.data[7] = 0;
	memcpy(&tSendData.ui8byte[0], &msg.data[0], 8);
	tSendID.ui32Id = 0x454;
	tSendID.ui8IdType = MSGTYPE_STD;
	tSendData.ui8NumberOfBytes = 0x08;
	//msg.type = 0x01;

	CAN_UpdateAndSendMessage(CAN1, 3, tSendID, tSendData);
	//CAN1_SendMessage(&msg);

}

void BCAN_Lost_handle(void)   //100ms moudle task
{
	  //100ms
	  /*
	  U8 front_moudle_count = 0;
	  U8 middle_moudle_count = 0;
	  U8 back_moudle_count = 0;

	  */
		F100ms= 1;

		//CAN�ڵ���ϼ�ʱ3s
		if (front_moudle_count >= CAN_TIME)
		{
			front_moudle_count = CAN_TIME;
		}
		else
		{
			front_moudle_count++;
		}
		if (middle_moudle_count >= CAN_TIME)
		{
			middle_moudle_count = CAN_TIME;
		}
		else 
		{
			middle_moudle_count++;
		}
		if (back_moudle_count >= CAN_TIME)
		{
			back_moudle_count = CAN_TIME;
		}
		else 
		{
			back_moudle_count++;
		}

		BAT24V_count = BAT24V_count >= BAT24_TIME ? BAT24_TIME : BAT24V_count + 1;

		/*if (cnt4 >= 9) {//1s
		 cnt4 = 0;
		 F1000ms = 1;
		 FLASH_1s = !FLASH_1s;
		 //PCAN_send_mile();
		 //���5�룬10����ѭ��
		 if (DMC_Rpm * 0.5 < 16300) count7 = 0;
		 else count7++;

		 if ((count7 > 0) && (count7 < 6)) {
		 F5s = 1;
		 } else if (count7 < 605) {
		 F5s = 0;
		 } else count7 = 0;
		 //����6�룬10����ѭ������
		 if (1) count8 = 0;
		 else count8++;

		 if ((count8 > 0) && (count8 < 6)) {
		 F_3 = 1;
		 } else if (count8 < 605) {
		 F_3 = 0;
		 }else count8 = 0;

		 } else cnt4++;*/
	

}

void Moudle_Logic_handle(void)  //50ms Logic handle
{
	//BCAN_send_mile(); 
	
	if (M_ON&& (wake_up3 || wake_up2))
	{ //������main������ᵼ����˸Ƶ���쳣wake_up3��0ʱ��M_ON����������0����Ϊ����������ʱ
	//	LED_Logic();
//		LED_Out();  //���ֻҪ�򿪾ͻᵼ��Ƶ������ ��Ϊ����ǿպ���
		SYSTME_Logic();
	}

	BCAN_SendCtl_720();
	BCAN_SendCtl_721();
	BCAN_SendCtl_722();
	F50ms= 1;
}


void moudle_task_10ms(void)
{
	static unsigned int cnt3 = 9;
	static unsigned int Fcount = 0;
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
