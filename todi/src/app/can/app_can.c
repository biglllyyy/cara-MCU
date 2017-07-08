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

can_signal_t can0 = { 0 };
#if 1
can_signal_t can1= {0};
can_signal_t can2= {0};
can_signal_t can3= {0};
can_signal_t can4= {0};
can_signal_t can5= {0};
#endif
//UDS_id_766  uds766;

U16 can_id_100017EF_lost_timecnt = 0xffff;
U16 can_id_1801FBEF_lost_timecnt = 0xffff;
U16 can_id_1811FBEF_lost_timecnt = 0xffff;
U16 can_id_18FF08F2_lost_timecnt = 0xffff;
U16 can_id_10F8159E_lost_timecnt = 0xffff;
U16 can_id_10F8169E_lost_timecnt = 0xffff;
U16 can_id_18F8179E_lost_timecnt = 0xffff;
U16 can_id_18F8189E_lost_timecnt = 0xffff;
U16 can_id_18F8199E_lost_timecnt = 0xffff;
U16 can_id_18F81A9E_lost_timecnt = 0xffff;
U16 can_id_18FF12F7_lost_timecnt = 0xffff;
U16 can_id_18FF0AF8_lost_timecnt = 0xffff;
U16 can_id_18FF0BF8_lost_timecnt = 0xffff;
U16 can_id_18FF0CF9_lost_timecnt = 0xffff;
U16 can_id_18FF0DF9_lost_timecnt = 0xffff;
U16 can_id_1801EFA9_lost_timecnt = 0xffff;
U16 can_id_104C1000_lost_timecnt = 0xffff;



U16 can_id_7E7_lost_timecnt=0xffff;


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

	app_init_variable();
	can_chip_mode_switch(&io_can_stb, &io_can_en, NORMAL_MODE);
	/* receive id bound,add your code here,the id need to be received */
	app_bound_id(PC_T0_MCU_ID, 0xfff, 8, STAND_ID, 1, PERIOD_MS(100));
	
	app_bound_id(ID_REC_01_100017EF, 0x1FFFFFFF, 8, EXTERN_ID, 2,  ID_100017EF_period);
	app_bound_id(ID_REC_02_1801FBEF, 0x1FFFFFFF, 8, EXTERN_ID, 3,  ID_1801FBEF_period);
	app_bound_id(ID_REC_03_1811FBEF, 0x1FFFFFFF, 8, EXTERN_ID, 4,  ID_1811FBEF_period);
	app_bound_id(ID_REC_04_18FF08F2, 0x1FFFFFFF, 8, EXTERN_ID, 5,  ID_18FF08F2_period);
	app_bound_id(ID_REC_05_10F8159E, 0x1FFFFFFF, 8, EXTERN_ID, 6,  ID_10F8159E_Period);
	app_bound_id(ID_REC_06_10F8169E, 0x1FFFFFFF, 8, EXTERN_ID, 7,  ID_10F8169E_Period);
	app_bound_id(ID_REC_07_18F8179E, 0x1FFFFFFF, 8, EXTERN_ID, 8,  ID_18F8179E_Period);
	app_bound_id(ID_REC_08_18F8189E, 0x1FFFFFFF, 8, EXTERN_ID, 9,  ID_18F8189E_Period);
	app_bound_id(ID_REC_09_18F8199E, 0x1FFFFFFF, 8, EXTERN_ID, 10, ID_18F8199E_Period);
	app_bound_id(ID_REC_10_18F81A9E, 0x1FFFFFFF, 8, EXTERN_ID, 11, ID_18F81A9E_Period);
	app_bound_id(ID_REC_11_18FF12F7, 0x1FFFFFFF, 8, EXTERN_ID, 12, ID_18FF12F7_Period);
	app_bound_id(ID_REC_12_18FF0AF8, 0x1FFFFFFF, 8, EXTERN_ID, 13, ID_18FF0AF8_Period);
	app_bound_id(ID_REC_13_18FF0BF8, 0x1FFFFFFF, 8, EXTERN_ID, 14, ID_18FF0BF8_Period);
	app_bound_id(ID_REC_14_18FF0CF9, 0x1FFFFFFF, 8, EXTERN_ID, 15, ID_18FF0CF9_Period);
	app_bound_id(ID_REC_15_18FF0DF9, 0x1FFFFFFF, 8, EXTERN_ID, 16, ID_18FF0DF9_Period);
	app_bound_id(ID_REC_16_1801EFA9, 0x1FFFFFFF, 8, EXTERN_ID, 17, ID_1801EFA9_Period);
	app_bound_id(ID_REC_17_104C1000, 0x1FFFF000, 8, EXTERN_ID, 18, ID_104C1000_Period);
	
	

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
		can_id_7E7_lost_timecnt = 0;
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
		data_copy((U8*) &can0.ID_100017EF[0], (U8*) &msg->data[0], msg->dlc);
		can_id_100017EF_lost_timecnt = 0;
		break;
	case CAN_LOST:
		break;
	default:
		break;
	}
}
static void can_id_1801FBEF_analyse(can_msg_t *msg, can_pro_way_e way) {
	switch (way) {
	case CAN_PARSE:
		data_copy((U8*) &can0.ID_1801FBEF[0], (U8*) &msg->data[0], msg->dlc);
		can_id_1801FBEF_lost_timecnt = 0;
		break;
	case CAN_LOST:
		break;
	default:
		break;
	}
}
static void can_id_1811FBEF_analyse(can_msg_t *msg, can_pro_way_e way) {
	switch (way) {
	case CAN_PARSE:
		data_copy((U8*) &can0.ID_1811FBEF[0], (U8*) &msg->data[0], msg->dlc);
		can_id_1811FBEF_lost_timecnt = 0;
		break;
	case CAN_LOST:
		break;
	default:
		break;
	}
}
static void can_id_18FF08F2_analyse(can_msg_t *msg, can_pro_way_e way) {
	switch (way) {
	case CAN_PARSE:
		data_copy((U8*) &can0.ID_18FF08F2[0], (U8*) &msg->data[0], msg->dlc);
		can_id_18FF08F2_lost_timecnt = 0;
		break;
	case CAN_LOST:
		break;
	default:
		break;
	}
}
static void can_id_10F8159E_analyse(can_msg_t *msg, can_pro_way_e way) {
	switch (way) {
	case CAN_PARSE:
		data_copy((U8*) &can0.ID_10F8159E[0], (U8*) &msg->data[0], msg->dlc);
		can_id_10F8159E_lost_timecnt = 0;
		break;
	case CAN_LOST:
		break;
	default:
		break;
	}
}
static void can_id_10F8169E_analyse(can_msg_t *msg, can_pro_way_e way) {
	switch (way) {
	case CAN_PARSE:
		data_copy((U8*) &can0.ID_10F8169E[0], (U8*) &msg->data[0], msg->dlc);
		can_id_10F8169E_lost_timecnt = 0;
		break;
	case CAN_LOST:
		break;
	default:
		break;
	}
}
static void can_id_18F8179E_analyse(can_msg_t *msg, can_pro_way_e way) {
	switch (way) {
	case CAN_PARSE:
		data_copy((U8*) &can0.ID_18F8179E[0], (U8*) &msg->data[0], msg->dlc);
		can_id_18F8179E_lost_timecnt = 0;
		break;
	case CAN_LOST:
		break;
	default:
		break;
	}
}
static void can_id_18F8189E_analyse(can_msg_t *msg, can_pro_way_e way) {
	switch (way) {
	case CAN_PARSE:
		data_copy((U8*) &can0.ID_18F8189E[0], (U8*) &msg->data[0], msg->dlc);
		can_id_18F8189E_lost_timecnt = 0;
		break;
	case CAN_LOST:
		break;
	default:
		break;
	}
}
static void can_id_18F8199E_analyse(can_msg_t *msg, can_pro_way_e way) {
	switch (way) {
	case CAN_PARSE:
		data_copy((U8*) &can0.ID_18F8199E[0], (U8*) &msg->data[0], msg->dlc);
		can_id_18F8199E_lost_timecnt = 0;
		break;
	case CAN_LOST:
		break;
	default:
		break;
	}
}
static void can_id_18F81A9E_analyse(can_msg_t *msg, can_pro_way_e way) {
	switch (way) {
	case CAN_PARSE:
		data_copy((U8*) &can0.ID_18F81A9E[0], (U8*) &msg->data[0], msg->dlc);
		can_id_18F81A9E_lost_timecnt = 0;
		break;
	case CAN_LOST:
		break;
	default:
		break;
	}
}
static void can_id_18FF12F7_analyse(can_msg_t *msg, can_pro_way_e way) {
	switch (way) {
	case CAN_PARSE:
		data_copy((U8*) &can0.ID_18FF12F7[0], (U8*) &msg->data[0], msg->dlc);
		can_id_18FF12F7_lost_timecnt = 0;
		break;
	case CAN_LOST:
		break;
	default:
		break;
	}
}
static void can_id_18FF0AF8_analyse(can_msg_t *msg, can_pro_way_e way) {
	switch (way) {
	case CAN_PARSE:
		data_copy((U8*) &can0.ID_18FF0AF8[0], (U8*) &msg->data[0], msg->dlc);
		can_id_18FF0AF8_lost_timecnt = 0;
		break;
	case CAN_LOST:
		break;
	default:
		break;
	}
}
static void can_id_18FF0BF8_analyse(can_msg_t *msg, can_pro_way_e way) {
	switch (way) {
	case CAN_PARSE:
		data_copy((U8*) &can0.ID_18FF0BF8[0], (U8*) &msg->data[0], msg->dlc);
		can_id_18FF0BF8_lost_timecnt = 0;
		break;
	case CAN_LOST:
		break;
	default:
		break;
	}
}
static void can_id_18FF0CF9_analyse(can_msg_t *msg, can_pro_way_e way) {
	switch (way) {
	case CAN_PARSE:
		data_copy((U8*) &can0.ID_18FF0CF9[0], (U8*) &msg->data[0], msg->dlc);
		can_id_18FF0CF9_lost_timecnt = 0;
		break;
	case CAN_LOST:
		break;
	default:
		break;
	}
}
static void can_id_18FF0DF9_analyse(can_msg_t *msg, can_pro_way_e way) {
	switch (way) {
	case CAN_PARSE:
		data_copy((U8*) &can0.ID_18FF0DF9[0], (U8*) &msg->data[0], msg->dlc);
		can_id_18FF0DF9_lost_timecnt = 0;
		break;
	case CAN_LOST:
		break;
	default:
		break;
	}
}
static void can_id_1801EFA9_analyse(can_msg_t *msg, can_pro_way_e way) {
	switch (way) {
	case CAN_PARSE:
		data_copy((U8*) &can0.ID_1801EFA9[0], (U8*) &msg->data[0], msg->dlc);
		can_id_1801EFA9_lost_timecnt = 0;
		break;
	case CAN_LOST:
		break;
	default:
		break;
	}
}
static void can_id_104C1000_analyse(can_msg_t *msg, can_pro_way_e way) {
	switch (way) {
	case CAN_PARSE:
		data_copy((U8*) &can0.ID_104C1000[0], (U8*) &msg->data[0], msg->dlc);
		can_id_104C1000_lost_timecnt = 0;
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

void MCU_TO_PC_send(void) {
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
void can_id_1800F328_send(void) {
	U8 msg_box;

	//发送总里程信息的报文
	msg_box = ID_RECV_NUM_ALL + 2;
	can_msg[msg_box - 1].buffer_num = msg_box;

	total_odo.odo32 = info.Odo;

	can_msg[msg_box - 1].data[0] = 1;
	can_msg[msg_box - 1].data[1] = total_odo.odo8[1];
	can_msg[msg_box - 1].data[2] = total_odo.odo8[2];
	can_msg[msg_box - 1].data[3] = total_odo.odo8[3];
	can_msg[msg_box - 1].data[4] = 0;
	can_msg[msg_box - 1].data[5] = 0;
	can_msg[msg_box - 1].data[6] = 0;
	can_msg[msg_box - 1].data[7] = 0;

	if (IPconfig.Sts == 1)			//总里程有效时，才往外发里程相关的CAN 报文
			{
		hal_can_sent(CAN_CHN, &can_msg[msg_box - 1]);
	} else {
#if 1  //just for test can send by niujianlong
		hal_can_sent(CAN_CHN, &can_msg[msg_box - 1]);
#endif

	}
}

void can_id_1882BBAB_send(void) {
	U8 msg_box;

	//发送总里程信息的报文
	msg_box = ID_RECV_NUM_ALL + 3;
	can_msg[msg_box - 1].buffer_num = msg_box;

	can_msg[msg_box - 1].data[0] = 2;
	can_msg[msg_box - 1].data[1] = 16;
	can_msg[msg_box - 1].data[2] = 3;
	can_msg[msg_box - 1].data[3] = 0;
	can_msg[msg_box - 1].data[4] = 0;
	can_msg[msg_box - 1].data[5] = 0;
	can_msg[msg_box - 1].data[6] = 0;
	can_msg[msg_box - 1].data[7] = 0;

	if (IPconfig.Sts == 1)			//仪表未激活的情况下，才发送配置代码请求帧
			{
		//hal_can_sent(CAN_CHN,&can_msg[msg_box-1]);
	} else			//仪表激活的情况下，就不会发送配置代码请求帧了
	{

	}
}

void can_id_18AB9B28_send(T_UDS *data, U8 len) {
	U8 msg_box;

	if (data->len == 0)
		return;

	//发送总里程信息的报文
	msg_box = ID_RECV_NUM_ALL + 4;
	can_msg[msg_box - 1].buffer_num = msg_box;

	can_msg[msg_box - 1].data[0] = data->len;
	can_msg[msg_box - 1].data[1] = data->id;
	can_msg[msg_box - 1].data[2] = data->data[0];
	can_msg[msg_box - 1].data[3] = data->data[1];
	can_msg[msg_box - 1].data[4] = data->data[2];
	can_msg[msg_box - 1].data[5] = data->data[3];
	can_msg[msg_box - 1].data[6] = data->data[4];
	can_msg[msg_box - 1].data[7] = data->data[5];

//	data_copy(can_msg[msg_box-1].data, (U8*)&data, len);

	hal_can_sent(CAN_CHN, &can_msg[msg_box - 1]);

	memset(&uds_sent, 0, sizeof(uds_sent));

}

void can_id_18FFA017_send(void) {
	U8 msg_box;

	//发送总里程信息的报文
	msg_box = ID_RECV_NUM_ALL + 5;
	can_msg[msg_box - 1].buffer_num = msg_box;

	can_msg[msg_box - 1].data[0] = 2;
	can_msg[msg_box - 1].data[1] = 16;
	can_msg[msg_box - 1].data[2] = 3;
	can_msg[msg_box - 1].data[3] = 0;
	can_msg[msg_box - 1].data[4] = 0;
	can_msg[msg_box - 1].data[5] = 0;
	can_msg[msg_box - 1].data[6] = 0;
	can_msg[msg_box - 1].data[7] = 0;

	if (IPconfig.Sts == 1)			//仪表未激活的情况下，才发送配置代码请求帧
			{
		//hal_can_sent(CAN_CHN,&can_msg[msg_box-1]);
	} else			//仪表激活的情况下，就不会发送配置代码请求帧了
	{

	}
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
		can_id_1800F328_send();
#endif

	}
}

void app_can_lost_time_cnt_100ms(void) {
	static U8 g_u8IgnSts_bak = OFF;

	if ((ON == g_u8IgnSts) && (OFF == g_u8IgnSts_bak)) {
		
		can_id_100017EF_lost_timecnt = 0xffff;
		can_id_1801FBEF_lost_timecnt = 0xffff;
		can_id_1811FBEF_lost_timecnt = 0xffff;
		can_id_18FF08F2_lost_timecnt = 0xffff;
		can_id_10F8159E_lost_timecnt = 0xffff;
		can_id_10F8169E_lost_timecnt = 0xffff;
		can_id_18F8179E_lost_timecnt = 0xffff;
		can_id_18F8189E_lost_timecnt = 0xffff;
		can_id_18F8199E_lost_timecnt = 0xffff;
		can_id_18F81A9E_lost_timecnt = 0xffff;
		can_id_18FF12F7_lost_timecnt = 0xffff;
		can_id_18FF0AF8_lost_timecnt = 0xffff;
		can_id_18FF0BF8_lost_timecnt = 0xffff;
		can_id_18FF0CF9_lost_timecnt = 0xffff;
		can_id_18FF0DF9_lost_timecnt = 0xffff;
		can_id_1801EFA9_lost_timecnt = 0xffff;
		can_id_104C1000_lost_timecnt = 0xffff;
		
		memset(&can0, 0, sizeof(can0));

		g_u8IgnSts_bak = g_u8IgnSts;
	} else if ((OFF == g_u8IgnSts) && (ON == g_u8IgnSts_bak)) {
		
		can_id_100017EF_lost_timecnt = 0;
		can_id_1801FBEF_lost_timecnt = 0;
		can_id_1811FBEF_lost_timecnt = 0;
		can_id_18FF08F2_lost_timecnt = 0;
		can_id_10F8159E_lost_timecnt = 0;
		can_id_10F8169E_lost_timecnt = 0;
		can_id_18F8179E_lost_timecnt = 0;
		can_id_18F8189E_lost_timecnt = 0;
		can_id_18F8199E_lost_timecnt = 0;
		can_id_18F81A9E_lost_timecnt = 0;
		can_id_18FF12F7_lost_timecnt = 0;
		can_id_18FF0AF8_lost_timecnt = 0;
		can_id_18FF0BF8_lost_timecnt = 0;
		can_id_18FF0CF9_lost_timecnt = 0;
		can_id_18FF0DF9_lost_timecnt = 0;
		can_id_1801EFA9_lost_timecnt = 0;
		can_id_104C1000_lost_timecnt = 0;
		

		memset(&can0, 0, sizeof(can0));

		g_u8IgnSts_bak = g_u8IgnSts;
	} else {
		if (can_id_100017EF_lost_timecnt < 0xffff) {
			can_id_100017EF_lost_timecnt++;
		}
		if (can_id_1801FBEF_lost_timecnt < 0xffff) {
			can_id_1801FBEF_lost_timecnt++;
		}
		if (can_id_1811FBEF_lost_timecnt < 0xffff) {
			can_id_1811FBEF_lost_timecnt++;
		}
		if (can_id_18FF08F2_lost_timecnt < 0xffff) {
			can_id_18FF08F2_lost_timecnt++;
		}
		if (can_id_10F8159E_lost_timecnt < 0xffff) {
			can_id_10F8159E_lost_timecnt++;
		}
		if (can_id_10F8169E_lost_timecnt < 0xffff) {
			can_id_10F8169E_lost_timecnt++;
		}
		if (can_id_18F8179E_lost_timecnt < 0xffff) {
			can_id_18F8179E_lost_timecnt++;
		}
		if (can_id_18F8189E_lost_timecnt < 0xffff) {
			can_id_18F8189E_lost_timecnt++;
		}
		if (can_id_18F8199E_lost_timecnt < 0xffff) {
			can_id_18F8199E_lost_timecnt++;
		}
		if (can_id_18F81A9E_lost_timecnt < 0xffff) {
			can_id_18F81A9E_lost_timecnt++;
		}
		if (can_id_18FF12F7_lost_timecnt < 0xffff) {
			can_id_18FF12F7_lost_timecnt++;
		}
		if (can_id_18FF0AF8_lost_timecnt < 0xffff) {
			can_id_18FF0AF8_lost_timecnt++;
		}
		if (can_id_18FF0BF8_lost_timecnt < 0xffff) {
			can_id_18FF0BF8_lost_timecnt++;
		}
		if (can_id_18FF0CF9_lost_timecnt < 0xffff) {
			can_id_18FF0CF9_lost_timecnt++;
		}
		if (can_id_18FF0DF9_lost_timecnt < 0xffff) {
			can_id_18FF0DF9_lost_timecnt++;
		}
		if (can_id_1801EFA9_lost_timecnt < 0xffff) {
			can_id_1801EFA9_lost_timecnt++;
		}
		if (can_id_104C1000_lost_timecnt < 0xffff) {
			can_id_104C1000_lost_timecnt++;
		}
		
	}
}
