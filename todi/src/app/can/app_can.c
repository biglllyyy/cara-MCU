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

/*ÊµÏÖÒ»¸öbyteÄÚµÄÈÎÒâÊı¾İ³¤¶ÈÊı¾İ»ñÈ¡*/
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
unsigned int Fcan_count = 0; //Ç°²¿Ä£¿
unsigned int Rcan_count = 0; //¶¥²¿Ä£¿
unsigned int Mcan_count = 0; //ÖĞ²¿Ä£¿

/*******************·Ö°ü´¦Àí*****************/
unsigned int moto_voltage; //µç»úµçÑ¹ 0.1v
unsigned int moto_current=10000; //µç»úµçÁ÷ 0.1A
unsigned int moto_speed; //µç»ú×ªËÙ 1r/min
unsigned char Motor_Temperature=40; //µç»úÎÂ¶È
unsigned char Inverter_t=40; //Äæ±äÆ÷ÎÂ¶È

unsigned char Accelerator; //ÓÍÃÅÌ¤°å
unsigned int Engine_Speed; //·¢¶¯»ú×ªËÙ

unsigned int Fule_rate; //ÓÍºÄ
unsigned char water_temp=40; //Ë®ÎÂ
unsigned char Engine_Fuhe; //·¢¶¯»ú¸ººÉ
unsigned char Niaosu; //ÄòËØ;
unsigned char Accelerator_Shiji; //ÓÍÃÅÌ¤°åÊµ¼Ê
unsigned char Engine_oil_press; //»úÓÍÑ¹Á¦
unsigned char ambient_air_temperature=40; //ÖÜÎ§¿ÕÆøÎÂ¶È

unsigned char Current_Gear; //µµÎ»
unsigned char TCU_Position; //ÀëºÏÎ»ÖÃ
unsigned char TCU_warn; //±äËÙÏä¹ÊÕÏ
unsigned char STOP; //Í£³µ¹ÊÕÏ
unsigned int Speed_rmp; //Êä³öÖá×ªËÙ
unsigned char Car_mode; //Õû³µÄ£Ê½
unsigned char TCU_code; //±äËÙÏä¹ÊÕÏ´úÂë
unsigned char TCU_level; //±äËÙÏä¹ÊÕÏµÈ¼¶

DATA_BIT CAN_KEY[4];
unsigned char Brake_percent; //ÖÆ¶¯Ì¤°å¿ª¶È
unsigned char Speed_percent; //¼ÓËÙÌ¤°å¿ª¶È
unsigned char Digit_input3; //Êı×ÖÁ¿ÊäÈë3

unsigned long Program; //³ÌĞò°æ±¾
unsigned long tcu_spn; //TCU¹ÊÕÏÂë
unsigned char tcu_fmi;
unsigned long tcu_cm;
unsigned char tcu_oc;
unsigned char Car_LIFE; //Õû³µ¿ØÖÆÆ÷

unsigned int BMS_V=10000; //µç³ØµçÑ¹
unsigned char BMS_SOC; //µç³ØSOC
unsigned int BMS_A=10000; //µç³ØµçÁ÷
unsigned char Status_Flag1;
unsigned char Status_Flag2;
unsigned char Status_Flag3;

unsigned int BMS_A_charge=10000; //×î´ó³äµçµçÁ÷
unsigned int BMS_A_discharge=10000; //×î´óµçÁ÷
unsigned char Warn_level; //¹ÊÕÏµÈ¼¶
unsigned int BMS_V_average=10000; //µç³ØÆ½¾ùµçÑ¹
unsigned char BMS_T_H; //µç³Ø×î¸ßÎÂ¶È

unsigned int Oum_zheng; //¾øÔµÕı×èÖµ
unsigned int Oum_fu; //¾øÔµ¸º×èÖµ
unsigned int Battery_single_H=10000; //µç³Øµ¥Ìå¸ßµçÑ¹
unsigned int Battery_single_L=10000; //µç³Øµ¥ÌåµÍµçÑ¹

unsigned char Battery_number[10]; //µç³Ø±àºÅ
unsigned int Battery_voltage[10]; //µç³Øµ¥ÌåµçÑ¹

unsigned char Battery_number_t[10]; //µç³ØÎÂ¶È±àºÅ
unsigned int Battery_temp[10]; //µç³Øµ¥ÌåÎÂ¶È

unsigned int DCAC_W=10000;
unsigned int DCAC_V=10000;
unsigned int DCAC_U=10000;
unsigned char Sanreqi_t=40; //É¢ÈÈÆ÷ÎÂ¶È
unsigned char DCAC_Warn_code; //¹ÊÕÏ´úÂë

unsigned char AC_Warn_code; //AC¹ÊÕÏ´úÂë
DATA_BIT AC_KEY[2];
unsigned char Indoor_t=60; //ÊÒÄÚÎÂ¶È
unsigned char Outdoor_t=60; //ÊÒÍâÎÂ¶È
unsigned char Set_t=60; //Éè¶¨
unsigned char AC_req; //ÖÆÀäÇëÇó
unsigned char AC_LIFE; //ACÉúÃü



/**************************************************/
unsigned char battle_t; //µç³Ø¾ùÎÂ
unsigned char battle_t_warn; //µç³Ø¹ıÎÂ±¨¾¯
unsigned char battle_jueyuan_warn; //¾øÔµ¹ıµÍ±¨¾¯
unsigned char battle_higtv1_warn; //¸ßÑ¹µç³Ø1¹ÊÕÏ
unsigned char battle_higtv2_warn; //¸ßÑ¹µç³Ø2¹ÊÕÏ
unsigned char moto_t_warn; //µç»ú¹ıÎÂ
unsigned char moto_ready; //readyĞÅºÅ
unsigned char xudc; //Ğîµç³ØÒì³£
unsigned char moto_warn; //µç»ú¹ÊÕÏ

unsigned char BMS_offline_warn; //µç³Ø²»ÔÚÏß
unsigned char MCU_offline_warn; //µç»ú²»ÔÚÏß
unsigned char TCU_offline_warn; //±äËÙÏä²»ÔÚÏß
unsigned char Engine_offline_warn; //·¢¶¯»ú²»ÔÚÏß
unsigned char Engine_wait; //·¢¶¯»úµ¡ËÙ
unsigned char EV; //Ä£Ê½
unsigned char Signal_charge; //³äµçĞÅºÅ

unsigned char mil_lamp_request; //»ÆÉ«Èı¼¶±¨¾¯
unsigned char fail_mode; //ÎÄ×Ö±¨¾¯
unsigned char mil_buzz_request; //·äÃùÆ÷Ïì
unsigned long hcu_spn; //HCU¹ÊÕÏÂë
unsigned char hcu_fmi;

unsigned char Eng_static_test; //·¢¶¯»úÎÈÌ¬×ªËÙ²âÊÔ
unsigned char tcu_start_allow; //·¢¶¯»úÔËĞĞÆô¶¯

unsigned char Lamp_status_YELLOW; //±äËÙÏä»ÆµÆ×´Ì¬
unsigned char Lamp_status_RED; //±äËÙÏäºìµÆ×´Ì¬

unsigned long ccu_spn; //CCU¹ÊÕÏÂë
unsigned char ccu_fmi;

unsigned long ecu_spn; //ECU¹ÊÕÏÂë
unsigned char ecu_fmi;

unsigned int ENG_start_req; //ÒÇ±í¿ØÖÆÆô¶¯
unsigned long mot_work_time; //µç»ú¹¤×÷ÀÛ»ıÊ±¼ä
unsigned char mot_work_percent; //µç»ú¹¤×÷Ê±¼ä±È
unsigned char DCDC_ctrl; //DCDC×´Ì¬
unsigned char hcu_starter_forbid; //Æğ¶¯»ú¹¤×÷
unsigned char aircompress_enable; //Ñ¹Ëõ»ú
unsigned char Speed_lim_mode; //ÏŞËÙÄ£Ê½

unsigned char water_level; //Ë®Î»

unsigned char Wait_start; //µÈ´ıÆô¶¯
unsigned char Maintain_warn; //Î¬»¤±¨¾¯
unsigned char Stop_warn; //·¢¶¯»úÍ£»ú±¨¾¯
unsigned char Engine_warn; //·¢¶¯»ú¹ÊÕÏ

U32 Vcan_timeout_cfg;
U32 Bcan_timeout_cfg;

/******************************************/
unsigned char PRESS[2]; //ÆøÑ¹Öµ
unsigned char ccvs_eng_req; //Æô¶¯ÇëÇó


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
        Accelerator_Shiji = msg->data[5]; //ÓÍÃÅÌ¤°åÊµ¼Ê
        Engine_oil_press = msg->data[6]; //»úÓÍÑ¹Á¦
        ambient_air_temperature = msg->data[7]; //ÖÜÎ§¿ÕÆøÎÂ¶È
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
			//·¢ËÍ×ÜÀï³ÌĞÅÏ¢µÄ±¨ÎÄ
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

void MCU_TO_PC_send(void) {  //¶ÔÓ¦±¨ÎÄ0x7EF
	U8 msg_box;

	//·¢ËÍMCU TO PCµÄ±¨ÎÄ
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
	switch (way) {
	case CAN_PARSE:
			switch (msg->id) {
	            case 0x681:
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
	switch (way) {
	case CAN_PARSE:
		switch (msg->id) {
			case 0x561:
				Fcan_count = 0;
                fPOUT.BYTES[0] = msg->data[0];
                fPOUT.BYTES[1] = msg->data[1];
                for (j = 0; j < 4; j++) {//Êä³ö×´Ì¬²É¼¯
                    for (i = 0; i < 4; i++) {
                        fPF[i + 4 * j] = (msg->data[j + 2] >> (i * 2))&0x03;
                    }
                }
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
    can_msg[0].data[0] = gCTL[0].byte;
    can_msg[0].data[1] = gCTL[1].byte;
    can_msg[0].data[2] = gCTL[2].byte;
    can_msg[0].data[3] = gCTL[3].byte;
    can_msg[0].data[4] = gCTL[4].byte;
    can_msg[0].data[5] = gCTL[5].byte;
    can_msg[0].data[6] = ((M_ON && wake_up3) || wake_up2) + (IN14 << 1)+(IN15 << 2)+(IN16 << 3)+(IN21 << 4);//Ä£¿éÓê¹Î¿ØÖÆ
    can_msg[0].data[7] = 0;
	hal_can_sent(1, &can_msg[0]);
}

void BCAN_send_mile(void) {
	 can_msg[1].data[0] = (unsigned char) (e_total_miles); //µÍ°ËÎ»
	 can_msg[1].data[1] = (unsigned char) (e_total_miles >> 8); //¶ş¼¶°ËÎ»
	 can_msg[1].data[2] = (unsigned char) (e_total_miles >> 16); //Èı¼¶°ËÎ»
	 can_msg[1].data[3] = (unsigned char) (e_total_miles >> 24); //¸ß°ËÎ»
	 can_msg[1].data[4] = 0; 
	 can_msg[1].data[5] = 0;
	 can_msg[1].data[6] = 0;
	 can_msg[1].data[7] = 0;
	 hal_can_sent(1, &can_msg[1]);
}

void PCAN_CCVS(void) {
	U8 msg_box;

	msg_box = ID_RECV_NUM_ALL + 2;
	can_msg[msg_box - 1].buffer_num = msg_box;
    can_msg[msg_box - 1].data[0] = (unsigned char) (M_ON << 6)//ONµµ
            + (unsigned char) (IN4 << 5)//STµµ
            + (unsigned char) (IN27 << 4)//ºóÃÅ¿ªĞÅºÅ
            + (unsigned char) (IN20 << 3)//Ç°Ãæ¿ªĞÅºÅ
            + (unsigned char) (rKL15 << 2)//ºó²ÕÃÅ¿ªĞÅºÅ
            + (unsigned char) (rKL8 << 1)//×¤³µĞÅºÅ
            + (unsigned char) (rKL10 || rKL12) ;//ÆøÑ¹µÍ±¨¾¯ĞÅºÅ
   can_msg[msg_box - 1].data[1] = (unsigned char) (LED4 << 7)//×ó×ªÏò
            + (unsigned char) (LED17 << 6)//Ô¶¹âµÆ
            + (unsigned char) (LED19 << 5)//Ç°ÎíµÆ
            + (unsigned char) (LED8 << 4)//ÓÒ×ªÏò
            + (unsigned char) (IN33 << 3)//ÓÍÁ¿±¨¾¯
            + (unsigned char) (0 << 2)//·¢¶¯»úÔ¤ÈÈ
            + (unsigned char) (LED22 << 1)//½ü¹âµÆ
            + (unsigned char) (rLED_flag) ;//ºóÎíµÆ
    can_msg[msg_box - 1].data[2] = 0xff;
    can_msg[msg_box - 1].data[3] = (unsigned char) (pSpeed * 2);
    can_msg[msg_box - 1].data[4] = PRESS[0]*100;
    can_msg[msg_box - 1].data[5] = (unsigned char) (PRESS[0]*100 >> 8);
    can_msg[msg_box - 1].data[6] = PRESS[1]*100;
    can_msg[msg_box - 1].data[7] = (unsigned char) (PRESS[1]*100 >> 8);
    hal_can_sent(CAN_CHN, &can_msg[msg_box - 1]);
}
void PCAN_send_mile(void) {
	U8 msg_box;
    msg_box = ID_RECV_NUM_ALL + 3;
	can_msg[msg_box - 1].buffer_num = msg_box;;
    can_msg[msg_box - 1.data[0] = (unsigned char) (e_total_miles); //µÍ°ËÎ»
    can_msg[msg_box - 1.data[1] = (unsigned char) (e_total_miles >> 8); //¶ş¼¶°ËÎ»
    can_msg[msg_box - 1.data[2] = (unsigned char) (e_total_miles >> 16); //Èı¼¶°ËÎ»
    can_msg[msg_box - 1.data[3] = (unsigned char) (e_total_miles >> 24); //¸ß°ËÎ»
    can_msg[msg_box - 1.data[4] = 0; 
    can_msg[msg_box - 1.data[5] = 0;
    can_msg[msg_box - 1.data[6] = 0;
    can_msg[msg_box - 1.data[7] = 0;
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
	BCAN_SendCtl();
	BCAN_send_mile();
 	PCAN_CCVS();
	PCAN_send_mile();
	PCAN_send_req();
}

void app_can_lost_time_cnt_100ms(void) 
{


}
