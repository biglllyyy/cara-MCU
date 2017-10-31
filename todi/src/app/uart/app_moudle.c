#include "app_moudle.h"
#include "hal_GPIO.h"
#include "uart_cfg.h"
#include "app_info.h"
#include "mb91520.h"

//U8 pSpeed = 0; //外部CAN这算车速
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

U16 Ncan_count = 0; //发动机ECU模块
U16 BAT24V_count = 0; //蓄电池低压报警计时

U8 front_moudle_count = 0;
U8 middle_moudle_count = 0;
U8 back_moudle_count = 0;


U16 M_PO11_count = 1000; //输出10秒计时

DBCM_KEY fKEY; //前模块功率开关状态
DBCM_KEY mKEY; //中（顶）模块功率开关状态
DBCM_KEY rKEY; //后模块功率开关状态

U16 fFreq; //前模块频率
U16 mFreq; //中（顶）模块频率
U16 rFreq; //后模块频率

U16 fSpeed; //前模块折算车速
U16 mSpeed; //中（顶）模块折算车速
U16 rSpeed; //后模块折算车速
U8 pSpeed = 0; //外部CAN这算车速 KM/H

U16 fADR[2]; //前模块电阻值
U16 mADR[2]; //中（顶）模块电阻值
U16 rADR[2]; //后模块电阻值

U16 ADR[8] =
{ 0 }; //模拟量采集   10/15
U16 ADV[8] =
{ 0 }; //模拟量采集

DBCM_POUT fPOUT =
{ 0 }; //前模块功率输出状态
DBCM_POUT mPOUT =
{ 0 }; //中（顶）模块功率输出状态
DBCM_POUT rPOUT =
{ 0 }; //后模块功率输出状态

U8 fPF[16] =
{ 0 }; //前模块功率输出故障
U8 mPF[16] =
{ 0 }; //中（顶）模块功率故障
U8 rPF[16] =
{ 0 }; //后模块功率输出故障

U16 fpcur[15] =
{ 0 }; //前模块功率输出电流
U16 mpcur[15] =
{ 0 }; //中模块功率输出电流
U16 rpcur[15] =
{ 0 }; //后模块功率输出电流

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
//	F_PO1 = (IN16 && M_ON); //雨刮快档
//	F_PO2 = ((IN14 || IN15 || IN21)&& M_ON); //雨刮慢档
//	F_PO3 = (IN21 && M_ON); //喷水电机
	F_PO4 = (IN6 && IN9); //左前雾灯
	F_PO5 = (IN6); //小灯（开关照明，开关小灯)
	F_PO6 = (FLASH && (IN5 || wake_up1)); //右前转向灯
	F_PO7 = (FLASH && (IN1 || wake_up1)); //左前转向灯
	F_PO8 = fKL3; //电喇叭
	F_PO9 = (IN7); //远关灯
	F_PO10 = (IN6 && mKL15); //前门踏步灯
	F_PO11 = (IN7); //远光灯
	F_PO12 = rKL3; //刹车灯
	F_PO13 = (IN6 && IN8); //近光灯
	F_PO14 = (IN6); //小灯(前位置灯，广告灯箱)
	F_PO15 = !(IN6 || IN7 || IN8);//日间行车灯

	M_PO1 = (IN20 && (pSpeed < 5)); //前门开门电磁阀
	M_PO2 = (IN28); //前门关电磁阀
	M_PO3 = (IN27 && (pSpeed < 5)); //前门开电磁阀
	M_PO4 = (IN26); //中门关电磁阀
	M_PO5 = (mKL5); //车速灯3（左厢灯2档）
	M_PO6 = (mKL7); //车速灯2（右厢灯2档）
	M_PO7 = (mKL4); //左日光灯（左厢灯1档）
	M_PO8 = (mKL6); //右日光灯（右厢灯1档）
	M_PO9 = (M_ON);	//钥匙2档（空调电源）
	M_PO10 = (M_ON); //钥匙2档（车载电视）
	M_PO11 = (M_ON); //钥匙2档（换气扇电源）
	M_PO12 = (mKL1 && mKL3); //司机灯12
	M_PO13 = (mKL8); //路牌灯
	M_PO14 = (mKL9); //车内显示屏
	M_PO15 = (IN6); //小灯（顶位置灯）

	R_PO1 = (mKH1 && IN6); //中门踏步灯
	R_PO2 = (IN6);//小灯(后位置灯)
	R_PO3 = (rKL3); //刹车灯
	R_PO4 = (mKL8);//侧路牌
	R_PO5 = (IN6 && IN9 && IN10); //后雾灯
	R_PO6 = (M_ON); //总电源继电器(延时3s断电)
	R_PO7 = (M_ON && (vcu_msg.ID_0C018980_byte4.bit02 ==0x04)); //倒车灯
	R_PO8 = M_ON; //干燥器电源
	R_PO9 = (rKL3); //冷凝器电源
	R_PO10 = 0; //排气电磁阀输出
	R_PO11 = (rKL1); //后仓灯
	R_PO12 = (IN6); //左转向灯（侧）
	R_PO13 = (mKH1 && IN6); //左转向灯（LED）
	R_PO14 = (IN19 && M_ON);//右转向灯（侧）
	R_PO15 = (FLASH && IN1 && M_ON); //右转向灯（LED）
}


void LED_Logic(void)
{}

//前模块的发送函数
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
	temp.bits.bit2 = IN14; //慢档
	temp.bits.bit3 = IN16; //快档
	temp.bits.bit4 = IN15; //间歇 
	temp.bits.bit5 = IN21; //喷水
	if (IN14 && IN16)
		temp.bits.bit2 = 0;
	if (IN21)
	{
		temp.bits.bit2 = 0; //慢档
		temp.bits.bit3 = 0; //间歇
		temp.bits.bit4 = 0; //快档
	}
	
	message_POUT.data[5] = temp.byte;//((temp.bits.bit5<<5)|(temp.bits.bit4<<4)|(temp.bits.bit3<<3)|(temp.bits.bit2<<2)|(temp.bits.bit1));//

	message_POUT.data[6] = 0; //pwm 频率输出
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

//中模块的发送函数
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

//后模块的发送函数
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

	//CAN节点故障计时3s
    if (Fcan_count >= CAN_TIME)Fcan_count = CAN_TIME;
    else Fcan_count++;
    if (Rcan_count >= CAN_TIME)Rcan_count = CAN_TIME;
    else Rcan_count++;
    if (Mcan_count >= CAN_TIME)Mcan_count = CAN_TIME;
    else Mcan_count++;
	if (IN10 && IN6 && (IN7 || IN8) && IN9) { //后雾灯开关是常开开关，消抖
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
	{ //若放在main函数里，会导致闪烁频率异常wake_up3置0时，M_ON不会立马置0，因为存在消抖延时
	//	LED_Logic();
//		LED_Out();  //这个只要打开就会导致频繁重启 因为这个是空函数
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
	if (wake_up3) { //ON开关判断//消抖//M_ON在掉电的一瞬间会使上拉电源无效，而M_ON在断电会保持300ms，从而影响开关量采集
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
