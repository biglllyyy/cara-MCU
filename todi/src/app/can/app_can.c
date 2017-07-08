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

U16 can_id_18AA28F3_lost_timecnt = 0xffff;
U16 can_id_1818D0F3_lost_timecnt = 0xffff;
U16 can_id_1819D0F3_lost_timecnt = 0xffff;
U16 can_id_181AD0F3_lost_timecnt = 0xffff;
U16 can_id_181BD0F3_lost_timecnt = 0xffff;
U16 can_id_181CD0F3_lost_timecnt = 0xffff;
U16 can_id_181DD0F3_lost_timecnt = 0xffff;
U16 can_id_181ED0F3_lost_timecnt = 0xffff;
U16 can_id_181FD0F3_lost_timecnt = 0xffff;
U16 can_id_1800F328_lost_timecnt = 0xffff;
U16 can_id_180028F3_lost_timecnt = 0xffff;
U16 can_id_180028F4_lost_timecnt = 0xffff;
U16 can_id_1882BBAB_lost_timecnt = 0xffff;
U16 can_id_1002FF1E_lost_timecnt = 0xffff;
U16 can_id_1003FF1E_lost_timecnt = 0xffff;
U16 can_id_1004FF1E_lost_timecnt = 0xffff;
U16 can_id_18FF1103_lost_timecnt = 0xffff;
U16 can_id_1429289B_lost_timecnt = 0xffff;
U16 can_id_142A289B_lost_timecnt = 0xffff;
U16 can_id_18AB9B28_lost_timecnt = 0xffff;
U16 can_id_18FFA017_lost_timecnt = 0xffff;
U16 can_id_1828272B_lost_timecnt = 0xffff;
U16 can_id_142A2895_lost_timecnt = 0xffff;

U16 can_id_7E7_lost_timecnt=0xffff;


extern led_check_t led_check;
/* function declare */
static void can_id_7E7_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_18AA28F3_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_1818D0F3_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_1819D0F3_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_181AD0F3_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_181BD0F3_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_181CD0F3_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_181DD0F3_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_181ED0F3_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_181FD0F3_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_180028F3_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_180028F4_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_1002FF1E_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_1003FF1E_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_1004FF1E_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_1429289B_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_142A289B_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_1828272B_analyse(can_msg_t *msg, can_pro_way_e way);
static void can_id_142A2895_analyse(can_msg_t *msg, can_pro_way_e way);

static void app_can_process(can_msg_t *msg, can_pro_way_e way);

static void app_bound_id(U32 id, U8 dlc, CAN_ID_FORMAT_e format, U8 msg_buf,
		U16 period) {

	if ((msg_buf_num <= ID_PROCESS_ALL) && (msg_buf >= 1)) {
		can_msg[msg_buf_num].id = id;
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
	app_bound_id(PC_T0_MCU_ID, 8, STAND_ID, 1, PERIOD_MS(100));
	app_bound_id(ID_REC_1_18AA28F3, 8, EXTERN_ID, 2, CAN_ID_18AA28F3_PERIOD);
	app_bound_id(ID_REC_2_1818D0F3, 8, EXTERN_ID, 3, CAN_ID_1818D0F3_PERIOD);
	app_bound_id(ID_REC_3_1819D0F3, 8, EXTERN_ID, 4, CAN_ID_1819D0F3_PERIOD);
	app_bound_id(ID_REC_4_181AD0F3, 8, EXTERN_ID, 5, CAN_ID_181AD0F3_PERIOD);
	app_bound_id(ID_REC_5_181BD0F3, 8, EXTERN_ID, 6, CAN_ID_181BD0F3_PERIOD);
	app_bound_id(ID_REC_6_181CD0F3, 8, EXTERN_ID, 7, CAN_ID_181CD0F3_PERIOD);
	app_bound_id(ID_REC_7_181DD0F3, 8, EXTERN_ID, 8, CAN_ID_181DD0F3_PERIOD);
	app_bound_id(ID_REC_8_181ED0F3, 8, EXTERN_ID, 9, CAN_ID_181ED0F3_PERIOD);
	app_bound_id(ID_REC_9_181FD0F3, 8, EXTERN_ID, 10, CAN_ID_181FD0F3_PERIOD);
	app_bound_id(ID_REC_10_180028F3, 8, EXTERN_ID, 11, CAN_ID_180028F3_PERIOD);
	app_bound_id(ID_REC_11_180028F4, 8, EXTERN_ID, 12, CAN_ID_180028F4_PERIOD);
	app_bound_id(ID_REC_12_1002FF1E, 8, EXTERN_ID, 13, CAN_ID_1002FF1E_PERIOD);
	app_bound_id(ID_REC_13_1003FF1E, 8, EXTERN_ID, 14, CAN_ID_1003FF1E_PERIOD);
	app_bound_id(ID_REC_14_1004FF1E, 8, EXTERN_ID, 15, CAN_ID_1004FF1E_PERIOD);
	app_bound_id(ID_REC_15_1429289B, 8, EXTERN_ID, 16, CAN_ID_1429289B_PERIOD);
	app_bound_id(ID_REC_16_142A289B, 8, EXTERN_ID, 17, CAN_ID_142A289B_PERIOD);
	app_bound_id(ID_REC_17_1828272B, 8, EXTERN_ID, 18, CAN_ID_1828272B_PERIOD);
	app_bound_id(ID_REC_18_142A2895, 8, EXTERN_ID, 19, CAN_ID_142A2895_PERIOD);

	/* sent id bound,add your code here */
	app_bound_id(MCU_TO_PC_ID, 8, STAND_ID, (ID_RECV_NUM_ALL + 1),
			PERIOD_MS(100));
	app_bound_id(ID_SEND_1_1800F328, 8, EXTERN_ID, (ID_RECV_NUM_ALL + 2),
			CAN_ID_1800F328_PERIOD);
	app_bound_id(ID_SEND_2_1882BBAB, 8, EXTERN_ID, (ID_RECV_NUM_ALL + 3),
			CAN_ID_1882BBAB_PERIOD);
	app_bound_id(ID_SEND_3_18AB9B28, 8, EXTERN_ID, (ID_RECV_NUM_ALL + 4),
			CAN_ID_18AB9B28_PERIOD);
	app_bound_id(ID_SEND_4_18FFA017, 8, EXTERN_ID, (ID_RECV_NUM_ALL + 4),
			CAN_ID_18FFA017_PERIOD);

	/* register the can id process function */
	can_rx_pro = app_can_process;
	/*register the can id process */
	can_rx_handle[0] = can_id_7E7_analyse;
	can_rx_handle[1] = can_id_18AA28F3_analyse;
	can_rx_handle[2] = can_id_1818D0F3_analyse;
	can_rx_handle[3] = can_id_1819D0F3_analyse;
	can_rx_handle[4] = can_id_181AD0F3_analyse;
	can_rx_handle[5] = can_id_181BD0F3_analyse;
	can_rx_handle[6] = can_id_181CD0F3_analyse;
	can_rx_handle[7] = can_id_181DD0F3_analyse;
	can_rx_handle[8] = can_id_181ED0F3_analyse;
	can_rx_handle[9] = can_id_181FD0F3_analyse;
	can_rx_handle[10] = can_id_180028F3_analyse;
	can_rx_handle[11] = can_id_180028F4_analyse;
	can_rx_handle[12] = can_id_1002FF1E_analyse;
	can_rx_handle[13] = can_id_1003FF1E_analyse;
	can_rx_handle[14] = can_id_1004FF1E_analyse;
	can_rx_handle[15] = can_id_1429289B_analyse;
	can_rx_handle[16] = can_id_142A289B_analyse;
	can_rx_handle[17] = can_id_1828272B_analyse;
	can_rx_handle[18] = can_id_142A2895_analyse;

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

static void can_id_18AA28F3_analyse(can_msg_t *msg, can_pro_way_e way) {
	switch (way) {
	case CAN_PARSE:
		data_copy((U8*) &can0.ID_18AA28F3, (U8*) &msg->data[0], msg->dlc);
		can_id_18AA28F3_lost_timecnt = 0;
		break;
	case CAN_LOST:
		break;
	default:
		break;
	}
}
static void can_id_1818D0F3_analyse(can_msg_t *msg, can_pro_way_e way) {
	switch (way) {
	case CAN_PARSE:
		data_copy((U8*) &can0.ID_1818D0F3, (U8*) &msg->data[0], msg->dlc);
		can_id_1818D0F3_lost_timecnt = 0;
		break;
	case CAN_LOST:
		break;
	default:
		break;
	}
}
static void can_id_1819D0F3_analyse(can_msg_t *msg, can_pro_way_e way) {
	switch (way) {
	case CAN_PARSE:
		data_copy((U8*) &can0.ID_1819D0F3, (U8*) &msg->data[0], msg->dlc);
		can_id_1819D0F3_lost_timecnt = 0;
		break;
	case CAN_LOST:
		break;
	default:
		break;
	}
}
static void can_id_181AD0F3_analyse(can_msg_t *msg, can_pro_way_e way) {
	switch (way) {
	case CAN_PARSE:
		data_copy((U8*) &can0.ID_181AD0F3, (U8*) &msg->data[0], msg->dlc);
		can_id_181AD0F3_lost_timecnt = 0;
		break;
	case CAN_LOST:
		break;
	default:
		break;
	}
}
static void can_id_181BD0F3_analyse(can_msg_t *msg, can_pro_way_e way) {
	switch (way) {
	case CAN_PARSE:
		data_copy((U8*) &can0.ID_181BD0F3, (U8*) &msg->data[0], msg->dlc);
		can_id_181BD0F3_lost_timecnt = 0;
		break;
	case CAN_LOST:
		break;
	default:
		break;
	}
}
static void can_id_181CD0F3_analyse(can_msg_t *msg, can_pro_way_e way) {
	switch (way) {
	case CAN_PARSE:
		data_copy((U8*) &can0.ID_181CD0F3, (U8*) &msg->data[0], msg->dlc);
		can_id_181CD0F3_lost_timecnt = 0;
		break;
	case CAN_LOST:
		break;
	default:
		break;
	}
}
static void can_id_181DD0F3_analyse(can_msg_t *msg, can_pro_way_e way) {
	switch (way) {
	case CAN_PARSE:
		data_copy((U8*) &can0.ID_181DD0F3, (U8*) &msg->data[0], msg->dlc);
		can_id_181DD0F3_lost_timecnt = 0;
		break;
	case CAN_LOST:
		break;
	default:
		break;
	}
}
static void can_id_181ED0F3_analyse(can_msg_t *msg, can_pro_way_e way) {
	switch (way) {
	case CAN_PARSE:
		data_copy((U8*) &can0.ID_181ED0F3, (U8*) &msg->data[0], msg->dlc);
		can_id_181ED0F3_lost_timecnt = 0;
		break;
	case CAN_LOST:
		break;
	default:
		break;
	}
}
static void can_id_181FD0F3_analyse(can_msg_t *msg, can_pro_way_e way) {
	switch (way) {
	case CAN_PARSE:
		data_copy((U8*) &can0.ID_181FD0F3, (U8*) &msg->data[0], msg->dlc);
		can_id_181FD0F3_lost_timecnt = 0;
		break;
	case CAN_LOST:
		break;
	default:
		break;
	}
}
static void can_id_180028F3_analyse(can_msg_t *msg, can_pro_way_e way) {
	switch (way) {
	case CAN_PARSE:
		data_copy((U8*) &can0.ID_180028F3, (U8*) &msg->data[0], msg->dlc);
		can_id_180028F3_lost_timecnt = 0;
		break;
	case CAN_LOST:
		break;
	default:
		break;
	}
}
static void can_id_180028F4_analyse(can_msg_t *msg, can_pro_way_e way) {
	switch (way) {
	case CAN_PARSE:
		data_copy((U8*) &can0.ID_180028F4, (U8*) &msg->data[0], msg->dlc);
		can_id_180028F4_lost_timecnt = 0;
		break;
	case CAN_LOST:
		break;
	default:
		break;
	}
}
static void can_id_1002FF1E_analyse(can_msg_t *msg, can_pro_way_e way) {
	switch (way) {
	case CAN_PARSE:
		data_copy((U8*) &can0.ID_1002FF1E, (U8*) &msg->data[0], msg->dlc);
		can_id_1002FF1E_lost_timecnt = 0;
		break;
	case CAN_LOST:
		break;
	default:
		break;
	}
}
static void can_id_1003FF1E_analyse(can_msg_t *msg, can_pro_way_e way) {
	switch (way) {
	case CAN_PARSE:
		data_copy((U8*) &can0.ID_1003FF1E, (U8*) &msg->data[0], msg->dlc);
		can_id_1003FF1E_lost_timecnt = 0;
		break;
	case CAN_LOST:
		break;
	default:
		break;
	}
}
static void can_id_1004FF1E_analyse(can_msg_t *msg, can_pro_way_e way) {
	switch (way) {
	case CAN_PARSE:
		data_copy((U8*) &can0.ID_1004FF1E, (U8*) &msg->data[0], msg->dlc);
		can_id_1004FF1E_lost_timecnt = 0;
		break;
	case CAN_LOST:
		break;
	default:
		break;
	}
}
static void can_id_1429289B_analyse(can_msg_t *msg, can_pro_way_e way) {
	switch (way) {
	case CAN_PARSE:
		data_copy((U8*) &can0.ID_1429289B, (U8*) &msg->data[0], msg->dlc);
		can_id_1429289B_lost_timecnt = 0;
		break;
	case CAN_LOST:
		break;
	default:
		break;
	}
}
static void can_id_142A289B_analyse(can_msg_t *msg, can_pro_way_e way) {
	switch (way) {
	case CAN_PARSE:
		data_copy((U8*) &can0.ID_142A289B, (U8*) &msg->data[0], msg->dlc);
		can_id_142A289B_lost_timecnt = 0;
		break;
	case CAN_LOST:
		break;
	default:
		break;
	}
}
static void can_id_1828272B_analyse(can_msg_t *msg, can_pro_way_e way) {
	switch (way) {
	case CAN_PARSE:
		data_copy((U8*) &can0.ID_1828272B, (U8*) &msg->data[0], msg->dlc);
		can_id_1828272B_lost_timecnt = 0;
		break;
	case CAN_LOST:
		break;
	default:
		break;
	}
}
static void can_id_142A2895_analyse(can_msg_t *msg, can_pro_way_e way) {
	switch (way) {
	case CAN_PARSE:
		data_copy((U8*) &can0.ID_142A2895, (U8*) &msg->data[0], msg->dlc);
		can_id_142A2895_lost_timecnt = 0;
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
		can_id_18AA28F3_lost_timecnt = 0xffff;
		can_id_1818D0F3_lost_timecnt = 0xffff;
		can_id_1819D0F3_lost_timecnt = 0xffff;
		can_id_181AD0F3_lost_timecnt = 0xffff;
		can_id_181BD0F3_lost_timecnt = 0xffff;
		can_id_181CD0F3_lost_timecnt = 0xffff;
		can_id_181DD0F3_lost_timecnt = 0xffff;
		can_id_181ED0F3_lost_timecnt = 0xffff;
		can_id_181FD0F3_lost_timecnt = 0xffff;
		can_id_1800F328_lost_timecnt = 0xffff;
		can_id_180028F3_lost_timecnt = 0xffff;
		can_id_180028F4_lost_timecnt = 0xffff;
		can_id_1882BBAB_lost_timecnt = 0xffff;
		can_id_1002FF1E_lost_timecnt = 0xffff;
		can_id_1003FF1E_lost_timecnt = 0xffff;
		can_id_1004FF1E_lost_timecnt = 0xffff;
		can_id_18FF1103_lost_timecnt = 0xffff;
		can_id_1429289B_lost_timecnt = 0xffff;
		can_id_142A289B_lost_timecnt = 0xffff;
		can_id_18AB9B28_lost_timecnt = 0xffff;
		can_id_18FFA017_lost_timecnt = 0xffff;
		can_id_1828272B_lost_timecnt = 0xffff;
		can_id_142A2895_lost_timecnt = 0xffff;

		memset(&can0, 0, sizeof(can0));

		g_u8IgnSts_bak = g_u8IgnSts;
	} else if ((OFF == g_u8IgnSts) && (ON == g_u8IgnSts_bak)) {
		can_id_18AA28F3_lost_timecnt = 0;
		can_id_1818D0F3_lost_timecnt = 0;
		can_id_1819D0F3_lost_timecnt = 0;
		can_id_181AD0F3_lost_timecnt = 0;
		can_id_181BD0F3_lost_timecnt = 0;
		can_id_181CD0F3_lost_timecnt = 0;
		can_id_181DD0F3_lost_timecnt = 0;
		can_id_181ED0F3_lost_timecnt = 0;
		can_id_181FD0F3_lost_timecnt = 0;
		can_id_1800F328_lost_timecnt = 0;
		can_id_180028F3_lost_timecnt = 0;
		can_id_180028F4_lost_timecnt = 0;
		can_id_1882BBAB_lost_timecnt = 0;
		can_id_1002FF1E_lost_timecnt = 0;
		can_id_1003FF1E_lost_timecnt = 0;
		can_id_1004FF1E_lost_timecnt = 0;
		can_id_18FF1103_lost_timecnt = 0;
		can_id_1429289B_lost_timecnt = 0;
		can_id_142A289B_lost_timecnt = 0;
		can_id_18AB9B28_lost_timecnt = 0;
		can_id_18FFA017_lost_timecnt = 0;
		can_id_1828272B_lost_timecnt = 0;
		can_id_142A2895_lost_timecnt = 0;

		memset(&can0, 0, sizeof(can0));

		g_u8IgnSts_bak = g_u8IgnSts;
	} else {
		if (can_id_18AA28F3_lost_timecnt < 0xffff) {
			can_id_18AA28F3_lost_timecnt++;
		}
		if (can_id_1818D0F3_lost_timecnt < 0xffff) {
			can_id_1818D0F3_lost_timecnt++;
		}
		if (can_id_1819D0F3_lost_timecnt < 0xffff) {
			can_id_1819D0F3_lost_timecnt++;
		}
		if (can_id_181AD0F3_lost_timecnt < 0xffff) {
			can_id_181AD0F3_lost_timecnt++;
		}
		if (can_id_181BD0F3_lost_timecnt < 0xffff) {
			can_id_181BD0F3_lost_timecnt++;
		}
		if (can_id_181CD0F3_lost_timecnt < 0xffff) {
			can_id_181CD0F3_lost_timecnt++;
		}
		if (can_id_181DD0F3_lost_timecnt < 0xffff) {
			can_id_181DD0F3_lost_timecnt++;
		}
		if (can_id_181ED0F3_lost_timecnt < 0xffff) {
			can_id_181ED0F3_lost_timecnt++;
		}
		if (can_id_181FD0F3_lost_timecnt < 0xffff) {
			can_id_181FD0F3_lost_timecnt++;
		}
		if (can_id_1800F328_lost_timecnt < 0xffff) {
			can_id_1800F328_lost_timecnt++;
		}
		if (can_id_180028F3_lost_timecnt < 0xffff) {
			can_id_180028F3_lost_timecnt++;
		}
		if (can_id_180028F4_lost_timecnt < 0xffff) {
			can_id_180028F4_lost_timecnt++;
		}
		if (can_id_1882BBAB_lost_timecnt < 0xffff) {
			can_id_1882BBAB_lost_timecnt++;
		}
		if (can_id_1002FF1E_lost_timecnt < 0xffff) {
			can_id_1002FF1E_lost_timecnt++;
		}
		if (can_id_1003FF1E_lost_timecnt < 0xffff) {
			can_id_1003FF1E_lost_timecnt++;
		}
		if (can_id_1004FF1E_lost_timecnt < 0xffff) {
			can_id_1004FF1E_lost_timecnt++;
		}
		if (can_id_18FF1103_lost_timecnt < 0xffff) {
			can_id_18FF1103_lost_timecnt++;
		}
		if (can_id_1429289B_lost_timecnt < 0xffff) {
			can_id_1429289B_lost_timecnt++;
		}
		if (can_id_142A289B_lost_timecnt < 0xffff) {
			can_id_142A289B_lost_timecnt++;
		}
		if (can_id_18AB9B28_lost_timecnt < 0xffff) {
			can_id_18AB9B28_lost_timecnt++;
		}
		if (can_id_18FFA017_lost_timecnt < 0xffff) {
			can_id_18FFA017_lost_timecnt++;
		}
		if (can_id_1828272B_lost_timecnt < 0xffff) {
			can_id_1828272B_lost_timecnt++;
		}
		if (can_id_142A2895_lost_timecnt < 0xffff) {
			can_id_142A2895_lost_timecnt++;
		}
	}
}
