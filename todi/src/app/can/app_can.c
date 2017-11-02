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
#include "Fw_version.h"
#include "app_moudle.h"
#include "app_printf.h"
#include "app_main_interface.h"
#include "app_warning.h"
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
static can_msg_t can1_rx_msg[20]={{0}} ;
static can_msg_t can1_tx_msg[] = {
	{0},
	{0},
	{0},
};

static pCanAnalyse can_rx_handle[ID_RECV_NUM_ALL] = { NULL };


U32 can_rx_msg_id_ads[ID_RECV_NUM_ALL] = { 0 };
unsigned int Fcan_count = 0; //前部模 
unsigned int Rcan_count = 0; //顶部模 
unsigned int Mcan_count = 0; //中部模 

/*******************分包处理*****************/
unsigned int moto_voltage; //电机电压 0.1v
unsigned int moto_current=10000; //电机电流 0.1A
unsigned int moto_speed; //电机转速 1r/min
unsigned char Motor_Temperature=40; //电机温度
unsigned char Inverter_t=40; //逆变器温度

unsigned char Accelerator; //油门踏板
unsigned int Engine_Speed; //发动机转速

unsigned int Fule_rate; //油耗
unsigned char water_temp=40; //水温
unsigned char Engine_Fuhe; //发动机负荷
unsigned char Niaosu; //尿素;
unsigned char Accelerator_Shiji; //油门踏板实际
unsigned char Engine_oil_press; //机油压力
unsigned char ambient_air_temperature=40; //周围空气温度

unsigned char Current_Gear; //档位
unsigned char TCU_Position; //离合位置
unsigned char TCU_warn; //变速箱故障
unsigned char STOP; //停车故障
unsigned int Speed_rmp; //输出轴转速
unsigned char Car_mode; //整车模式
unsigned char TCU_code; //变速箱故障代码
unsigned char TCU_level; //变速箱故障等级

DATA_BIT CAN_KEY[4];
unsigned char Brake_percent; //制动踏板开度
unsigned char Speed_percent; //加速踏板开度
unsigned char Digit_input3; //数字量输入3

unsigned long Program; //程序版本
unsigned long tcu_spn; //TCU故障码
unsigned char tcu_fmi;
unsigned long tcu_cm;
unsigned char tcu_oc;
unsigned char Car_LIFE; //整车控制器

unsigned int BMS_V=10000; //电池电压
unsigned char BMS_SOC; //电池SOC
unsigned int BMS_A=10000; //电池电流
unsigned char Status_Flag1;
unsigned char Status_Flag2;
unsigned char Status_Flag3;

unsigned int BMS_A_charge=10000; //最大充电电流
unsigned int BMS_A_discharge=10000; //最大电流
unsigned char Warn_level; //故障等级
unsigned int BMS_V_average=10000; //电池平均电压
unsigned char BMS_T_H; //电池最高温度

unsigned int Oum_zheng; //绝缘正阻值
unsigned int Oum_fu; //绝缘负阻值
unsigned int Battery_single_H=10000; //电池单体高电压
unsigned int Battery_single_L=10000; //电池单体低电压

unsigned char Battery_number[10]; //电池编号
unsigned int Battery_voltage[10]; //电池单体电压

unsigned char Battery_number_t[10]; //电池温度编号
unsigned int Battery_temp[10]; //电池单体温度

unsigned int DCAC_W=10000;
unsigned int DCAC_V=10000;
unsigned int DCAC_U=10000;
unsigned char Sanreqi_t=40; //散热器温度
unsigned char DCAC_Warn_code; //故障代码

unsigned char AC_Warn_code; //AC故障代码
DATA_BIT AC_KEY[2];
unsigned char Indoor_t=30; //室内温度
unsigned char Outdoor_t=30; //室外温度
unsigned char Set_t=30; //设定
unsigned char AC_req; //制冷请求
unsigned char AC_LIFE; //AC生命

/**************************************************/
unsigned char battle_t; //电池均温
unsigned char battle_t_warn; //电池过温报警
unsigned char battle_jueyuan_warn; //绝缘过低报警
unsigned char battle_higtv1_warn; //高压电池1故障
unsigned char battle_higtv2_warn; //高压电池2故障
unsigned char moto_t_warn; //电机过温
unsigned char moto_ready; //ready信号
unsigned char xudc; //蓄电池异常
unsigned char moto_warn; //电机故障

unsigned char BMS_offline_warn; //电池不在线
unsigned char MCU_offline_warn; //电机不在线
unsigned char TCU_offline_warn; //变速箱不在线
unsigned char Engine_offline_warn; //发动机不在线
unsigned char Engine_wait; //发动机怠速
unsigned char EV; //模式
unsigned char Signal_charge; //充电信号

unsigned char mil_lamp_request; //黄色三级报警
unsigned char fail_mode; //文字报警
unsigned char mil_buzz_request; //蜂鸣器响
unsigned long hcu_spn; //HCU故障码
unsigned char hcu_fmi;

unsigned char Eng_static_test; //发动机稳态转速测试
unsigned char tcu_start_allow; //发动机运行启动

unsigned char Lamp_status_YELLOW; //变速箱黄灯状态
unsigned char Lamp_status_RED; //变速箱红灯状态

unsigned long ccu_spn; //CCU故障码
unsigned char ccu_fmi;

unsigned long ecu_spn; //ECU故障码
unsigned char ecu_fmi;

unsigned int ENG_start_req; //仪表控制启动
unsigned long mot_work_time; //电机工作累积时间
unsigned char mot_work_percent; //电机工作时间比
unsigned char DCDC_ctrl; //DCDC状态
unsigned char hcu_starter_forbid; //起动机工作
unsigned char aircompress_enable; //压缩机
unsigned char Speed_lim_mode; //限速模式

unsigned char water_level; //水位

unsigned char Wait_start; //等待启动
unsigned char Maintain_warn; //维护报警
unsigned char Stop_warn; //发动机停机报警
unsigned char Engine_warn; //发动机故障

U32 Vcan_timeout_cfg;
U32 Bcan_timeout_cfg;
U32 Dcan_timeout_cfg;
U32 Acan_timeout_cfg;

/******************************************/
unsigned int PRESS[2]; //气压值 KPA/BIT
unsigned char ccvs_eng_req; //启动请求

BatteryManagement bms_msg;
BatteryVoltage bat_vol_msg;
BatteryTemperater bat_temp_msg;
MenuVcuControl vcu_msg;
AirPump air_pump_msg;
OilPump oil_pump_msg;
DcdcControl dcdc_msg;
U16 vol[112*4];			//高 4 位为电池箱号（0-15），低 12 位为电压： 0.01V/ bit	448个
U8 temperater[26*8];			//温度点1-96		1℃/bit， -40    96个
Voltage vol_temp_msg[4];

//UDS_id_766  uds766;

extern led_check_t led_check;
/* function declare */
static void can_id_7E7_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_180689F4_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_180789F4_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_181B80F4_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_0C008980_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_0C018980_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_0C028980_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_0C068980_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_0C058980_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_0C048980_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_18XX89F4_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_610_analyse(can_msg_t *msg, can_pro_way_e way);

static  void can_updata_analyse(can_msg_t *msg, can_pro_way_e way);

static void app_can_process(can_msg_t *msg, can_pro_way_e way);
static void app_can1_process(can_msg_t *msg, can_pro_way_e way);


static void can_id_68X_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_67X_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_62X_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_63X_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_64X_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_65X_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_56X_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_45X_analyse(can_msg_t *msg, can_pro_way_e way);

static pCanAnalyse can1_rx_handle[]=
{
	can_id_68X_analyse,
	can_id_67X_analyse,
	can_id_62X_analyse,
	can_id_63X_analyse,
	can_id_64X_analyse,
	can_id_65X_analyse,
	can_id_56X_analyse,
	can_id_45X_analyse,
	NULL,
};

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
static void app_configuration_can(can_msg_t *msg,U32 id,U32 id_mask, U8 dlc, CAN_ID_FORMAT_e format, U8 msg_buf,
		U16 period)	
{
	msg[0].id = id;
	msg[0].id_mask= id_mask;
	msg[0].dlc = dlc;
	msg[0].format = format;
	msg[0].buffer_num = msg_buf;
	if (period >= PERIOD_MS(1000)) {
		msg[0].period_ms = period >> 1;
	} else {
		msg[0].period_ms = period;
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
	app_bound_id(ID_REC_01_180689F4, 0x1FFFFFFF, 8, EXTERN_ID, 2, (ID_180689F4_period*50));
	app_bound_id(ID_REC_02_180789F4, 0x1FFFFFFF, 8, EXTERN_ID, 3, (ID_180789F4_period*50));
	app_bound_id(ID_REC_03_181B80f4, 0x1FFFFFFF, 8, EXTERN_ID, 4, (ID_181B80f4_period*50));
	app_bound_id(ID_REC_04_0C008980, 0x1FFFFFFF, 8, EXTERN_ID, 5, (ID_0C008980_period*50));
	app_bound_id(ID_REC_05_0C018980, 0x1FFFFFFF, 8, EXTERN_ID, 6, (ID_0C018980_Period*50));
	app_bound_id(ID_REC_06_0C028980, 0x1FFFFFFF, 8, EXTERN_ID, 7, (ID_0C028980_Period*50));
	app_bound_id(ID_REC_07_0C068980, 0x1FFFFFFF, 8, EXTERN_ID, 8, (ID_0C068980_Period*50));
	app_bound_id(ID_REC_08_0C058980, 0x1FFFFFFF, 8, EXTERN_ID, 9, (ID_0C058980_Period*50));
	app_bound_id(ID_REC_09_0C048980, 0x1FFFFFFF, 8, EXTERN_ID, 10, (ID_0C048980_Period*50));
	app_bound_id(ID_REC_10_18XX89F4, 0x1F00FFFF, 8, EXTERN_ID, 11, (ID_18xx89F4_Period*50));
	app_bound_id(ID_REC_11_610, 0XFFFFFFFF, 8, EXTERN_ID, 12, (ID_610_Period*50));
	app_bound_id(UPDATA_ID         , 0x1FFFFFF0, 8, EXTERN_ID, 13, (PERIOD_MS(1000)));

	/* sent id bound,add your code here */
	app_bound_id(MCU_TO_PC_ID, 0xfff, 8, STAND_ID, (ID_RECV_NUM_ALL + 1),PERIOD_MS(100));
	app_bound_id(ID_SEND_1_0C19A7A1, 0x1FFFFFFF, 8, EXTERN_ID, (ID_RECV_NUM_ALL + 2),PERIOD_MS(100));
	app_bound_id(ID_SEND_2_0C1AA7A1, 0x1FFFFFFF, 8, EXTERN_ID, (ID_RECV_NUM_ALL + 3),PERIOD_MS(100));
	app_bound_id(ID_SEND_3_18FED917, 0x1FFFFFFF, 8, EXTERN_ID, (ID_RECV_NUM_ALL + 4),PERIOD_MS(100));
	app_bound_id(ACK_UPDATA_ID, 	 0x1FFFFFFF, 8, EXTERN_ID, (ID_RECV_NUM_ALL + 5),PERIOD_MS(100));
	
	
	
	/* register the can id process function */
	can_rx_pro = app_can_process;
	/*register the can id process */
	
	
	can_rx_handle[0] = can_id_7E7_analyse;
	can_rx_handle[1] = can_id_180689F4_analyse;
	can_rx_handle[2] = can_id_180789F4_analyse;
	can_rx_handle[3] = can_id_181B80F4_analyse;
	can_rx_handle[4] = can_id_0C008980_analyse;
	can_rx_handle[5] = can_id_0C018980_analyse;
	can_rx_handle[6] = can_id_0C028980_analyse;
	can_rx_handle[7] = can_id_0C068980_analyse;
	can_rx_handle[8] = can_id_0C058980_analyse;
	can_rx_handle[9] = can_id_0C048980_analyse;
	can_rx_handle[10] = can_id_18XX89F4_analyse;
	can_rx_handle[11] = can_id_610_analyse;
	can_rx_handle[12] = can_updata_analyse;


	mid_can_init(CAN_CHN, CAN_CHIP);

	mid_can1_prepare(can1_tx_msg,can1_rx_msg);
	memset(can1_rx_msg,0,sizeof(can1_rx_msg));
	memset(can1_tx_msg,0,sizeof(can1_tx_msg));
	app_configuration_can(&can1_rx_msg[0],ID_REC_01_68X, 0x7F0, 8, STAND_ID, 3, (ID_180689F4_period*50));
	app_configuration_can(&can1_rx_msg[1],ID_REC_02_67X, 0x7F0, 8, STAND_ID, 4, (ID_180689F4_period*50));
	app_configuration_can(&can1_rx_msg[2],ID_REC_03_62X, 0x7F0, 8, STAND_ID, 5, (ID_180689F4_period*50));
	app_configuration_can(&can1_rx_msg[3],ID_REC_04_63X, 0x7F0, 8, STAND_ID, 6, (ID_180689F4_period*50));
	app_configuration_can(&can1_rx_msg[4],ID_REC_05_64X, 0x7F0, 8, STAND_ID, 7, (ID_180689F4_period*50));
	app_configuration_can(&can1_rx_msg[5],ID_REC_06_65X, 0x7F0, 8, STAND_ID, 8, (ID_180689F4_period*50));
	app_configuration_can(&can1_rx_msg[6],ID_REC_07_56X, 0x7F0, 8, STAND_ID, 9, (ID_180689F4_period*50));
	app_configuration_can(&can1_rx_msg[7],ID_REC_08_45X, 0x7F0, 8, STAND_ID, 10, (ID_180689F4_period*50));
	

	app_configuration_can(&can1_tx_msg[0],BCAN_ID_SEND_6A4, 0x7FF, 8, STAND_ID, 1 , (ID_180689F4_period*50));
	app_configuration_can(&can1_tx_msg[1],BCAN_ID_SEND_454, 0x7FF, 8, STAND_ID, 2 , (ID_180689F4_period*50));
	can1_rx_pro = app_can1_process;

	hal_can_init(1);
	wdg_feed();


}

static void app_can_process(can_msg_t *msg, can_pro_way_e way) {
	{
		if (can_rx_handle[msg->buffer_num - 1] != NULL) {
			can_rx_handle[msg->buffer_num - 1](msg, way);
		}
	}
}
static void app_can1_process(can_msg_t *msg, can_pro_way_e way) {
	if (msg->buffer_num <= ID1_PROCESS_ALL) {
		if (can1_rx_handle[msg->buffer_num -ID1_SENT_NUM_ALL -1] != NULL) {
			can1_rx_handle[msg->buffer_num - ID1_SENT_NUM_ALL -1](msg, way);
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

static void can_id_180689F4_analyse(can_msg_t *msg, can_pro_way_e way){
	U8 i  = 0;
	switch (way) {
	case CAN_PARSE:
		Vcan_timeout_cfg = 0;
		memcpy(&bms_msg.ID_180689F4_byte12,msg->data,8);
		memcpy(&warning_data.ID_180689f4_byte7-1,&bms_msg.ID_180689F4_byte7,1);
		memcpy(&warning_data.ID_180689f4_byte7,&bms_msg.ID_180689F4_byte7,1);
		break;
	case CAN_LOST:
		Vcan_timeout_cfg = 1;
		memset(&bms_msg.ID_180689F4_byte12,0,8);
		memset(&warning_data.ID_180689f4_byte7,0,1);
		memset(&warning_data.ID_180689f4_byte7-1,0,1);			
		WORD_WRITE(bms_msg.ID_180689F4_byte34,32000);//32000偏移
		bms_msg.ID_180689F4_byte6 = 40;
		break;
	default:
		break;
	}
}
static void can_id_180789F4_analyse(can_msg_t *msg, can_pro_way_e way){
	U8 i  = 0;
	switch (way) {
	case CAN_PARSE:
		Vcan_timeout_cfg = 0;
		memcpy(&bms_msg.ID_180789F4_byte12,msg->data,8);
		break;
	case CAN_LOST:
		Vcan_timeout_cfg = 1;
		memset(bms_msg.ID_180789F4_byte12,0,8);
		bms_msg.ID_180789F4_byte5 = 40;
		bms_msg.ID_180789F4_byte6 = 40;
		break;
	default:
		break;
	}
}
static void can_id_181B80F4_analyse(can_msg_t *msg, can_pro_way_e way){
	U8 i  = 0;
	switch (way) {
	case CAN_PARSE:
		Vcan_timeout_cfg = 0;
		memcpy(&bms_msg.ID_181B80F4_byte12,msg->data,8);
		memcpy(&warning_data.ID_181B80f4_byte5-1,&bms_msg.ID_181B80F4_byte5,1);
		memcpy(&warning_data.ID_181B80f4_byte5,&bms_msg.ID_181B80F4_byte5,1);
		memcpy(&warning_data.ID_181B80f4_byte6-1,&bms_msg.ID_181B80F4_byte6,1);
		memcpy(&warning_data.ID_181B80f4_byte6,&bms_msg.ID_181B80F4_byte6,1);
		memcpy(&warning_data.ID_181B80f4_byte7-1,&bms_msg.ID_181B80F4_byte7,1);
		memcpy(&warning_data.ID_181B80f4_byte7,&bms_msg.ID_181B80F4_byte7,1);
		memcpy(&warning_data.ID_181B80f4_byte8-1,&bms_msg.ID_181B80F4_byte8,1);
		memcpy(&warning_data.ID_181B80f4_byte8,&bms_msg.ID_181B80F4_byte8,1);
		break;
	case CAN_LOST:
		Vcan_timeout_cfg = 1;
		memset(&bms_msg.ID_181B80F4_byte12,0,8);
		memset(&warning_data.ID_181B80f4_byte5-1,0,1);
		memset(&warning_data.ID_181B80f4_byte5,0,1);
		memset(&warning_data.ID_181B80f4_byte6-1,0,1);
		memset(&warning_data.ID_181B80f4_byte6,0,1);
		memset(&warning_data.ID_181B80f4_byte7-1,0,1);
		memset(&warning_data.ID_181B80f4_byte7,0,1);
		memset(&warning_data.ID_181B80f4_byte8-1,0,1);
		memset(&warning_data.ID_181B80f4_byte8,0,1);
		WORD_WRITE(bms_msg.ID_181B80F4_byte12,32000);//32000偏移
		WORD_WRITE(bms_msg.ID_181B80F4_byte34,32000);//32000偏移
		break;
	default:
		break;
	}
}
static void can_id_0C008980_analyse(can_msg_t *msg, can_pro_way_e way){
	U8 i  = 0;
	switch (way) {
	case CAN_PARSE:
		Vcan_timeout_cfg = 0;
		memcpy(&vcu_msg.ID_0C008980_byte12,msg->data,8);
		break;
	case CAN_LOST:
		Vcan_timeout_cfg = 1;
		WORD_WRITE(vcu_msg.ID_0C008980_byte12,10000);//10000偏移
		vcu_msg.ID_0C008980_byte3  = 40;
		vcu_msg.ID_0C008980_byte4  = 40;
		WORD_WRITE(vcu_msg.ID_0C008980_byte56,10000);//10000偏移
		memset(&vcu_msg.ID_0C008980_byte78,0,2);
		break;
	default:
		break;
	}
}
static void can_id_0C018980_analyse(can_msg_t *msg, can_pro_way_e way){
	U8 i  = 0;
	switch (way) {
	case CAN_PARSE:
		Vcan_timeout_cfg =1;
		memcpy(&vcu_msg.ID_0C018980_byte1,msg->data,8);
		break;
	case CAN_LOST:
		Vcan_timeout_cfg =1;
		memset(&vcu_msg.ID_0C018980_byte1,0,8);
		break;
	default:
		break;
	}
}
static void can_id_0C028980_analyse(can_msg_t *msg, can_pro_way_e way){
	U8 i  = 0;
	switch (way) {
	case CAN_PARSE:
		Vcan_timeout_cfg = 0;
		memcpy(&vcu_msg.ID_0C028980_byte12,msg->data,8);
		break;
	case CAN_LOST:
		Vcan_timeout_cfg = 1;
		memset(vcu_msg.ID_0C028980_byte12,0,8);
		WORD_WRITE(vcu_msg.ID_0C028980_byte12,32000);//32000偏移
		break;
	default:
		break;
	}
}
static void can_id_0C068980_analyse(can_msg_t *msg, can_pro_way_e way){
	U8 i  = 0;
	switch (way) {
	case CAN_PARSE:
		Bcan_timeout_cfg = 0;
		memcpy(&air_pump_msg.ID_0C068980_byte12,msg->data,8);
		memcpy(&warning_data.ID_0C068980_byte8-1,&air_pump_msg.ID_0C068980_byte8,1);
		memcpy(&warning_data.ID_0C068980_byte8,&air_pump_msg.ID_0C068980_byte8,1);
		break;
	case CAN_LOST:
		Bcan_timeout_cfg = 1;
		memset(air_pump_msg.ID_0C068980_byte12,0,8);
		memset(&warning_data.ID_0C068980_byte8-1,0,1);
		memset(&warning_data.ID_0C068980_byte8,0,1);
		WORD_WRITE(air_pump_msg.ID_0C068980_byte12,10000);//10000偏移
		WORD_WRITE(air_pump_msg.ID_0C068980_byte34,10000);//10000偏移
		WORD_WRITE(air_pump_msg.ID_0C068980_byte56,10000);//10000偏移
		air_pump_msg.ID_0C068980_byte7  = 40;
		break;
	default:
		break;
	}
}
static void can_id_0C058980_analyse(can_msg_t *msg, can_pro_way_e way){
	U8 i  = 0;
	switch (way) {
	case CAN_PARSE:
		Bcan_timeout_cfg = 0;
		memcpy(&oil_pump_msg.ID_0C058980_byte12,msg->data,8);
		memcpy(&warning_data.ID_0C058980_byte8-1,&oil_pump_msg.ID_0C058980_byte8,1);
		memcpy(&warning_data.ID_0C058980_byte8,&oil_pump_msg.ID_0C058980_byte8,1);	
		break;
	case CAN_LOST:
		Bcan_timeout_cfg = 1;
		memset(&oil_pump_msg.ID_0C058980_byte12,0,8);
		memset(&warning_data.ID_0C058980_byte8-1,0,1);
		memset(&warning_data.ID_0C058980_byte8,0,1);	
		WORD_WRITE(oil_pump_msg.ID_0C058980_byte12,10000);//10000偏移
		WORD_WRITE(oil_pump_msg.ID_0C058980_byte34,10000);//10000偏移
		WORD_WRITE(oil_pump_msg.ID_0C058980_byte56,10000);//10000偏移
		oil_pump_msg.ID_0C058980_byte7  = 40;
		break;
	default:
		break;
	}
}
static void can_id_0C048980_analyse(can_msg_t *msg, can_pro_way_e way){
	U8 i  = 0;
	switch (way) {
	case CAN_PARSE:
		Bcan_timeout_cfg = 0;
		memcpy(&dcdc_msg.ID_0C048980_byte12,msg->data,8);
		memcpy(&warning_data.ID_0C048980_byte8-1,&dcdc_msg.ID_0C048980_byte8,1);
		memcpy(&warning_data.ID_0C048980_byte8,&dcdc_msg.ID_0C048980_byte8,1);	
		break;
	case CAN_LOST:
		memset(&dcdc_msg.ID_0C048980_byte12,0,8);
		memset(&warning_data.ID_0C048980_byte8-1,0,1);
		memset(&warning_data.ID_0C048980_byte8,0,1);		
		WORD_WRITE(dcdc_msg.ID_0C048980_byte12,10000);//10000偏移
		WORD_WRITE(dcdc_msg.ID_0C048980_byte34,10000);//10000偏移
		WORD_WRITE(dcdc_msg.ID_0C048980_byte56,10000);//10000偏移
		dcdc_msg.ID_0C048980_byte7  = 40;
		Bcan_timeout_cfg = 1;
		break;
	default:
		break;
	}
}
static void single_battery_init(void)
{
	memset(vol,0xFF,sizeof(vol));
	wdg_feed();
	memset(temperater,0XFF,sizeof(temperater));
	wdg_feed();
}
static void can_id_18XX89F4_analyse(can_msg_t *msg, can_pro_way_e way){
	U8 pf;
	U8 idx;
	int i;
	switch (way) {
		case CAN_PARSE:
			pf = (msg->id>>16)&0xFF;
			if (pf <0x10)
			{
				break;
			}
			if (pf<=0x7F)
			{
				idx = pf - 0x10;
				memcpy(&vol_temp_msg[0],msg->data,8);
				for (i = 0; i < 4; ++i)vol[4*idx+i]=vol_temp_msg[i].bit4;
			}
			else if (pf<=0x99)
			{
				idx = pf - 0x80;
				memcpy(&temperater[idx*8],msg->data,8);
			}
			break;
		case CAN_LOST:
			single_battery_init();
			break;
		default:
			break;
	}

}

static void can_id_610_analyse(can_msg_t *msg, can_pro_way_e way){
	switch (way) {
		case CAN_PARSE:
		dbg_printf("%d %d %d %d\n",msg->data[0],msg->data[1],msg->data[2],msg->data[3]);
		info.Odo = (U32)msg->data[1]+(U32)msg->data[2]*256+(U32)msg->data[3]*256*256+(U32)msg->data[4]*256*256*256;
		write_total_trip(info.Odo);
	}
}

#define OnlineCheckID	(UPDATA_ID | 1)
#define ExcuteAppID     (UPDATA_ID | 9)
static void can_updata_analyse(can_msg_t *msg, can_pro_way_e way)
{
	U8 i  = 0;
	switch (way) {
	case CAN_PARSE:
		if (msg->id == OnlineCheckID)
		{
			U8 msg_box;
			//发送总里程信息的报文
			msg_box = ID_RECV_NUM_ALL + 5;
			can_msg[msg_box - 1].buffer_num = msg_box;
			can_msg[msg_box - 1].data[0] = (MCU_VERSION_H>>8)&0xFF;
			can_msg[msg_box - 1].data[1] = MCU_VERSION_H&0xFF;
			can_msg[msg_box - 1].data[2] = (MCU_VERSION_M>>8)&0xFF;
			can_msg[msg_box - 1].data[3] = MCU_VERSION_M&0xFF;
			can_msg[msg_box - 1].data[4] = 0;
			can_msg[msg_box - 1].data[5] = 0;
			can_msg[msg_box - 1].data[6] = 0;
			can_msg[msg_box - 1].data[7] = 1;
			hal_can_sent(CAN_CHN, &can_msg[msg_box - 1]);
		}
		else if (msg->id == ExcuteAppID)
		{
			__DI();
			__wait_nop();
			//hal_soft_reset();
			#pragma asm
			LDI:32 #070000H, R0;  //jmp to address 70000H to run
			JMP @R0;
			#pragma endasm
		}
		//data_copy((U8*) &can0.ID_104C1000[0], (U8*) &msg->data[0], msg->dlc);
		break;
	case CAN_LOST:
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

/*********************************************************************/


static void can_id_68X_analyse(can_msg_t *msg, can_pro_way_e way){
	U8 i  = 0;
//	dbg_printf("68XID = %x\n",msg->id);
	switch (way) {
	case CAN_PARSE:
			switch (msg->id) {
	            case 0x681:
					//dbg_printf("data =%2x,%2x,%2x,%2x,%2x,%2x,%2x,%2x",msg->data[0],msg->data[1],msg->data[2],msg->data[3],msg->data[4],msg->data[5],msg->data[6],msg->data[7]);
	                Fcan_count = 0;
	                fKEY.BYTES[0] = msg->data[0];
	                fKEY.BYTES[1] = msg->data[1];
	                fKEY.BYTES[2] = msg->data[2];
	                fFreq = msg->data[3]+((unsigned int)msg->data[4] << 8);
	                fSpeed = msg->data[5]+((unsigned int) msg->data[6] << 8);
	                break;
	            case 0x682:
	                Mcan_count = 0;
	                mKEY.BYTES[0] = msg->data[0];
	                mKEY.BYTES[1] = msg->data[1];
	                mKEY.BYTES[2] = msg->data[2];
	                mFreq = msg->data[3]+((unsigned int) msg->data[4] << 8);
	                mSpeed = msg->data[5]+((unsigned int) msg->data[6] << 8);
	                break;
	            case 0x683:
	                Rcan_count = 0;
	                rKEY.BYTES[0] = msg->data[0];
	                rKEY.BYTES[1] = msg->data[1];
	                rKEY.BYTES[2] = msg->data[2];
	                rFreq = msg->data[3]+((unsigned int) msg->data[4] << 8);
	                rSpeed = msg->data[5]+((unsigned int) msg->data[6] << 8);
	                break;
				default:
				     break;
		     }
		break;
	case CAN_LOST: 
		break;
	default:
		break;
	}
}
	
static void can_id_67X_analyse(can_msg_t *msg, can_pro_way_e way){
	U8 i  = 0;
//	dbg_printf("67XID = %x\n",msg->id);
	switch (way) {
	case CAN_PARSE:
		switch (msg->id) {
			case 0x671:
				Fcan_count = 0;
				fADR[0] =msg->data[0]+((unsigned int) msg->data[1] << 8);
				fADR[1] = msg->data[2]+((unsigned int) msg->data[3] << 8);
				break;
			case 0x672:
				Mcan_count = 0;
				mADR[0] = msg->data[0]+((unsigned int) msg->data[1] << 8);
				mADR[1] = msg->data[2]+((unsigned int) msg->data[3] << 8);
				break;
			case 0x673:
				Rcan_count = 0;
				rADR[0] = msg->data[0]+((unsigned int) msg->data[1] << 8);
				rADR[1] = msg->data[2]+((unsigned int) msg->data[3] << 8);
				break;
			 default:
				break;
		}
		break;
	case CAN_LOST:
		break;
	default:
		break;
	}
}
static void can_id_62X_analyse(can_msg_t *msg, can_pro_way_e way){
	U8 i  = 0;
//	dbg_printf("62XID = %x\n",msg->id);
	switch (way) {
	case CAN_PARSE:
		switch (msg->id) {
			case 0x621:
                Fcan_count = 0;
                fpcur[0] = msg->data[0]+((unsigned int) msg->data[1] << 8);
                fpcur[1] = msg->data[2]+((unsigned int) msg->data[3] << 8);
                fpcur[2] = msg->data[4]+((unsigned int) msg->data[5] << 8);
                fpcur[3] = msg->data[6]+((unsigned int) msg->data[7] << 8);
                break;
			case 0x622:
                Mcan_count = 0;
                mpcur[0] = msg->data[0]+((unsigned int) msg->data[1] << 8);
                mpcur[1] = msg->data[2]+((unsigned int) msg->data[3] << 8);
                mpcur[2] = msg->data[4]+((unsigned int) msg->data[5] << 8);
                mpcur[3] = msg->data[6]+((unsigned int) msg->data[7] << 8);
                break;
			case 0x623:
                Rcan_count = 0;
                rpcur[0] = msg->data[0]+((unsigned int) msg->data[1] << 8);
                rpcur[1] = msg->data[2]+((unsigned int) msg->data[3] << 8);
                rpcur[2] = msg->data[4]+((unsigned int) msg->data[5] << 8);
                rpcur[3] = msg->data[6]+((unsigned int) msg->data[7] << 8);
                break;
			default:
				break;
		}
		break;
	case CAN_LOST:
		break;
	default:
		break;
	}
}
static void can_id_63X_analyse(can_msg_t *msg, can_pro_way_e way){
	U8 i  = 0;
//	dbg_printf("63XID = %x\n",msg->id);
	switch (way) {
	case CAN_PARSE:
		switch (msg->id) {
			case 0x631:
                Fcan_count = 0;
                fpcur[4] = msg->data[0]+((unsigned int) msg->data[1] << 8);
                fpcur[5] = msg->data[2]+((unsigned int) msg->data[3] << 8);
                fpcur[6] = msg->data[4]+((unsigned int) msg->data[5] << 8);
                fpcur[7] = msg->data[6]+((unsigned int) msg->data[7] << 8);
                break;
			case 0x632:
                Mcan_count = 0;
                mpcur[4] = msg->data[0]+((unsigned int) msg->data[1] << 8);
                mpcur[5] = msg->data[2]+((unsigned int) msg->data[3] << 8);
                mpcur[6] = msg->data[4]+((unsigned int) msg->data[5] << 8);
                mpcur[7] = msg->data[6]+((unsigned int) msg->data[7] << 8);
                break;
			case 0x633:
                Rcan_count = 0;
                rpcur[4] = msg->data[0]+((unsigned int) msg->data[1] << 8);
                rpcur[5] = msg->data[2]+((unsigned int) msg->data[3] << 8);
                rpcur[6] = msg->data[4]+((unsigned int) msg->data[5] << 8);
                rpcur[7] = msg->data[6]+((unsigned int) msg->data[7] << 8);
                break;
			default:
				break;
		}
		break;
	case CAN_LOST:
		break;
	default:
		break;
	}
}
static void can_id_64X_analyse(can_msg_t *msg, can_pro_way_e way){
	U8 i  = 0;
//	dbg_printf("64XID = %x\n",msg->id);
	switch (way) {
	case CAN_PARSE:
		switch (msg->id) {
			case 0x641:
                Fcan_count = 0;
                fpcur[8] = msg->data[0]+((unsigned int) msg->data[1] << 8);
                fpcur[9] = msg->data[2]+((unsigned int) msg->data[3] << 8);
                fpcur[10] = msg->data[4]+((unsigned int) msg->data[5] << 8);
                fpcur[11] = msg->data[6]+((unsigned int) msg->data[7] << 8);
                break;
			case 0x642:
                Mcan_count = 0;
                mpcur[8] = msg->data[0]+((unsigned int) msg->data[1] << 8);
                mpcur[9] = msg->data[2]+((unsigned int) msg->data[3] << 8);
                mpcur[10] = msg->data[4]+((unsigned int) msg->data[5] << 8);
                mpcur[11] = msg->data[6]+((unsigned int) msg->data[7] << 8);
                break;
			case 0x643:
                Rcan_count = 0;
                rpcur[8] = msg->data[0]+((unsigned int) msg->data[1] << 8);
                rpcur[9] = msg->data[2]+((unsigned int) msg->data[3] << 8);
                rpcur[10] = msg->data[4]+((unsigned int) msg->data[5] << 8);
                rpcur[11] = msg->data[6]+((unsigned int) msg->data[7] << 8);
                break;
			default:
				break;
		}
		break;
	case CAN_LOST:
		break;
	default:
		break;
	}
}
static void can_id_65X_analyse(can_msg_t *msg, can_pro_way_e way){
	U8 i  = 0;
//	dbg_printf("65XID = %x\n",msg->id);
	switch (way) {
	case CAN_PARSE:
		switch (msg->id) {
			case 0x651:
                Fcan_count = 0;
                fpcur[12] = msg->data[0]+((unsigned int) msg->data[1] << 8);
                fpcur[13] = msg->data[2]+((unsigned int) msg->data[3] << 8);
                fpcur[14] = msg->data[4]+((unsigned int) msg->data[5] << 8);
                break;
			case 0x652:
                Mcan_count = 0;
                mpcur[12] = msg->data[0]+((unsigned int) msg->data[1] << 8);
                mpcur[13] = msg->data[2]+((unsigned int) msg->data[3] << 8);
                mpcur[14] = msg->data[4]+((unsigned int) msg->data[5] << 8);
                break;
			case 0x653:
                Rcan_count = 0;
                rpcur[12] = msg->data[0]+((unsigned int) msg->data[1] << 8);
                rpcur[13] = msg->data[2]+((unsigned int) msg->data[3] << 8);
                rpcur[14] = msg->data[4]+((unsigned int) msg->data[5] << 8);
                break;
			default:
				break;
		}
		break;
	case CAN_LOST:
		break;
	default:
		break;
	}
}
static void can_id_56X_analyse(can_msg_t *msg, can_pro_way_e way){
	U8 i  = 0,j=0;
//	dbg_printf("56XID = %x\n",msg->id);
	switch (way) {
	case CAN_PARSE:
		switch (msg->id) {
			case 0x561:
				Fcan_count = 0;
                fPOUT.BYTES[0] = msg->data[0];
                fPOUT.BYTES[1] = msg->data[1];
                for (j = 0; j < 4; j++) {//输出状态采集
                    for (i = 0; i < 4; i++) {
                        fPF[i + 4 * j] = (msg->data[j + 2] >> (i * 2))&0x03;
                    }
                }
				Fversion = msg->data[6];
                break;
            case 0x562:
                Mcan_count = 0;
                mPOUT.BYTES[0] = msg->data[0];
                mPOUT.BYTES[1] = msg->data[1];
                for (j = 0; j < 4; j++) {
                    for (i = 0; i < 4; i++) {
                        mPF[i + 4 * j] = (msg->data[j + 2] >> (i * 2))&0x03;
                    }
                }
				Mversion = msg->data[6];
                break;
            case 0x563:
                Rcan_count = 0;
                rPOUT.BYTES[0] = msg->data[0];
                rPOUT.BYTES[1] = msg->data[1];
                for (j = 0; j < 4; j++) {
                    for (i = 0; i < 4; i++) {
                        rPF[i + 4 * j] = (msg->data[j + 2] >> (i * 2))&0x03;
                    }
                }
				Rversion = msg->data[6];
                break;
			default:
				break;
		}
		break;
	case CAN_LOST:
		break;
	default:
		break;
	}
}
static void can_id_45X_analyse(can_msg_t *msg, can_pro_way_e way){
	U8 i  = 0;
//	dbg_printf("45XID = %x\n",msg->id);
	switch (way) {
	case CAN_PARSE:
		switch (msg->id) {
			 case 0x451:
                Fcan_count = 0;
                fSingle_miles = msg->data[4]+((unsigned int) msg->data[5] << 8);
                break;
            case 0x452:
                Mcan_count = 0;
                mSingle_miles = msg->data[4]+((unsigned int) msg->data[5] << 8);
                break;
            case 0x453:
                Rcan_count = 0;
                rSingle_miles = msg->data[4]+((unsigned int) msg->data[5] << 8);
                break;
			default:
				break;
		}
		break;
	case CAN_LOST:
		break;
	default:
		break;
	}
}

void BCAN_SendCtl(void) {
    can1_tx_msg[0].data[0] = gCTL[0].byte;
    can1_tx_msg[0].data[1] = gCTL[1].byte;
    can1_tx_msg[0].data[2] = gCTL[2].byte;
    can1_tx_msg[0].data[3] = gCTL[3].byte;
    can1_tx_msg[0].data[4] = gCTL[4].byte;
    can1_tx_msg[0].data[5] = gCTL[5].byte;
    can1_tx_msg[0].data[6] = ((M_ON && wake_up3) || wake_up2) + (IN14 << 1)+(IN15 << 2)+(IN16 << 3)+(IN21 << 4);//模块雨刮控制
    can1_tx_msg[0].data[7] = 0;
	hal_can_sent(1, &can1_tx_msg[0]);
}

void BCAN_send_mile(void) {
	 can1_tx_msg[1].data[0] = (unsigned char) (e_total_miles); //低八位
	 can1_tx_msg[1].data[1] = (unsigned char) (e_total_miles >> 8); //二级八位
	 can1_tx_msg[1].data[2] = (unsigned char) (e_total_miles >> 16); //三级八位
	 can1_tx_msg[1].data[3] = (unsigned char) (e_total_miles >> 24); //高八位
	 can1_tx_msg[1].data[4] = 0; 
	 can1_tx_msg[1].data[5] = 0;
	 can1_tx_msg[1].data[6] = 0;
	 can1_tx_msg[1].data[7] = 0;
	 hal_can_sent(1, &can1_tx_msg[1]);
}

void PCAN_CCVS(void) {
	U8 msg_box;

	msg_box = ID_RECV_NUM_ALL + 2;
	can_msg[msg_box - 1].buffer_num = msg_box;
    can_msg[msg_box - 1].data[0] = (unsigned char) (wake_up3 << 6)//ON档
            + (unsigned char) (IN4 << 5)//ST档
            + (unsigned char) (IN27 << 4)//后门开信号
            + (unsigned char) (IN20 << 3)//前面开信号
            + (unsigned char) (rKL15 << 2)//后舱门开信号
            + (unsigned char) (rKL8 << 1)//驻车信号
            + (unsigned char) (rKL10 || rKL12) ;//气压低报警信号
    can_msg[msg_box - 1].data[1] = (unsigned char) (IN1 << 7)//左转向
            + (unsigned char) (IN7 << 6)//远光灯
            + (unsigned char) (IN9 << 5)//前雾灯
            + (unsigned char) (IN5 << 4)//右转向
            + (unsigned char) (IN3 << 3)//油量报警
            + (unsigned char) (0 << 2)//发动机预热
            + (unsigned char) (IN8 << 1)//近光灯
            + (unsigned char) (IN10) ;//后雾灯
    can_msg[msg_box - 1].data[2] = 0xff;
    can_msg[msg_box - 1].data[3] = (unsigned char) (pSpeed * 2);
    can_msg[msg_box - 1].data[4] = PRESS[0]*10;
    can_msg[msg_box - 1].data[5] = (unsigned char) (PRESS[0]*100 >> 8);
    can_msg[msg_box - 1].data[6] = PRESS[1]*10;
    can_msg[msg_box - 1].data[7] = (unsigned char) (PRESS[1]*100 >> 8);
    hal_can_sent(CAN_CHN, &can_msg[msg_box - 1]);
}
void PCAN_send_mile(void) {
	U8 msg_box;
    msg_box = ID_RECV_NUM_ALL + 3;
	can_msg[msg_box - 1].buffer_num = msg_box;
    can_msg[msg_box - 1].data[0] = (unsigned char) (e_total_miles); //低八位
    can_msg[msg_box - 1].data[1] = (unsigned char) (e_total_miles >> 8); //二级八位
    can_msg[msg_box - 1].data[2] = (unsigned char) (e_total_miles >> 16); //三级八位
    can_msg[msg_box - 1].data[3] = (unsigned char) (e_total_miles >> 24); //高八位
    can_msg[msg_box - 1].data[4] = 0; 
    can_msg[msg_box - 1].data[5] = 0;
    can_msg[msg_box - 1].data[6] = 0;
    can_msg[msg_box - 1].data[7] = 0;
    hal_can_sent(CAN_CHN, &can_msg[msg_box - 1]);

}
void PCAN_send_req(void) {

	U8 msg_box;
    msg_box = ID_RECV_NUM_ALL + 4;
    can_msg[msg_box - 1].data[0] = 0xff;
    can_msg[msg_box - 1].data[1] = (unsigned char) 0x0f
            + (unsigned char) ((ccvs_eng_req & 0x03) << 4)
            +(unsigned char) (0x03 << 6);
    can_msg[msg_box - 1].data[2] = 0xFF;
    can_msg[msg_box - 1].data[3] = 0xFF;
    can_msg[msg_box - 1].data[4] = 0xFF;
    can_msg[msg_box - 1].data[5] = 0xFF;
    can_msg[msg_box - 1].data[6] = 0xFF;
    can_msg[msg_box - 1].data[7] = 0xFF;
    hal_can_sent(CAN_CHN, &can_msg[msg_box - 1]);

}



void app_can_sent_task(void) {
	static U32 cont1 = 0;
 	PCAN_CCVS();
	if (cont1++>=10){
		cont1 = 0;
		PCAN_send_mile();
	}
	//PCAN_send_req();
}

void app_can_lost_time_cnt_100ms(void) 
{


}
