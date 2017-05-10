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
can_msg_t can_msg[ID_PROCESS_ALL]={0};
static pCanAnalyse can_rx_handle[ID_RECV_NUM_ALL] = {NULL};
 U32      can_rx_msg_id_ads[ID_RECV_NUM_ALL] = {0};

car0_can_signal_t can0={0};
car1_can_signal_t can1={0};
car2_can_signal_t can2={0};
car3_can_signal_t can3={0};
car4_can_signal_t can4={0};
car5_can_signal_t can5={0};
//UDS_id_766  uds766;

U16 can_id_095_lost_timecnt=0xffff;
U16 can_id_111_lost_timecnt=0xffff;
U16 can_id_131_lost_timecnt=0xffff;
U16 can_id_265_lost_timecnt=0xffff;
U16 can_id_271_lost_timecnt=0xffff;
U16 can_id_311_lost_timecnt=0xffff;
U16 can_id_351_lost_timecnt=0xffff;
U16 can_id_371_lost_timecnt=0xffff;
U16 can_id_7E7_lost_timecnt=0xffff;
U16 can_id_3df_lost_timecnt=0xffff;
U16 can_id_345_lost_timecnt=0xffff;
U16 can_id_221_lost_timecnt=0xffff;
U16 can_id_7a6_lost_timecnt=0xffff;
U16 can_id_251_lost_timecnt=0xffff;
U16 can_id_116_lost_timecnt=0xffff;
U16	can_id_211_lost_timecnt=0xffff;
U16	can_id_242_lost_timecnt=0xffff;
U16	can_id_0A5_lost_timecnt=0xffff;
U16	can_id_365_lost_timecnt=0xffff;
U16	can_id_08B_lost_timecnt=0xffff;
U16	can_id_485_lost_timecnt=0xffff;

extern led_check_t  led_check;
/* function declare */
static void  app_can_process(can_msg_t *msg,can_pro_way_e way);
static void  can_id_7E7_analyse(can_msg_t *msg,can_pro_way_e way);
static void  can_id_265_analyse(can_msg_t *msg,can_pro_way_e way);
static void  can_id_111_analyse(can_msg_t *msg,can_pro_way_e way);
static void  can_id_271_analyse(can_msg_t *msg,can_pro_way_e way);
static void  can_id_131_analyse(can_msg_t *msg,can_pro_way_e way);
static void  can_id_095_analyse(can_msg_t *msg,can_pro_way_e way);
static void  can_id_311_analyse(can_msg_t *msg,can_pro_way_e way);
static void  can_id_371_analyse(can_msg_t *msg,can_pro_way_e way);
static void  can_id_351_analyse(can_msg_t *msg,can_pro_way_e way);
static void  can_id_3df_analyse(can_msg_t *msg,can_pro_way_e way);
static void  can_id_345_analyse(can_msg_t *msg,can_pro_way_e way);
static void  can_id_221_analyse(can_msg_t *msg,can_pro_way_e way);
static void  can_id_7A6_analyse(can_msg_t *msg,can_pro_way_e way);
static void  can_id_251_analyse(can_msg_t *msg,can_pro_way_e way);
static void  can_id_116_analyse(can_msg_t *msg,can_pro_way_e way);
static void  can_id_211_analyse(can_msg_t *msg,can_pro_way_e way);
static void  can_id_242_analyse(can_msg_t *msg,can_pro_way_e way);
static void  can_id_0A5_analyse(can_msg_t *msg,can_pro_way_e way);
static void  can_id_766_analyse(can_msg_t *msg,can_pro_way_e way);
static void  can_id_365_analyse(can_msg_t *msg,can_pro_way_e way);
static void  can_id_08B_analyse(can_msg_t *msg,can_pro_way_e way);
static void  can_id_485_analyse(can_msg_t *msg,can_pro_way_e way);



static void app_bound_id(U32 id,U8 dlc,CAN_ID_FORMAT_e  format,U8 msg_buf,U16 period)
{

	if((msg_buf_num <= ID_PROCESS_ALL)&&(msg_buf >= 1))
	{
		can_msg[msg_buf_num].id =  id;
		can_msg[msg_buf_num].dlc = dlc;
		can_msg[msg_buf_num].format = format;
		can_msg[msg_buf_num].buffer_num = msg_buf;
		if(period >= PERIOD_MS(1000))
		{
			can_msg[msg_buf_num].period_ms= period>>1;  
		}
		else
		{
			can_msg[msg_buf_num].period_ms= period;  
		}
	
		msg_buf_num++;
	}
}


void app_init_variable(void)
{
	U8 i =0;
	for(i = 0; i < ID_RECV_NUM_ALL; i++)
	{
		memset(&can_msg[i],0,sizeof(can_msg_t));
		can_msg[i].lost = 1;
	}
	msg_buf_num = 0;
}

void app_init_can(void)
{
	mid_can_prepare(ID_RECV_NUM_ALL, ID_SENT_NUM_ALL,can_msg);

	app_init_variable();
	can_chip_mode_switch(&io_can_stb, &io_can_en, NORMAL_MODE);
	/* receive id bound,add your code here,the id need to be received */
	app_bound_id(PC_T0_MCU_ID,8,STAND_ID,1,PERIOD_MS(100));
	app_bound_id(ID_REC_1_265,8,STAND_ID,2,PERIOD_MS(20));
	app_bound_id(ID_REC_2_111,8,STAND_ID,3,PERIOD_MS(20));
	app_bound_id(ID_REC_3_271,8,STAND_ID,4,PERIOD_MS(20));
	app_bound_id(ID_REC_4_131,8,STAND_ID,5,PERIOD_MS(10));
	app_bound_id(ID_REC_5_095,8,STAND_ID,6,PERIOD_MS(500));
	app_bound_id(ID_REC_6_311,8,STAND_ID,7,PERIOD_MS(50));
	app_bound_id(ID_REC_7_371,8,STAND_ID,8,PERIOD_MS(20));
	app_bound_id(ID_REC_8_351,8,STAND_ID,9,PERIOD_MS(100));
	app_bound_id(ID_REC_9_3DF,8,STAND_ID,10,PERIOD_MS(100));
	app_bound_id(ID_REC_10_345,8,STAND_ID,11,PERIOD_MS(50));
	app_bound_id(ID_REC_11_221,8,STAND_ID,12,PERIOD_MS(50));
	app_bound_id(ID_REC_12_7A6,8,STAND_ID,13,PERIOD_MS(100));
	app_bound_id(ID_REC_13_251,8,STAND_ID,14,PERIOD_MS(100));
	app_bound_id(ID_REC_14_116,8,STAND_ID,15,PERIOD_MS(100));
	app_bound_id(ID_REC_15_211,8,STAND_ID,16,PERIOD_MS(100));
	app_bound_id(ID_REC_16_242,8,STAND_ID,17,PERIOD_MS(200));
	app_bound_id(ID_REC_17_0A5,8,STAND_ID,18,PERIOD_MS(200));
	app_bound_id(ID_REC_18_766,8,STAND_ID,19,PERIOD_MS(200));
	app_bound_id(ID_REC_19_365,8,STAND_ID,20,PERIOD_MS(200));
	app_bound_id(ID_REC_20_08B,8,STAND_ID,21,PERIOD_MS(500));
	app_bound_id(ID_REC_21_485,8,STAND_ID,22,PERIOD_MS(500));

	/* sent id bound,add your code here */
	app_bound_id(MCU_TO_PC_ID,8,STAND_ID,(ID_RECV_NUM_ALL+1),PERIOD_MS(100));
	app_bound_id(ID_SEND_1_345,8,STAND_ID,(ID_RECV_NUM_ALL+2),PERIOD_MS(50));
	app_bound_id(ID_SEND_2_766,8,STAND_ID,(ID_RECV_NUM_ALL+3),PERIOD_MS(100));
	app_bound_id(ID_SEND_3_7A6,8,STAND_ID,(ID_RECV_NUM_ALL+4),PERIOD_MS(500));
	
	/* register the can id process function */
	can_rx_pro = app_can_process;
	/*register the can id process */
	can_rx_handle[0] = can_id_7E7_analyse;
	can_rx_handle[1] = can_id_265_analyse;
	can_rx_handle[2] = can_id_111_analyse;
	can_rx_handle[3] = can_id_271_analyse;
	can_rx_handle[4] = can_id_131_analyse;
	can_rx_handle[5] = can_id_095_analyse;
	can_rx_handle[6] = can_id_311_analyse;
	can_rx_handle[7] = can_id_371_analyse;
	can_rx_handle[8] = can_id_351_analyse;
	can_rx_handle[9] = can_id_3df_analyse;
	can_rx_handle[10] = can_id_345_analyse;
	can_rx_handle[11] = can_id_221_analyse;
	can_rx_handle[12] = can_id_7A6_analyse;
	can_rx_handle[13] = can_id_251_analyse;
	can_rx_handle[14] = can_id_116_analyse;
	can_rx_handle[15] = can_id_211_analyse;
	can_rx_handle[16] = can_id_242_analyse;
	can_rx_handle[17] = can_id_0A5_analyse;
	can_rx_handle[18] = can_id_766_analyse;
	can_rx_handle[19] = can_id_365_analyse;
	can_rx_handle[20] = can_id_08B_analyse;
	can_rx_handle[21] = can_id_485_analyse;
	mid_can_init(CAN_CHN, CAN_CHIP);

}

static void app_can_process(can_msg_t *msg,can_pro_way_e way)
{
	if(msg->buffer_num <= ID_RECV_NUM_ALL)
	{
		if(can_rx_handle[msg->buffer_num-1] != NULL)
		{
			can_rx_handle[msg->buffer_num-1](msg,way);
		}
	}
}
void can_id_7E7_analyse(can_msg_t *msg,can_pro_way_e way)
{
	switch(way)
	{
			case CAN_PARSE:
				if (memcmp(msg->data, "RESETMCU", 8) == 0)
				{
					dbg_string("Firmware upgrade request from CAN. rebooting...\n");
					app_soft_reset();		/* MCU reset */
				}
				else
				if (memcmp(msg->data, "UPSTART", 7) == 0)
				{
					if(OFF == g_u8IgnSts)
					{
						mid_set_io_sts(&IO_MCU_CTL_START_UP,ON);
						g_u8_A20_update_sts = 1;
					}
				}
				//dbg_string("CAN CMD:%s\n", msg->data);
			 	can_id_7E7_lost_timecnt=0;
			 break;
		case CAN_LOST:
			
			break;
		default:
			break;
	}
}

static void  can_id_311_analyse(can_msg_t *msg,can_pro_way_e way)
{
	switch(way)
	{
		case CAN_PARSE:
			switch(IPconfig.protocol)
			{
				case HAVAL_2013_2P4L_AT_2WD:
					//dbg_string("HAVAL_2013_2P4L_AT_2WD\n");
					data_copy((U8*)&can0.id_311,(U8*)&msg->data[0],msg->dlc);
					break;
				case HAVAL_2013_1P5T_MT_2WD:
					//dbg_string("HAVAL_2013_1P5T_MT_2WD\n");
					data_copy((U8*)&can1.id_311,(U8*)&msg->data[0],msg->dlc);
					break;
				case HAVAL_2012_1P5T_MT_2WD:
					//dbg_string("HAVAL_2012_1P5T_MT_2WD:\n");
					data_copy((U8*)&can2.id_311,(U8*)&msg->data[0],msg->dlc);
					break;
				case HAVAL_2015_1P5T_MT_2WD: 
				case HAVAL_2015_1P5T_6AT_2WD:
					//dbg_string("HAVAL_2015_1P5T\n");
					data_copy((U8*)&can3.id_311,(U8*)&msg->data[0],msg->dlc);
					break;
				case HAVAL_2011_2P0T_MT_2WD:
					//dbg_string("HAVAL_2011_2P0T_MT_2WD\n");
					data_copy((U8*)&can4.id_311,(U8*)&msg->data[0],msg->dlc);
					break;
				case HAVAL_2013_1P5T_MT_4WD:
					//dbg_string("HAVAL_2013_1P5T_MT_4WD\n");
					data_copy((U8*)&can5.id_311,(U8*)&msg->data[0],msg->dlc);
					break;
			}
			
			can_id_311_lost_timecnt=0;
			break;
		case CAN_LOST:
			//memset(&can.id_311,0,msg->dlc);
			break;
		default:break;
	}
}

static void  can_id_371_analyse(can_msg_t *msg,can_pro_way_e way)
{
	U16 temp=0;
	U32 this_trip_fuel = 0;
	
	switch(way)
	{
		case CAN_PARSE:
			switch(IPconfig.protocol)
			{
				case HAVAL_2013_2P4L_AT_2WD:
					data_copy((U8*)&can0.id_371,(U8*)&msg->data[0],msg->dlc);
					FuelSigTn=can0.id_371.ECM_Fuelconsumption;
					if(FuelSigTn_bak==0)
					{
						
					}
					else
					{
						if((FuelSigTn-FuelSigTn_bak)<0)
						{
							temp=FuelSigTn-FuelSigTn_bak+65536;
						}
						else
						{
							temp=FuelSigTn-FuelSigTn_bak;
						}
						
						if(temp>3000)//TBD
						{
							temp=3000;
						}
						this_trip_fuel = get_this_trip_used_fuel()+ (temp<<1);
						set_this_trip_used_fuel(this_trip_fuel);
						
						//此处左移一位的作用是*2；因为油耗信息值的单位是2uL
						AFE_fuel_used+=(temp<<1);
						s_u32_instance_accumulation+=(temp<<1);
					}
					
					FuelSigTn_bak=FuelSigTn;
			
					break;
				case HAVAL_2013_1P5T_MT_2WD:
					data_copy((U8*)&can1.id_371,(U8*)&msg->data[0],msg->dlc);
					
					app_set_temp(can1.id_371.ECM_EngCoolantTemp);
					break;
				case HAVAL_2012_1P5T_MT_2WD:
					data_copy((U8*)&can2.id_371,(U8*)&msg->data[0],msg->dlc);
					
					app_set_temp(can2.id_371.ECM_EngCoolantTemp);
					break;
				case HAVAL_2015_1P5T_MT_2WD: 
				case HAVAL_2015_1P5T_6AT_2WD:
					data_copy((U8*)&can3.id_371,(U8*)&msg->data[0],msg->dlc);
					
					if(can3.id_371.ECM_EngCoolantTempValid)
					{
						app_set_temp(can3.id_371.ECM_EngCoolantTemp);
					}
					else
					{
						app_set_temp(0xff);
					}
					break;
				case HAVAL_2011_2P0T_MT_2WD:
					break;
				case HAVAL_2013_1P5T_MT_4WD:
					data_copy((U8*)&can5.id_371,(U8*)&msg->data[0],msg->dlc);
					
					app_set_temp(can5.id_371.ECM_EngCoolantTemp);
					break;
			}

			can_id_371_lost_timecnt=0;
			break;
		case CAN_LOST:
			//memset(&can.id_371,0,msg->dlc);
			 
			 break;
		default:break;
	}
}

static void  can_id_351_analyse(can_msg_t *msg,can_pro_way_e way)
{
	switch(way)
	{
		case CAN_PARSE:
			switch(IPconfig.protocol)
			{
				case HAVAL_2013_2P4L_AT_2WD:
					data_copy((U8*)&can0.id_351,(U8*)&msg->data[0],msg->dlc);
					break;
				case HAVAL_2013_1P5T_MT_2WD:
					data_copy((U8*)&can1.id_351,(U8*)&msg->data[0],msg->dlc);
					break;
				case HAVAL_2012_1P5T_MT_2WD:
					data_copy((U8*)&can2.id_351,(U8*)&msg->data[0],msg->dlc);
					break;
				case HAVAL_2015_1P5T_MT_2WD: 
				case HAVAL_2015_1P5T_6AT_2WD:
					data_copy((U8*)&can3.id_351,(U8*)&msg->data[0],msg->dlc);
					break;
				case HAVAL_2011_2P0T_MT_2WD:
					data_copy((U8*)&can4.id_351,(U8*)&msg->data[0],msg->dlc);
					break;
				case HAVAL_2013_1P5T_MT_4WD:
					data_copy((U8*)&can5.id_351,(U8*)&msg->data[0],msg->dlc);
					break;
			}
			 
			 can_id_351_lost_timecnt=0;
			 break;
		case CAN_LOST:
			//memset(&can.id_351,0,msg->dlc);
			 
			 break;
		default:break;
	}
}

static void  can_id_265_analyse(can_msg_t *msg,can_pro_way_e way)
{
	switch(way)
	{
		case CAN_PARSE:
			switch(IPconfig.protocol)
			{
				case HAVAL_2013_2P4L_AT_2WD:
					data_copy((U8*)&can0.id_265,(U8*)&msg->data[0],msg->dlc);
					
					if(can0.id_265.ABS_VehicleSpeedValidSignal)//当车速信号有效时，才获取车速
					{
						app_set_spd(can0.id_265.ABS_VehicleSpeed);
					}
					else
					{
						app_set_spd(0);
					}
					break;
				case HAVAL_2013_1P5T_MT_2WD:
					data_copy((U8*)&can1.id_265,(U8*)&msg->data[0],msg->dlc);
					
					if(can1.id_265.ABS_VehicleSpeedValidSignal)//当车速信号有效时，才获取车速
					{
						app_set_spd(can1.id_265.ABS_VehicleSpeed);
					}
					else
					{
						app_set_spd(0);
					}
					break;
				case HAVAL_2012_1P5T_MT_2WD:
					data_copy((U8*)&can2.id_265,(U8*)&msg->data[0],msg->dlc);
					
					if(can2.id_265.ABS_VehicleSpeedValidSignal)//当车速信号有效时，才获取车速
					{
						app_set_spd(can2.id_265.ABS_VehicleSpeed);
					}
					else
					{
						app_set_spd(0);
					}
					break;
				case HAVAL_2015_1P5T_MT_2WD: 
				case HAVAL_2015_1P5T_6AT_2WD:
					data_copy((U8*)&can3.id_265,(U8*)&msg->data[0],msg->dlc);
					
					if(can3.id_265.ABS_VehicleSpeedValidSignal)//当车速信号有效时，才获取车速
					{
						app_set_spd(can3.id_265.ABS_VehicleSpeed);
					}
					else
					{
						app_set_spd(0);
					}
					break;
				case HAVAL_2011_2P0T_MT_2WD:
					data_copy((U8*)&can4.id_265,(U8*)&msg->data[0],msg->dlc);
					
					if(can4.id_265.ABS_VehicleSpeedValidSignal)//当车速信号有效时，才获取车速
					{
						app_set_spd(can4.id_265.ABS_VehicleSpeed);
					}
					else
					{
						app_set_spd(0);
					}
					break;
				case HAVAL_2013_1P5T_MT_4WD:
					data_copy((U8*)&can5.id_265,(U8*)&msg->data[0],msg->dlc);
					
					if(can5.id_265.ABS_VehicleSpeedValidSignal)//当车速信号有效时，才获取车速
					{
						app_set_spd(can5.id_265.ABS_VehicleSpeed);
					}
					else
					{
						app_set_spd(0);
					}
					break;
			}

			can_id_265_lost_timecnt=0;
			break;
		case CAN_LOST:
			//memset(&can.id_265,0,msg->dlc);
			//app_set_spd(can.id_265.ABS_VehicleSpeed);//CAN丢失时，车速为0
			 
			 break;
		default:break;
	}
}
 
static void  can_id_111_analyse(can_msg_t *msg,can_pro_way_e way)
{
	switch(way)
	{
		case CAN_PARSE:
			switch(IPconfig.protocol)
			{
				case HAVAL_2013_2P4L_AT_2WD:
					data_copy((U8*)&can0.id_111,(U8*)&msg->data[0],msg->dlc);
					if(can0.id_111.ECM_EngSpdFailure==0)
			 		{
			 				app_set_rpm(can0.id_111.ECM_EngSpd);
			 		}
			 		else
			 		{
			 				app_set_rpm(0);
			 		}
					break;
				case HAVAL_2013_1P5T_MT_2WD:
					data_copy((U8*)&can1.id_111,(U8*)&msg->data[0],msg->dlc);
					
					app_set_rpm(can1.id_111.ECM_EngSpd);
					break;
				case HAVAL_2012_1P5T_MT_2WD:
					data_copy((U8*)&can2.id_111,(U8*)&msg->data[0],msg->dlc);
					
					app_set_rpm(can2.id_111.ECM_EngSpd);
					break;
				case HAVAL_2015_1P5T_MT_2WD: 
				case HAVAL_2015_1P5T_6AT_2WD:
					data_copy((U8*)&can3.id_111,(U8*)&msg->data[0],msg->dlc);
					
					if(can3.id_111.ECM_EngSpdValid)
					{
						app_set_rpm(can3.id_111.ECM_EngSpd);
					}
					else
					{
						app_set_rpm(0);
					}
					break;
				case HAVAL_2011_2P0T_MT_2WD:
					//data_copy((U8*)&can4.id_111,(U8*)&msg->data[0],msg->dlc);
					
					//app_set_rpm(can4.id_111.ECM_EngSpd);//2015/10/10
					break;
				case HAVAL_2013_1P5T_MT_4WD:
					data_copy((U8*)&can5.id_111,(U8*)&msg->data[0],msg->dlc);
					app_set_rpm(can5.id_111.ECM_EngSpd);
					
					break;
			}
			can_id_111_lost_timecnt=0;
			break;
		case CAN_LOST:
			//memset(&can.id_111,0,msg->dlc);
			//pp_set_rpm(can.id_111.ECM_EngSpd);//CAN丢失时，转速为0
			
			break;
		default:
			break;
	}
}

static void  can_id_271_analyse(can_msg_t *msg,can_pro_way_e way)
{
	uint16_t temp = 0;
	U32 this_trip_fuel = 0;
	
	switch(way)
	{
		case CAN_PARSE:
			switch(IPconfig.protocol)
			{
				case HAVAL_2013_2P4L_AT_2WD:
					data_copy((U8*)&can0.id_271,(U8*)&msg->data[0],msg->dlc);
					
					temp=can0.id_271.ECM_EngCoolantTemp;
					
					if(temp!=0xff)
			 		{
			 			app_set_temp(temp);
			 		}
			 		else//无效值
			 		{
			 			app_set_temp(0xff);//水温报警
			 		}
					break;
				case HAVAL_2013_1P5T_MT_2WD:
					data_copy((U8*)&can1.id_271,(U8*)&msg->data[0],msg->dlc);
					
					FuelSigTn=can1.id_271.ECM_Fuelconsumption;
					if(FuelSigTn_bak==0)
					{
						
					}
					else
					{
						if((FuelSigTn-FuelSigTn_bak)<0)
						{
							temp=FuelSigTn-FuelSigTn_bak+65536;
						}
						else
						{
							temp=FuelSigTn-FuelSigTn_bak;
						}
						
						if(temp>3000)//TBD
						{
							temp=3000;
						}
						
						this_trip_fuel = get_this_trip_used_fuel()+ (temp<<1);
						set_this_trip_used_fuel(this_trip_fuel);
						
						//此处左移一位的作用是*2；因为油耗信息值的单位是2uL
						AFE_fuel_used+=(temp<<1);
						s_u32_instance_accumulation+=(temp<<1);
					}
					
					FuelSigTn_bak=FuelSigTn;
				
					break;
				case HAVAL_2012_1P5T_MT_2WD:
					data_copy((U8*)&can2.id_271,(U8*)&msg->data[0],msg->dlc);
					
					FuelSigTn=can2.id_271.ECM_Fuelconsumption;
					if(FuelSigTn_bak==0)
					{
						
					}
					else
					{
						if((FuelSigTn-FuelSigTn_bak)<0)
						{
							temp=FuelSigTn-FuelSigTn_bak+65536;
						}
						else
						{
							temp=FuelSigTn-FuelSigTn_bak;
						}
						
						if(temp>3000)//TBD
						{
							temp=3000;
						}
						this_trip_fuel = get_this_trip_used_fuel()+ (temp<<1);
						set_this_trip_used_fuel(this_trip_fuel);
						//此处左移一位的作用是*2；因为油耗信息值的单位是2uL
						AFE_fuel_used+=(temp<<1);
						s_u32_instance_accumulation+=(temp<<1);
					}
					
					FuelSigTn_bak=FuelSigTn;
					break;
				case HAVAL_2015_1P5T_MT_2WD: 
				case HAVAL_2015_1P5T_6AT_2WD:
					data_copy((U8*)&can3.id_271,(U8*)&msg->data[0],msg->dlc);
					
					FuelSigTn=can3.id_271.ECM_Fuelconsumption;
					if(FuelSigTn_bak==0)
					{
						
					}
					else
					{
						if((FuelSigTn-FuelSigTn_bak)<0)
						{
							temp=FuelSigTn-FuelSigTn_bak+65536;
						}
						else
						{
							temp=FuelSigTn-FuelSigTn_bak;
						}
						
						if(temp>3000)//TBD
						{
							temp=3000;
						}
						
						//test
						//temp = 50;
						
						this_trip_fuel = get_this_trip_used_fuel()+ (temp<<1);
						set_this_trip_used_fuel(this_trip_fuel);
						//此处左移一位的作用是*2；因为油耗信息值的单位是2uL
						AFE_fuel_used+=(temp<<1);
						s_u32_instance_accumulation+=(temp<<1);
					}
					
					FuelSigTn_bak=FuelSigTn;
					break;
				case HAVAL_2011_2P0T_MT_2WD:
					
					break;
				case HAVAL_2013_1P5T_MT_4WD:
					data_copy((U8*)&can5.id_271,(U8*)&msg->data[0],msg->dlc);
					
					FuelSigTn=can5.id_271.ECM_Fuelconsumption;
					if(FuelSigTn_bak==0)
					{
						
					}
					else
					{
						if((FuelSigTn-FuelSigTn_bak)<0)
						{
							temp=FuelSigTn-FuelSigTn_bak+65536;
						}
						else
						{
							temp=FuelSigTn-FuelSigTn_bak;
						}
						
						if(temp>3000)//TBD
						{
							temp=3000;
						}
						this_trip_fuel = get_this_trip_used_fuel()+ (temp<<1);
						set_this_trip_used_fuel(this_trip_fuel);
						//此处左移一位的作用是*2；因为油耗信息值的单位是2uL
						AFE_fuel_used+=(temp<<1);
						s_u32_instance_accumulation+=(temp<<1);
					}
					
					FuelSigTn_bak=FuelSigTn;
				
					break;
			}
			 
			 can_id_271_lost_timecnt=0;
			 break;
		case CAN_LOST:
			//memset(&can.id_271,0,msg->dlc);
			//app_set_temp(0xff);//水温报警
			break;
		default:
			break;
	}
}


static void  can_id_131_analyse(can_msg_t *msg,can_pro_way_e way)
{
	switch(way)
	{
		case CAN_PARSE:
			switch(IPconfig.protocol)
			{
				case HAVAL_2013_2P4L_AT_2WD:
					data_copy((U8*)&can0.id_131,(U8*)&msg->data[0],msg->dlc);
					break;
				case HAVAL_2013_1P5T_MT_2WD:
					break;
				case HAVAL_2012_1P5T_MT_2WD:
					break;
				case HAVAL_2015_1P5T_MT_2WD: 
				case HAVAL_2015_1P5T_6AT_2WD:
					break;
				case HAVAL_2011_2P0T_MT_2WD:
					break;
				case HAVAL_2013_1P5T_MT_4WD:
					break;
			}
			 
			can_id_131_lost_timecnt=0;
			break;
		case CAN_LOST:
			break;
		default:
			break;
	}
}

static void  can_id_221_analyse(can_msg_t *msg,can_pro_way_e way)
{
	switch(way)
	{
		case CAN_PARSE:
			switch(IPconfig.protocol)
			{
				case HAVAL_2013_2P4L_AT_2WD:
					break;
				case HAVAL_2013_1P5T_MT_2WD:
					break;
				case HAVAL_2012_1P5T_MT_2WD:
					break;
				case HAVAL_2015_1P5T_MT_2WD: 
				case HAVAL_2015_1P5T_6AT_2WD:
					data_copy((U8*)&can3.id_221,(U8*)&msg->data[0],msg->dlc);
					break;
				case HAVAL_2011_2P0T_MT_2WD:
					break;
				case HAVAL_2013_1P5T_MT_4WD:
					break;
			}

			can_id_221_lost_timecnt=0;
			break;
		case CAN_LOST:
			break;
		default:break;
	}
}

/* 胎压 */
static void  can_id_095_analyse(can_msg_t *msg,can_pro_way_e way)
{
	switch(way)
	{
		case CAN_PARSE:
			switch(IPconfig.protocol)
			{
				case HAVAL_2013_2P4L_AT_2WD:
					data_copy((U8*)&can0.id_095,(U8*)&msg->data[0],msg->dlc);
					break;
				case HAVAL_2013_1P5T_MT_2WD:
					data_copy((U8*)&can1.id_095,(U8*)&msg->data[0],msg->dlc);
					break;
				case HAVAL_2012_1P5T_MT_2WD:
					data_copy((U8*)&can2.id_095,(U8*)&msg->data[0],msg->dlc);
					break;
				case HAVAL_2015_1P5T_MT_2WD: 
				case HAVAL_2015_1P5T_6AT_2WD:
					data_copy((U8*)&can3.id_095,(U8*)&msg->data[0],msg->dlc);
					break;
				case HAVAL_2011_2P0T_MT_2WD:
					data_copy((U8*)&can4.id_095,(U8*)&msg->data[0],msg->dlc);
					break;
				case HAVAL_2013_1P5T_MT_4WD:
					data_copy((U8*)&can5.id_095,(U8*)&msg->data[0],msg->dlc);
					
					break;
			}
			
			 can_id_095_lost_timecnt=0;
			 break;
		case CAN_LOST:
			//memset(&can.id_095,0,msg->dlc);
			 break;
		default:break;
	}
}


static void  can_id_3df_analyse(can_msg_t *msg,can_pro_way_e way) 
{
	switch(way)
	{
		case CAN_PARSE:
			switch(IPconfig.protocol)
			{
				case HAVAL_2013_2P4L_AT_2WD:
					data_copy((U8*)&can0.id_3df,(U8*)&msg->data[0],msg->dlc);
					break;
				case HAVAL_2013_1P5T_MT_2WD:
					data_copy((U8*)&can1.id_3df,(U8*)&msg->data[0],msg->dlc);
					break;
				case HAVAL_2012_1P5T_MT_2WD:
					break;
				case HAVAL_2015_1P5T_MT_2WD: 
				case HAVAL_2015_1P5T_6AT_2WD:
					data_copy((U8*)&can3.id_3df,(U8*)&msg->data[0],msg->dlc);
					break;
				case HAVAL_2011_2P0T_MT_2WD:
					break;
				case HAVAL_2013_1P5T_MT_4WD:
					data_copy((U8*)&can5.id_3df,(U8*)&msg->data[0],msg->dlc);
					break;
			}
			
			 can_id_3df_lost_timecnt=0;
			 break;
		case CAN_LOST:
			//memset(&can.id_3df,0,msg->dlc);
			 break;
		default:break;
	}
}


static void  can_id_345_analyse(can_msg_t *msg,can_pro_way_e way) 
{
	switch(way)
	{
		case CAN_PARSE:
			switch(IPconfig.protocol)
			{
				case HAVAL_2013_2P4L_AT_2WD:
					data_copy((U8*)&can0.id_345,(U8*)&msg->data[0],msg->dlc);
					
					if(can0.id_345.VehTotalMileageValid)
			 		{
			 			IPconfig.Odo=can0.id_345.VehTotalMileage; 
			 		}
			 		else
			 		{
			 			IPconfig.Odo=0;
			 		}
					break;
				case HAVAL_2013_1P5T_MT_2WD:
					data_copy((U8*)&can1.id_345,(U8*)&msg->data[0],msg->dlc);
					
					if(can1.id_345.VehTotalMileageValid)
			 		{
			 			IPconfig.Odo=can1.id_345.VehTotalMileage; 
			 		}
			 		else
			 		{
			 			IPconfig.Odo=0;
			 		}
					break;
				case HAVAL_2012_1P5T_MT_2WD:
					data_copy((U8*)&can2.id_345,(U8*)&msg->data[0],msg->dlc);
					
					if(can2.id_345.VehTotalMileageValid)
			 		{
			 			IPconfig.Odo=can2.id_345.VehTotalMileage; 
			 		}
			 		else
			 		{
			 			IPconfig.Odo=0;
			 		}
					break;
				case HAVAL_2015_1P5T_MT_2WD: 
				case HAVAL_2015_1P5T_6AT_2WD:
					data_copy((U8*)&can3.id_345,(U8*)&msg->data[0],msg->dlc);
					
					if(can3.id_345.VehTotalMileageValid)
			 		{
			 			IPconfig.Odo=can3.id_345.VehTotalMileage; 
			 		}
			 		else
			 		{
			 			IPconfig.Odo=0;
			 		}
					break;
				case HAVAL_2011_2P0T_MT_2WD:
					data_copy((U8*)&can4.id_345,(U8*)&msg->data[0],msg->dlc);
					
					if(can4.id_345.VehTotalMileageValid)
			 		{
			 			IPconfig.Odo=can4.id_345.VehTotalMileage; 
			 		}
			 		else
			 		{
			 			IPconfig.Odo=0;
			 		}
					break;
				case HAVAL_2013_1P5T_MT_4WD:
					data_copy((U8*)&can5.id_345,(U8*)&msg->data[0],msg->dlc);
					
					if(can5.id_345.VehTotalMileageValid)
			 		{
			 			IPconfig.Odo=can5.id_345.VehTotalMileage; 
			 		}
			 		else
			 		{
			 			IPconfig.Odo=0;
			 		}
					break;
			}		
			
			 can_id_345_lost_timecnt=0;
			 break;
		case CAN_LOST:
			//memset(&can.id_345,0,msg->dlc);
			 break;
		default:break;
	}
}


static void  can_id_7A6_analyse(can_msg_t *msg,can_pro_way_e way) 
{
/*
	U8 i;
	
	switch(way)
	{
		case CAN_PARSE:
			switch(IPconfig.protocol)
			{
				case HAVAL_2013_2P4L_AT_2WD:
					data_copy((U8*)&can0.id_7a6,(U8*)&msg->data[0],msg->dlc);
					
					for(i=0;i<8;i++)
					{
						IPconfig.Msg[i]=can0.id_7a6.config[i];
					}
					
					break;
				case HAVAL_2013_1P5T_MT_2WD:
					data_copy((U8*)&can1.id_7a6,(U8*)&msg->data[0],msg->dlc);
					
					for(i=0;i<8;i++)
					{
						IPconfig.Msg[i]=can1.id_7a6.config[i];
					}
					
					break;
				case HAVAL_2012_1P5T_MT_2WD:
					data_copy((U8*)&can2.id_7a6,(U8*)&msg->data[0],msg->dlc);
					
					for(i=0;i<8;i++)
					{
						IPconfig.Msg[i]=can2.id_7a6.config[i];
					}
					
					break;
				case HAVAL_2015_1P5T_6AT_2WD:
					data_copy((U8*)&can3.id_7a6,(U8*)&msg->data[0],msg->dlc);
					
					for(i=0;i<8;i++)
					{
						IPconfig.Msg[i]=can3.id_7a6.config[i];
					}
					
					break;
				case HAVAL_2011_2P0T_MT_2WD:
					data_copy((U8*)&can4.id_7a6,(U8*)&msg->data[0],msg->dlc);
					
					for(i=0;i<8;i++)
					{
						IPconfig.Msg[i]=can4.id_7a6.config[i];
					}
					
					break;
				case HAVAL_2013_1P5T_MT_4WD:
					data_copy((U8*)&can5.id_7a6,(U8*)&msg->data[0],msg->dlc);
					
					for(i=0;i<8;i++)
					{
						IPconfig.Msg[i]=can5.id_7a6.config[i];
					}
					
					break;
			}
			
			can_id_7a6_lost_timecnt=0;
			break;
		case CAN_LOST:
			//memset(&can.id_7a6,0,msg->dlc);
			
			 break;
		default:break;
	}
	*/
} 


static void  can_id_251_analyse(can_msg_t *msg,can_pro_way_e way) 
{
	switch(way)
	{
		case CAN_PARSE:
			switch(IPconfig.protocol)
			{
				case HAVAL_2013_2P4L_AT_2WD:
					
					break;
				case HAVAL_2013_1P5T_MT_2WD:
					
					break;
				case HAVAL_2012_1P5T_MT_2WD:
					
					break;
				case HAVAL_2015_1P5T_6AT_2WD:
					
					break;
				case HAVAL_2011_2P0T_MT_2WD:
					
					break;
				case HAVAL_2013_1P5T_MT_4WD:
					data_copy((U8*)&can5.id_251,(U8*)&msg->data[0],msg->dlc);
					break;
			}
			
			can_id_251_lost_timecnt=0;
			break;
		case CAN_LOST:
			//memset(&can.id_251,0,msg->dlc);
			
			 break;
		default:break;
	}
} 


static void  can_id_116_analyse(can_msg_t *msg,can_pro_way_e way) 
{
	switch(way)
	{
		case CAN_PARSE:
			switch(IPconfig.protocol)
			{
				case HAVAL_2013_2P4L_AT_2WD:
					
					break;
				case HAVAL_2013_1P5T_MT_2WD:
					
					break;
				case HAVAL_2012_1P5T_MT_2WD:
					
					break;
				case HAVAL_2015_1P5T_6AT_2WD:
					data_copy((U8*)&can3.id_116,(U8*)&msg->data[0],msg->dlc);
					break;
				case HAVAL_2011_2P0T_MT_2WD:
					
					break;
				case HAVAL_2013_1P5T_MT_4WD:
					
					break;
			}
			
			can_id_116_lost_timecnt=0;
			break;
		case CAN_LOST:
			//memset(&can.id_251,0,msg->dlc);
			
			 break;
		default:break;
	}
} 


static void  can_id_211_analyse(can_msg_t *msg,can_pro_way_e way) 
{
	switch(way)
	{
		case CAN_PARSE:
			switch(IPconfig.protocol)
			{
				case HAVAL_2013_2P4L_AT_2WD:
					//data_copy((U8*)&can0.id_211,(U8*)&msg->data[0],msg->dlc);
					break;
				case HAVAL_2013_1P5T_MT_2WD:
					data_copy((U8*)&can1.id_211,(U8*)&msg->data[0],msg->dlc);
					break;
				case HAVAL_2012_1P5T_MT_2WD:
					data_copy((U8*)&can2.id_211,(U8*)&msg->data[0],msg->dlc);
					break;
				case HAVAL_2015_1P5T_MT_2WD: 
				case HAVAL_2015_1P5T_6AT_2WD:
					data_copy((U8*)&can3.id_211,(U8*)&msg->data[0],msg->dlc);
					break;
				case HAVAL_2011_2P0T_MT_2WD:
					data_copy((U8*)&can4.id_211,(U8*)&msg->data[0],msg->dlc);
					break;
				case HAVAL_2013_1P5T_MT_4WD:
					data_copy((U8*)&can5.id_211,(U8*)&msg->data[0],msg->dlc);
					break;
			}
			
			can_id_211_lost_timecnt=0;
			break;
		case CAN_LOST:
			//memset(&can.id_211,0,msg->dlc);
			
			 break;
		default:break;
	}
} 



static void  can_id_242_analyse(can_msg_t *msg,can_pro_way_e way) 
{
	switch(way)
	{
		case CAN_PARSE:
			switch(IPconfig.protocol)
			{
				case HAVAL_2013_2P4L_AT_2WD:
					//data_copy((U8*)&can0.id_242,(U8*)&msg->data[0],msg->dlc);
					break;
				case HAVAL_2013_1P5T_MT_2WD:
					//data_copy((U8*)&can1.id_242,(U8*)&msg->data[0],msg->dlc);
					break;
				case HAVAL_2012_1P5T_MT_2WD:
					//data_copy((U8*)&can2.id_242,(U8*)&msg->data[0],msg->dlc);
					break;
				case HAVAL_2015_1P5T_MT_2WD: 
				case HAVAL_2015_1P5T_6AT_2WD:
					data_copy((U8*)&can3.id_242,(U8*)&msg->data[0],msg->dlc);
					break;
				case HAVAL_2011_2P0T_MT_2WD:
					//data_copy((U8*)&can4.id_242,(U8*)&msg->data[0],msg->dlc);
					break;
				case HAVAL_2013_1P5T_MT_4WD:
					//data_copy((U8*)&can5.id_242,(U8*)&msg->data[0],msg->dlc);
					break;
			}
			
			can_id_242_lost_timecnt=0;
			break;
		case CAN_LOST:
			
			 break;
		default:break;
	}
} 


static void  can_id_0A5_analyse(can_msg_t *msg,can_pro_way_e way) 
{
	switch(way)
	{
		case CAN_PARSE:
			switch(IPconfig.protocol)
			{
				case HAVAL_2013_2P4L_AT_2WD:
					//data_copy((U8*)&can0.id_0A5,(U8*)&msg->data[0],msg->dlc);
					break;
				case HAVAL_2013_1P5T_MT_2WD:
					data_copy((U8*)&can1.id_0A5,(U8*)&msg->data[0],msg->dlc);
					break;
				case HAVAL_2012_1P5T_MT_2WD:
					//data_copy((U8*)&can2.id_0A5,(U8*)&msg->data[0],msg->dlc);
					break;
				case HAVAL_2015_1P5T_MT_2WD: 
				case HAVAL_2015_1P5T_6AT_2WD:
					data_copy((U8*)&can3.id_0A5,(U8*)&msg->data[0],msg->dlc);
					break;
				case HAVAL_2011_2P0T_MT_2WD:
					//data_copy((U8*)&can4.id_0A5,(U8*)&msg->data[0],msg->dlc);
					break;
				case HAVAL_2013_1P5T_MT_4WD:
					data_copy((U8*)&can5.id_0A5,(U8*)&msg->data[0],msg->dlc);
					break;
			}
			
			can_id_0A5_lost_timecnt=0;
			break;
		case CAN_LOST:
			
			 break;
		default:break;
	}
} 

static void  can_id_766_analyse(can_msg_t *msg,can_pro_way_e way) 
{
	switch(way)
	{
		case CAN_PARSE:
			data_copy((U8*)&uds_recv,(U8*)&msg->data[0],msg->dlc);			
			break;
		case CAN_LOST:
			
			 break;
		default:break;
	}
}

static void  can_id_365_analyse(can_msg_t *msg,can_pro_way_e way) 
{
	switch(way)
	{
		case CAN_PARSE:
			switch(IPconfig.protocol)
			{
				case HAVAL_2013_2P4L_AT_2WD:
					data_copy((U8*)&can0.id_365,(U8*)&msg->data[0],msg->dlc);
					break;
				case HAVAL_2013_1P5T_MT_2WD:
					data_copy((U8*)&can1.id_365,(U8*)&msg->data[0],msg->dlc);
					break;
				case HAVAL_2012_1P5T_MT_2WD:
					//data_copy((U8*)&can2.id_365,(U8*)&msg->data[0],msg->dlc);
					break;
				case HAVAL_2015_1P5T_MT_2WD: 
				case HAVAL_2015_1P5T_6AT_2WD:
					data_copy((U8*)&can3.id_365,(U8*)&msg->data[0],msg->dlc);
					break;
				case HAVAL_2011_2P0T_MT_2WD:
					//data_copy((U8*)&can4.id_365,(U8*)&msg->data[0],msg->dlc);
					break;
				case HAVAL_2013_1P5T_MT_4WD:
					data_copy((U8*)&can5.id_365,(U8*)&msg->data[0],msg->dlc);
					break;
			}
			
			can_id_365_lost_timecnt=0;
			break;
		case CAN_LOST:
			
			 break;
		default:break;
	}
} 



static void  can_id_08B_analyse(can_msg_t *msg,can_pro_way_e way) 
{
	switch(way)
	{
		case CAN_PARSE:
			switch(IPconfig.protocol)
			{
				case HAVAL_2013_2P4L_AT_2WD:
					data_copy((U8*)&can0.id_08B,(U8*)&msg->data[0],msg->dlc);
					break;
				case HAVAL_2013_1P5T_MT_2WD:
					data_copy((U8*)&can1.id_08B,(U8*)&msg->data[0],msg->dlc);
					break;
				case HAVAL_2012_1P5T_MT_2WD:
					data_copy((U8*)&can2.id_08B,(U8*)&msg->data[0],msg->dlc);
					break;
				case HAVAL_2015_1P5T_MT_2WD: 
				case HAVAL_2015_1P5T_6AT_2WD:
					data_copy((U8*)&can3.id_08B,(U8*)&msg->data[0],msg->dlc);
					break;
				case HAVAL_2011_2P0T_MT_2WD:
					data_copy((U8*)&can4.id_08B,(U8*)&msg->data[0],msg->dlc);
					break;
				case HAVAL_2013_1P5T_MT_4WD:
					data_copy((U8*)&can5.id_08B,(U8*)&msg->data[0],msg->dlc);
					break;
			}
			
			can_id_08B_lost_timecnt=0;
			break;
		case CAN_LOST:
			
			 break;
		default:break;
	}
} 

static void  can_id_485_analyse(can_msg_t *msg,can_pro_way_e way) 
{
	switch(way)
	{
		case CAN_PARSE:
			switch(IPconfig.protocol)
			{
				case HAVAL_2013_2P4L_AT_2WD:
					data_copy((U8*)&can0.id_485,(U8*)&msg->data[0],msg->dlc);
					break;
				case HAVAL_2013_1P5T_MT_2WD:
					data_copy((U8*)&can1.id_485,(U8*)&msg->data[0],msg->dlc);
					break;
				case HAVAL_2012_1P5T_MT_2WD:
					data_copy((U8*)&can2.id_485,(U8*)&msg->data[0],msg->dlc);
					break;
				case HAVAL_2015_1P5T_MT_2WD: 
				case HAVAL_2015_1P5T_6AT_2WD:
					data_copy((U8*)&can3.id_485,(U8*)&msg->data[0],msg->dlc);
					break;
				case HAVAL_2011_2P0T_MT_2WD:
					data_copy((U8*)&can4.id_485,(U8*)&msg->data[0],msg->dlc);
					break;
				case HAVAL_2013_1P5T_MT_4WD:
					data_copy((U8*)&can5.id_485,(U8*)&msg->data[0],msg->dlc);
					break;
			}
			
			can_id_485_lost_timecnt=0;
			break;
		case CAN_LOST:
			
			 break;
		default:break;
	}
}



//MCU is big endian
union
{
	U32 odo32;
	U8	odo8[4];
}total_odo;

void MCU_TO_PC_send(void)
{
	U8 msg_box;
	
	//发送MCU TO PC的报文
	 msg_box = ID_RECV_NUM_ALL+1;
	can_msg[msg_box-1].buffer_num = msg_box;
	can_msg[msg_box-1].data[0] =  0;
	can_msg[msg_box-1].data[1] = 	0;
	can_msg[msg_box-1].data[2] =  0; 
	can_msg[msg_box-1].data[3] =  0;
	can_msg[msg_box-1].data[4] =  0;
	can_msg[msg_box-1].data[5] = 	0;
	can_msg[msg_box-1].data[6] = 	0;
	can_msg[msg_box-1].data[7] = 	0;
  
	hal_can_sent(CAN_CHN,&can_msg[msg_box-1]);
}
void can_id_345_send(void)
{
	U8 msg_box;
	
	//发送总里程信息的报文
	msg_box = ID_RECV_NUM_ALL+2;
	can_msg[msg_box-1].buffer_num = msg_box;
	
	total_odo.odo32=info.Odo;
	
	can_msg[msg_box-1].data[0] = 1;
	can_msg[msg_box-1].data[1] = total_odo.odo8[1];
	can_msg[msg_box-1].data[2] = total_odo.odo8[2]; 
	can_msg[msg_box-1].data[3] = total_odo.odo8[3];
	can_msg[msg_box-1].data[4] = 0;
	can_msg[msg_box-1].data[5] = 0;
	can_msg[msg_box-1].data[6] = 0;
	can_msg[msg_box-1].data[7] = 0;
	
	if(IPconfig.Sts==1)//总里程有效时，才往外发里程相关的CAN 报文
	{
		hal_can_sent(CAN_CHN,&can_msg[msg_box-1]);
	}
	else
	{
		
	}
}

void can_id_766_send(void)
{
	U8 msg_box;
	
	//发送总里程信息的报文
	msg_box = ID_RECV_NUM_ALL+3;
	can_msg[msg_box-1].buffer_num = msg_box;

	can_msg[msg_box-1].data[0] = 2;
	can_msg[msg_box-1].data[1] = 16;
	can_msg[msg_box-1].data[2] = 3; 
	can_msg[msg_box-1].data[3] = 0;
	can_msg[msg_box-1].data[4] = 0;
	can_msg[msg_box-1].data[5] = 0;
	can_msg[msg_box-1].data[6] = 0;
	can_msg[msg_box-1].data[7] = 0;
	
	if(IPconfig.Sts==1)//仪表未激活的情况下，才发送配置代码请求帧
	{
		//hal_can_sent(CAN_CHN,&can_msg[msg_box-1]);
	}
	else//仪表激活的情况下，就不会发送配置代码请求帧了
	{
		
	}
}


void can_id_7A6_send(T_UDS *data, U8 len)
{
	U8 msg_box;

	if(data->len == 0) return;
	
	//发送总里程信息的报文
	msg_box = ID_RECV_NUM_ALL+4;
	can_msg[msg_box-1].buffer_num = msg_box;

	can_msg[msg_box-1].data[0] = data->len;
	can_msg[msg_box-1].data[1] = data->id;
	can_msg[msg_box-1].data[2] = data->data[0]; 
	can_msg[msg_box-1].data[3] = data->data[1];
	can_msg[msg_box-1].data[4] = data->data[2];
	can_msg[msg_box-1].data[5] = data->data[3];
	can_msg[msg_box-1].data[6] = data->data[4];
	can_msg[msg_box-1].data[7] = data->data[5];

//	data_copy(can_msg[msg_box-1].data, (U8*)&data, len);

	hal_can_sent(CAN_CHN,&can_msg[msg_box-1]);

	memset(&uds_sent,0,sizeof(uds_sent));


}

void app_can_sent_task(void)
{
	//MCU_TO_PC_send();
	if(uart_special_data.specialInfo.LedWarning.third.bit.IPconfig==2)
	{
		can_id_345_send();
		can_id_7A6_send(&uds_sent, sizeof(uds_sent)/sizeof(U8));
	}
	else if(uart_special_data.specialInfo.LedWarning.third.bit.IPconfig==1)
	{
		can_id_766_send();
	}
	else
	{
		
	}
}

void app_can_lost_time_cnt_100ms(void)
{
	static U8 g_u8IgnSts_bak=OFF;
	
	if((ON == g_u8IgnSts)&&(OFF==g_u8IgnSts_bak))
	{
		can_id_095_lost_timecnt=0xffff;
		can_id_111_lost_timecnt=0xffff;
		can_id_131_lost_timecnt=0xffff;
		can_id_265_lost_timecnt=0xffff;
		can_id_271_lost_timecnt=0xffff;
		can_id_311_lost_timecnt=0xffff;
		can_id_351_lost_timecnt=0xffff;
		can_id_371_lost_timecnt=0xffff;
		can_id_7E7_lost_timecnt=0xffff;
		can_id_3df_lost_timecnt=0xffff;
		can_id_345_lost_timecnt=0xffff;
		can_id_221_lost_timecnt=0xffff;
		can_id_7a6_lost_timecnt=0xffff;
		can_id_251_lost_timecnt=0xffff;
		can_id_116_lost_timecnt=0xffff;
		can_id_211_lost_timecnt=0xffff;
		can_id_242_lost_timecnt=0xffff;
		can_id_0A5_lost_timecnt=0xffff;
		can_id_365_lost_timecnt=0xffff;
		can_id_08B_lost_timecnt=0xffff;
		can_id_485_lost_timecnt=0xffff;
		
		memset(&can0,0,sizeof(can0));
		memset(&can1,0,sizeof(can1));
		memset(&can2,0,sizeof(can2));
		memset(&can3,0,sizeof(can3));
		memset(&can4,0,sizeof(can4));
		memset(&can5,0,sizeof(can5));
		
		g_u8IgnSts_bak=g_u8IgnSts;
	}
	else if((OFF == g_u8IgnSts)&&(ON==g_u8IgnSts_bak))
	{
		can_id_095_lost_timecnt=0;
		can_id_111_lost_timecnt=0;
		can_id_131_lost_timecnt=0;
		can_id_265_lost_timecnt=0;
		can_id_271_lost_timecnt=0;
		can_id_311_lost_timecnt=0;
		can_id_351_lost_timecnt=0;
		can_id_371_lost_timecnt=0;
		can_id_7E7_lost_timecnt=0;
		can_id_3df_lost_timecnt=0;
		can_id_345_lost_timecnt=0;
		can_id_221_lost_timecnt=0;
		can_id_7a6_lost_timecnt=0;
		can_id_251_lost_timecnt=0;
		can_id_116_lost_timecnt=0;
		can_id_211_lost_timecnt=0;
		can_id_242_lost_timecnt=0;
		can_id_0A5_lost_timecnt=0;
		can_id_365_lost_timecnt=0;
		can_id_08B_lost_timecnt=0;
		can_id_485_lost_timecnt=0;
		
		memset(&can0,0,sizeof(can0));
		memset(&can1,0,sizeof(can1));
		memset(&can2,0,sizeof(can2));
		memset(&can3,0,sizeof(can3));
		memset(&can4,0,sizeof(can4));
		memset(&can5,0,sizeof(can5));
		
		g_u8IgnSts_bak=g_u8IgnSts;
	}
	else
	{
		if(can_id_095_lost_timecnt<0xffff)
		{
			can_id_095_lost_timecnt++;
		}
		if(can_id_111_lost_timecnt<0xffff)
		{
			can_id_111_lost_timecnt++;
		}
		if(can_id_131_lost_timecnt<0xffff)
		{
			can_id_131_lost_timecnt++;
		}
		if(can_id_265_lost_timecnt<0xffff)
		{
			can_id_265_lost_timecnt++;
		}
		if(can_id_271_lost_timecnt<0xffff)
		{
			can_id_271_lost_timecnt++;
		}
		if(can_id_311_lost_timecnt<0xffff)
		{
			can_id_311_lost_timecnt++;
		}
		if(can_id_351_lost_timecnt<0xffff)
		{
			can_id_351_lost_timecnt++;
		}
		if(can_id_371_lost_timecnt<0xffff)
		{
			can_id_371_lost_timecnt++;
		}
		if(can_id_7E7_lost_timecnt<0xffff)
		{
			can_id_7E7_lost_timecnt++;
		}
		if(can_id_3df_lost_timecnt<0xffff)
		{
			can_id_3df_lost_timecnt++;
		}
		if(can_id_345_lost_timecnt<0xffff)
		{
			can_id_345_lost_timecnt++;
		}
		if(can_id_221_lost_timecnt<0xffff)
		{
			can_id_221_lost_timecnt++;
		}
		if(can_id_7a6_lost_timecnt<0xffff)
		{
			can_id_7a6_lost_timecnt++;
		}
		if(can_id_251_lost_timecnt<0xffff)
		{
			can_id_251_lost_timecnt++;
		}
		if(can_id_116_lost_timecnt<0xffff)
		{
			can_id_116_lost_timecnt++;
		}
		if(can_id_211_lost_timecnt<0xffff)
		{
			can_id_211_lost_timecnt++;
		}
		if(can_id_242_lost_timecnt<0xffff)
		{
			can_id_242_lost_timecnt++;
		}
		if(can_id_0A5_lost_timecnt<0xffff)
		{
			can_id_0A5_lost_timecnt++;
		}
		if(can_id_365_lost_timecnt<0xffff)
		{
			can_id_365_lost_timecnt++;
		}
		if(can_id_08B_lost_timecnt<0xffff)
		{
			can_id_08B_lost_timecnt++;
		}
		if(can_id_485_lost_timecnt<0xffff)
		{
			can_id_485_lost_timecnt++;
		}
	}
}
