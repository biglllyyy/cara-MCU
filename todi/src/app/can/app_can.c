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
static can_msg_t can1_rx_msg[]={{0}} ;
static can_msg_t can1_tx_msg[] = {
	{0},
	{0},
	{0},
};

static pCanAnalyse can_rx_handle[ID_RECV_NUM_ALL] = { NULL };


U32 can_rx_msg_id_ads[ID_RECV_NUM_ALL] = { 0 };

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
unsigned char Indoor_t=60; //室内温度
unsigned char Outdoor_t=60; //室外温度
unsigned char Set_t=60; //设定
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

/******************************************/


//UDS_id_766  uds766;

extern led_check_t led_check;
/* function declare */
static void can_id_7E7_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_0C03A1A7_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_0C04A1A7_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_0C05A1A7_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_0C06A1A7_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_0C07A1A7_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_0C08A1A7_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_1818D0F3_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_1819D0F3_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_181AD0F3_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_180028F3_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_180128F3_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_180228F3_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_180328F3_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_180428F3_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_180028F4_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_180128F4_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_180228F4_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_0C09A79B_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_18FFC09E_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_18FECA00_analyse(can_msg_t *msg, can_pro_way_e way);

static  void can_updata_analyse(can_msg_t *msg, can_pro_way_e way);



static void app_can_process(can_msg_t *msg, can_pro_way_e way);


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
	
	app_bound_id(ID_REC_01_0C03A1A7, 0x1FFFFFFF, 8, EXTERN_ID, 2 , (ID_0C03A1A7_period*50)); 
	app_bound_id(ID_REC_02_0C04A1A7, 0x1FFFFFFF, 8, EXTERN_ID, 3 , (ID_0C04A1A7_period*50)); 
	app_bound_id(ID_REC_03_0C05A1A7, 0x1FFFFFFF, 8, EXTERN_ID, 4 , (ID_0C05A1A7_period*50)); 
	app_bound_id(ID_REC_04_0C06A1A7, 0x1FFFFFFF, 8, EXTERN_ID, 5 , (ID_0C06A1A7_period*50)); 
	app_bound_id(ID_REC_05_0C07A1A7, 0x1FFFFFFF, 8, EXTERN_ID, 6 , (ID_0C07A1A7_Period*50)); 
	app_bound_id(ID_REC_06_0C08A1A7, 0x1FFFFFFF, 8, EXTERN_ID, 7 , (ID_0C08A1A7_Period*50)); 
	app_bound_id(ID_REC_07_1818D0F3, 0x1FFFFFFF, 8, EXTERN_ID, 8 , (ID_1818D0F3_Period*50)); 
	app_bound_id(ID_REC_08_1819D0F3, 0x1FFFFFFF, 8, EXTERN_ID, 9 , (ID_1819D0F3_Period*50)); 
	app_bound_id(ID_REC_09_181AD0F3, 0x1FFFFFFF, 8, EXTERN_ID, 10, (ID_181AD0F3_Period*50)); 
	app_bound_id(ID_REC_10_180028F3, 0x1FFFFFFF, 8, EXTERN_ID, 11, (ID_180028F3_Period*50)); 
	app_bound_id(ID_REC_11_180128F3, 0x1FFFFFFF, 8, EXTERN_ID, 12, (ID_180128F3_Period*50)); 
	app_bound_id(ID_REC_12_180228F3, 0x1FFFFFFF, 8, EXTERN_ID, 13, (ID_180228F3_Period*50)); 
	app_bound_id(ID_REC_13_180328F3, 0x1FFFFFFF, 8, EXTERN_ID, 14, (ID_180328F3_Period*50)); 
	app_bound_id(ID_REC_14_180428F3, 0x1FFFFFFF, 8, EXTERN_ID, 15, (ID_180428F3_Period*50)); 
	app_bound_id(ID_REC_15_180028F4, 0x1FFFFFFF, 8, EXTERN_ID, 16, (ID_180028F4_Period*50)); 
	app_bound_id(ID_REC_16_180128F4, 0x1FFFFFFF, 8, EXTERN_ID, 17, (ID_180128F4_Period*50)); 
	app_bound_id(ID_REC_17_180228F4, 0x1FFFFFFF, 8, EXTERN_ID, 18, (ID_180228F4_Period*50)); 
	app_bound_id(ID_REC_18_0C09A79B, 0x1FFFFFFF, 8, EXTERN_ID, 19, (ID_0C09A79B_period*50)); 
	app_bound_id(ID_REC_19_18FFC09E, 0x1FFFFFFF, 8, EXTERN_ID, 20, (ID_18FFC09E_period*50)); 
	app_bound_id(ID_REC_20_18FECA00, 0x1FFFFFFF, 8, EXTERN_ID, 21, (ID_18FECA00_period*50)); 


	app_bound_id(UPDATA_ID         , 0x1FFFFFF0, 8, EXTERN_ID, 22, (PERIOD_MS(1000)));
	

	/* sent id bound,add your code here */
	app_bound_id(MCU_TO_PC_ID, 0xfff, 8, STAND_ID, (ID_RECV_NUM_ALL + 1),PERIOD_MS(100));
	app_bound_id(ID_SEND_1_0C19A7A1, 0x1FFFFFFF, 8, EXTERN_ID, (ID_RECV_NUM_ALL + 2),PERIOD_MS(100));
	app_bound_id(ID_SEND_2_0C1AA7A1, 0x1FFFFFFF, 8, EXTERN_ID, (ID_RECV_NUM_ALL + 3),PERIOD_MS(100));
	app_bound_id(ID_SEND_3_18FED917, 0x1FFFFFFF, 8, EXTERN_ID, (ID_RECV_NUM_ALL + 4),PERIOD_MS(100));
	app_bound_id(ACK_UPDATA_ID, 	 0x1FFFFFFF, 8, EXTERN_ID, (ID_RECV_NUM_ALL + 5),PERIOD_MS(100));
	
	
	
	/* register the can id process function */
	can_rx_pro = app_can_process;
	/*register the can id process */
	
	
	can_rx_handle[0]  = can_id_7E7_analyse; 	
	can_rx_handle[1]  = can_id_0C03A1A7_analyse;
	can_rx_handle[2]  = can_id_0C04A1A7_analyse;
	can_rx_handle[3]  = can_id_0C05A1A7_analyse;
	can_rx_handle[4]  = can_id_0C06A1A7_analyse;
	can_rx_handle[5]  = can_id_0C07A1A7_analyse;
	can_rx_handle[6]  = can_id_0C08A1A7_analyse;
	can_rx_handle[7]  = can_id_1818D0F3_analyse;
	can_rx_handle[8]  = can_id_1819D0F3_analyse;
	can_rx_handle[9]  = can_id_181AD0F3_analyse;
	can_rx_handle[10] = can_id_180028F3_analyse;
	can_rx_handle[11] = can_id_180128F3_analyse;
	can_rx_handle[12] = can_id_180228F3_analyse;
	can_rx_handle[13] = can_id_180328F3_analyse;
	can_rx_handle[14] = can_id_180428F3_analyse;
	can_rx_handle[15] = can_id_180028F4_analyse;
	can_rx_handle[16] = can_id_180128F4_analyse;
	can_rx_handle[17] = can_id_180228F4_analyse;
	can_rx_handle[18] = can_id_0C09A79B_analyse;
	can_rx_handle[19] = can_id_18FFC09E_analyse;
	can_rx_handle[20] = can_id_18FECA00_analyse;
	can_rx_handle[21] = can_updata_analyse; 	
	

	mid_can_init(CAN_CHN, CAN_CHIP);

	mid_can1_prepare(can1_tx_msg,can1_rx_msg);

	app_configuration_can(&can1_rx_msg[0],ID_REC_01_68X, 0x7F0, 8, STAND_ID, 3	, (ID_0C03A1A7_period*50));
	app_configuration_can(&can1_rx_msg[1],ID_REC_02_67X, 0x7F0, 8, STAND_ID, 4	, (ID_0C03A1A7_period*50));
	app_configuration_can(&can1_rx_msg[2],ID_REC_03_62X, 0x7F0, 8, STAND_ID, 5	, (ID_0C03A1A7_period*50));
	app_configuration_can(&can1_rx_msg[3],ID_REC_04_63X, 0x7F0, 8, STAND_ID, 6	, (ID_0C03A1A7_period*50));
	app_configuration_can(&can1_rx_msg[4],ID_REC_05_64X, 0x7F0, 8, STAND_ID, 7	, (ID_0C03A1A7_period*50));
	app_configuration_can(&can1_rx_msg[5],ID_REC_06_65X, 0x7F0, 8, STAND_ID, 8	, (ID_0C03A1A7_period*50));
	app_configuration_can(&can1_rx_msg[6],ID_REC_07_56X, 0x7F0, 8, STAND_ID, 9	, (ID_0C03A1A7_period*50));
	app_configuration_can(&can1_rx_msg[7],ID_REC_08_45X, 0x7F0, 8, STAND_ID, 10 , (ID_0C03A1A7_period*50));


	app_configuration_can(&can1_tx_msg[0],BCAN_ID_SEND_6A4, 0x7FF, 8, STAND_ID, 1 , (ID_0C03A1A7_period*50));
	app_configuration_can(&can1_tx_msg[1],BCAN_ID_SEND_454, 0x7FF, 8, STAND_ID, 2 , (ID_0C03A1A7_period*50));
	can1_rx_pro = can1_rx_handle;
	
	hal_can_init(1);
	wdg_feed();


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


static void can_id_0C03A1A7_analyse(can_msg_t *msg, can_pro_way_e way){
	U8 i  = 0;
	switch (way) {
	case CAN_PARSE:
			moto_voltage = (unsigned char) msg->data[0]+((unsigned int) msg->data[1] << 8);
			moto_current = (unsigned char) msg->data[2]+((unsigned int) msg->data[3] << 8);
            moto_speed = (unsigned char) msg->data[4]+((unsigned int) msg->data[5] << 8)/2;
            Motor_Temperature = msg->data[6];
            Inverter_t = msg->data[7];
		break;
	case CAN_LOST:
			moto_voltage = 10000;
			moto_current = 10000;
			moto_speed = 0;
            Motor_Temperature = 40;
            Inverter_t = 40;
		break;
	default:
		break;
	}
}
static void can_id_0C04A1A7_analyse(can_msg_t *msg, can_pro_way_e way){
	U8 i  = 0;
	switch (way) {
	case CAN_PARSE:
		Accelerator = (unsigned char) msg->data[4];
        Engine_Speed = ((unsigned char) msg->data[5]+((unsigned int) msg->data[6] << 8))/8;
		break;
	case CAN_LOST:
		Accelerator = 0;
		Engine_Speed = 0;
		break;
	default:
		break;
	}
}
static void can_id_0C05A1A7_analyse(can_msg_t *msg, can_pro_way_e way){
	U8 i  = 0;
	switch (way) {
	case CAN_PARSE:
		Fule_rate = (unsigned char) msg->data[0]+((unsigned int) msg->data[1] << 8);
        water_temp = msg->data[2];
        Engine_Fuhe = msg->data[3];
        Niaosu = msg->data[4];
        Accelerator_Shiji = msg->data[5]; //油门踏板实际
        Engine_oil_press = msg->data[6]; //机油压力
        ambient_air_temperature = msg->data[7]; //周围空气温度
		break;
	case CAN_LOST:
		Fule_rate = 0;
		water_temp = 40;
		Engine_Fuhe = 0;
		Niaosu = 0;
		Accelerator_Shiji = 0;
		Engine_oil_press = 0;
		ambient_air_temperature = 40;
		break;
	default:
		break;
	}
}
static void can_id_0C06A1A7_analyse(can_msg_t *msg, can_pro_way_e way){
	U8 i  = 0;
	switch (way) {
	case CAN_PARSE:
		Current_Gear = msg->data[1]&0x0f;
        TCU_Position = (msg->data[1] >> 4)&0x0f;
        TCU_warn = msg->data[2]&0x03;
        STOP = (msg->data[2] >> 2)&0x03;
        Speed_rmp = ((unsigned char) msg->data[3]+((unsigned int) msg->data[4] << 8))/8;
        Car_mode = msg->data[5];
        TCU_code = msg->data[6];
        TCU_level = msg->data[7];
		break;
	case CAN_LOST:
		Current_Gear = 0;
		TCU_Position = 0;
		TCU_warn =0 ;
		STOP = 0;
		Speed_rmp = 0;
		Car_mode = 0;
		TCU_code = 0;
		TCU_level = 0;
		break;
	default:
		break;
	}
}
static void can_id_0C07A1A7_analyse(can_msg_t *msg, can_pro_way_e way){
	U8 i  = 0;
	switch (way) {
	case CAN_PARSE:
		CAN_ON = msg->data[0]&0x01;
        CAN_ACC = (msg->data[0] >> 1)&0x01;
        Diagnosis = (msg->data[0] >> 2)&0x01;
        Mode_S = (msg->data[0] >> 3)&0x01;
        AC_SWITCH = (msg->data[0] >> 4)&0x01;
        Hybrid = (msg->data[0] >> 5)&0x01;
        Electric = (msg->data[0] >> 6)&0x01;
        Tradition = (msg->data[0] >> 7)&0x01;
        High_Voltage = (msg->data[1] >> 4)&0x01;
        Charge_Check = (msg->data[1] >> 5)&0x01;
        Digit_input3 = msg->data[2];
        Battery_Kt = (msg->data[3] >> 1)&0x01;
        Brake_Pedal = msg->data[4]&0x01;
        Speed_percent = msg->data[5];
        Brake_percent = msg->data[6];
		break;
	case CAN_LOST:
		CAN_KEY[0].byte = 0;
		CAN_KEY[1].byte = 0;
		Digit_input3 = 0;
		CAN_KEY[2].byte = 0;
		CAN_KEY[3].byte = 0;
		Speed_percent = 0;
		Brake_percent = 0;
		break;
	default:
		break;
	}
}
static void can_id_0C08A1A7_analyse(can_msg_t *msg, can_pro_way_e way){
	U8 i  = 0;
	switch (way) {
	case CAN_PARSE:
		Program = ((unsigned char) msg->data[0]
                +((unsigned int) msg->data[1] << 8)
                +((unsigned long) msg->data[2] << 16));
        tcu_spn = ((unsigned char) msg->data[3]
                +((unsigned int) msg->data[4] << 8)
                +((unsigned long) (msg->data[5]&0xE0) << 11));
        tcu_fmi = msg->data[5]&0x1F;
        tcu_cm = msg->data[6] >> 7;
        tcu_oc = msg->data[6]&0x7F;
        Car_LIFE = msg->data[7];
		break;
	case CAN_LOST:
		Program = 0;
		tcu_spn = 0;
		tcu_fmi = 0;
		tcu_cm = 0;
		tcu_oc = 0;
		Car_LIFE = 0;
		break;
	default:
		break;
	}
}
static void can_id_1818D0F3_analyse(can_msg_t *msg, can_pro_way_e way){
	U8 i  = 0;
	switch (way) {
	case CAN_PARSE:
		BMS_V = ((unsigned char) msg->data[0]
                +((unsigned int) msg->data[1] << 8));
        BMS_A = ((unsigned char) msg->data[2]
                +((unsigned int) msg->data[3] << 8));
        BMS_SOC = msg->data[4];
        Status_Flag1 = msg->data[5];
        Status_Flag2 = msg->data[6];
        Status_Flag3 = msg->data[7]&0x0C;
		break;
	case CAN_LOST:
		BMS_V = 10000;
		BMS_A = 10000;
		BMS_SOC = 0;
		Status_Flag1 = 0;
		Status_Flag2 = 0;
		Status_Flag3 = 0;
		break;
	default:
		break;
	}
}
static void can_id_1819D0F3_analyse(can_msg_t *msg, can_pro_way_e way){
	U8 i  = 0;
	switch (way) {
	case CAN_PARSE:
		BMS_A_charge = ((unsigned char) msg->data[0]
                +((unsigned int) msg->data[1] << 8));
        BMS_A_discharge = ((unsigned char) msg->data[2]
                +((unsigned int) msg->data[3] << 8));
        Warn_level = msg->data[4];
        BMS_V_average = ((unsigned char) msg->data[5]
                +((unsigned int) msg->data[6] << 8));
        BMS_T_H = msg->data[7];
		break;
	case CAN_LOST:
		BMS_A_charge = 10000;
		BMS_A_discharge = 10000;
		Warn_level = 0;
		BMS_V_average = 10000;
		BMS_T_H = 0;
		break;
	default:
		break;
	}
}
static void can_id_181AD0F3_analyse(can_msg_t *msg, can_pro_way_e way){
	U8 i  = 0;
	switch (way) {
	case CAN_PARSE:
		Oum_zheng = ((unsigned char) msg->data[0]
                +((unsigned int) msg->data[1] << 8));
        Oum_fu = ((unsigned char) msg->data[2]
                +((unsigned int) msg->data[3] << 8));
        Battery_single_H = ((unsigned char) msg->data[4]
                +((unsigned int) msg->data[5] << 8));
        Battery_single_L = ((unsigned char) msg->data[6]
                +((unsigned int) msg->data[7] << 8));
		break;
	case CAN_LOST:
		Oum_zheng = 0;
		Oum_fu = 0;
		Battery_single_H = 10000;
		Battery_single_L = 10000;
		break;
	default:
		break;
	}
}
static void can_id_180028F3_analyse(can_msg_t *msg, can_pro_way_e way){
	U8 i  = 0;
	switch (way) {
	case CAN_PARSE:
		Battery_number[0] = msg->data[0];
        Battery_voltage[0] = ((unsigned char) msg->data[1]
                +((unsigned int) msg->data[2] << 8));
        Battery_number[1] = msg->data[3];
        Battery_voltage[1] = ((unsigned char) msg->data[4]
                +((unsigned int) msg->data[5] << 8));
		break;
	case CAN_LOST:
		Battery_number[0] = 0;
		Battery_voltage[0] = 0;
		Battery_number[1] = 0;
		Battery_voltage[1] = 0;
		break;
	default:
		break;
	}
}
static void can_id_180128F3_analyse(can_msg_t *msg, can_pro_way_e way){
	U8 i  = 0;
	switch (way) {
	case CAN_PARSE:
		Battery_number[2] = msg->data[0];
        Battery_voltage[2] = ((unsigned char) msg->data[1]
                +((unsigned int) msg->data[2] << 8));
        Battery_number[3] = msg->data[3];
        Battery_voltage[3] = ((unsigned char) msg->data[4]
                +((unsigned int) msg->data[5] << 8));
		break;
	case CAN_LOST:
		Battery_number[2] = 0;
		Battery_voltage[2] = 0;
		Battery_number[3] = 0;
		Battery_voltage[3] = 0;
		break;
	default:
		break;
	}
}
static void can_id_180228F3_analyse(can_msg_t *msg, can_pro_way_e way){
	U8 i  = 0;
	switch (way) {
	case CAN_PARSE:
		Battery_number[4] = msg->data[0];
        Battery_voltage[4] = ((unsigned char) msg->data[1]
                +((unsigned int) msg->data[2] << 8));
        Battery_number[5] = msg->data[3];
        Battery_voltage[5] = ((unsigned char) msg->data[4]
                +((unsigned int) msg->data[5] << 8));
		break;
	case CAN_LOST:
		Battery_number[4] = 0;
		Battery_voltage[4] = 0;
		Battery_number[5] = 0;
		Battery_voltage[5] = 0;
		break;
	default:
		break;
	}
}
static void can_id_180328F3_analyse(can_msg_t *msg, can_pro_way_e way){
	U8 i  = 0;
	switch (way) {
	case CAN_PARSE:
		Battery_number[6] = msg->data[0];
        Battery_voltage[6] = ((unsigned char) msg->data[1]
                +((unsigned int) msg->data[2] << 8));
        Battery_number[7] = msg->data[3];
        Battery_voltage[7] = ((unsigned char) msg->data[4]
                +((unsigned int) msg->data[5] << 8));
		break;
	case CAN_LOST:
		Battery_number[6] = 0;
		Battery_voltage[6] = 0;
		Battery_number[7] = 0;
		Battery_voltage[7] = 0;
		break;
	default:
		break;
	}
}
static void can_id_180428F3_analyse(can_msg_t *msg, can_pro_way_e way){
	U8 i  = 0;
	switch (way) {
	case CAN_PARSE:
		Battery_number[8] = msg->data[0];
        Battery_voltage[8] = ((unsigned char) msg->data[1]
                +((unsigned int) msg->data[2] << 8));
        Battery_number[9] = msg->data[3];
        Battery_voltage[9] = ((unsigned char) msg->data[4]
                +((unsigned int) msg->data[5] << 8));
		break;
	case CAN_LOST:
		Battery_number[8] = 0;
		Battery_voltage[8] = 0;
		Battery_number[9] = 0;
		Battery_voltage[9] = 0;
		break;
	default:
		break;
	}
}
static void can_id_180028F4_analyse(can_msg_t *msg, can_pro_way_e way){
	U8 i  = 0;
	switch (way) {
	case CAN_PARSE:
		Battery_number_t[0] = msg->data[0];
        Battery_temp[0] = msg->data[1];
        Battery_number_t[1] = msg->data[2];
        Battery_temp[1] = msg->data[3];
        Battery_number_t[2] = msg->data[4];
        Battery_temp[2] = msg->data[5];
        Battery_number_t[3] = msg->data[6];
        Battery_temp[3] = msg->data[7];
		break;
	case CAN_LOST:
		Battery_number_t[0] = 0;
		Battery_temp[0] = 0;
		Battery_number_t[1] = 0;
		Battery_temp[1] = 0;
		Battery_number_t[2] = 0;
		Battery_temp[2] = 0;
		Battery_number_t[3] = 0;
		Battery_temp[3] = 0;
		break;
	default:
		break;
	}
}
static void can_id_180128F4_analyse(can_msg_t *msg, can_pro_way_e way){
	U8 i  = 0;
	switch (way) {
	case CAN_PARSE:
		Battery_number_t[4] = msg->data[0];
        Battery_temp[4] = msg->data[1];
        Battery_number_t[5] = msg->data[2];
        Battery_temp[5] = msg->data[3];
        Battery_number_t[6] = msg->data[4];
        Battery_temp[6] = msg->data[5];
        Battery_number_t[7] = msg->data[6];
        Battery_temp[7] = msg->data[7];
		break;
	case CAN_LOST:
		Battery_number_t[4] = 0;
		Battery_temp[4] = 0;
		Battery_number_t[5] = 0;
		Battery_temp[5] = 0;
		Battery_number_t[6] = 0;
		Battery_temp[6] = 0;
		Battery_number_t[7] = 0;
		Battery_temp[7] = 0;
		break;
	default:
		break;
	}
}
static void can_id_180228F4_analyse(can_msg_t *msg, can_pro_way_e way){
	U8 i  = 0;
	switch (way) {
	case CAN_PARSE:
		Battery_number_t[8] = msg->data[0];
        Battery_temp[8] = msg->data[1];
        Battery_number_t[9] = msg->data[2];
        Battery_temp[9] = msg->data[3];
		break;
	case CAN_LOST:
		Battery_number_t[8] = 0;
		Battery_temp[8] = 0;
		Battery_number_t[9] = 0;
		Battery_temp[9] = 0;
		break;
	default:
		break;
	}
}
static void can_id_0C09A79B_analyse(can_msg_t *msg, can_pro_way_e way){
	U8 i  = 0;
	switch (way) {
	case CAN_PARSE:
		DCAC_W = ((unsigned char) msg->data[0]
                +((unsigned int) msg->data[1] << 8));
        DCAC_V = ((unsigned char) msg->data[2]
                +((unsigned int) msg->data[3] << 8));
        DCAC_U = ((unsigned char) msg->data[4]
                +((unsigned int) msg->data[5] << 8));
        Sanreqi_t = msg->data[6];
        DCAC_Warn_code = msg->data[7];
		break;
	case CAN_LOST:
		DCAC_W = 10000;
		DCAC_V = 10000;
		DCAC_U = 10000;
		Sanreqi_t = 40;
		DCAC_Warn_code = 0;
		break;
	default:
		break;
	}
}
static void can_id_18FFC09E_analyse(can_msg_t *msg, can_pro_way_e way){
	U8 i  = 0;
	switch (way) {
	case CAN_PARSE:
		AC_Warn_code = msg->data[0];
        AC_opean = (msg->data[1] >> 5)&0x01;
        AC_mind_speed = (msg->data[1] >> 6)&0x01;
        AC_High_speed = (msg->data[1] >> 7)&0x01;
        AC_warn1 = msg->data[2]&0x01;
        AC_cold1_shuang = (msg->data[2] >> 1)&0x01;
        AC_wind = (msg->data[2] >> 2)&0x01;
        AC_save = (msg->data[2] >> 3)&0x01;
        AC_cold2_shuang = (msg->data[2] >> 4)&0x01;
        AC_cold2 = (msg->data[2] >> 5)&0x01;
        AC_cold1 = (msg->data[2] >> 7)&0x01;
        Indoor_t = msg->data[3];
        Outdoor_t = msg->data[4];
        Set_t = msg->data[5];
        AC_req = msg->data[6]&0x01;
        AC_LIFE = msg->data[7];

		break;
	case CAN_LOST:
		AC_Warn_code = 0;
		AC_KEY[0].byte = 0;
		AC_KEY[1].byte = 0;
		Indoor_t = 60;
		Outdoor_t = 60;
		Set_t = 60;
		AC_req = 0;
		AC_LIFE = 0;
		break;
	default:
		break;
	}
}
static void can_id_18FECA00_analyse(can_msg_t *msg, can_pro_way_e way)
{
	U8 i  = 0;
	switch (way) {
	case CAN_PARSE:

		break;
	case CAN_LOST:
		break;
	default:
		break;
	}
}
#define OnlineCheckID	0x121
#define ExcuteAppID     0x129
static  void can_updata_analyse(can_msg_t *msg, can_pro_way_e way)
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

void BCAN_SendCtl(void) {
    can_msg[0].data[0] = gCTL[0].byte;
    can_msg[0].data[1] = gCTL[1].byte;
    can_msg[0].data[2] = gCTL[2].byte;
    can_msg[0].data[3] = gCTL[3].byte;
    can_msg[0].data[4] = gCTL[4].byte;
    can_msg[0].data[5] = gCTL[5].byte;
    can_msg[0].data[6] = ((M_ON && wake_up3) || wake_up2) + (IN14 << 1)+(IN15 << 2)+(IN16 << 3)+(IN21 << 4);//模块雨刮控制
    can_msg[0].data[7] = 0;
	hal_can_sent(1, &can_msg[0]);
}

void BCAN_send_mile(void) {
	 can_msg[1].data[0] = (unsigned char) (e_total_miles); //低八位
	 can_msg[1].data[1] = (unsigned char) (e_total_miles >> 8); //二级八位
	 can_msg[1].data[2] = (unsigned char) (e_total_miles >> 16); //三级八位
	 can_msg[1].data[3] = (unsigned char) (e_total_miles >> 24); //高八位
	 can_msg[1].data[4] = 0; 
	 can_msg[1].data[5] = 0;
	 can_msg[1].data[6] = 0;
	 can_msg[1].data[7] = 0;
	 hal_can_sent(1, &can_msg[1]);
}


















void app_can_sent_task(void) {
	BCAN_SendCtl();
	BCAN_send_mile();


}

void app_can_lost_time_cnt_100ms(void) 
{


}
