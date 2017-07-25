#include <stdlib.h>
#include "app_can.h"
#include "hal_can.h"
#include "can_cfg.h"
#include "spd_cfg.h"
#include "rpm_cfg.h"
#include "app_spd.h"
#include "app_rpm.h"
#include "app_temp.h"
#include "temp_cfg.h"
#include "mid_string.h"
#include "mid_can.h"
#include "string.h"
#include "g_variable.h"
#include "hal_wdg.h"
#include "app_io.h"
#include "app_fuel.h"
#include "app_service.h"
#include "app_backlight.h"
#include "app_lin.h"
#include "app_led.h"
#include "app_trip.h"
#include "app_IPconfig.h"
#include "mid_dbg.h"
#include "app_uart_cmd.h"
#include "app_system.h"
#include "app_fuel_method2.h"
#include "app_uds.h"
#include "app_info.h"
#include "app_moudle.h"

#define  PERIOD_MS(time)		(time)

/******************************************************************************/
/*
 byte  =  0    1    2     3     4     5     6     7
 start = 7~0,15~8,23~16,31~24,39~32,47~40,55~48,63~56
 end   = 7~0,15~8,23~16,31~24,39~32,47~40,55~48,63~56
 (start >= end) && ((start / 8) == (end / 8))
 */
/*
 the  following define macro used cont the bit in the can msg. for simply the operation to shift
 */
#define BITS_COUNT(start_bit, end_bit) ((end_bit) - (start_bit) + 1)

#define CAN_BYTE8(byte) ((U8)(msg->data[(byte)]))

/*实现一个byte内的任意数据长度数据获取*/
#define CAN_BITS8(start_bit, end_bit) \
	((U8)(((msg->data[(end_bit) / 8]) >> ((start_bit) % 8)) & BitMask8[(end_bit) - (start_bit)]))

#define CAN_BITS16(hi_start, hi_end, lo_start, lo_end) \
	(((U16)CAN_BITS8(hi_start, hi_end) << BITS_COUNT(lo_start, lo_end)) | \
	((U16)CAN_BITS8(lo_start, lo_end)))

#define CAN_BYTE16(hibyte, lobyte) \
	(((U16)CAN_BYTE8(hibyte) << 8) | CAN_BYTE8(lobyte))

/******************************************************************************/
/* variable define */
static U8 msg_buf_num = 0;	//record msg buf mum
can_msg_t can_msg[ID_PROCESS_ALL] = { 0 };
static pCanAnalyse can_rx_handle[ID_RECV_NUM_ALL] = { NULL };
U32 can_rx_msg_id_ads[ID_RECV_NUM_ALL] = { 0 };
/*******************分包处理*****************/
//VCU

U8 VCU_TM_Brake_percent = 0; //制动踏板开度
U8 VCU_TM_Speed_percent = 0; //加速踏板开度
DATA_BIT VCU_Status_Flag1 = {0}; //状态1
U8 VCU_Gear = 0; //档位
DATA_BIT VCU_Mode = {0}; //整车模式
U8 VCU_Life = 0; //整车控制器LIFE
DATA_BIT VCU_Status_Flag2 = {0};
U8 VCU_Code = 0; //整车故障码
// VCU2TERMINAL_MOTOR01
U8 TM_Number = 0; //电机个数
U8 TM_Serial = 0; //电机序号
U8 TM_WorkStatus = 0; //TM驱动电机状态
U8 TM_Control_Temp = 40; //TM电机控制器温度 main
U16  TM_Feedback_RPM = 20000; //TM电机转速 main
U16  TM_Feedback_NM = 20000; //TM电机转矩 main
// VCU2TERMINAL_MOTOR02
U8 TM_Temp = 40; //TM电机温度   main
U16  TM_Voltage_DC = 0; //TM电机直流电压
U16  TM_Current_DC = 10000; //TM电机直流电流

//IRM01
U32 IRM_Ohm_Positive = 0; //总正绝缘阻值
U32 IRM_Ohm_Negative = 0; //总负绝缘阻值
U8 IRM_Fault_Level = 0; //设备故障等级
U8 IRM_Insulation_Level = 0; //绝缘等级
U8 IRM_Life = 0; //绝缘LIFE


U8 BMS_Mode = 0; //电池充放电模式
U8 BMS_Status = 0; //电池状态
U8 BAT_Temp_Average = 40; //电池平均温度
U8 BMS_SOC = 0; //SOC   main
U16  BMS_Current = 32000; //电池系统电流 main
U16  BMS_Voltage = 0; //电池系统内总电压  main
U8 BMS_Kt = 0; //高压继电器状态

U8 BAT_Temp_H_Limit = 40; //最高允许电池单体温度
U8 BAT_Temp_L_Limit = 40; //最低允许电池单体温度
U8 BMS_SOC_L_Limit = 0; //最低允许SOC值
U16  BAT_Current_Discharge_Limit = 32000; //最高允许放电电流
U16  BAT_Current_Charge_Limit = 32000; //最高允许充电电流

U8 BAT_Temp_L = 40; //电池单体最低温度  main
U8 BAT_Temp_L_Number = 0; //电池单体低温度序号
U8 BAT_Temp_L_Case = 0; //电池单体低温度箱号
U8 BAT_Temp_H = 40; //电池单体最高温度  main
U8 BAT_Temp_H_Number = 0; //电池单体高温度序号
U8 BAT_Temp_H_Case = 0; //电池单体高温度箱号

U8 BAT_Voltage_Fault = 0; //总压故障
U8 BAT_Single_Fault = 0; //单压故障
U8 BAT_Temp_Fault = 0; //温度故障
U8 BAT_Insulation_Fault = 0; //绝缘故障
U8 BAT_Consistency_Fault = 0; //电池一致性故障
U8 BAT_SOC_Fault = 0; //SOC故障
U8 BAT_Current_Fault = 0; //电流故障
U8 BAT_Lock_Fault = 0; //高压互锁故障
U8 BMS_Comm_Fault = 0; //BMS通讯故障
U8 BMS_System_Fault = 0; //BMS系统故障

U16  BMS_Ohm = 0; //绝缘阻值
U8 BMS_Number = 0; //单体电压数量
U8 BMS_Temp_Number = 0; //单体温度数量
U8 BMS_System_Unit = 0; //系统数量
U8 BMS_System_Number = 0; //系统号

U16  BAT_Single_L = 0; //电池单体低电压  main
U8 BAT_Single_L_Number = 0; //电池单体低电压位置
U8 BAT_Single_L_Case = 0; //电池单体低电压箱号

U16  BAT_Single_H = 0; //电池单体高电压  main
U8 BAT_Single_H_Number = 0; //电池单体高电压位置
U8 BAT_Single_H_Case = 0; //电池单体高电压箱号


U16  BAT_Cell_Voltage[400]={0};
U8 BAT_Cell_Temperature[128]={0};

U16  DCDC_Voltage = 0; //DCDC输出电压
U16  DCDC_Current = 0; //DCDC输出电流
U8 DCDC_Status = 0; //DCDC工作状态
U8 DCDC_Output_Cut = 0; //DCDC 输出切断
U8 DCDC_intput_Cut = 0; //DCDC 输入切断
U8 DCDC_Fault_Code = 0; //DCDC故障码
U8 DCDC_Level_Fault = 0; //DCDC故障等级
U8 DCDC_Temp_Warn = 0; //DCDC温度报警
U8 DCDC_Fault_Warn = 0; //DCDC故障报警
U8 DCDC_Temp = 40; //DCDC模块温度

U16  OILDCAC_U_Voltage = 0; //DCAC输出U电压
U16  OILDCAC_U_Current = 0; //DCAC输出U电流
U8 OILDCAC_Status = 0; //DCAC工作状态
U8 OILDCAC_Output_Cut = 0; //DCAC 输出切断
U8 OILDCAC_intput_Cut = 0; //DCAC 输入切断
U8 OILDCAC_Fault_Code = 0; //DCAC故障代码
U8 OILDCAC_Level_Fault = 0; //DCAC故障等级
U8 OILDCAC_Temp_Warn = 0; //DCDC温度报警
U8 OILDCAC_Fault_Warn = 0; //DCDC故障报警
U8 OILDCAC_Temp = 40; //DCAC模块温度

U16  OILDCAC_V_Voltage = 0; //DCAC输出V电压
U16  OILDCAC_V_Current = 0; //DCAC输出V电流
U16  OILDCAC_W_Voltage = 0; //DCAC输出W电压
U16  OILDCAC_W_Current = 0; //DCAC输出W电流

U16  AIRDCAC_U_Voltage = 0; //DCAC输出U电压
U16  AIRDCAC_U_Current = 0; //DCAC输出U电流
U8 AIRDCAC_Status = 0; //DCAC工作状态
U8 AIRDCAC_Output_Cut = 0; //DCAC 输出切断
U8 AIRDCAC_intput_Cut = 0; //DCAC 输入切断
U8 AIRDCAC_Fault_Code = 0; //DCAC故障代码
U8 AIRDCAC_Level_Fault = 0; //DCAC故障等级
U8 AIRDCAC_Temp_Warn = 0; //DCDC温度报警
U8 AIRDCAC_Fault_Warn = 0; //DCDC故障报警
U8 AIRDCAC_Temp = 40; //DCAC模块温度

U16  AIRDCAC_V_Voltage = 0; //DCAC输出V电压
U16  AIRDCAC_V_Current = 0; //DCAC输出V电流
U16  AIRDCAC_W_Voltage = 0; //DCAC输出W电压
U16  AIRDCAC_W_Current = 0; //DCAC输出W电流

DATA_DOUBLE ICS01_Status1 = {0}; //高压采集状态1
DATA_DOUBLE ICS01_Status2 = {0}; //高压采集状态2
DATA_DOUBLE ICS01_Status3 = {0}; //高压采集状态3
DATA_DOUBLE ICS01_Status4 = {0}; //高压采集状态4

U16  ICS01_Voltage_Front = 0; //前端电压
U16  ICS01_Voltage_Rear = 0; //后端电压
U8 ICS01_LIFE = 0; //LIFE

/******************************************/

can_signal_t can0 = { 0 };
#if 1
can_signal_t can1= {0};
can_signal_t can2= {0};
can_signal_t can3= {0};
can_signal_t can4= {0};
can_signal_t can5= {0};
#endif
//UDS_id_766  uds766;

extern led_check_t led_check;
/* function declare */
static void can_id_7E7_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_100017EF_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_1801FBEF_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_1811FBEF_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_18FF08F2_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_10F8159E_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_10F8169E_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_18F8179E_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_18F8189E_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_18F8199E_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_18F81A9E_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_18FF12F7_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_18FF0AF8_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_18FF0BF8_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_18FF0CF9_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_18FF0DF9_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_1801EFA9_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_104C1000_analyse(can_msg_t *msg, can_pro_way_e way);



static void app_can_process(can_msg_t *msg, can_pro_way_e way);

static void app_bound_id(U32 id,U32 id_mask, U8 dlc, CAN_ID_FORMAT_e format, U8 msg_buf,
		U16 period) {

	if ((msg_buf_num <= ID_PROCESS_ALL) && (msg_buf >= 1)) {
		can_msg[msg_buf_num].id = id;
		can_msg[msg_buf_num].id_mask= id_mask;
		can_msg[msg_buf_num].dlc = dlc;
		can_msg[msg_buf_num].format = format;
		can_msg[msg_buf_num].buffer_num = msg_buf;
		if (period >= PERIOD_MS(1000)) {
			can_msg[msg_buf_num].period_ms = period >> 1;
		} else {
			can_msg[msg_buf_num].period_ms = period;
		}

		msg_buf_num++;
	}
}

void app_init_variable(void) {
	U8 i = 0;
	for (i = 0; i < ID_RECV_NUM_ALL; i++) {
		memset(&can_msg[i], 0, sizeof(can_msg_t));
		can_msg[i].lost = 1;
	}
	msg_buf_num = 0;
}

void app_init_can(void) {
	mid_can_prepare(ID_RECV_NUM_ALL, ID_SENT_NUM_ALL, can_msg);
	//mid_can_init(CAN_CHN, CAN_CHIP);
	app_init_variable();
	can_chip_mode_switch(&io_can_stb, &io_can_en, NORMAL_MODE);
	/* receive id bound,add your code here,the id need to be received */
	app_bound_id(PC_T0_MCU_ID, 0xfff, 8, STAND_ID, 1, PERIOD_MS(100));
	
	app_bound_id(ID_REC_01_100017EF, 0x1FFFFFFF, 8, EXTERN_ID, 2,  (ID_100017EF_period*50));
	app_bound_id(ID_REC_02_1801FBEF, 0x1FFFFFFF, 8, EXTERN_ID, 3,  (ID_1801FBEF_period*50));
	app_bound_id(ID_REC_03_1811FBEF, 0x1FFFFFFF, 8, EXTERN_ID, 4,  (ID_1811FBEF_period*50));
	app_bound_id(ID_REC_04_18FF08F2, 0x1FFFFFFF, 8, EXTERN_ID, 5,  (ID_18FF08F2_period*50));
	app_bound_id(ID_REC_05_10F8159E, 0x1FFFFFFF, 8, EXTERN_ID, 6,  (ID_10F8159E_Period*50));
	app_bound_id(ID_REC_06_10F8169E, 0x1FFFFFFF, 8, EXTERN_ID, 7,  (ID_10F8169E_Period*50));
	app_bound_id(ID_REC_07_18F8179E, 0x1FFFFFFF, 8, EXTERN_ID, 8,  (ID_18F8179E_Period*50));
	app_bound_id(ID_REC_08_18F8189E, 0x1FFFFFFF, 8, EXTERN_ID, 9,  (ID_18F8189E_Period*50));
	app_bound_id(ID_REC_09_18F8199E, 0x1FFFFFFF, 8, EXTERN_ID, 10, (ID_18F8199E_Period*50));
	app_bound_id(ID_REC_10_18F81A9E, 0x1FFFFFFF, 8, EXTERN_ID, 11, (ID_18F81A9E_Period*50));
	app_bound_id(ID_REC_11_18FF12F7, 0x1FFFFFFF, 8, EXTERN_ID, 12, (ID_18FF12F7_Period*50));
	app_bound_id(ID_REC_12_18FF0AF8, 0x1FFFFFFF, 8, EXTERN_ID, 13, (ID_18FF0AF8_Period*50));
	app_bound_id(ID_REC_13_18FF0BF8, 0x1FFFFFFF, 8, EXTERN_ID, 14, (ID_18FF0BF8_Period*50));
	app_bound_id(ID_REC_14_18FF0CF9, 0x1FFFFFFF, 8, EXTERN_ID, 15, (ID_18FF0CF9_Period*50));
	app_bound_id(ID_REC_15_18FF0DF9, 0x1FFFFFFF, 8, EXTERN_ID, 16, (ID_18FF0DF9_Period*50));
	//app_bound_id(ID_REC_17_104C1000, 0x1FFFF000, 8, EXTERN_ID, 18, (ID_104C1000_Period*50));
	app_bound_id(ID_REC_16_1801EFA9, 0x1FFFFFFF, 8, EXTERN_ID, 17, (ID_1801EFA9_Period*50));
	app_bound_id(ID_REC_17_104C1000, 0x1FFFF000, 8, EXTERN_ID, 18, (ID_104C1000_Period*50));
	
	

	/* sent id bound,add your code here */
	app_bound_id(MCU_TO_PC_ID, 0xfff, 8, STAND_ID, (ID_RECV_NUM_ALL + 1),PERIOD_MS(100));
	app_bound_id(ID_SEND_1_1801EF17, 0x1FFFFFFF, 8, EXTERN_ID, (ID_RECV_NUM_ALL + 2),ID_1801EF17_Period);
	app_bound_id(ID_SEND_2_1802EF17, 0x1FFFFFFF, 8, EXTERN_ID, (ID_RECV_NUM_ALL + 3),ID_1802EF17_Period);
	

	/* register the can id process function */
	can_rx_pro = app_can_process;
	/*register the can id process */
	
	can_rx_handle[0]  = can_id_7E7_analyse;
	can_rx_handle[1]  = can_id_100017EF_analyse;
	can_rx_handle[2]  = can_id_1801FBEF_analyse;
	can_rx_handle[3]  = can_id_1811FBEF_analyse;
	can_rx_handle[4]  = can_id_18FF08F2_analyse;
	can_rx_handle[5]  = can_id_10F8159E_analyse;
	can_rx_handle[6]  = can_id_10F8169E_analyse;
	can_rx_handle[7]  = can_id_18F8179E_analyse;
	can_rx_handle[8]  = can_id_18F8189E_analyse;
	can_rx_handle[9]  = can_id_18F8199E_analyse;
	can_rx_handle[10] = can_id_18F81A9E_analyse;
	can_rx_handle[11] = can_id_18FF12F7_analyse;
	can_rx_handle[12] = can_id_18FF0AF8_analyse;
	can_rx_handle[13] = can_id_18FF0BF8_analyse;
	can_rx_handle[14] = can_id_18FF0CF9_analyse;
	can_rx_handle[15] = can_id_18FF0DF9_analyse;
	can_rx_handle[16] = can_id_1801EFA9_analyse;
	can_rx_handle[17] = can_id_104C1000_analyse;
	

	mid_can_init(CAN_CHN, CAN_CHIP);

}

static void app_can_process(can_msg_t *msg, can_pro_way_e way) {
	if (msg->buffer_num <= ID_RECV_NUM_ALL) {
		if (can_rx_handle[msg->buffer_num - 1] != NULL) {
			can_rx_handle[msg->buffer_num - 1](msg, way);
		}
	}
}
static void can_id_7E7_analyse(can_msg_t *msg, can_pro_way_e way) {
	switch (way) {
	case CAN_PARSE:
		if (memcmp(msg->data, "RESETMCU", 8) == 0) {
			dbg_string("Firmware upgrade request from CAN. rebooting...\n");
			app_soft_reset(); /* MCU reset */
		} else if (memcmp(msg->data, "UPSTART", 7) == 0) {
			if (OFF == g_u8IgnSts) {
				mid_set_io_sts(&IO_MCU_CTL_START_UP, ON);
				g_u8_A20_update_sts = 1;
			}
		}
		//dbg_string("CAN CMD:%s\n", msg->data);
		break;
	case CAN_LOST:

		break;
	default:
		break;
	}
}

static void can_id_100017EF_analyse(can_msg_t *msg, can_pro_way_e way) {
	switch (way) {
	case CAN_PARSE:
		VCU_TM_Speed_percent = (U8) msg->data[0];
        VCU_TM_Brake_percent = (U8) msg->data[1];
        VCU_Status_Flag1.byte = (U8) msg->data[2];
        VCU_Gear = msg->data[3]&0x0F;
        VCU_Mode.byte = msg->data[3] & 0xF0;
        VCU_Life = msg->data[4]&0x0F;
        VCU_Status_Flag2.byte = msg->data[4];
        VCU_Code = msg->data[7];
		break;
	case CAN_LOST:
		VCU_TM_Speed_percent = 0;
        VCU_TM_Brake_percent = 0;
        VCU_Status_Flag1.byte = 0;
        VCU_Gear = 0;
        VCU_Mode.byte = 0;
        VCU_Life = 0;
        VCU_Status_Flag2.byte = 0;
        VCU_Code = 0;
		break;
	default:
		break;
	}
}
static void can_id_1801FBEF_analyse(can_msg_t *msg, can_pro_way_e way) {
	switch (way) {
	case CAN_PARSE:
		TM_Number = msg->data[0];
        TM_Serial = msg->data[1];
        TM_WorkStatus = msg->data[2];
        TM_Control_Temp = msg->data[3];
        TM_Feedback_RPM = (U8) msg->data[4]+((U16) msg->data[5] << 8);
        TM_Feedback_NM = (U8) msg->data[6]+((U16) msg->data[7] << 8);
		break;
	case CAN_LOST:
		TM_Number = 0;
        TM_Serial = 0;
        TM_WorkStatus = 0;
        TM_Control_Temp = 40;
        TM_Feedback_RPM = 20000;
        TM_Feedback_NM = 20000;
		break;
	default:
		break;
	}
}
static void can_id_1811FBEF_analyse(can_msg_t *msg, can_pro_way_e way) {
	switch (way) {
	case CAN_PARSE:
		TM_Temp = msg->data[0];
        TM_Voltage_DC = (U8) msg->data[1]+((U16) msg->data[2] << 8);
        TM_Current_DC = (U8) msg->data[3]+((U16) msg->data[4] << 8);
		break;
	case CAN_LOST:
		TM_Temp = 40;
        TM_Voltage_DC = 0;
        TM_Current_DC = 10000;
		break;
	default:
		break;
	}
}
static void can_id_18FF08F2_analyse(can_msg_t *msg, can_pro_way_e way) {
	DATA_DOUBLE CAN_DOUBLE;
	switch (way) {
	case CAN_PARSE:
		IRM_Ohm_Positive = (U8) msg->data[0]+((U16) msg->data[1] << 8)+((U32) msg->data[2] << 16);
		IRM_Ohm_Negative = (U8) msg->data[3]+((U16) msg->data[4] << 8)+((U32) msg->data[5] << 16);

		CAN_DOUBLE.byte = msg->data[6];
		IRM_Fault_Level = CAN_DOUBLE.bits.bit4;
		IRM_Insulation_Level = CAN_DOUBLE.bits.bit3;

		IRM_Life = msg->data[7];

		break;
	case CAN_LOST:
		IRM_Ohm_Positive = 0;
		IRM_Ohm_Negative = 0;

		CAN_DOUBLE.byte = 0;
		IRM_Fault_Level = CAN_DOUBLE.bits.bit1;
		IRM_Insulation_Level = CAN_DOUBLE.bits.bit2;

		IRM_Life = 0;
		break;
	default:
		break;
	}
}
static void can_id_10F8159E_analyse(can_msg_t *msg, can_pro_way_e way) {
	switch (way) {
	case CAN_PARSE:
		BMS_Mode = msg->data[0]&0x07;
        BMS_Status = (msg->data[0] >> 6)&0x03;
        BAT_Temp_Average = msg->data[1];
        BMS_SOC = msg->data[2];
        BMS_Current = (U8) msg->data[3]+((U16) msg->data[4] << 8);
        BMS_Voltage = (U8) msg->data[5]+((U16) msg->data[6] << 8);
        BMS_Kt = (msg->data[7] >> 4)&0x0F;
		break;
	case CAN_LOST:
		BMS_Mode = 0;
        BMS_Status = 0;
        BAT_Temp_Average = 40;
        BMS_SOC = 0;
        BMS_Current = 32000;
        BMS_Voltage = 0;
        BMS_Kt = 0;
		break;
	default:
		break;
	}
}
static void can_id_10F8169E_analyse(can_msg_t *msg, can_pro_way_e way) {
	switch (way) {
	case CAN_PARSE:
		BAT_Temp_H_Limit = msg->data[0];
        BAT_Temp_L_Limit = msg->data[1];
        BMS_SOC_L_Limit = msg->data[2];
        BAT_Current_Discharge_Limit = (U8) msg->data[3]+((U16) msg->data[4] << 8);
        BAT_Current_Charge_Limit = (U8) msg->data[5]+((U16) msg->data[6] << 8);
		break;
	case CAN_LOST:
		BAT_Temp_H_Limit = 40;
        BAT_Temp_L_Limit = 40;
        BMS_SOC_L_Limit = 0;
        BAT_Current_Discharge_Limit = 32000;
        BAT_Current_Charge_Limit = 32000;
		break;
	default:
		break;
	}
}
static void can_id_18F8179E_analyse(can_msg_t *msg, can_pro_way_e way) {
	switch (way) {
	case CAN_PARSE:
		BAT_Temp_L = msg->data[0];
		BAT_Temp_L_Number = msg->data[1];
		BAT_Temp_L_Case = msg->data[2];
		BAT_Temp_H = msg->data[3];
		BAT_Temp_H_Number = msg->data[4];
		BAT_Temp_H_Case = msg->data[5];
		break;
	case CAN_LOST:
		BAT_Temp_L = 40;
		BAT_Temp_L_Number = 0;
		BAT_Temp_L_Case = 0;
		BAT_Temp_H = 40;
		BAT_Temp_H_Number = 0;
		BAT_Temp_H_Case = 0;
		break;
	default:
		break;
	}
}
static void can_id_18F8189E_analyse(can_msg_t *msg, can_pro_way_e way) {
	switch (way) {
	case CAN_PARSE:
		BAT_Voltage_Fault = msg->data[0]&0x0F;
		BAT_Single_Fault = (msg->data[0] >> 4)&0x0F;
		BAT_Temp_Fault = msg->data[1]&0x3F;
		BAT_Insulation_Fault = (msg->data[1] >> 6)&0x3F;
		BAT_Consistency_Fault = msg->data[2]&0x03;
		BAT_SOC_Fault = (msg->data[2] >> 2)&0x3F;
		BAT_Current_Fault = msg->data[3]&0x0F;
		BAT_Lock_Fault = (msg->data[3] >> 4)&0x03;
		BMS_Comm_Fault = (msg->data[3] >> 6)&0x03;
		BMS_System_Fault = msg->data[4]&0x01;
		break;
	case CAN_LOST:
		BAT_Voltage_Fault = 0;
		BAT_Single_Fault = 0;
		BAT_Temp_Fault = 0;
		BAT_Insulation_Fault = 0;
		BAT_Consistency_Fault = 0;
		BAT_SOC_Fault = 0;
		BAT_Current_Fault = 0;
		BAT_Lock_Fault = 0;
		BMS_Comm_Fault = 0;
		BMS_System_Fault = 0;
		break;
	default:
		break;
	}
}
static void can_id_18F8199E_analyse(can_msg_t *msg, can_pro_way_e way) {
	switch (way) {
	case CAN_PARSE:
		BMS_Ohm = (U8) msg->data[0]+((U16) msg->data[1] << 8);
		BMS_Number = msg->data[2];
		BMS_Temp_Number = msg->data[3];
		BMS_System_Unit = msg->data[4];
		BMS_System_Number = msg->data[5];
		break;
	case CAN_LOST:
		BMS_Ohm = 0;
		BMS_Number = 0;
		BMS_Temp_Number = 0;
		BMS_System_Unit = 0;
		BMS_System_Number = 0;
		break;
	default:
		break;
	}
}
static void can_id_18F81A9E_analyse(can_msg_t *msg, can_pro_way_e way) {
	switch (way) {
	case CAN_PARSE:
		BAT_Single_L = (U8) msg->data[0]+((U16) msg->data[1] << 8);
		BAT_Single_L_Number = msg->data[2];
		BAT_Single_L_Case = msg->data[3];
		BAT_Single_H = (U8) msg->data[4]+((U16) msg->data[5] << 8);
		BAT_Single_H_Number = msg->data[6];
		BAT_Single_H_Case = msg->data[7];
		break;
	case CAN_LOST:
		BAT_Single_L = 0;
		BAT_Single_L_Number = 0;
		BAT_Single_L_Case = 0;
		BAT_Single_H = 0;
		BAT_Single_H_Number = 0;
		BAT_Single_H_Case = 0;
		break;
	default:
		break;
	}
}
static void can_id_18FF12F7_analyse(can_msg_t *msg, can_pro_way_e way) {
	switch (way) {
	case CAN_PARSE:
		DCDC_Voltage = (U8) msg->data[0]+((U16) msg->data[1] << 8);
		DCDC_Current = (U8) msg->data[2]+((U16) msg->data[3] << 8);
		DCDC_Status = msg->data[4]&0x03;
		DCDC_Output_Cut = (msg->data[4] >> 2)&0x01;
		DCDC_intput_Cut = (msg->data[4] >> 3)&0x01;
		DCDC_Fault_Code = msg->data[5];
		DCDC_Level_Fault = msg->data[6]&0x03;
		DCDC_Temp_Warn = (msg->data[6] >> 2)&0x01;
		DCDC_Fault_Warn = (msg->data[6] >> 3)&0x01;
		DCDC_Temp = msg->data[7];
		break;
	case CAN_LOST:
		DCDC_Voltage = 0;
		DCDC_Current = 0;
		DCDC_Status = 0;
		DCDC_Output_Cut = 0;
		DCDC_intput_Cut = 0;
		DCDC_Fault_Code = 0;
		DCDC_Level_Fault = 0;
		DCDC_Temp_Warn = 0;
		DCDC_Fault_Warn = 0;
		DCDC_Temp = 40;
		break;
	default:
		break;
	}
}
static void can_id_18FF0AF8_analyse(can_msg_t *msg, can_pro_way_e way) {
	switch (way) {
	case CAN_PARSE:
		OILDCAC_U_Voltage = (U8) msg->data[0]+((U16) msg->data[1] << 8);
		OILDCAC_U_Current = (U8) msg->data[2]+((U16) msg->data[3] << 8);
		OILDCAC_Status = msg->data[4]&0x03;
		OILDCAC_Output_Cut = (msg->data[4] >> 2)&0x01;
		OILDCAC_intput_Cut = (msg->data[4] >> 3)&0x01;
		OILDCAC_Fault_Code = msg->data[5];
		OILDCAC_Level_Fault = msg->data[6]&0x03;
		OILDCAC_Temp_Warn = (msg->data[6] >> 2)&0x01;
		OILDCAC_Fault_Warn = (msg->data[6] >> 3)&0x01;
		OILDCAC_Temp = msg->data[7];
		break;
	case CAN_LOST:
		OILDCAC_U_Voltage = 0;
		OILDCAC_U_Current = 0;
		OILDCAC_Status = 0;
		OILDCAC_Output_Cut = 0;
		OILDCAC_intput_Cut = 0;
		OILDCAC_Fault_Code = 0;
		OILDCAC_Level_Fault = 0;
		OILDCAC_Temp_Warn = 0;
		OILDCAC_Fault_Warn = 0;
		OILDCAC_Temp = 40;
		break;
	default:
		break;
	}
}
static void can_id_18FF0BF8_analyse(can_msg_t *msg, can_pro_way_e way) {
	switch (way) {
	case CAN_PARSE:
		OILDCAC_V_Voltage = (U8) msg->data[0]+((U16) msg->data[1] << 8);
		OILDCAC_V_Current = (U8) msg->data[2]+((U16) msg->data[3] << 8);
		OILDCAC_W_Voltage = (U8) msg->data[4]+((U16) msg->data[5] << 8);
		OILDCAC_W_Current = (U8) msg->data[6]+((U16) msg->data[7] << 8);
		break;
	case CAN_LOST:
		OILDCAC_V_Voltage = 0;
		OILDCAC_V_Current = 0;
		OILDCAC_W_Voltage = 0;
		OILDCAC_W_Current = 0;
		break;
	default:
		break;
	}
}
static void can_id_18FF0CF9_analyse(can_msg_t *msg, can_pro_way_e way) {
	switch (way) {
	case CAN_PARSE:
		AIRDCAC_U_Voltage = (U8) msg->data[0]+((U16) msg->data[1] << 8);
		AIRDCAC_U_Current = (U8) msg->data[2]+((U16) msg->data[3] << 8);
		AIRDCAC_Status = msg->data[4]&0x03;
		AIRDCAC_Output_Cut = (msg->data[4] >> 2)&0x01;
		AIRDCAC_intput_Cut = (msg->data[4] >> 3)&0x01;
		AIRDCAC_Fault_Code = msg->data[5];
		AIRDCAC_Level_Fault = msg->data[6]&0x03;
		AIRDCAC_Temp_Warn = (msg->data[6] >> 2)&0x01;
		AIRDCAC_Fault_Warn = (msg->data[6] >> 3)&0x01;
		AIRDCAC_Temp = msg->data[7];
		break;
	case CAN_LOST:
		AIRDCAC_U_Voltage = 0;
		AIRDCAC_U_Current = 0;
		AIRDCAC_Status = 0;
		AIRDCAC_Output_Cut = 0;
		AIRDCAC_intput_Cut = 0;
		AIRDCAC_Fault_Code = 0;
		AIRDCAC_Level_Fault = 0;
		AIRDCAC_Temp_Warn = 0;
		AIRDCAC_Fault_Warn = 0;
		AIRDCAC_Temp = 40;
		break;
	default:
		break;
	}
}
static void can_id_18FF0DF9_analyse(can_msg_t *msg, can_pro_way_e way) {
	switch (way) {
	case CAN_PARSE:
		AIRDCAC_V_Voltage = (U8) msg->data[0]+((U16) msg->data[1] << 8);
		AIRDCAC_V_Current = (U8) msg->data[2]+((U16) msg->data[3] << 8);
		AIRDCAC_W_Voltage = (U8) msg->data[4]+((U16) msg->data[5] << 8);
		AIRDCAC_W_Current = (U8) msg->data[6]+((U16) msg->data[7] << 8);
		break;
	case CAN_LOST:
		AIRDCAC_V_Voltage = 0;
		AIRDCAC_V_Current = 0;
		AIRDCAC_W_Voltage = 0;
		AIRDCAC_W_Current = 0;
		break;
	default:
		break;
	}
}
static void can_id_1801EFA9_analyse(can_msg_t *msg, can_pro_way_e way) {
	switch (way) {
	case CAN_PARSE:
		ICS01_Status1.byte = msg->data[0];
		ICS01_Status2.byte = msg->data[1];
		ICS01_Status3.byte = msg->data[2];
		ICS01_Status4.byte = msg->data[3];
		ICS01_Voltage_Front = (U8) msg->data[4]+((U16) (msg->data[5]&0x0F) << 8);
		ICS01_Voltage_Rear = (U8) (msg->data[5] >> 4)+((U16) msg->data[6] << 4);
		ICS01_LIFE = msg->data[7];
		break;
	case CAN_LOST:
		ICS01_Status1.byte = 0;
		ICS01_Status2.byte = 0;
		ICS01_Status3.byte = 0;
		ICS01_Status4.byte = 0;
		ICS01_Voltage_Front = 0;
		ICS01_Voltage_Rear = 0;
		ICS01_LIFE = 0;
		break;
	default:
		break;
	}
}
static void can_id_104C1000_analyse(can_msg_t *msg, can_pro_way_e way) {
	U8 i  = 0;
	switch (way) {
	case CAN_PARSE:
		for (i = 0; i < 100; i++) {
            if (msg->id == (0x104C19A4 + i)) {
                BAT_Cell_Voltage[i * 4] = (U8) msg->data[0]+((U16) msg->data[1] << 8);
                BAT_Cell_Voltage[(i * 4) + 1] = (U8) msg->data[2]+((U16) msg->data[3] << 8);
                BAT_Cell_Voltage[(i * 4) + 2] = (U8) msg->data[4]+((U16) msg->data[5] << 8);
                BAT_Cell_Voltage[(i * 4) + 3] = (U8) msg->data[6]+((U16) msg->data[7] << 8);
				dbg_printf("id_%d = %d,%d,%d,%d\n",i,BAT_Cell_Voltage[i * 4],BAT_Cell_Voltage[(i * 4) + 1],BAT_Cell_Voltage[(i * 4) + 2],BAT_Cell_Voltage[(i * 4) + 3]);
            }
        }
        for (i = 0; i < 16; i++) {
            if (msg->id == (0x104C1A18 + i)) {
                BAT_Cell_Temperature[i * 8] = (U8) msg->data[0];
                BAT_Cell_Temperature[(i * 8) + 1] = (U8) msg->data[1];
                BAT_Cell_Temperature[(i * 8) + 2] = (U8) msg->data[2];
                BAT_Cell_Temperature[(i * 8) + 3] = (U8) msg->data[3];
                BAT_Cell_Temperature[(i * 8) + 4] = (U8) msg->data[4];
                BAT_Cell_Temperature[(i * 8) + 5] = (U8) msg->data[5];
                BAT_Cell_Temperature[(i * 8) + 6] = (U8) msg->data[6];
                BAT_Cell_Temperature[(i * 8) + 7] = (U8) msg->data[7];
            }
        }
		//data_copy((U8*) &can0.ID_104C1000[0], (U8*) &msg->data[0], msg->dlc);
		break;
	case CAN_LOST:
		memset(BAT_Cell_Voltage,1,(400*sizeof(U16)));
		
		memset(BAT_Cell_Temperature,0,(128*sizeof(U8)));
		break;
	default:
		break;
	}
}


//MCU is big endian
union {
	U32 odo32;
	U8 odo8[4];
} total_odo;

void MCU_TO_PC_send(void) {  //对应报文0x7EF
	U8 msg_box;

	//发送MCU TO PC的报文
	msg_box = ID_RECV_NUM_ALL + 1;
	can_msg[msg_box - 1].buffer_num = msg_box;
	can_msg[msg_box - 1].data[0] = 0;
	can_msg[msg_box - 1].data[1] = 0;
	can_msg[msg_box - 1].data[2] = 0;
	can_msg[msg_box - 1].data[3] = 0;
	can_msg[msg_box - 1].data[4] = 0;
	can_msg[msg_box - 1].data[5] = 0;
	can_msg[msg_box - 1].data[6] = 0;
	can_msg[msg_box - 1].data[7] = 0;

	hal_can_sent(CAN_CHN, &can_msg[msg_box - 1]);
}

void can_id_1801EF17_send(void) { //对应报文0x1801EF17
	U8 msg_box;
	U16 temp;
    DATA_BIT CAN_DATA;

	//发送总里程信息的报文
	msg_box = ID_RECV_NUM_ALL + 2;
	can_msg[msg_box - 1].buffer_num = msg_box;

	if (ADV[3] < 500) 
	{
		temp = 0;
	}
    else if (ADV[3] < 3000) 
	{
		temp = (ADV[3] - 500) * 0.04;
    }
    else
	{	
		temp = 100;
    }
    can_msg[msg_box - 1].data[0] = temp; //前气压

    if (ADV[4] < 500) 
	{
		temp = 0;
    }
    else if (ADV[4] < 3000) 
	{
		temp = (ADV[4] - 500) * 0.04;
    }
    else 
	{
		temp = 100;
    }
    can_msg[msg_box - 1].data[1] = temp; //后气压    

    can_msg[msg_box - 1].data[2] = 0;
    can_msg[msg_box - 1].data[3] = 0;

    CAN_DATA.bits.bit1 = M_ON; //key on
    CAN_DATA.bits.bit2 = rKL8; //手制动
    CAN_DATA.bits.bit3 = (VCU_Gear == 13); //倒挡
    CAN_DATA.bits.bit4 = 0;
    CAN_DATA.bits.bit5 = 0;
    CAN_DATA.bits.bit6 = 0;
    CAN_DATA.bits.bit7 = rKL11; //后舱门
    CAN_DATA.bits.bit8 = 0;
    can_msg[msg_box - 1].data[4] = CAN_DATA.byte;

    CAN_DATA.bits.bit1 = 0;//LED2; //后门
    CAN_DATA.bits.bit2 = LED16; //蓄电池电量低
    CAN_DATA.bits.bit3 = 0;
    CAN_DATA.bits.bit4 = 0;
    CAN_DATA.bits.bit5 = 0;//LED1; //前门
    CAN_DATA.bits.bit6 = 0;
    CAN_DATA.bits.bit7 = 0;
    CAN_DATA.bits.bit8 = 0;
    can_msg[msg_box - 1].data[5] = CAN_DATA.byte;

    can_msg[msg_box - 1].data[6] = 0;
    can_msg[msg_box - 1].data[7] = 0;
	

#if 1  //just for test can send by niujianlong
	hal_can_sent(CAN_CHN, &can_msg[msg_box - 1]);
#endif

	
}


void can_id_1802EF17_send(void) { ////对应报文0x1802EF17
	U8 msg_box;
	U16 temp = 0;

	//发送总里程信息的报文
	msg_box = ID_RECV_NUM_ALL + 3;
	can_msg[msg_box - 1].buffer_num = msg_box;

	can_msg[msg_box - 1].data[0] = (U8) (info.Odo); //低八位
	can_msg[msg_box - 1].data[1] = (U8) ((info.Odo) >> 8); //二级八位
	can_msg[msg_box - 1].data[2] = (U8) ((info.Odo) >> 16); //三级八位
	can_msg[msg_box - 1].data[3] = (U8) ((info.Odo) >> 24); //高八位
	temp = pSpeed * 10;
	can_msg[msg_box - 1].data[4] = (temp);
	can_msg[msg_box - 1].data[5] = (temp >> 8);
	can_msg[msg_box - 1].data[6] = 0;
	can_msg[msg_box - 1].data[7] = 0;

	
	hal_can_sent(CAN_CHN,&can_msg[msg_box-1]);
	
}


void app_can_sent_task(void) {
	//MCU_TO_PC_send();
	if (uart_special_data.specialInfo.LedWarning.third.bit.IPconfig == 2) {
		//can_id_345_send();
		//can_id_7A6_send(&uds_sent, sizeof(uds_sent) / sizeof(U8));
	} else if (uart_special_data.specialInfo.LedWarning.third.bit.IPconfig
			== 1) {
		//can_id_766_send();
	} else {
#if 1  //just for test can send by niujianlong
		can_id_1801EF17_send();
		can_id_1802EF17_send();

#endif

	}
}

void app_can_lost_time_cnt_100ms(void) 
{


}
